/////////////////////////////////////////////////////////////////////////////
// Name:        libNetShares.cpp
// Encoding:	UTF-8
// Purpose:     Library for managing network shares for The Proxxi Login Client
// Author:      Erik Welander
// Modified:    2015-01-22
// Copyright:   Erik Welander
// Licence:     GNU GPL
/////////////////////////////////////////////////////////////////////////////


// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#include "../headers/libNetShares.h"

// ----------------------------------------------------------------------------
//   Declarations
// ----------------------------------------------------------------------------

void libNetShares::mountNetworkShares()
{
	#if defined _WIN32
		string skyrimMountCMD = "net use S: \\\\10.0.0.20\\Skyrim /P:No";
		string morrowindMountCMD = "net use M: \\\\10.0.0.20\\Morrowind /P:No";
		string neosNetworkShareCMD = "net use N: \\\\10.0.0.1\\NeosServer /P:No";
		libSys::runSilentCMD("echo Mounting network shares... & " + skyrimMountCMD + " & " + morrowindMountCMD + " & " + neosNetworkShareCMD + " & exit");
	#elif defined __linux
		string skyrimMountCLI = "gvfs-mount smb://10.0.0.20/Skyrim";
		string morrowindMountCLI = "gvfs-mount smb://10.0.0.20/Morrowind";
		string neosNetworkShareCLI = "gvfs-mount smb://10.0.0.1/NeosServer";
		libSys::runSilentCMD(skyrimMountCLI+" ; "+morrowindMountCLI+" ; "+neosNetworkShareCLI);
	#endif
}
void libNetShares::disconnectNetworkShares()
{
	#if defined _WIN32
		string unmountSkyrimCMD = "net use S: /D /Y";
		string unmountMorrowindCMD = "net use M: /D /Y";
		string unmountNeosServerCMD = "net use N: /D /Y";
		string removeShortCuts = "del /F /Q \"\"C:\\Users\\%USERNAME%\\Desktop\\Skyrim.lnk\"\"";
		removeShortCuts += " & del /F /Q \"\"C:\\Users\\%USERNAME%\\Desktop\\Morrowind.lnk\"\"";
		removeShortCuts += " & del /F /Q \"\"C:\\Users\\%USERNAME%\\Desktop\\NeosServer.lnk\"\"";
		libSys::runSilentCMD("echo Disconnecting network shares... & " + unmountSkyrimCMD + " & " + unmountMorrowindCMD + " & " + unmountNeosServerCMD + " & echo Removing shortcuts... & " + removeShortCuts);
	#elif defined __linux
		string unmountSkyrimCMD = "gvfs-mount -u smb://10.0.0.20/Skyrim";
		string unmountMorrowindCMD = "gvfs-mount -u smb://10.0.0.20/Morrowind";
		string unmountNeosServerCMD = "gvfs-mount -u smb://10.0.0.1/NeosServer";
		libSys::runSilentCMD(unmountSkyrimCMD + " ; " + unmountMorrowindCMD + " ; "+unmountNeosServerCMD);
	#endif
}

void libNetShares::placeSharesOnDesktop()
{
#if defined _WIN32
	string currentDirectory = libSys::getCurrentDirectory();
	string copyShortcutsCMD = "copy \"\"" + currentDirectory + "\\Shortcuts\\*.lnk\"\" \"C:\\Users\\%USERNAME%\\Desktop\\\" /Y";
	libSys::runSilentCMD("echo Copying shortcuts... & " + copyShortcutsCMD);
#endif
}