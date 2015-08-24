/////////////////////////////////////////////////////////////////////////////
// Name:        libWXHelper.h
// Encoding:	UTF-8
// Purpose:     Various UI helper methonds independent from main application
// Author:      Erik Welander
// Modified:    2015-01-20
// Copyright:   Erik Welander
// Licence:     GNU GPL
/////////////////////////////////////////////////////////////////////////////


// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#include <wx/msgdlg.h>
#include <wx/textdlg.h>
#include <wx/string.h>

// ----------------------------------------------------------------------------
//   Declarations
// ----------------------------------------------------------------------------
//#define wxICON_EXCLAMATION      0x00000100
//#define wxICON_HAND             0x00000200
//#define wxICON_WARNING          wxICON_EXCLAMATION
//#define wxICON_ERROR            wxICON_HAND
//#define wxICON_QUESTION         0x00000400
//#define wxICON_INFORMATION      0x00000800
//#define wxICON_STOP             wxICON_HAND
//#define wxICON_ASTERISK         wxICON_INFORMATION

namespace libWXHelper
{
	void displayMessage(wxWindow *parent, const wxString msg, const wxString title);
	const bool askYesNoQuestion(wxWindow *parent, const wxString msg, const wxString title);
	const wxString askQuestion(wxWindow *parent, const wxString msg, const wxString title);
	const wxString askPasswordQuestion(wxWindow *parent, const wxString msg, const wxString title);
}