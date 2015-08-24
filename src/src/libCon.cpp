/////////////////////////////////////////////////////////////////////////////
// Name:        libCon.cpp
// Encoding:	UTF-8
// Purpose:     Various APIs for data conversations and formatting
// Author:      Erik Welander
// Modified:    2014-08-09
// Copyright:   Erik Welander
// Licence:     GNU GPL
/////////////////////////////////////////////////////////////////////////////


// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#include "../headers/libCon.h"

// ----------------------------------------------------------------------------
//   Declarations
// ----------------------------------------------------------------------------

#if defined _WIN32
	LPWSTR libCon::stringToLPCTSTR(const string& str)
	{
		LPWSTR ws = new wchar_t[str.size() + 1]; // +1 for zero at the end
		copy(str.begin(), str.end(), ws);
		ws[str.size()] = 0; // zero at the end
		return ws;
	}
#endif
wxString libCon::stringToWxString(const string str)
{
	return wxString::From8BitData(str.c_str());
}
string libCon::wxStringToString(const wxString wxStr)
{
	return string(wxStr.mb_str());
}
wxString libCon::intToWxString(const int num)
{
	return wxString::Format(_T("%i"), num);
}
string libCon::intToString(const int num)
{
	std::stringstream ss;//create a stringstream
	ss << num;//add number to the stream
	return ss.str();//return a string with the contents of the stream
}

int libCon::stringToInt(const string str)
{
	std::stringstream ss;
	int num = 0;
	ss.str(str);
	if (!(ss >> num))    //The string could not be converted to a number!
		num = -1;
	return num;
}

int libCon::wxStringToInt(const wxString wxStr)
{
	return wxAtoi(wxStr);
}

string libCon::stringToUpper(string str)
{
	string alfHi = "ABCDEFGHIJKLMNOPQRSTUVWXYZÅÄÖ";
	string alfLow = "abcdefghijklmnopqrstuvwxyzåäö";
	for (int i = 0; i < alfLow.length(); i++)
	{
		replace(str.begin(), str.end(), alfLow[i], alfHi[i]);
	}
	return str;
}
string libCon::stringToUpper(string str, int pos)
{
	string alfHi = "ABCDEFGHIJKLMNOPQRSTUVWXYZÅÄÖ";
	string alfLow = "abcdefghijklmnopqrstuvwxyzåäö";
	int inAlf = alfLow.find(str[pos]);
	if (inAlf != str.npos)
	{
		str[pos] = alfHi[inAlf];
	}
	return str;
}

string libCon::stringToLower(string str)
{
	string alfHi = "ABCDEFGHIJKLMNOPQRSTUVWXYZÅÄÖ";
	string alfLow = "abcdefghijklmnopqrstuvwxyzåäö";
	for (int i = 0; i < alfLow.length(); i++)
	{
		replace(str.begin(), str.end(), alfHi[i], alfLow[i]);
	}
	return str;
}

string libCon::stringToLower(string str, int pos)
{
	string alfHi = "ABCDEFGHIJKLMNOPQRSTUVWXYZÅÄÖ";
	string alfLow = "abcdefghijklmnopqrstuvwxyzåäö";
	int inAlf = alfHi.find(str[pos]);
	if (inAlf != str.npos)
	{
		str[pos] = alfLow[inAlf];
	}
	return str;
}

string libCon::timeToString(const int sec)
{
	return libCon::intToString(((sec / 60) / 60) % 24) + "H " + libCon::intToString((sec / 60) % 60) + "M " + libCon::intToString(sec % 60) + "S";
}