/////////////////////////////////////////////////////////////////////////////
// Name:        libCon.h
// Encoding:	UTF-8
// Purpose:     Various APIs for data conversations and formatting
// Author:      Erik Welander
// Modified:    2015-01-22
// Copyright:   Erik Welander
// Licence:     GNU GPL
/////////////////////////////////////////////////////////////////////////////


// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#ifdef _WIN32
	#include <windows.h>
#endif
#include <string>
#include <sstream>
#include <stdio.h>
#include <algorithm>
#include <wx/string.h>

// ----------------------------------------------------------------------------
//   Declarations
// ----------------------------------------------------------------------------
using std::string;
using std::size_t;
namespace libCon
{
	#if defined _WIN32
		LPWSTR stringToLPCTSTR(const string& str);
	#endif
		wxString stringToWxString(const string str);
		string wxStringToString(const wxString wxStr);
		wxString intToWxString(const int num);
		string intToString(const int num);
		int stringToInt(const string str);
		int wxStringToInt(const wxString wxStr);
		string stringToUpper(string str);
		string stringToUpper(string str, int pos);
		string stringToLower(string str);
		string stringToLower(string str, int pos);

		string timeToString(const int sec);
}