/////////////////////////////////////////////////////////////////////////////
// Name:        ProxxiLoginClient.cpp
// Encoding:	UTF-8
// Purpose:     Proxxi Login Client main application file
// Author:      Erik Welander
// Modified:    2015-01-04
// Copyright:   Erik Welander
// Licence:     GNU GPL
/////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#include "headers/ProxxiLoginClient.h"
#include "headers/GUI.h"
IMPLEMENT_APP(ProxxiLoginClient);

bool ProxxiLoginClient::OnInit()
{
	bool wxsOK = true;
	wxInitAllImageHandlers();
	if (wxsOK)
	{
		GUI* Frame = new GUI(0);
		Frame->Show();
		SetTopWindow(Frame);
	}
	return wxsOK;

}
