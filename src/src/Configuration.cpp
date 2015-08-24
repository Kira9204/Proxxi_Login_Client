/////////////////////////////////////////////////////////////////////////////
// Name:        Configuration.cpp
// Encoding:	UTF-8
// Purpose:     Configuration settings management and storage
// Author:      Erik Welander
// Modified:    2014-08-11
// Copyright:   Erik Welander
// Licence:     GNU GPL
/////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#include "../headers/Configuration.h"

// ----------------------------------------------------------------------------
//   Declarations
// ----------------------------------------------------------------------------

Configuration::Configuration()
{
	m_cfgName = "PLC_cfg.ini";
	m_cfgArr = new int[CFG::NUM_ELEMENTS];
	for (int i = 0; i < CFG::NUM_ELEMENTS; i++)
	{
		m_cfgArr[i] = 1;
	}
}

const bool Configuration::readConfiguration()
{
	ifstream reader(m_cfgName.c_str());
	if(reader)
	{
		for (int i = 0; i < CFG::NUM_ELEMENTS;)
		{
			string line = "";
			std::getline(reader, line);
			if (line.substr(0, 1).compare("#") != 0)
			{
				line = line.substr(line.find("=") + 2);
				m_cfgArr[i] = libCon::stringToInt(line);
				i++;
			}

		}
		if (m_cfgArr[CFG::UPATE_FREQUENCY] < 10)
		{
			m_cfgArr[CFG::UPATE_FREQUENCY] = 10;
		}

		//PILS session timeout is at 62 sconds, any value higher than 31 will cause problems.
		if (m_cfgArr[CFG::UPATE_FREQUENCY] > 31)
		{
			m_cfgArr[CFG::UPATE_FREQUENCY] = 31;
		}
		return true;
	}
	return false;
}

int Configuration::getProperty(enum CFG prop)
{
	return m_cfgArr[prop];
}

const string Configuration::getConfigName()
{
	return m_cfgName;
}

void Configuration::generateDefaultConfiguration()
{
	ofstream writer(m_cfgName.c_str());
	writer << "MountNetworkShares = 1\r\n";
	writer << "NetworkSharesOnDesktop = 1\r\n";
	writer << "CloseButtonEnabled = 1\r\n";
	writer << "SystemTrayMessages = 1\r\n";
	writer << "SystemTrayMessagesTime = 5\r\n";
	writer << "CleanOnLogout = 0\r\n";
	writer << "NetworkRefreshTime = 10\r\n";
	writer << "RequireValidCert = 0\r\n";
	writer.close();
}