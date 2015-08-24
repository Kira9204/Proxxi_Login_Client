#ifndef PROXXI_LOGIN_CLIENT_GUI
#define PROXXI_LOGIN_CLIENT_GUI
/////////////////////////////////////////////////////////////////////////////
// Name:        GUI.h
// Encoding:	UTF-8
// Purpose:     Main Application GUI
// Author:      Erik Welander
// Modified:    2015-01-23
// Copyright:   Erik Welander
// Licence:     Creative Commons "by-nc-nd"
/////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
//==========WxWidgets segment=========
#include <wx/wx.h>

//==========Windows headers==========
#if defined _WIN32
#include <windows.h>
#include <Lmcons.h>
#else
//==========UNIX headers==========
#include <stdlib.h>
#endif

//==========Application headers===========
#include "Configuration.h"
#include "Authentication.h"
#include "Member.h"
#include "libWXHelper.h"
#include "libNetShares.h"

// ----------------------------------------------------------------------------
//   Declarations
// ----------------------------------------------------------------------------
using std::string;

class GUI : public wxFrame
{
	public:
		GUI(wxWindow* parent, wxWindowID id = -1);
	private:
		static const wxString m_title,
							  m_version;

		static const long ID_logoTop,
						  ID_logoBot,
						  ID_tfUsername,
						  ID_tfPassword,
						  ID_btLogin,
						  ID_btLogout,
						  ID_lblStatusText,
						  ID_lblInfoText,
						  ID_lblCopyright,
						  ID_menuAdminInterface,
						  ID_menuNewMember,
						  ID_menuAddMultiLogins,
						  ID_menuRemoveMultiLogins,
						  ID_menuConfig,
						  ID_menuClose,
						  ID_menuUserInfo,
						  ID_menuSystemInfo,
						  ID_menuAbout,
						  ID_TimerTicker,
						  ID_Panel;

		wxString m_logoTop,
				 m_logoBot,
				 m_statusText,
				 m_infoText,
				 m_networkInfo;

	#ifndef _WIN32
		wxString m_windowIcon;
	#endif
		string *m_otherLogins;
		string m_userName,
			   m_password;

		bool m_connected,
			m_multiLoginFromClient;

		int m_timerTicks,
			m_timerRetryCountdown,
			m_timerFails,
			m_loginTime,
			m_numOtherLogins,
			m_drawCalls;

		wxPanel *panel;
		wxStaticBitmap *m_bitmLogoTop,
					   *m_bitmLogoBot;
		wxTextCtrl  *m_tfUserName,
					*m_tfPassword;
		wxStaticText *m_lblStatusText,
					 *m_lblInfoText,
					 *m_lblCopyright;
		wxButton *m_btLogin,
				 *m_btLogout;
		wxMenuBar *m_menuBar;
		wxMenu *m_menuTools,
			   *m_menuInfo;
		wxTimer m_timerTicker;

		Configuration m_config;
		Authentication m_auth;

		void initVars();
		void initGUI(wxWindow* parent, wxWindowID id);

		void onButtonLogin(wxCommandEvent& event);
		void onButtonLogout(wxCommandEvent& event);
		void onTFPasswordEnter(wxCommandEvent& event);
		void onMenuAdminInterface(wxCommandEvent& event);
		void onMenuNewMember(wxCommandEvent& event);
		void onMenuAddMultiLogin(wxCommandEvent& event);
		void onMenuRemoveMultiLogin(wxCommandEvent& event);
		void onMenuConfiguration(wxCommandEvent& event);
		void onMenuExit(wxCommandEvent& event);
		void onMenuClose(wxCommandEvent& event);
		void onMenuUserInfo(wxCommandEvent& event);
		void onMenuSystemInfo(wxCommandEvent& event);
		void onMenuAbout(wxCommandEvent& event);
		void onUpdateTick(wxTimerEvent& event);
		void onTimertick(wxCommandEvent& event);
		void onClose(wxCloseEvent& event);
		void isUnicodeBuildCheck();
		void isAlreadyRunningCheck();
		void close();

		void openConfiguration();
		void openAdminInterface();
		void openUserInfo();
		void openSystemInfo();
		void openAbout();
		void openAddMultiLogin();
		void openRemoveMultiLogin();
		void openNewMember();

		void timertick();
		void login();
		void setGUILoginStatus(int statusCode);
		void updateNetworkInfo();
		void updateInfoText();
		void logout();
		DECLARE_EVENT_TABLE()
};

#endif