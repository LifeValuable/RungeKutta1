
// RungeKuttaDlg.cpp: файл реализации
//

#include "pch.h"
#include <cmath>
#include <algorithm>
#include "framework.h"
#include "RungeKutta.h"
#include "RungeKuttaDlg.h"
#include "afxdialogex.h"
#include "RHelpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
public:

};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)

END_MESSAGE_MAP()


// Диалоговое окно CRungeKuttaDlg



CRungeKuttaDlg::CRungeKuttaDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RUNGEKUTTA_DIALOG, pParent)
	, n_max(5000)
	, h(0.001)
	, eps(0.0000000001)
	, x_max (0.05)
	, controlx (FALSE)
	, controleps (FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRungeKuttaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//функции считывания данных с экрана
	DDX_Text(pDX, IDC_h, h);
	DDX_Text(pDX, IDC_n, n_max);
	DDX_Text(pDX, IDC_eps, eps);
	DDX_Text(pDX, IDC_xmax, x_max);
	DDX_Control(pDX, IDC_LIST1, list);
}

BEGIN_MESSAGE_MAP(CRungeKuttaDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CRungeKuttaDlg::Solve)
	ON_BN_CLICKED(IDC_CHECK1, &CRungeKuttaDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECKEPS, &CRungeKuttaDlg::OnBnClickedCheckeps)
	ON_BN_CLICKED(IDCANCEL, &CRungeKuttaDlg::OnBnClickedCancel)
	ON_BN_CLICKED(ID_HELP, &CRungeKuttaDlg::OnBnClickedHelp)
END_MESSAGE_MAP()


// Обработчики сообщений CRungeKuttaDlg

BOOL CRungeKuttaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// редактирование шрифта текста
	m_Font.CreateFont(10, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, 0, 0, 0, 0, 0, L"Tahoma");
	GetDlgItem(IDC_1)->SetFont(&m_Font);
	GetDlgItem(IDC_2)->SetFont(&m_Font);
	GetDlgItem(IDC_3)->SetFont(&m_Font);
	GetDlgItem(IDC_4)->SetFont(&m_Font);
	GetDlgItem(IDC_5)->SetFont(&m_Font);
	GetDlgItem(IDC_6)->SetFont(&m_Font);
	GetDlgItem(IDC_8)->SetFont(&m_Font);
	GetDlgItem(IDC_9)->SetFont(&m_Font);

	U1.Create(GetDlgItem(IDC_u1)->GetSafeHwnd()); //передача hwnd для рисования графика
	U2.Create(GetDlgItem(IDC_u2)->GetSafeHwnd()); //передача hwnd для рисования графика
	U1U2.Create(GetDlgItem(IDC_u1u2)->GetSafeHwnd()); //передача hwnd для рисования графика
	E1E2.Create(GetDlgItem(IDC_E1E2)->GetSafeHwnd()); //передача hwnd для рисования графика

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CRungeKuttaDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CRungeKuttaDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CRungeKuttaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



vector <long double> u1; //контейнер хранения значений u1
vector <long double> u2; //контейнер хранения значений u2
vector <long double> x;  //контейнер хранения значений x
vector <long double> e1;  //контейнер хранения значений E1
vector <long double> e2;  //контейнер хранения значений E2


//точное решение системы дифференциальных уравнений
long double uu1(long double x) {
	return -3 * exp(-1000 * x) + 10 * exp(-0.01 * x);
}
long double uu2(long double x) {
	return 3 * exp(-1000 * x) + 10 * exp(-0.01 * x);
}

