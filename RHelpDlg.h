#pragma once


// Диалоговое окно RHelpDlg

class RHelpDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RHelpDlg)

public:
	RHelpDlg(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~RHelpDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HELP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	
	DECLARE_MESSAGE_MAP()
public:

	int n;
	double h_max;
	double h_min;
	double E1;
	double E2;
	CString e1;
	CString e2;

};
