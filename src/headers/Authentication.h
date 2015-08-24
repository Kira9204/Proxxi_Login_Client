#ifndef Authentication_H
#define Authentication_H
/////////////////////////////////////////////////////////////////////////////
// Name:        Authentication.h
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
#include <string>
#include <ctype.h>  //Used for char uppercase
#include "CURLComm.h"
#include "Member.h"
#include "libSys.h"

// ----------------------------------------------------------------------------
//   Declarations
// ----------------------------------------------------------------------------
using std::string;
using std::size_t;

enum LOGIN_REPLY
{
	NO_REPLY_FROM_HOST = -1,
	NOT_ACCEPTED,
	EXPIRED_MEMBERSHIP,
	INTERNET_REVOKED,
	BANNED_FROM_PROXXI,
	OK

};

class Authentication
{
	public:
		Authentication();
		const int login(const string username, const string password);
		const bool isLoggedIn();
		const bool keepAliveRefresh();
		const bool multiLoginComputer(const string hostname, const string password);
		const bool removeMultiLogin(const string hostname, const string password);
		const int getMaxOtherLogins();
		int getNumOtherLogins();
		string* getOtherLogins();
		Member getMember();
		int getStatusNum(enum LOGIN_REPLY prop);
		void logout();

	private:
		void updateOtherLoginsList();
		const bool multiLoginAction(const string& action, const string& hostname, const string& password);
		string ipToHostname(const string& ip);
		const bool isIP(const string& ip);
		const string formatHostname(string hostname);

		string m_baseURL;
		string m_cookieName;
		string m_pageCache;
		int m_minAcceptedLoginLength;
		int m_maxMultiLogins;
		CURLComm m_curl;
		Member m_member;
		string* m_otherLogins;
};

#endif