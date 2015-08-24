/////////////////////////////////////////////////////////////////////////////
// Name:        GUI.cpp
// Encoding:	UTF-8
//
// Purpose:     Main Application GUI
//
// Author:      Erik Welander (mail@erikwelander.se)
// Modified:    2015-08-16
// Copyright:   Erik Welander
// Licence:     Creative Commons "by-nc-nd"
/////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#include "../headers/GUI.h"

// ----------------------------------------------------------------------------
//   Declarations
// ----------------------------------------------------------------------------
const wxString GUI::m_version = _(" V2015-08-16");
const wxString GUI::m_title = _("Proxxi Login Client");

const long GUI::ID_logoTop = wxNewId();
const long GUI::ID_logoBot = wxNewId();
const long GUI::ID_tfUsername = wxNewId();
const long GUI::ID_tfPassword = wxNewId();
const long GUI::ID_btLogin = wxNewId();
const long GUI::ID_btLogout = wxNewId();
const long GUI::ID_lblStatusText = wxNewId();
const long GUI::ID_lblInfoText = wxNewId();
const long GUI::ID_lblCopyright = wxNewId();
const long GUI::ID_menuAdminInterface = wxNewId();
const long GUI::ID_menuNewMember = wxNewId();
const long GUI::ID_menuAddMultiLogins = wxNewId();
const long GUI::ID_menuRemoveMultiLogins = wxNewId();
const long GUI::ID_menuConfig = wxNewId();
const long GUI::ID_menuClose = wxNewId();
const long GUI::ID_menuUserInfo = wxNewId();
const long GUI::ID_menuSystemInfo = wxNewId();
const long GUI::ID_menuAbout = wxNewId();
const long GUI::ID_TimerTicker = wxNewId();
const long GUI::ID_Panel = wxNewId();

BEGIN_EVENT_TABLE(GUI, wxFrame)
END_EVENT_TABLE()

GUI::GUI(wxWindow* parent, wxWindowID id)
{
	initVars();
	initGUI(parent, id);
	isUnicodeBuildCheck();
	#ifdef _WIN32
		isAlreadyRunningCheck();
	#endif
}


// ----------------------------------------------------------------------------
//   Private methods
// ----------------------------------------------------------------------------


void GUI::initVars()
{
	m_config = Configuration();
	if (!m_config.readConfiguration())
	{
		libWXHelper::displayMessage(this, _("Ingen konfigurationsfil hittad!\nAnvänder standard inställningar, dessa kan ändras i menyn."), m_title);
		m_config.generateDefaultConfiguration();
		m_config.readConfiguration();
	}
	m_auth = Authentication();

	#if defined _WIN32
		m_logoTop = libCon::stringToWxString(libSys::getCurrentDirectory()+"\\Images\\logoTop.png");
		m_logoBot = libCon::stringToWxString(libSys::getCurrentDirectory()+"\\Images\\logoBot.png");
	#elif __APPLE__
		m_logoTop = _("./Proxxi_Login_Client.app/Contents/Resources/logoTop.png");
		m_logoTop = _("./Proxxi_Login_Client.app/Contents/Resources/logoBot.png");
		m_windowIcon = _("./Proxxi_Login_Client.app/Contents/Resources/windowIcon.png");
	#else
		m_logoTop = _("./Images/logoTop.png");
		m_logoBot = _("./Images/logoBot.png");
		m_windowIcon = _("./Images/windowIcon.png");
	#endif
	m_statusText = _("");
	m_infoText = _("");

	m_userName = "";
	m_password = "";

	m_otherLogins = m_auth.getOtherLogins();

	m_connected = false;
	m_multiLoginFromClient = false;

	m_timerTicks = 0;
	m_timerRetryCountdown = 0;
	m_timerFails = 0;
	m_loginTime = 0;
	m_numOtherLogins = 0;
	m_drawCalls = 0;
}

