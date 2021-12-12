// RHelpDlg.cpp: файл реализации
//

#include "pch.h"
#include "RungeKutta.h"
#include "RHelpDlg.h"
#include "afxdialogex.h"


// Диалоговое окно RHelpDlg

IMPLEMENT_DYNAMIC(RHelpDlg, CDialogEx)

RHelpDlg::RHelpDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HELP, pParent)
{

}

RHelpDlg::~RHelpDlg()
{
}

void RHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, ID_n, n);
	DDX_Text(pDX, ID_hmin, h_min);
	DDX_Text(pDX, ID_hmax, h_max);
	DDX_Text(pDX, ID_E1, E1);
	DDX_Text(pDX, ID_E2, E2);
}


BEGIN_MESSAGE_MAP(RHelpDlg, CDialogEx)
END_MESSAGE_MAP()


// Обработчики сообщений RHelpDlg
