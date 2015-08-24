#ifndef Configuration_H
#define Configuration_H
/////////////////////////////////////////////////////////////////////////////
// Name:        Configuration.h
// Encoding:	UTF-8
// Purpose:     Configuration settings management and storage
// Author:      Erik Welander
// Modified:    2014-08-10
// Copyright:   Erik Welander
// Licence:     GNU GPL
/////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#include <fstream>
#include <string>
#include "libSys.h"
#include "libCon.h"

// ----------------------------------------------------------------------------
//   Declarations
// ----------------------------------------------------------------------------
using std::string;
using std::ifstream;
using std::ofstream;

enum CFG
{
	MOUNT_NETWORK_SHARES,
	PLACE_MOUNTS_ON_DESKTOP,
	ALLOW_CLOSE_BUTTON,
	DISPLAY_TRAY_MESSAGES,
	TRAY_MESSAGES_TIME,
	CLEAN_COMPUTER_ON_LOGOUT,
	UPATE_FREQUENCY,
	REQURE_VALID_CERT,
	NUM_ELEMENTS
};

class Configuration
{
	public:
		Configuration();
		const bool readConfiguration();
		int getProperty(enum CFG prop);
		const string getConfigName();
		void generateDefaultConfiguration();
	private:
		string m_cfgName;
		int* m_cfgArr;
};

#endif