void GUI::initGUI(wxWindow* parent, wxWindowID id)
{

	if (m_config.getProperty(CFG::ALLOW_CLOSE_BUTTON))
	{
		Create(parent, wxID_ANY, m_title + m_version, wxDefaultPosition, wxDefaultSize, wxMINIMIZE_BOX | wxCLOSE_BOX | wxSYSTEM_MENU | wxCAPTION | wxTAB_TRAVERSAL);
	}
	else
	{
		Create(parent, wxID_ANY, m_title + m_version, wxDefaultPosition, wxDefaultSize, wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxTAB_TRAVERSAL);
	}
	wxSize windowSize(346, 506);
	SetClientSize(windowSize);
	Center();
	SetFocus();

	#ifdef _WIN32
		SetIcon(wxICON(MYICON));
	#else
		wxIcon FrameIcon;
		FrameIcon.CopyFromBitmap(wxBitmap(wxImage(m_windowIcon)));
		SetIcon(FrameIcon);
	#endif
	
	wxString username = libSys::getUsername();

	panel = new wxPanel(this, ID_Panel);
	panel->SetBackgroundColour(wxColour(240, 240, 240));
	panel->SetSize(windowSize);

	m_bitmLogoTop = new wxStaticBitmap(panel, ID_logoTop, wxBitmap(wxImage(m_logoTop)));
	m_bitmLogoTop->SetPosition(wxPoint(16, 16));

	m_bitmLogoBot = new wxStaticBitmap(panel, ID_logoBot, wxBitmap(wxImage(m_logoBot)));
	m_bitmLogoBot->SetPosition(wxPoint(0, windowSize.GetY()-136));

	wxFont tfFont(10, wxSWISS, wxFONTSTYLE_NORMAL, wxNORMAL, false, _T("Sans"), wxFONTENCODING_DEFAULT);
	wxFont btFont(8, wxSWISS, wxFONTSTYLE_NORMAL, wxNORMAL, false, _T("Sans"), wxFONTENCODING_DEFAULT);
	wxFont lblStatusFont(18, wxSWISS, wxFONTSTYLE_NORMAL, wxBOLD, false, _T("Sans"), wxFONTENCODING_DEFAULT);
	wxFont lblInfoTextFont(11, wxSWISS, wxFONTSTYLE_NORMAL, wxNORMAL, false, _T("Sans"), wxFONTENCODING_DEFAULT);
	wxFont lblCopyrightFont(8, wxSWISS, wxFONTSTYLE_NORMAL, wxNORMAL, false, _T("Tahoma"), wxFONTENCODING_DEFAULT);

	m_tfUserName = new wxTextCtrl(panel, ID_tfUsername, username, wxPoint(72, 96), wxSize(200, 21), 0, wxDefaultValidator);
	m_tfUserName->SetFont(tfFont);

	m_tfPassword = new wxTextCtrl(panel, ID_tfPassword, wxEmptyString, wxPoint(72, 136), wxSize(200, 21), wxTE_PROCESS_ENTER | wxTE_PASSWORD, wxDefaultValidator);
	m_tfPassword->SetFont(tfFont);

	m_lblStatusText = new wxStaticText(panel, ID_lblStatusText, _("Vänligen logga in\n" + username), wxPoint(72, 168), wxSize(288, 48), 0);
	m_lblStatusText->SetFont(lblStatusFont);

	m_btLogin = new wxButton(panel, ID_btLogin, _("Logga in"), wxPoint(72, 256), wxDefaultSize, 0, wxDefaultValidator);
	m_btLogin->SetFont(btFont);
	m_btLogin->SetForegroundColour(wxColour(240, 240, 240));
	m_btLogin->SetBackgroundColour(wxColour(0, 72, 36));

	m_btLogout = new wxButton(panel, ID_btLogout, _("Logga ut"), wxPoint(200, 256), wxDefaultSize, 0, wxDefaultValidator);
	m_btLogout->SetFont(btFont);
	m_btLogout->SetForegroundColour(wxColour(240, 240, 240));
	m_btLogout->SetBackgroundColour(wxColour(117, 0, 3));
	m_btLogout->Disable();

	m_lblInfoText = new wxStaticText(panel, ID_lblInfoText, m_infoText, wxPoint(80, 288), wxDefaultSize, 0);
	m_lblInfoText->SetFont(lblInfoTextFont);

	m_lblCopyright = new wxStaticText(panel, ID_lblInfoText, _("© Erik Welander (Kira)"), wxPoint(windowSize.GetX() - 140, windowSize.GetY() - 40), wxDefaultSize, 0);
	m_lblCopyright->SetForegroundColour(wxColour(240, 240, 240));
	m_lblCopyright->SetBackgroundColour(wxColour(22, 112, 148));
	m_lblCopyright->SetFont(lblCopyrightFont);

	m_menuBar = new wxMenuBar();

	m_menuTools = new wxMenu();
	m_menuTools->Append(ID_menuAdminInterface, _("Administrationsgränssnitt"));
	m_menuTools->Append(ID_menuNewMember, _("Registrera ny medlem"));
	m_menuTools->Append(ID_menuAddMultiLogins, _("Aktivera multi-inlogg"));
	m_menuTools->Append(ID_menuRemoveMultiLogins, _("Deaktivera multi-inlogg"));
	m_menuTools->Append(ID_menuConfig, _("Ändra Konfiguration"));
	m_menuTools->Append(ID_menuClose, _("Avsluta\tAlt+F4"));
	m_menuBar->Append(m_menuTools, _("Verktyg"));

	m_menuInfo = new wxMenu();
	m_menuInfo->Append(ID_menuUserInfo, _("Användarinfo\tF3"));
	m_menuInfo->Append(ID_menuSystemInfo, _("Systeminfo\tF2"));
	m_menuInfo->Append(ID_menuAbout, _("Om\tF1"));
	m_menuBar->Append(m_menuInfo, _("Information"));

	SetMenuBar(m_menuBar);

	m_timerTicker.SetOwner(this, ID_TimerTicker);

	Connect(ID_tfPassword, wxEVT_COMMAND_TEXT_ENTER, (wxObjectEventFunction)&GUI::onTFPasswordEnter);
	Connect(ID_btLogin, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&GUI::onButtonLogin);
	Connect(ID_btLogout, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&GUI::onButtonLogout);
	Connect(ID_menuAdminInterface, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&GUI::onMenuAdminInterface);
	Connect(ID_menuNewMember, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&GUI::onMenuNewMember);
	Connect(ID_menuAddMultiLogins, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&GUI::onMenuAddMultiLogin);
	Connect(ID_menuRemoveMultiLogins, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&GUI::onMenuRemoveMultiLogin);
	Connect(ID_menuConfig, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&GUI::onMenuConfiguration);
	Connect(ID_menuClose, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&GUI::onMenuExit);
	Connect(ID_menuUserInfo, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&GUI::onMenuUserInfo);
	Connect(ID_menuSystemInfo, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&GUI::onMenuSystemInfo);
	Connect(ID_menuAbout, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&GUI::onMenuAbout);

	Connect(wxID_ANY, wxEVT_CLOSE_WINDOW, (wxObjectEventFunction)&GUI::onClose);
	Connect(ID_TimerTicker, wxEVT_TIMER, (wxObjectEventFunction)&GUI::onTimertick);

	updateNetworkInfo();
	updateInfoText();
}

