
// RungeKuttaDlg.h: файл заголовка
//

#pragma once
#include <vector> //поключаем контейнер 
#include "Drawer.h"
using namespace std;//используем стандартное пространство имен




// Диалоговое окно CRungeKuttaDlg
class CRungeKuttaDlg : public CDialogEx
{
// Создание
public:
	CRungeKuttaDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RUNGEKUTTA_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	CFont m_Font;
public:
	afx_msg void Solve(); //функция нажатия кнопки "Решить"
	double x_max; //максимальное значение X
	BOOL controlx; //контроль максимального значения X
	BOOL controleps; //контроль eps
	double h; //величина шага
	int n_max; //максимальное количество шагов
	int n; //количество шагов
	double eps; //погрешность
	CListBox list; //список для отображения значений
	Drawer U1; //объект класса drawer для отрисовки графика u1 (x)
	Drawer U2; //объект класса drawer для отрисовки графика u2 (x)
	Drawer U1U2; //объект класса drawer для отрисовки графика u1 (x), u2 (x)
	Drawer E1E2; //объект класса drawer для отрисовки графика E1,E2
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheckeps();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedHelp();
};

