#ifndef MEMBER_H
#define MEMBER_H
/////////////////////////////////////////////////////////////////////////////
// Name:        Member.h
// Encoding:	UTF-8
// Purpose:     Stores information about a logged on user
// Author:      Erik Welander
// Modified:    2014-08-21
// Copyright:   Erik Welander
// Licence:     GNU GPL
/////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#include <string>
#include "StringParser.h"

// ----------------------------------------------------------------------------
//   Declarations
// ----------------------------------------------------------------------------
using std::string;

namespace MEMBER_INFO
{
	enum PROP
	{
		NAME_FIRST,
		NAME_LAST,
		NAME_ACCOUNT,
		MEM_ACTIVE,
		MEM_BANNED,
		RIGHTS,
		ADDRESS,
		EMAIL,
		PHONE_MOBILE,
		PHONE_ALT,
		NUM_ELEMENTS
	};
}

namespace MEMBER_RIGHTS
{
	enum PROP
	{
		INTERNET,
		MULTILOGG,
		NUM_ELEMENTS
	};
}


class Member
{
public:
	Member();
	Member(const string& data);
	void parseData(const string& data);
	void clearData();
	const string getProperty(enum MEMBER_INFO::PROP query);
	const bool getRight(enum MEMBER_RIGHTS::PROP query);
	const bool stringToBool(const string& str);
private:
	string* m_info;
	bool* m_rights;
};
#endif