void GUI::isUnicodeBuildCheck()
{
	#ifndef wxUSE_UNICODE
		wxCHECK_RET( wxUSE_UNICODE, "This build of WXWidgets does not support unicode!" );
		close();
	#endif
}

void GUI::isAlreadyRunningCheck()
{
	if (libSys::isApplicationAlreadyRunning())
	{
		libWXHelper::displayMessage(this, _("En annan instans utav klienten körs rendan!\nEndast en instans utav klienten får köras samtidigt."), m_title);
		close();
	}
}

void GUI::onTFPasswordEnter(wxCommandEvent& event)
{
	login();
}

void GUI::onButtonLogin(wxCommandEvent& event)
{
	login();
}

void GUI::onButtonLogout(wxCommandEvent& event)
{
	logout();
}

void GUI::onMenuAdminInterface(wxCommandEvent& event)
{
	openAdminInterface();
}

void GUI::onMenuNewMember(wxCommandEvent& event)
{
	openNewMember();
}

void GUI::onMenuAddMultiLogin(wxCommandEvent& event)
{
	openAddMultiLogin();
}

void GUI::onMenuRemoveMultiLogin(wxCommandEvent& event)
{
	openRemoveMultiLogin();
}

void GUI::onMenuConfiguration(wxCommandEvent& event)
{
	openConfiguration();
}

