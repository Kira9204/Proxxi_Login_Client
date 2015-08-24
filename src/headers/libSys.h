/////////////////////////////////////////////////////////////////////////////
// Name:        libSys.h
// Encoding:	UTF-8
// Purpose:     OS independent system calls library
// Author:      Erik Welander
// Modified:    2015-01-22
// Copyright:   Erik Welander
// Licence:     Creative Commons "by-nc-nd"
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
#include <ctype.h>  //Used for char uppercase
#include "libCon.h"
// ----------------------------------------------------------------------------
//   Declarations
// ----------------------------------------------------------------------------
using std::string;
using std::size_t;
namespace libSys
{
	const string getEnvVar(const string name);
	const string getUsername();
	const string getExeName();
	const string getCurrentDirectory();
	string getExePath();

	//This API returns a string array with the following indexes:
	//string[0]: The hostname of the computer. The first char will be uppercase and the rest lowervase
	//string[1]: The IPv4 address of the computer.
	string* getIPv4HostAddressInfo(); 

	//Returns true of there is more then one instance on said program
	bool isAlreadyRunning(const string proName);
	bool isApplicationAlreadyRunning();
	void runCMD(const string cmd);
	void runSilentCMD(const string cmd);
	string runAndCapture(const string cmd);
	void openWebPage(const string url);

	//The following APi relies upon a separate program called "notifu"(http://www.paralint.com/projects/notifu/index.html)
	//Consider replacing this with "Shell_NotifyIcon"
	void notify(const string title, const string message, const string type, int time);
}