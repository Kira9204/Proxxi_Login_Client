/////////////////////////////////////////////////////////////////////////////
// Name:        Member.h
// Encoding:	UTF-8
// Purpose:     Stores information about a logged on user
// Author:      Erik Welander
// Modified:    2014-11-30
// Copyright:   Erik Welander
// Licence:     GNU GPL
/////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#include "../headers/Member.h"

// ----------------------------------------------------------------------------
//   Declarations
// ----------------------------------------------------------------------------

Member::Member()
{
	m_info = new string[MEMBER_INFO::NUM_ELEMENTS];
	m_rights = new bool[MEMBER_RIGHTS::NUM_ELEMENTS];
	clearData();
}

Member::Member(const string& data)
{
	m_info = new string[MEMBER_INFO::NUM_ELEMENTS];
	m_rights = new bool[MEMBER_RIGHTS::NUM_ELEMENTS];
	parseData(data);
}

void Member::parseData(const string& data)
{
	StringParser sp(data);
	sp.jumpLines(6); // Jump past all the junk data before we start to parse the data
	for (int i = 0; i < MEMBER_INFO::NUM_ELEMENTS; i++)
	{
		m_info[i] = sp.getLine();
		m_info[i] = m_info[i].substr(26, m_info[i].find_first_of('<') - 26);
	}

	if ((m_info[MEMBER_INFO::RIGHTS].find("\"internet\"", 0)) != string::npos)
	{
		m_rights[MEMBER_RIGHTS::INTERNET] = true;
	}

	if ((m_info[MEMBER_INFO::RIGHTS].find("\"multlogg\"", 0)) != string::npos)
	{
		m_rights[MEMBER_RIGHTS::MULTILOGG] = true;
	}
}

void Member::clearData()
{
	for (int i = 0; i < MEMBER_INFO::NUM_ELEMENTS; i++)
	{
		m_info[i] = "";
	}
	for (int i = 0; i < MEMBER_RIGHTS::NUM_ELEMENTS; i++)
	{
		m_rights[i] = false;
	}
}

const string Member::getProperty(enum MEMBER_INFO::PROP query)
{
	return m_info[query];
}

const bool Member::getRight(enum MEMBER_RIGHTS::PROP query)
{
	return m_rights[query];
}

const bool Member::stringToBool(const string& str)
{
	if (str.find("yes") != string::npos)
	{
		return true;
	}
	return false;
}