void GUI::onMenuExit(wxCommandEvent& event)
{
	close();
}
void GUI::onClose(wxCloseEvent& event)
{
	close();
}

void GUI::onMenuUserInfo(wxCommandEvent& event)
{
	openUserInfo();
}

void GUI::onMenuSystemInfo(wxCommandEvent& event)
{
	openSystemInfo();
}

void GUI::onMenuAbout(wxCommandEvent& event)
{
	openAbout();
}

void GUI::onTimertick(wxCommandEvent& event)
{
	timertick();
}

void GUI::close()
{
	if (m_timerTicker.IsRunning())
	{
		logout();
	}
	exit(0);
}


void GUI::openAdminInterface()
{
	libSys::openWebPage("https://bruse.proxxi.org");
}

void GUI::openNewMember()
{
	libSys::openWebPage("https://bruse.proxxi.org/create.php");
}

void GUI::openAddMultiLogin()
{
	if (m_connected)
	{
		Member member = m_auth.getMember();
		if (member.getRight(MEMBER_RIGHTS::MULTILOGG))
		{
			string passStr = libCon::wxStringToString(libWXHelper::askPasswordQuestion(this, _("För att kunna fortsätta så måste du identifiera dig själv\nSkriv in ditt lösenord för att låsa upp funktionen"), _("Lås upp")));
			if (m_password.compare(passStr) == 0)
			{
				string hostname = libCon::wxStringToString(libWXHelper::askQuestion(this, _("Lösenordet accepterades!\nAnge nu datornamnet eller IP addressen till enheten som du vill lägga till."), _("Ange adress")));
				if (m_auth.multiLoginComputer(hostname, m_password))
				{
					if (m_config.getProperty(CFG::DISPLAY_TRAY_MESSAGES))
					{
						libSys::notify(libCon::wxStringToString(m_title), "Internet access granted to host: " + hostname, "info", m_config.getProperty(CFG::TRAY_MESSAGES_TIME));
					}
					m_multiLoginFromClient = true;
					m_otherLogins = m_auth.getOtherLogins();
					m_numOtherLogins = m_auth.getNumOtherLogins();
				}
			}
			else
			{
				libWXHelper::displayMessage(this, _("Felaktigt lösenord!"), _("Låst"));
			}
		}
		else
		{
			libWXHelper::displayMessage(this, _("Din användare har inte rättigheter att ha flera samtida inloggningar!"), _("Saknad rättighet"));
		}
	}
	else
	{
		libWXHelper::displayMessage(this, _("Du måste logga in först!"), _("Logga in"));
	}
}
void GUI::openRemoveMultiLogin()
{
	if (m_connected)
	{
		if (m_auth.getOtherLogins() > 0)
		{
			string hostname = libCon::wxStringToString(libWXHelper::askQuestion(this, _("Ange nu datornamnet eller IP addressen till den enhet som du vill ta bort."), _("Ange adress")));
			if (m_auth.removeMultiLogin(hostname, m_password))
			{
				m_otherLogins = m_auth.getOtherLogins();
				m_numOtherLogins = m_auth.getNumOtherLogins();

				if (m_numOtherLogins == 0)
				{
					m_multiLoginFromClient = false;
				}

				if (m_config.getProperty(CFG::DISPLAY_TRAY_MESSAGES))
				{
					libSys::notify(libCon::wxStringToString(m_title), "Internet access to host: " + hostname + "has been revoked", "info", m_config.getProperty(CFG::TRAY_MESSAGES_TIME));
				}

			}
		}
		else
		{
			libWXHelper::displayMessage(this, _("Du har inga aktiva multi-logins!"), _("Låst"));
		}
	}
	else
	{
		libWXHelper::displayMessage(this, _("Du måste logga in först!"), _("Låst"));
	}
}

void GUI::openConfiguration()
{
	string toRun = "";
	#if defined _WIN32
		toRun = "@echo --Proxxi Login Client Configuration-- & @echo Configuration will be reloaded upon file close... & notepad \"" + m_config.getConfigName()+"\"";
	#elif defined __linux
		toRun = "gedit \"" + m_config.getConfigName() + "\"";
	#elif __APPLE__
		toRun = "open \"" + m_config.getConfigName() + "\"";
	#endif
	libSys::runCMD(toRun);
	m_config.readConfiguration();
}

