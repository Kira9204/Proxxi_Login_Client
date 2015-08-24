/////////////////////////////////////////////////////////////////////////////
// Name:        CURLComm.cpp
// Encoding:	UTF-8
// Purpose:     OS independent CURL API Library
// Author:      Erik Welander
// Modified:    2015-01-22
// Copyright:   Erik Welander
// Licence:     GNU GPL
// Special thanks to: Zenwood (http://www.zedwood.com/article/cpp-libcurl-static-class)
/////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#include "../headers/CURLComm.h"

// ----------------------------------------------------------------------------
//   Declarations
// ----------------------------------------------------------------------------

CURLComm::CURLComm()
{
	m_cookieFile = "";
	m_cert = false;
}

CURLComm::CURLComm(const string cookieFile, const bool requreValidCertificate)
{
	m_cookieFile = cookieFile;
	m_cert = requreValidCertificate;
}

const string CURLComm::get(const string url)
{
	return CURLComm::curlIt(url, "", true);
}

const string CURLComm::post(const string url, const string postData)
{
	return CURLComm::curlIt(url, postData, true);
}

const bool CURLComm::postSuccessful(const string url, const string postData)
{
	if (CURLComm::curlIt(url, postData, false).compare("OK") == 0)
	{
		return true;
	}
	return false;
}

void CURLComm::cleanCookies()
{
#if defined _WIN32
	libSys::runSilentCMD("del /F \"" + m_cookieFile + "\"");
#else
	libSys::runSilentCMD("rm -f\"" + m_cookieFile + "\"");
#endif
}


// ----------------------------------------------------------------------------
//   Private methods
// ----------------------------------------------------------------------------

const string CURLComm::curlIt(const string url, const string postData, const bool retData)
{
	string data = "";
	CURL *curl;
	CURLcode result;

	//Init all CURL functions, In windows, this will also init the winsock stuff
		curl_global_init(CURL_GLOBAL_ALL);
	//get a curl handle
	curl = curl_easy_init();
	if (curl)
	{
		//First set the URL that we want to access, this can be a http:// address as well as an https:// address
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		//Sets the timoeout limit in seconds that we will wait for the host to reply.
			curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5);
		//Sets the ip protocol that we want to use. Currently Proxxi's PILS system only supports IPv4.
			curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
		//Include header information in the output?
			curl_easy_setopt(curl, CURLOPT_HEADER, 0L);
		//Follow any redirects from the webserver?
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0L);
		if (m_cert)
		{
			//Enable SSL certificate verification?
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
			//Does the hostname match the hostname in the certificate?
			//Used to verify that the hostname matches the hostname in the certificate.
			//1 to check the existence of a common name in the SSL peer certificate.
			//2 to check the existence of a common name and also verify that it matches the hostname provided.
			//This should i pretty much in all cases be at 2
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
			curl_easy_setopt(curl, CURLOPT_SSLENGINE_DEFAULT, 1L);
			curl_easy_setopt(curl, CURLOPT_SSLCERTTYPE, "PEM");
			curl_easy_setopt(curl, CURLOPT_SSLCERT, "cacert.pem");
		}
		else
		{
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		}
		if (m_cookieFile.length() > 0)
		{
			//Specifies the file that we will read our cookies from
				curl_easy_setopt(curl, CURLOPT_COOKIEFILE, m_cookieFile.c_str());
			//Specifies the file that we will write our cookies to
				curl_easy_setopt(curl, CURLOPT_COOKIEJAR, m_cookieFile.c_str());
		}
		if (postData.compare("") != 0)
		{
			curl_easy_setopt(curl, CURLOPT_POST, 1);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
		}

		//The writefuction that will be used to recieve all output data
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_string);
		//The string buffer that we will use to recieve all of our data
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

		//And off we go!
		//Attempt to retrieve the remote page
			result = curl_easy_perform(curl);
		//Clean up CURL
			curl_easy_cleanup(curl);
			curl_global_cleanup();
		// So how did it go?
		if (result == CURLE_OK)
		{
			if (!retData)
			{
				return "OK";
			}
			return data;
		}
		//return "ERROR IN TRANSMISSION";
	}
	//If CURL failed to initialize itself
	//return "FAILED TO INITIALIZE CURL";
	return "";
}

//Thank Zenwood for this fuction:
//http://www.zedwood.com/article/cpp-libcurl-static-class
int CURLComm::write_to_string(char *data, size_t size, size_t nmemb, string *buffer)
{
	int result = 0;
	if (buffer != NULL)
	{
		buffer->append(data, size * nmemb);
		result = size * nmemb;
	}
	return result;
}