//функция метода Рунге Кутта с контролем eps
void RungeKuttawithcontrol(vector <long double>& u1, vector <long double>& u2, vector <long double>& x, double h, double eps, int&i) {
		if (x.size() == i+2) x[i + 1] = x[i] + h;
		else x.push_back(x[i] + h);
		long double a1,a2, k1u1, k2u1, k1u2, k2u2, a(-500.005), b(499.995), H((3 + sqrt(3)) * h / 6), sqrt1(-sqrt(3) / 3), sqrt2((3 + sqrt(3)) / 6);//коэффициенты метода и переменные для упрощения записи
		//коэффициенты получены решением системы линейных уравнений 
		
		k1u1 = -(a * a * H * u1[i] - a * u1[i] - b * b * H * u1[i] - b * u2[i]) / (a * a * H * H - 2 * a * H - b * b * H * H + 1);
		k1u2 = (-H * a * a * u2[i] + a * u2[i] + b * b * H * u2[i] + b * u1[i]) / (a * a * H * H - 2 * a * H - b * b * H * H + 1);
		k2u1 = -((-a * a * a * a * h * h * H * sqrt1 * sqrt2 * u1[i] + a * a * a * a * h * H * H * sqrt2 * u1[i] + a * a * h * h * a * sqrt1 * sqrt2 * u1[i] +
			a * a * a * h * H * sqrt1 * u1[i] - 2 * a * a * a * h * H * sqrt2 * u1[i] - a * a * a * H * H * u1[i] + 2 * a * b * h * a * b * h * H * sqrt1 * sqrt2 * u1[i] -
			2 * a * b * H * a * b * H * h * sqrt2 * u1[i] + a * a * b * h * h * sqrt1 * sqrt2 * u2[i] + a * a * b * h * H * sqrt1 * u2[i] - a * a * b * H * H * u2[i] -
			a * a * h * sqrt1 * u1[i] + a * a * h * sqrt2 * u1[i] + 2 * a * a * H * u1[i] - a * b * b * h * h * sqrt1 * sqrt2 * u1[i] - a * b * b * h * H * sqrt1 * u1[i] +
			2 * a * b * b * h * H * sqrt2 * u1[i] + a * b * b * H * H * u1[i] - 2 * a * b * h * sqrt1 * u2[i] + 2 * a * b * H * u2[i] - a * u1[i] -
			b * b * b * b * h * h * H * sqrt1 * sqrt2 * u1[i] + b * b * b * b * h * H * H * sqrt2 * u1[i] - b * b * b * h * h * sqrt1 * sqrt2 * u2[i] -
			b * b * b * h * H * sqrt1 * u2[i] + b * b * b * H * H * u2[i] - b * b * h * sqrt1 * u1[i] - b * b * h * sqrt2 * u1[i] - b * u2[i]) / 
			((a * a * H * H - 2 * a * H - b * b * H * H + 1) * (a * a * h * h * sqrt2 * sqrt2 - 2 * a * h * sqrt2 - b * b * h * h * sqrt2 * sqrt2 + 1)));
		k2u2 = -((-a * a * a * a * h * h * H * sqrt1 * sqrt2 * u2[i] + a * a * a * a * h * H * H * sqrt2 * u2[i] + a * a * h * h * a * sqrt1 * sqrt2 * u2[i] +
			a * a * a * h * H * sqrt1 * u2[i] - 2 * a * a * a * h * H * sqrt2 * u2[i] - a * a * a * H * H * u2[i] + 2 * a * b * h * a * b * h * H * sqrt1 * sqrt2 * u2[i] -
			2 * a * b * H * a * b * H * h * sqrt2 * u2[i] + a * a * b * h * h * sqrt1 * sqrt2 * u1[i] + a * a * b * h * H * sqrt1 * u1[i] - a * a * b * H * H * u1[i] -
			a * a * h * sqrt1 * u2[i] + a * a * h * sqrt2 * u2[i] + 2 * a * a * H * u2[i] - a * b * b * h * h * sqrt1 * sqrt2 * u2[i] - a * b * b * h * H * sqrt1 * u2[i] +
			2 * a * b * b * h * H * sqrt2 * u2[i] + a * b * b * H * H * u2[i] - 2 * a * b * h * sqrt1 * u1[i] + 2 * a * b * H * u1[i] - a * u2[i] -
			b * b * b * b * h * h * H * sqrt1 * sqrt2 * u2[i] + b * b * b * b * h * H * H * sqrt2 * u2[i] - b * b * b * h * h * sqrt1 * sqrt2 * u1[i] -
			b * b * b * h * H * sqrt1 * u1[i] + b * b * b * H * H * u1[i] - b * b *h * sqrt1 * u2[i] - b * b * h * sqrt2 * u2[i] - b * u1[i]) /
			((a * a * H * H - 2 * a * H - b * b * H * H + 1) * (a * a * h * h * sqrt2 * sqrt2 - 2 * a * h * sqrt2 - b * b * h * h * sqrt2 * sqrt2 + 1)));
		//записываем следующие значения u1
		if (u1.size() == i+2) u1[i + 1] = u1[i] + h * (k1u1 + k2u1) / 2;
		else u1.push_back(u1[i] + h * (k1u1 + k2u1) / 2);

		//записываем следующие значения u2
		if (u2.size() == i+2) u2[i + 1] = u2[i] + h * (k1u2 + k2u2) / 2; 
		else u2.push_back(u2[i] + h * (k1u2 + k2u2) / 2);

		//выполняем проверку на погрешность
		long double tempu1(u1[i]), tempu2(u2[i]);
		h /= 2;
		H /= 2;
		for (int j = 0; j < 2; j++) {
			k1u1 = -(-a * tempu1 + a * a * H * tempu1 - b * b * H * tempu1 - b * tempu2) / (1 - 2 * a * H + a * a * H * H - b * b * H * H); //рассчитываем коффициенты метода
			k1u2 = (b * tempu1 - a * a * H * tempu2 + b * b * H * tempu2 + a * tempu2) / (1 - 2 * a * H + a * a * H * H - b * b * H * H);
			k2u1 = -(a * h * k1u1 * sqrt1 + b * h * k1u2 * sqrt1 - a * a * h * h - k1u1 * sqrt1 * sqrt2 + b * b * h * h * k1u1 * sqrt1 * sqrt2 + a * tempu1 - a * a * h * sqrt2 * tempu1 + b * b * h * sqrt2 * tempu1 + b * tempu2) / (-1 + 2 * a * h * sqrt2 - a * a * h * h * sqrt2 * sqrt2 + b * b * h * h * sqrt2 * sqrt2);
			k2u2 = -(b * h * k1u1 * sqrt1 + a * h * k1u2 * sqrt1 - a * a * h * h - k1u2 * sqrt1 * sqrt2 + b * b * h * h * k1u2 * sqrt1 * sqrt2 + b * tempu1 + a * tempu2 - a * a * h * sqrt2 * tempu2 + b * b * h * sqrt2 * tempu2) / (-1 + 2 * a * h * sqrt2 - a * a * h * h * sqrt2 * sqrt2 + b * b * h * h * sqrt2 * sqrt2);
			tempu1 = tempu1 + h * (k1u1 + k2u1) / 2;
			tempu2 = tempu2 + h * (k1u2 + k2u2) / 2;
		}
		if (abs(tempu1 - u1[i + 1]) > eps || abs(tempu2 - u2[i + 1] > eps)) {
			RungeKuttawithcontrol(u1, u2, x, h, eps, i);
			i--;
		}
		h *= 4;
		H *= 4;
		
		i++;
}