void GUI::openUserInfo()
{
	if (m_connected)
	{
		Member member = m_auth.getMember();
		string text = "User infomation:";
		text += "\n\nFörnamn: \t" + member.getProperty(MEMBER_INFO::NAME_FIRST);
		text += "\n\nEfternamn: \t" + member.getProperty(MEMBER_INFO::NAME_LAST);
		text += "\n\nAktivt medlemskap: \t" + member.getProperty(MEMBER_INFO::MEM_ACTIVE);
		text += "\n\nTillåts i lokalen: \t" + member.getProperty(MEMBER_INFO::MEM_BANNED);
		text += "\n\nRättigheter: \t" + member.getProperty(MEMBER_INFO::RIGHTS);
		text += "\n\nAddress: \t" + member.getProperty(MEMBER_INFO::ADDRESS);
		text += "\n\nMail: \t" + member.getProperty(MEMBER_INFO::EMAIL);
		text += "\n\nPrimärnummer: \t" + member.getProperty(MEMBER_INFO::PHONE_MOBILE);
		text += "\n\nAlternativt nummer: \t" + member.getProperty(MEMBER_INFO::PHONE_ALT);

		libWXHelper::displayMessage(this, text, _("Medlemsinfo"));
	}
	else
	{
		libWXHelper::displayMessage(this, _("Du har inte loggat in än!"), _("Medlemsinfo"));
	}
}

void GUI::openSystemInfo()
{
	#ifdef _WIN32
		libSys::runSilentCMD("dxdiag /whql:off");
	#elif __APPLE__
		libSys::runSilentCMD("\"/Applications/Utilities/System Information.app/Contents/MacOS/System Information/System Information\"");
	#else
		libSys::runSilentCMD("gksu hwinfo");
	#endif
}

void GUI::openAbout()
{
	wxString msg = _("Proxxi Login Client (IX systems communicator)");
	msg += _("\nVersion: " + m_version);
	msg += _("\nSkapare: Erik Welander (Kira)");
	msg += _("\nKontakt:");
	msg += _("\nFacebook: https://www.facebook.com/erik.welander");
	msg += _("\nEmail: erik.welander@hotmail.com");
	msg += _("\nWeb: http://erikwelander.se/");
	msg += _("\nLicens: Creative Commons \"by-nc-nd\"");
	msg += _("\nAttribution (by), non-commersial. Consult me for changes (controlled development)");
	msg += _("\n\nSyfte: Detta program har jag skapat åt PROXXIS och IX medlemmar för att förbättra,");
	msg += _("\nunderlätta och säkra deras interaktion mot IX system.");
	msg += _("\nKlienten är intregerad med systemen och tillhandahåller");
	msg += _("\nmycket funktionalitet såväl som stabilitet och säkerhetsrutiner utöver bruses hemsida.");
	msg += _("\n\nKontakta mig ifall du stöter på några bugger eller har frågor.");

	libWXHelper::displayMessage(this, msg, m_title);
}
void GUI::timertick()
{
	++m_timerTicks;
	if (m_connected)
	{
		++m_loginTime;
	}
	else
	{
		if (m_timerRetryCountdown > 0)
		{
			--m_timerRetryCountdown;
		}
		//We are logged on, but the server did not reply to our session update request.
		//Indicate that we will attempt to remedy the situation.
		GUI::setGUILoginStatus(-2);
	}
	if (m_timerTicks >= m_config.getProperty(CFG::UPATE_FREQUENCY) && m_connected)
	{
		m_timerTicks = 0;
		if (m_auth.keepAliveRefresh())
		{
			m_timerRetryCountdown = 0;
			m_connected = true;
		}
		else
		{
			m_timerRetryCountdown = 10;
			++m_timerFails;
			m_connected = false;
			if (m_config.getProperty(CFG::DISPLAY_TRAY_MESSAGES))
			{
				libSys::notify(libCon::wxStringToString(m_title), "No reply from server! Retrying in 10 seconds...", "warn", m_config.getProperty(CFG::TRAY_MESSAGES_TIME));
			}
		}

		if (m_config.getProperty(CFG::DISPLAY_TRAY_MESSAGES))
		{
			if (m_numOtherLogins != m_auth.getNumOtherLogins())
			{
				string notify = "";
				if (m_auth.getNumOtherLogins() > 0)
				{
					notify = "Simultanious logins change";
				}
				else
				{
					notify = "Simultanious logins went offline";
				}
				libSys::notify(libCon::wxStringToString(m_title), notify, "info", m_config.getProperty(CFG::TRAY_MESSAGES_TIME));
			}
		}
		m_otherLogins = m_auth.getOtherLogins();
		m_numOtherLogins = m_auth.getNumOtherLogins();
	}
	else if (m_timerRetryCountdown < 1 && !m_connected)
	{
		m_timerTicks = 0;
		if (m_auth.keepAliveRefresh())
		{
			m_timerTicks = 0;
			m_timerRetryCountdown = 0;
			m_connected = true;
			GUI::setGUILoginStatus(m_auth.getStatusNum(LOGIN_REPLY::OK));
			if (m_config.getProperty(CFG::DISPLAY_TRAY_MESSAGES))
			{
				libSys::notify(libCon::wxStringToString(m_title), "Server session re-established", "info", m_config.getProperty(CFG::TRAY_MESSAGES_TIME));
			}
		}
		else
		{
			if (m_config.getProperty(CFG::DISPLAY_TRAY_MESSAGES))
			{
				libSys::notify(libCon::wxStringToString(m_title), "Server session dead, attempting re-logon", "warn", m_config.getProperty(CFG::TRAY_MESSAGES_TIME));
			}
			login();
			if (!m_connected)
			{
				m_timerRetryCountdown = 10;
			}
		}
	}
	updateInfoText();
}


