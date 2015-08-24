/////////////////////////////////////////////////////////////////////////////
// Name:        libSys.cpp
// Encoding:	UTF-8
// Purpose:     OS independent system calls library
// Author:      Erik Welander
// Modified:    2015-01-21
// Copyright:   Erik Welander
// Licence:     Creative Commons "by-nc-nd"
/////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#include "../headers/libSys.h"

// ----------------------------------------------------------------------------
//   Declarations
// ----------------------------------------------------------------------------
using std::string;


const string libSys::getEnvVar(const string name)
{
	return string(getenv(name.c_str()));
}

const string libSys::getUsername()
{
	#if defined _WIN32
		return string(getenv("username"));
	#else
		return string(getenv("USER"));
	#endif
}

string libSys::getExePath()
{
	#if defined _WIN32
		char filename[MAX_PATH];
		GetModuleFileNameA(NULL, filename, MAX_PATH);
		return string(filename);
	#else
		return "";
	#endif
}

const string libSys::getExeName()
{
	#ifdef _WIN32
		string path = getExePath();
		return path.substr(path.find_last_of("\\") + 1);
	#else	//implement
		return "";
	#endif
}

const string libSys::getCurrentDirectory()
{
	#if defined _WIN32
		TCHAR path[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, path);
		#ifdef UNICODE
				std::wstring wstr(&path[0]);
				string ret(wstr.begin(), wstr.end());
		#else
				string ret(path);
		#endif
		return ret;
	#else
		return runAndCapture("pwd");
	#endif

}

string* libSys::getIPv4HostAddressInfo()
{
	#if defined _WIN32
		string captured = runAndCapture("ipconfig /all");
		string* addrArr = new string[2];
		//Parse the hostname
		addrArr[0] = captured.substr(captured.find("Host Name") + 36);
		addrArr[0] = addrArr[0].substr(0, addrArr[0].find('\n'));
		addrArr[0] = libCon::stringToLower(addrArr[0]);
		addrArr[0] = libCon::stringToUpper(addrArr[0], 0);
		//Parse the IP address
		//Since this is not garanteed to be set, i will use an if statement for safety
		size_t loc = captured.find("IPv4", 0);
		if (loc != string::npos)
		{
			addrArr[1] = captured.substr(loc + 36, 20);
			addrArr[1] = addrArr[1].substr(0, addrArr[1].find("("));
		}
		return addrArr;
	#else
		string* addrArr = new string[2];
		addrArr[0] = runAndCapture("hostname");
		addrArr[0] = addrArr[0].substr(0, addrArr[0].size() - 1);  //In order to get rid of the newline character
		addrArr[0] = libCon::stringToLower(addrArr[0]);
		addrArr[0] = libCon::stringToUpper(addrArr[0], 0);
		string captured = runAndCapture("ifconfig | grep 10.0");
		captured = captured.substr(captured.find("10.0"), 10);
		for (int i = 0; i < captured.size(); i++)
		{
			if (captured[i] != ' ')
			{
				addrArr[1] += captured[i];
			}
			else
			{
				break;
			}
		}
		return addrArr;
	#endif
}

bool libSys::isAlreadyRunning(const string proName)
{
	#if defined _WIN32
		string captured = runAndCapture("tasklist");
		size_t loc = captured.find(proName, 0);
		if (loc != string::npos)
		{
			if ((captured.find(proName, loc + proName.length())) != string::npos)
			{
				return true;
			}
		}
		return false;
	#else
		return false;
	#endif

}

bool libSys::isApplicationAlreadyRunning()
{
	return isAlreadyRunning(getExeName());
}

void libSys::runCMD(const string cmd)
{
	system(cmd.c_str());
}

void libSys::runSilentCMD(const string command)
{
	#if defined _WIN32
		ShellExecute(NULL, TEXT("open"), TEXT("cmd"), TEXT("/Q /C " + command), NULL, SW_HIDE);
	#else
		system(command.c_str());
	#endif
}

string libSys::runAndCapture(const string command)
{
	string captured = "";
	#if defined _WIN32
		FILE* pipe = _popen(command.c_str(), "r");
	#else
		FILE* pipe = popen(command.c_str(), "r");
	#endif
		if (!pipe)
		{
			return "";
		}
		char buffer[128];
		while (!feof(pipe))
		{
			if (fgets(buffer, 128, pipe) != NULL)
			{
				captured += buffer;
			}
		}
	#if defined _WIN32
		_pclose(pipe);
	#else
		pclose(pipe);
	#endif
		return captured;
}

void libSys::openWebPage(const string url)
{
	#if defined _WIN32
		ShellExecute(NULL, TEXT("open"), TEXT(""+url), NULL, NULL, SW_HIDE);
	#elif defined __APPLE__
		string toRun = "open " + url;
		system(toRun.c_str());
	#elif __linux
		string toRun = "sensible-browser " + url;
		system(toRun.c_str());
	#endif
		
}

void libSys::notify(const string title, const string message, const string type, int time)
{
	string currentDirectory = getCurrentDirectory();
	#if defined _WIN32
		runSilentCMD("\"\"" + currentDirectory + "\\Notifu\\notifu.exe\" /p \""+title+"\" /m \"" + message + "\" /d \""+libCon::intToString(time)+ "\" /i \"" + currentDirectory + "\\Images\\Icon.ico\" /t " + type + "\" /w");
	#elif defined __linux
	runSilentCMD("notify-send -i \""+currentDirectory+"/Images/Icon.png\""+ ""+title+"\"" + message + "\" -t "+libCon::intToString(time*1000));
	#endif
}