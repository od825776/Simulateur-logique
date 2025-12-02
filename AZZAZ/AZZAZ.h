
// AZZAZ.h : fichier d'en-tête principal de l'application PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "incluez 'pch.h' avant d'inclure ce fichier pour PCH"
#endif

#include "resource.h"		// symboles principaux


// CAZZAZApp :
// Consultez AZZAZ.cpp pour l'implémentation de cette classe
//

class CAZZAZApp : public CWinApp
{
public:
	CAZZAZApp();

// Substitutions
public:
	virtual BOOL InitInstance();

// Implémentation

	DECLARE_MESSAGE_MAP()
};

extern CAZZAZApp theApp;