void GUI::login()
{
	if (!m_timerTicker.IsRunning())
	{
		m_userName = m_tfUserName->GetLineText(0);
		m_password = m_tfPassword->GetLineText(0);
		m_loginTime = 0;
		m_timerTicks = 0;
		m_timerRetryCountdown = 0;
		m_timerFails = 0;
		m_multiLoginFromClient = false;
		for (int i = 0; i < m_auth.getMaxOtherLogins(); i++)
		{
			m_otherLogins[i] = "";
		}
		m_auth.logout();
	}
	if (m_password.compare("") != 0 && !m_connected)
	{
		m_lblStatusText->SetForegroundColour(wxColor(0,0,0));
		m_lblStatusText->SetLabel(_("Loggar in\n" +m_userName+"..."));

		m_timerTicks = 0;
		int statusCode = m_auth.login(m_userName, m_password);

		if (statusCode == m_auth.getStatusNum(LOGIN_REPLY::NO_REPLY_FROM_HOST))
		{
			if (m_timerTicker.IsRunning())
			{
				++m_timerFails;
				m_timerRetryCountdown = 10;
			}
			else
			{
				m_timerRetryCountdown = 0;
				m_timerFails = 0;
				setGUILoginStatus(statusCode);
			}
			m_connected = false;
		}

		else if (statusCode < m_auth.getStatusNum(LOGIN_REPLY::OK))
		{
			setGUILoginStatus(statusCode);
		}

		else if (statusCode == m_auth.getStatusNum(LOGIN_REPLY::OK))
		{
			m_timerTicker.Start(1000);
			m_connected = true;
			if (m_multiLoginFromClient)
			{
				for (int i = 0; i < m_auth.getMaxOtherLogins(); i++)
				{
					if (m_otherLogins[i].compare("") != 0)
					{
						m_auth.multiLoginComputer(m_otherLogins[i], m_password);
					}
				}
			}
			else
			{
				m_otherLogins = m_auth.getOtherLogins();
			}

			if (m_config.getProperty(CFG::MOUNT_NETWORK_SHARES))
			{
				libNetShares::mountNetworkShares();
				if (m_config.getProperty(CFG::PLACE_MOUNTS_ON_DESKTOP))
				{
					libNetShares::placeSharesOnDesktop();
				}
			}
			if (m_config.getProperty(CFG::DISPLAY_TRAY_MESSAGES))
			{
				libSys::notify(libCon::wxStringToString(m_title), "Authenticated", "info", m_config.getProperty(CFG::TRAY_MESSAGES_TIME));
			}
			m_numOtherLogins = m_auth.getNumOtherLogins();
			m_otherLogins = m_auth.getOtherLogins();
			setGUILoginStatus(statusCode);
		}

	}
}

