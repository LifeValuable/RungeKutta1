
// RungeKutta.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CRungeKuttaApp:
// Сведения о реализации этого класса: RungeKutta.cpp
//

class CRungeKuttaApp : public CWinApp
{
public:
	CRungeKuttaApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CRungeKuttaApp theApp;
