#ifndef CURL_Comm_H
#define CURL_Comm_H
/////////////////////////////////////////////////////////////////////////////
// Name:        CURLComm.h
// Encoding:	UTF-8
// Purpose:     OS independent CURL API Library
// Author:      Erik Welander
// Modified:    2014-08-09
// Copyright:   Erik Welander
// Licence:     GNU GPL
/////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "curl/curl.h"
#include "libSys.h"

// ----------------------------------------------------------------------------
//   Declarations
// ----------------------------------------------------------------------------
using std::string;
class CURLComm
{
	public:
		CURLComm();
		CURLComm(const string cookieFile, const bool cert);
		const string get(const string url);
		const string post(const string url, const string postData);
		const bool postSuccessful(const string url, const string postData);
		void cleanCookies();

		protected: static int write_to_string(char *data, size_t size, size_t nmemb, string *buffer);

	private:
		const string curlIt(const string url, const string postData, const bool retData);
		string m_cookieFile;
		bool m_cert;
}; 
#endif //CURL_Comm.h