void GUI::setGUILoginStatus(int statusCode)
{

	if (statusCode == -2)
	{
		m_lblStatusText->SetLabel(_("Varning: Inget svar.\nFörsöker igen om:\n" + libCon::intToString(m_timerRetryCountdown) + " sekunder..."));
		m_lblStatusText->SetForegroundColour(wxColour(255, 212, 42));
		m_btLogin->Enable();
		m_btLogout->Enable();

	}
	else if (statusCode == m_auth.getStatusNum(LOGIN_REPLY::NO_REPLY_FROM_HOST))
	{
		m_lblStatusText->SetLabel(_("FEL: Servern svarade\ninte på förfrågan..."));
		m_lblStatusText->SetForegroundColour(wxColour(255, 0, 0));
		wxString msg = _("Det gick inte att kontakta servern \"bruse\" för att validera inloggingen.\n");
		msg += _("Kontrollera att datorn är ansluten till nätverket, om detta problem kvarstår så kontakta mig eller någon av IXs medlemmar.");
		libWXHelper::displayMessage(this, msg, m_title);
	}
	else if (statusCode == m_auth.getStatusNum(LOGIN_REPLY::NOT_ACCEPTED))
	{
		m_lblStatusText->SetLabel(_("Fel:\nIcke godkänd inlogg"));
		m_lblStatusText->SetForegroundColour(wxColour(255, 0, 0));
		m_tfPassword->Clear();
		m_btLogin->Enable();
		m_btLogout->Disable();
		m_timerTicker.Stop();
	}

	else if (statusCode == m_auth.getStatusNum(LOGIN_REPLY::OK))
	{
		Member m = m_auth.getMember();
		wxString name = _(m.getProperty(MEMBER_INFO::NAME_FIRST) + " " + m.getProperty(MEMBER_INFO::NAME_LAST));
		wxString text = _("Inloggad som:\n" + name);
		m_lblStatusText->SetLabel(text);
		m_lblStatusText->SetForegroundColour(wxColour(36, 148, 69));
		m_btLogin->Disable();
		m_btLogout->Enable();
		m_tfUserName->Disable();
		m_tfPassword->Clear();
		m_tfPassword->Disable();
	}

	else if (statusCode == m_auth.getStatusNum(LOGIN_REPLY::EXPIRED_MEMBERSHIP))
	{
		m_lblStatusText->SetLabel(_("Utgånget\nmedlemskap"));
		m_lblStatusText->SetForegroundColour(wxColour(255, 212, 42));

		m_btLogin->Enable();
		m_btLogout->Disable();

		Member m = m_auth.getMember();
		wxString name = _(m.getProperty(MEMBER_INFO::NAME_FIRST) + " " + m.getProperty(MEMBER_INFO::NAME_LAST));
		wxString msg = _("Medlemskapet för " + name + " har utgått.\n ");
		msg += _("Medlemskapet för en användare måste förnyas en gång varje år för att nyttja PROXXI's/IX's resurser.\n");
		msg += _("Kontakta en nyckelbärare för att förnya ditt medlemskap!");
		libWXHelper::displayMessage(this, msg, _("Utgånget medlemskap"));
	}

	else if (statusCode == m_auth.getStatusNum(LOGIN_REPLY::INTERNET_REVOKED))
	{
		m_lblStatusText->SetLabel(_("Rättighet\nSaknas!"));
		m_lblStatusText->SetForegroundColour(wxColour(255, 212, 42));

		m_btLogin->Enable();
		m_btLogout->Disable();

		Member m = m_auth.getMember();
		wxString name = _(m.getProperty(MEMBER_INFO::NAME_FIRST) + " " + m.getProperty(MEMBER_INFO::NAME_LAST));
		wxString msg = _("Medlemmen  " + name + " saknar behörighet till internet och IX resurser.\n ");
		msg += _("Om du nyligen registrerat dig och får detta medelande så har nyckelbäraren antagligen glömt att ge dig behörighet.\n");
		msg += _("Medlemskapet för en användare måste förnyas en gång varje år för att nyttja PROXXI's/IX's resurser.\n");
		msg += _("Kontakta en nyckelbärare för att förnya ditt medlemskap!");
		libWXHelper::displayMessage(this, msg, _("Saknad behörighet"));
	}

	else if (statusCode == m_auth.getStatusNum(LOGIN_REPLY::BANNED_FROM_PROXXI))
	{
		Member m = m_auth.getMember();
		wxString name = _(m.getProperty(MEMBER_INFO::NAME_FIRST));
		m_lblStatusText->SetLabel(_(name + "\nBannlyst!?"));
		m_lblStatusText->SetForegroundColour(wxColour(255, 0, 0));

		wxString msg = _("Medlemmen  " + name + " saknar behörighet att vistas i lokalen!");
		msg += _("Om du stöter på detta medelande så har din behörighet dragits in på grund av brott mot relgerna\n");
		msg += _("Kontakta en nyckelbärare om du vill bestrida detta.");

		libWXHelper::displayMessage(this, msg, _("Banlyst!?"));
	}

}


