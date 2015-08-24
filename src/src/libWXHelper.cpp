/////////////////////////////////////////////////////////////////////////////
// Name:        libWXHelper.cpp
// Encoding:	UTF-8
// Purpose:     Various UI helper methonds independent from the main application
// Author:      Erik Welander
// Modified:    2015-01-20
// Copyright:   Erik Welander
// Licence:     GNU GPL
/////////////////////////////////////////////////////////////////////////////


// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#include "../headers/libWXHelper.h"

// ----------------------------------------------------------------------------
//   Declarations
// ----------------------------------------------------------------------------

void libWXHelper::displayMessage(wxWindow *parent, const wxString msg, const wxString title)
{
	wxMessageDialog dialog(parent, msg, title, wxOK | wxICON_INFORMATION);
	dialog.ShowModal();
}
const bool libWXHelper::askYesNoQuestion(wxWindow *parent, const wxString msg, const wxString title)
{
	wxMessageDialog dialog(parent, msg, title, wxYES_NO | wxICON_QUESTION);
	if (dialog.ShowModal() == wxID_YES)
	{
		return true;
	}
	return false;
}
const wxString libWXHelper::askQuestion(wxWindow *parent, const wxString msg, const wxString title)
{
	wxTextEntryDialog dialog(parent, msg, title, _(""), wxOK | wxICON_QUESTION);
	if (dialog.ShowModal() == wxID_OK)
	{
		return dialog.GetValue();
	}
	return _("");
}
const wxString libWXHelper::askPasswordQuestion(wxWindow *parent, const wxString msg, const wxString title)
{
	wxPasswordEntryDialog dialog(parent, msg, title, _(""));
	if (dialog.ShowModal() == wxID_OK)
	{
		return dialog.GetValue();
	}
	return _("");
}