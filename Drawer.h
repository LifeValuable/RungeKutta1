#pragma once

#include <afxwin.h>
#include <vector>
#include <algorithm>
//#include <iostream>
#include <stdlib.h>
using namespace std;

class Drawer
{
	// ������������� ������� ���������.
	CRect frame;
	// ��������� �� ������ ����, ������� ���������.
	CWnd* wnd;
	// �������� ���������, ����������� � ������� ���������.
	CDC* dc;
	// �������� ���������, ����������� � ������.
	CDC memDC;
	// ������ ��� ��������� ��������� memDC.
	CBitmap bmp;
	// ���� ��� ������������ ��������� ������������� ������.
	bool init;

public:


	// ������������������� ������ ������ �� ������ HWND.
	void Create(HWND hWnd)
	{
		// �������� ��������� �� ����.
		wnd = CWnd::FromHandle(hWnd);
		// �������� ������������� ����.
		wnd->GetClientRect(frame);
		// �������� �������� ��� ��������� � ���� ����.
		dc = wnd->GetDC();

		// ������� �����-��������.
		memDC.CreateCompatibleDC(dc);
		// ������� ����� ��� ��������� ���������.
		bmp.CreateCompatibleBitmap(dc, frame.Width(), frame.Height());
		// �������� ����� ��� ������������� �����-����������.
		memDC.SelectObject(&bmp);
		init = true;
	}


	//������� ��� ��������� �����

	void DrawGrid(vector<long double> y_first_graph, int typefunc, vector<long double> x_first_graph, int N, long double y_min, long double y_max)
	{
		if (!init) return;

		CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200)); //����� ��� ��������� ��������
		CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));		//����� ��� ��������� �����
		CFont font;										//������� �����
		font.CreateFontW(18, 0, 0, 0,
			FW_DONTCARE,
			FALSE,				// ������
			FALSE,				// ������������
			FALSE,				// �������������
			DEFAULT_CHARSET,	// ����� ��������
			OUT_OUTLINE_PRECIS,	// �������� ������������.	
			CLIP_DEFAULT_PRECIS,//  
			100,	// ��������
			VARIABLE_PITCH,		//
			TEXT("Times New Roman")		//
		);
		//���������� ��� �������
		double padding = 30;
		double left_keys_padding = 20;
		double bottom_keys_padding = 10;

		double actual_width = frame.Width() - 2 * padding - left_keys_padding;
		double actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

		double actual_top = padding;
		double actual_bottom = actual_top + actual_height;
		double actual_left = padding + left_keys_padding;
		double actual_right = actual_left + actual_width;

		// ����� ���.
		memDC.FillSolidRect(frame, RGB(255, 255, 255));

		// ������ ����� � ��������.
		double grid_size = 10;

		memDC.SelectObject(&subgrid_pen);

		for (double i = 0.5; i < grid_size; i += 1.0)
		{
			memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
			memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
			memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
			memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
		}

		memDC.SelectObject(&grid_pen);

		for (double i = 0.0; i < grid_size + 1; i += 1.0)
		{
			memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
			memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
			memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
			memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
		}

		//��� ������ ������ ���������, �� ��������� "���������� �����" �������
		//������� ��������� ������� ������ �������

		long double data_y_min(0), data_y_max(0);
		long double data_x_min(0), data_x_max(0);

		data_y_min = y_min;
		data_y_max = y_max;
		data_x_min = x_first_graph[0];
		data_x_max = x_first_graph[N - 1];

		//������������ ������ ��� ����������� �������������
		vector<long double> y = convert_range(y_first_graph, actual_top, actual_bottom, data_y_max, data_y_min);
		vector<long double> x = convert_range(x_first_graph, actual_right, actual_left, data_x_max, data_x_min);

		memDC.SelectObject(&font);
		memDC.SetTextColor(RGB(0, 0, 0));

		//����������� �������� �� �����
		switch (typefunc) {
		case 1:
			for (int i = 0; i < grid_size / 2 + 1; i++)
			{
				CString str;
				str.Format(L"%.4f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
				memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

				str.Format(L"%.1f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
				memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
			}
			break;
		case 2:
			for (int i = 0; i < grid_size / 2 + 1; i++)
			{
				CString str;
				str.Format(L"%.4f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
				memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

				str.Format(L"%.1f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
				memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
			}
			break;
		case 3:
			for (int i = 0; i < grid_size / 2 + 1; i++)
			{
				CString str;
				str.Format(L"%.4f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
				memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

				str.Format(L"%.e", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
				memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
			}
			break;
		}
		


		dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
	}
	
	//������� ��������� ��������
	void DrawPlot(vector<long double>& y_first_graph, int typefunc, vector<long double>& x_first_graph, int N, long double y_min, long double y_max)
	{
		if (!init) return;

		if (y_first_graph.empty()) return;

		/*if (x_first_graph.size() != y_first_graph.size())
		{
			x_first_graph.resize(y_first_graph.size());
			for (int i = 0; i < x_first_graph.size(); i++)
			{
				x_first_graph[i] = i;
			}
		}*/

		//����� ��� ������� ��������
		CPen RedPen(PS_SOLID, 2, RGB(255, 0, 0));
		CPen BluePen(PS_SOLID, 2, RGB(0, 0, 255));

		memDC.SelectObject(&BluePen);

		switch (typefunc) {
		case 2:
			memDC.SelectObject(&RedPen);
		}

		//���������� ��� ���������� ���������
		double padding = 30;
		double left_keys_padding = 20;
		double bottom_keys_padding = 10;

		double actual_width = frame.Width() - 2 * padding - left_keys_padding;
		double actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

		double actual_top = padding;
		double actual_bottom = actual_top + actual_height;
		double actual_left = padding + left_keys_padding;
		double actual_right = actual_left + actual_width;

		long double data_y_min(0), data_y_max(0);
		long double data_x_min(0), data_x_max(0);
		
		data_y_min = y_min;
		data_y_max = y_max;
		data_x_min = x_first_graph[0];
		data_x_max = x_first_graph[N - 1];

		vector<long double> y = convert_range(y_first_graph, actual_top, actual_bottom, data_y_max, data_y_min);;
		vector<long double> x = convert_range(x_first_graph, actual_right, actual_left, data_x_max, data_x_min);;

		//������ ������
		for (int i = 0; i < N - 1; i++)
		{
			memDC.MoveTo(x[i], y[i]);
			memDC.LineTo(x[i + 1], y[i + 1]);
		}

		dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);

	}

	//������� ����������� ������
	vector<long double> convert_range(vector <long double>& data, long double outmax, long double outmin, long double inmax, long double inmin)
	{
		vector<long double> output = data;
		long double k = (outmax - outmin) / (inmax - inmin);
		for (auto& item : output)
		{
			item = (item - inmin) * k + outmin;
		}

		return output;
	}
};

