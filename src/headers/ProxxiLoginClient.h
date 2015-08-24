#ifndef PROXXI_LOGIN_CLIENT_MAIN
#define PROXXI_LOGIN_CLIENT_MAIN
/////////////////////////////////////////////////////////////////////////////
// Name:        ProxxiLoginClient.h
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
#include <wx/app.h>
#include <wx/image.h>

// ----------------------------------------------------------------------------
//   Declarations
// ----------------------------------------------------------------------------

class ProxxiLoginClient : public wxApp
{
public:
	// Overrides base class initialization virtuals
	// This is called during application startup and is a good place for application initialization
	virtual bool OnInit();
};

#endif