void GUI::updateNetworkInfo()
{
	string* addressInfo = libSys::getIPv4HostAddressInfo();
	string text = "Datornamn: " + addressInfo[0];
	text += "\nIP: " + addressInfo[1];
	m_networkInfo = libCon::stringToWxString(text);
}

void GUI::updateInfoText()
{
	string text = "Inloggningstid: " + libCon::timeToString(m_loginTime)+"\n";
	if (m_timerFails > 0)
	{
		text += "Förfrågningar utan svar: " + libCon::intToString(m_timerFails) + "\n";
	}
	text += libCon::wxStringToString(m_networkInfo);
	if (m_numOtherLogins > 0)
	{
		if (m_multiLoginFromClient)
		{
			text += "\nMulti-inloggade enheter: \n";
		}
		else
		{
			text += "\nÄven inloggad på: \n";
		}

		for (int i = 0; i < m_auth.getMaxOtherLogins(); i++)
		{
			if (m_otherLogins[i].compare("") != 0)
			{
				text += ">" + m_otherLogins[i] + "\n";
			}
		}
	}
	m_lblInfoText->SetLabel(text);
}

void GUI::logout()
{
	m_timerTicker.Stop();
	Member member = m_auth.getMember();
	wxString realName = _(member.getProperty(MEMBER_INFO::NAME_FIRST) + " " + member.getProperty(MEMBER_INFO::NAME_LAST));
	if (realName.length() > 20)
	{
		realName = _(member.getProperty(MEMBER_INFO::NAME_FIRST));
	}
	m_lblStatusText->SetForegroundColour(wxColour(0, 0, 0));
	m_lblStatusText->SetLabel(_("Loggar ut \n" + realName + "..."));
	m_auth.logout();
	m_lblStatusText->SetLabel(realName + _("\nÄr utloggad..."));
	
	if (m_config.getProperty(CFG::DISPLAY_TRAY_MESSAGES))
	{
		string notifyString = "Disconnected. Session time: ";
		notifyString += libCon::timeToString(m_loginTime);
		if (m_config.getProperty(CFG::CLEAN_COMPUTER_ON_LOGOUT))
		{
			notifyString += ". Cleaning user profile...";
		}
		libSys::notify(libCon::wxStringToString(m_title), notifyString, "info", 5);
	}

	if (m_config.getProperty(CFG::CLEAN_COMPUTER_ON_LOGOUT))
	{
		#if defined _WIN32
			libSys::runSilentCMD("\"\"C:\\Program Files\\CCleaner\\CCleaner.exe\" /AUTO\" & del \"%appdata%\\.minecraft\\launcher_profiles.json\"");
		#endif
	}

	if (m_config.getProperty(CFG::MOUNT_NETWORK_SHARES))
	{
		libNetShares::disconnectNetworkShares();
	}

	m_connected = false;
	m_multiLoginFromClient = false;
	m_userName = "";
	m_password = "";

	m_timerTicks = 0;
	m_timerRetryCountdown = 0;
	m_timerFails = 0;
	m_loginTime = 0;
	m_numOtherLogins = 0;

	m_tfUserName->Clear();
	m_tfUserName->Enable();
	m_tfPassword->Clear();
	m_tfPassword->Enable();
	m_btLogin->Enable();
	m_btLogout->Disable();

	updateInfoText();
}
