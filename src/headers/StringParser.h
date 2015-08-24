#ifndef StringParser_H
#define StringParser_H
/////////////////////////////////////////////////////////////////////////////
// Name:        StringParser.h
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
#include <string>

// ----------------------------------------------------------------------------
//   Declarations
// ----------------------------------------------------------------------------
using std::string;
class StringParser
{
public:
	StringParser();
	StringParser(const string& str);
	void assign(const string& str);
	void clear();
	const bool jumpLines(const int numJumps);
	const bool hasNext();
	string getLine();

private:
	string m_str;
	size_t m_counter;
};
#endif