//функция метода Рунге Кутта без контроля eps
void RungeKuttawithoutcontrol(vector <long double>& u1, vector <long double>& u2, vector <long double>& x, double h, int& i) {
	if (x.size() == i + 2) x[i + 1] = x[i] + h;
	else x.push_back(x[i] + h);
	long double a1, a2, k1u1, k2u1, k1u2, k2u2, a(-500.005), b(499.995), H((3 + sqrt(3)) * h / 6), sqrt1(-sqrt(3) / 3), sqrt2((3 + sqrt(3)) / 6);//коэффициенты метода и переменные для упрощения записи
	//коэффициенты получены решением системы линейных уравнений 

	k1u1 = -(a * a * H * u1[i] - a * u1[i] - b * b * H * u1[i] - b * u2[i]) / (a * a * H * H - 2 * a * H - b * b * H * H + 1);
	k1u2 = (-H * a * a * u2[i] + a * u2[i] + b * b * H * u2[i] + b * u1[i]) / (a * a * H * H - 2 * a * H - b * b * H * H + 1);
	k2u1 = -((-a * a * a * a * h * h * H * sqrt1 * sqrt2 * u1[i] + a * a * a * a * h * H * H * sqrt2 * u1[i] + a * a * h * h * a * sqrt1 * sqrt2 * u1[i] +
		a * a * a * h * H * sqrt1 * u1[i] - 2 * a * a * a * h * H * sqrt2 * u1[i] - a * a * a * H * H * u1[i] + 2 * a * b * h * a * b * h * H * sqrt1 * sqrt2 * u1[i] -
		2 * a * b * H * a * b * H * h * sqrt2 * u1[i] + a * a * b * h * h * sqrt1 * sqrt2 * u2[i] + a * a * b * h * H * sqrt1 * u2[i] - a * a * b * H * H * u2[i] -
		a * a * h * sqrt1 * u1[i] + a * a * h * sqrt2 * u1[i] + 2 * a * a * H * u1[i] - a * b * b * h * h * sqrt1 * sqrt2 * u1[i] - a * b * b * h * H * sqrt1 * u1[i] +
		2 * a * b * b * h * H * sqrt2 * u1[i] + a * b * b * H * H * u1[i] - 2 * a * b * h * sqrt1 * u2[i] + 2 * a * b * H * u2[i] - a * u1[i] -
		b * b * b * b * h * h * H * sqrt1 * sqrt2 * u1[i] + b * b * b * b * h * H * H * sqrt2 * u1[i] - b * b * b * h * h * sqrt1 * sqrt2 * u2[i] -
		b * b * b * h * H * sqrt1 * u2[i] + b * b * b * H * H * u2[i] - b * b * h * sqrt1 * u1[i] - b * b * h * sqrt2 * u1[i] - b * u2[i]) /
		((a * a * H * H - 2 * a * H - b * b * H * H + 1) * (a * a * h * h * sqrt2 * sqrt2 - 2 * a * h * sqrt2 - b * b * h * h * sqrt2 * sqrt2 + 1)));
	k2u2 = -((-a * a * a * a * h * h * H * sqrt1 * sqrt2 * u2[i] + a * a * a * a * h * H * H * sqrt2 * u2[i] + a * a * h * h * a * sqrt1 * sqrt2 * u2[i] +
		a * a * a * h * H * sqrt1 * u2[i] - 2 * a * a * a * h * H * sqrt2 * u2[i] - a * a * a * H * H * u2[i] + 2 * a * b * h * a * b * h * H * sqrt1 * sqrt2 * u2[i] -
		2 * a * b * H * a * b * H * h * sqrt2 * u2[i] + a * a * b * h * h * sqrt1 * sqrt2 * u1[i] + a * a * b * h * H * sqrt1 * u1[i] - a * a * b * H * H * u1[i] -
		a * a * h * sqrt1 * u2[i] + a * a * h * sqrt2 * u2[i] + 2 * a * a * H * u2[i] - a * b * b * h * h * sqrt1 * sqrt2 * u2[i] - a * b * b * h * H * sqrt1 * u2[i] +
		2 * a * b * b * h * H * sqrt2 * u2[i] + a * b * b * H * H * u2[i] - 2 * a * b * h * sqrt1 * u1[i] + 2 * a * b * H * u1[i] - a * u2[i] -
		b * b * b * b * h * h * H * sqrt1 * sqrt2 * u2[i] + b * b * b * b * h * H * H * sqrt2 * u2[i] - b * b * b * h * h * sqrt1 * sqrt2 * u1[i] -
		b * b * b * h * H * sqrt1 * u1[i] + b * b * b * H * H * u1[i] - b * b * h * sqrt1 * u2[i] - b * b * h * sqrt2 * u2[i] - b * u1[i]) /
		((a * a * H * H - 2 * a * H - b * b * H * H + 1) * (a * a * h * h * sqrt2 * sqrt2 - 2 * a * h * sqrt2 - b * b * h * h * sqrt2 * sqrt2 + 1)));
	//записываем следующие значения u1
	if (u1.size() == i + 2) u1[i + 1] = u1[i] + h * (k1u1 + k2u1) / 2;
	else u1.push_back(u1[i] + h * (k1u1 + k2u1) / 2);

	//записываем следующие значения u2
	if (u2.size() == i + 2) u2[i + 1] = u2[i] + h * (k1u2 + k2u2) / 2;
	else u2.push_back(u2[i] + h * (k1u2 + k2u2) / 2);
	i++;
}


