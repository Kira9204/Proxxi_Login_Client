/////////////////////////////////////////////////////////////////////////////
// Name:        Authentication.cpp
// Encoding:	UTF-8
// Purpose:     Account validation and information
// Author:      Erik Welander
// Modified:    2014-08-21
// Copyright:   Erik Welander
// Licence:     Creative Commons "by-nc-nd"
/////////////////////////////////////////////////////////////////////////////


// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#include "../headers/Authentication.h"

// ----------------------------------------------------------------------------
//   Declarations
// ----------------------------------------------------------------------------


Authentication::Authentication()
{
	m_baseURL = "https://bruse.proxxi.org/kira/";
	m_cookieName = "Proxxi_Login_Client_Cookie.txt";
	m_pageCache = "";

	//See example server replies.
	//A valid server server session will always provide both network as well as user information.
	//If the reply happends to be shorter than this specified number, the reply must indicate some kind of error.
	m_minAcceptedLoginLength = 30;
	m_maxMultiLogins = 5;

	//Where do we want to store the application Cookie file?
	#if defined _WIN32
		string cookiesPath = libSys::getEnvVar("APPDATA") + "\\" + m_cookieName;
	#elif defined __APPLE__
		string cookiesPath = libSys::getCurrentDirectory()+"/Proxxi_Login_Client.app/Contents/"+m_cookieName;
	#else
		string cookiesPath = libSys::getCurrentDirectory()+"/"+m_cookieName;
	#endif

	m_curl = CURLComm(cookiesPath, true);
	m_member = Member();
	m_otherLogins = new string[m_maxMultiLogins];
	for (int i = 0; i < m_maxMultiLogins; i++)
	{
		m_otherLogins[i] = "";
	}
}

const int Authentication::login(const string username, const string password)
{
	int replyLength = 0;
	m_pageCache = m_curl.post(m_baseURL + "login.php", "username=" + username + "&password=" + password);
	replyLength = m_pageCache.length();
	updateOtherLoginsList();
	if (replyLength < m_minAcceptedLoginLength)
	{
		return LOGIN_REPLY::NOT_ACCEPTED;
	}
	else if (replyLength == 0)
	{
		return LOGIN_REPLY::NO_REPLY_FROM_HOST;
	}
	else
	{
		m_member.parseData(m_pageCache);
		if (!m_member.getRight(MEMBER_RIGHTS::INTERNET))
		{
			return LOGIN_REPLY::INTERNET_REVOKED;
		}
		if (!m_member.stringToBool(m_member.getProperty(MEMBER_INFO::MEM_ACTIVE)))
		{
			return LOGIN_REPLY::EXPIRED_MEMBERSHIP;
		}
		if (!m_member.stringToBool(m_member.getProperty(MEMBER_INFO::MEM_BANNED)))
		{
			return LOGIN_REPLY::BANNED_FROM_PROXXI;
		}
		return LOGIN_REPLY::OK;
	}
}

const bool Authentication::isLoggedIn()
{
	return (m_curl.get(m_baseURL + "printSessionInfo.php").length() > m_minAcceptedLoginLength);
}

const bool Authentication::keepAliveRefresh()
{
	m_pageCache = m_curl.get(m_baseURL + "keepAlive.php");
	updateOtherLoginsList();
	return m_pageCache.length() > m_minAcceptedLoginLength;
}

const bool Authentication::multiLoginComputer(const string hostname, const string password)
{
	return multiLoginAction("login", hostname, password);
}

const bool Authentication::removeMultiLogin(const string hostname, const string password)
{
	return !multiLoginAction("logout", hostname, password);
}

const int Authentication::getMaxOtherLogins()
{
	return m_maxMultiLogins;
}

int Authentication::getNumOtherLogins()
{
	int num = 0;
	for (int i = 0; i < m_maxMultiLogins; i++)
	{
		if (m_otherLogins[i].compare("") != 0)
		{
			num++;
		}
	}
	return num;
}

string* Authentication::getOtherLogins()
{
	return m_otherLogins;
}

Member Authentication::getMember()
{
	return m_member;
}

int Authentication::getStatusNum(enum LOGIN_REPLY prop)
{
	return prop;
}

void Authentication::logout()
{
	m_curl.get(m_baseURL + "logout.php");
}


// ----------------------------------------------------------------------------
//   Private methods
// ----------------------------------------------------------------------------
void Authentication::updateOtherLoginsList()
{
	StringParser sp(m_pageCache);
	if (sp.jumpLines(17))
	{
		for (int i = 0; i < m_maxMultiLogins; i++)
		{
			m_otherLogins[i] = "";
			if (sp.hasNext())
			{
				string line = sp.getLine();
				m_otherLogins[i] = line.substr(0, line.find_first_of(' '));
			}
		}
	}
}

const bool Authentication::multiLoginAction(const string& action, const string& hostname, const string& password)
{
	m_curl.post(m_baseURL + "multiLogin.php?action=" + action, "host=" + hostname + "&password=" + password);
	keepAliveRefresh();
	for (int i = 0; i < m_maxMultiLogins; i++)
	{
		if (m_otherLogins[i].find(hostname))
		{
			return true;
		}
	}
	return false;
}


string Authentication::ipToHostname(const string& ip)
{
	size_t loc = ip.find("10.0");
	if (isIP(ip)) //This is actually an IP address
	{
		string hostname = m_curl.get(m_baseURL + "/kira/ipToHostName.php?ip=" + ip);
		return formatHostname(hostname);
	}
	return ip;	//This is already a hostname
}

const bool Authentication::isIP(const string& ip)
{
	if ((ip.find("10.0")) != string::npos)
	{
		return true;
	}
	return false;
}

const string Authentication::formatHostname(string hostname)
{
	hostname = hostname.substr(0, hostname.find("."));
	hostname[0] = toupper(hostname[0]);
	for (int i = 1; i < hostname.length(); i++)
	{
		hostname[i] = tolower(hostname[i]);
	}
	return hostname;
}
