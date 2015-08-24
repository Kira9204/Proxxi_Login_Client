/////////////////////////////////////////////////////////////////////////////
// Name:        StringParser.cpp
// Encoding:	UTF-8
// Purpose:     A string parser with the ability to jump and skip lines
// Author:      Erik Welander
// Modified:    2015-01-20
// Copyright:   Erik Welander
// Licence:     GNU GPL
/////////////////////////////////////////////////////////////////////////////


// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#include "../headers/StringParser.h"


StringParser::StringParser()
{
	m_str = "";
	m_counter = 0;
}

StringParser::StringParser(const string& str)
{
	m_str = str;
	m_counter = 0;
}

void StringParser::assign(const string& str)
{
	m_str = str;
	m_counter = 0;
}

void StringParser::clear()
{
	m_str = "";
	m_counter = 0;
}

const bool StringParser::hasNext()
{
	return m_counter < m_str.length();
}

const bool StringParser::jumpLines(const int numJumps)
{
	int jumps = 0;
	while (jumps < numJumps && m_counter < m_str.length())
	{
		if (m_str[m_counter] == '\n')
		{
			jumps++;
		}
		m_counter++;
	}
	m_str = m_str.substr(m_counter);
	m_counter = 0;
	return jumps == numJumps;
}

string StringParser::getLine()
{
	string line = m_str.substr(0, m_str.find_first_of('\n'));
	jumpLines(1);
	return line;
}