//функция для рассчета E1,E2
void Getmistakes(vector <long double> u1, vector <long double> u2, vector <long double>& e1, vector <long double>& e2, vector <long double> x) {
	for (int i = 0; i < u1.size(); i++) {
		long double temp = u1[i] - uu1(x[i]);
		e1.push_back(temp);
	}
	for (int i = 0; i < u2.size(); i++) {
		long double temp = u2[i] - uu2(x[i]);
		e2.push_back(temp);
	}
}



void CRungeKuttaDlg::Solve()
{
	//очистка памяти
	u1.clear();
	u2.clear();
	x.clear();
	e1.clear();
	e2.clear();

	list.ResetContent(); //очистка лист
	UpdateData(TRUE); //запишем в память значения n и h, введеные с экрана
	n = n_max;
	//добавим в контейнеры начальные значения
	u1.push_back(7);
	u2.push_back(13);
	x.push_back(0);

	/*u1.reserve(n + 1);
	u2.reserve(n + 1);
	x.reserve(n + 1);*/
	//создаем цикл для метода Рунге-Кутты
	if (controleps == TRUE) {//проверка на контроль eps
		for (int i = 0; i < n;) {
			if (controlx == TRUE) { //проверка на контроль x max
				if (x[i] > x_max) {
					n = i;
					break;
				}
			}
			if (u2[i] <= u1[i]) {
				h = 1;
				RungeKuttawithoutcontrol(u1, u2, x, h, i);
			}
			else RungeKuttawithcontrol(u1, u2, x, h, eps, i);
			if (u2[i] <= 1e-10 && u1[i] <= 1e-10) {
				n = i;
				break;
			}
		}
	}
	else {
		for (int i = 0; i < n;) {
			if (controlx == TRUE) {
				if (x[i] > x_max && i != 0) {
					n = i;
					break;
				}
			}
			if (u2[i] <= u1[i]) h = 1;
			RungeKuttawithoutcontrol(u1, u2, x, h, i);
			if (u2[i] <= 1e-10 && u1[i] <= 1e-10) {
				n = i;
				break;
			}
		}
	}

	//рассчитаем ошибки
	Getmistakes(u1, u2, e1, e2, x);

	//цикл записи данных в listbox
	CString stroka(L"n");
	for (int i = 0; i <  (int)abs(log10(h)) + 5; i++) {
		stroka += " ";
	}
	stroka += "h  ";
	for (int i = 0; i < 2 * (int)abs(log10(x[1]-x[0])) + 5; i++) {
		stroka += " ";
	}
	stroka += "x  ";
	for (int i = 0; i < 1.5*(int)abs(log10(h*eps))+5; i++) {
		stroka += " ";
	}
	stroka += "u1  ";
	for (int i = 0; i < 2 * (int)abs(log10(eps)) + 7; i++) {
		stroka += " ";
	}
	stroka += "u2  ";
	for (int i = 0; i < (int)abs(log10(eps)) + 5; i++) {
		stroka += " ";
	}
	stroka += "E1 ";
	for (int i = 0; i < (int)abs(log10(h)) + 7; i++) {
		stroka += " ";
	}
	stroka += "E2 ";

	list.AddString(stroka);
	for (int i = 0; i <= n; i++) {
		CString Str(L""), temp; //в эти строки будем записывать данные
		//записываем в строку n 
		temp.Format(_T("%.*f"), 0, (double)i);
		Str += temp;
		Str += "    ";
		//записываем в строку h
		if (i != 0)temp.Format(_T("%.*f"), (int)abs(log10(x[i] - x[i-1])) + 2,  x[i] - x[i - 1]);
		else temp.Format(_T("%.*f"), (int)abs(log10(h))+1, h);
		Str += temp;
		Str += "    ";
		//записываем в строку x[i]
		if (i != 0)temp.Format(_T("%.*f"), (int)abs(log10(x[i] - x[i-1])) + 2, x[i]);
		else temp.Format(_T("%.*f"), (int)abs(log10(x[i+1] - x[i])) + 2, x[i]);
		Str += temp;
		Str += "    ";
		//записываем в строку u1[i]
		temp.Format(_T("%.*f"), 11, u1[i]);
		if (x[i] > 300) temp.Format(_T("%.e"), u1[i]);
		Str += temp;
		Str += "    ";
		//записываем в строку u2[i]
		temp.Format(_T("%.*f"), 11, u2[i]);
		if (x[i] > 300) temp.Format(_T("%.e"), u2[i]);
		Str += temp;
		Str += "    ";
		//записываем в строку eps1
		temp.Format(_T("%.e"), e1[i]);
		Str += temp;
		Str += "    ";
		//записываем в строку eps2
		temp.Format(_T("%.e"), e2[i]);
		Str += temp;
		Str += "    ";
		list.AddString(Str);
	}
	//задаем ограничения на y_max, y_min для корректной отрисовки графиков
	long double y_max, y_min;

	//рисуем сетку и графики
	y_max = *max_element(u1.begin(), u1.end());
	y_min = *min_element(u1.begin(), u1.end());
	U1.DrawGrid(u1, 1, x, n + 1, y_min, y_max);
	U1.DrawPlot(u1, 1, x, n + 1, y_min, y_max);

	y_max = *max_element(u2.begin(), u2.end());
	y_min = *min_element(u2.begin(), u2.end());
	U2.DrawGrid(u2, 2, x, n+1, y_min, y_max);
	U2.DrawPlot(u2, 2, x, n+1, y_min, y_max);

	y_max = *max_element(u2.begin(), u2.end());
	y_min = *min_element(u1.begin(), u1.end());
	U1U2.DrawGrid(u1, 1, x, n + 1, y_min, y_max);
	U1U2.DrawPlot(u1, 1, x, n + 1, y_min, y_max);
	U1U2.DrawPlot(u2, 2, x, n + 1, y_min, y_max);



	y_max = *max_element(e1.begin(), e1.end());
	y_min = *min_element(e2.begin(), e2.end());
	E1E2.DrawGrid(e1, 3, x, n + 1, y_min, y_max);
	E1E2.DrawPlot(e1, 3, x, n + 1, y_min, y_max);
	E1E2.DrawPlot(e2, 2, x, n + 1, y_min, y_max);

	
}

//контроль xmax
void CRungeKuttaDlg::OnBnClickedCheck1()
{
	if (controlx == FALSE) controlx = TRUE;
	else controlx = FALSE;
}

//контроль eps
void CRungeKuttaDlg::OnBnClickedCheckeps()
{
	if (controleps == FALSE) controleps = TRUE;
	else controleps = FALSE;
}


void CRungeKuttaDlg::OnBnClickedCancel()
{
	// TODO: добавьте свой код обработчика уведомлений
	CDialogEx::OnCancel();
}


void CRungeKuttaDlg::OnBnClickedHelp()
{
	RHelpDlg nDlg;
	if (n != 0) {
		nDlg.E1 = *max_element(e1.begin(), e1.end());
		nDlg.E2 = -*min_element(e2.begin(), e2.end());
		nDlg.h_max = h;
		nDlg.h_min = h;
		for (int i = 0; i < x.size()-1; i++) {
			if (x[i + 1] - x[i] > nDlg.h_max) nDlg.h_max = x[i + 1] - x[i];
			if (x[i + 1] - x[i] < nDlg.h_min) nDlg.h_min = x[i + 1] - x[i];
		}
		nDlg.n = n;
	}
	nDlg.DoModal();
	
}
