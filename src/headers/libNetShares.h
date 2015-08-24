/////////////////////////////////////////////////////////////////////////////
// Name:        libNetShares.h
// Encoding:	UTF-8
// Purpose:     Library for managing network shares for The Proxxi Login Client
// Author:      Erik Welander
// Modified:    2014-08-09
// Copyright:   Erik Welander
// Licence:     GNU GPL
/////////////////////////////////////////////////////////////////////////////


// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#include "libSys.h"

// ----------------------------------------------------------------------------
//   Declarations
// ----------------------------------------------------------------------------
using std::string;

namespace libNetShares
{
	void mountNetworkShares();
	void placeSharesOnDesktop();
	void disconnectNetworkShares();
	void removeSharesFromDesktop();
}