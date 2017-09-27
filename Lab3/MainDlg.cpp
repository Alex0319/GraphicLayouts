
// MainDlg.cpp : файл реализации
//

#include "stdafx.h"
#include "CMainApp.h"
#include "MainDlg.h"
#include "afxdialogex.h"

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
//	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_WM_LBUTTONUP()
//	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LAB3_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DRAWING_AREA, m_PictureControl);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
//	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
//	ON_WM_QUERYDRAGICON()
//	ON_STN_CLICKED(IDC_DRAWING_AREA, &CMainDlg::OnStnClickedDrawingArea)
ON_COMMAND(ID_ABOUT, &CMainDlg::OnAbout)
ON_WM_DESTROY()
ON_COMMAND(ID_ADDSHAPE, &CMainDlg::OnAddShape)
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
//ON_WM_KEYDOWN()
//ON_WM_SETCURSOR()
ON_COMMAND(ID_ADDANIMATIONSHAPE, &CMainDlg::OnAddAnimationShape)
END_MESSAGE_MAP()


// обработчики сообщений CMainDlg

BOOL CMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);


	ChangeMenuItemCaption(ID_ABOUT, IDS_ABOUTBOX);
	ChangeMenuItemCaption(ID_ADDSHAPE, IDS_ADDSHAPE);
	ChangeMenuItemCaption(ID_ADDANIMATIONSHAPE, IDS_ADDANIMATIONSHAPE);

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	CRect rect;
	GetClientRect(&rect);
	shapesManager = new ShapesManager(IDB_TURN);
	drawingAreaManager = new DrawingAreaManager(&rect);
	menuAnimationCaption.LoadStringW(IDS_REMOVEANIMATION);
	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CMainDlg::Draw()
{
	CPaintDC dc(&m_PictureControl);
	CDC compatibleDC;
	CRect rect;
	CBitmap bitmap;

	GetClientRect(rect);
	bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	compatibleDC.CreateCompatibleDC(&dc);
	CBitmap* oldBitmap = compatibleDC.SelectObject(&bitmap);

	compatibleDC.FillRect(rect, WHITE_BRUSH);
	drawingAreaManager->DrawArea(&compatibleDC);
	shapesManager->Draw(&compatibleDC, drawingAreaManager->GetDrawingAreaRegion());
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &compatibleDC, 0, 0, SRCCOPY);

	compatibleDC.SelectObject(oldBitmap);
	bitmap.DeleteObject();
	compatibleDC.DeleteDC();
	CDialogEx::OnPaint();
}

void CMainDlg::OnPaint()
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
		Draw();
}

void CMainDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (shapesManager->CheckSelectedShape(point)) {
		currentPoint = new CPoint(point);
		isNothingSelect = false;
		InvalidateRect(NULL, FALSE);
	}
	else
	{
		if (!isNothingSelect)
		{
			isNothingSelect = true;
			InvalidateRect(NULL, FALSE);
		}
		if(drawingAreaManager->IsSelectedDrawingArea(&point))
			InvalidateRect(FALSE, NULL);
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CMainDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (currentPoint != NULL || prevPoint != NULL) 
	{
		DeletePoint(&currentPoint);
		DeletePoint(&prevPoint);
		InvalidateRect(NULL, FALSE);
	}
	drawingAreaManager->DeselectDrawingArea();
	CDialogEx::OnLButtonUp(nFlags, point);
}

void CMainDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (currentPoint != NULL)
	{
		if (prevPoint != NULL)
			delete prevPoint;
		prevPoint = currentPoint;
		currentPoint = new CPoint(point);
		shapesManager->ChangeSelectedShapePosition({ currentPoint->x - prevPoint->x, currentPoint->y - prevPoint->y});
		InvalidateRect(FALSE, NULL);
	}
	else
	{
		drawingAreaManager->ChangeCursorPosition(&point);
		if(drawingAreaManager->CheckDrawingAreaSelection(&point))
			InvalidateRect(FALSE, NULL);
	}
	CDialogEx::OnMouseMove(nFlags, point);
}

BOOL CMainDlg::PreTranslateMessage(MSG* msg)
{
	if (msg->message == WM_KEYDOWN)
	{
		float zoom = 1;
		switch (msg->wParam)
		{
			case 0x45: zoom = 0.5;
				break;
			case 0x52: zoom = 2;
				break;
		}
		if (shapesManager->ChangeSelectedShapeZoom(zoom))
			InvalidateRect(NULL, FALSE);
	}
	return CDialog::PreTranslateMessage(msg);
}

void CMainDlg::OnAddShape()
{
	vertexes.push_back({ 80, 90 });
	vertexes.push_back({ 180, 90 });
	vertexes.push_back({ 180, 180 });
	vertexes.push_back({ 80, 180 });
	shapesManager->AddShape(vertexes);
	vertexes.clear();

	vertexes.push_back({ 10, 80 });
	vertexes.push_back({ 160, 20 });
	vertexes.push_back({ 150, 130 });
	shapesManager->AddShape(vertexes);
	vertexes.clear();
	InvalidateRect(NULL, false);
}

void CMainDlg::OnAddAnimationShape()
{
	vertexes.push_back({ 80, 90 });
	vertexes.push_back({ 180, 90 });
	vertexes.push_back({ 180, 180 });
	vertexes.push_back({ 80, 180 });
	shapesManager->CreateAnimationShape(vertexes);
	vertexes.clear();
	shapesManager->ChangeAnimationShapeDrawing(this);
	menuAnimationCaption = ChangeMenuItemCaption(ID_ADDANIMATIONSHAPE, menuAnimationCaption);
	InvalidateRect(NULL, false);
}

void CMainDlg::OnAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CMainDlg::OnDestroy()
{
	delete shapesManager;
	delete drawingAreaManager;
	vertexes.clear();
	std::vector<POINT>().swap(vertexes);
	CDialogEx::OnDestroy();
}

CString CMainDlg::ChangeMenuItemCaption(int menuItemId, CString str)
{
	CString result;
	GetMenu()->GetMenuStringW(ID_ADDANIMATIONSHAPE, result, MF_BYCOMMAND);
	GetMenu()->ModifyMenuW(menuItemId, MF_BYCOMMAND, menuItemId, str);
	return result;
}

CString CMainDlg::ChangeMenuItemCaption(int menuItemId, int strId)
{
	CString str;
	str.LoadStringW(strId);
	return ChangeMenuItemCaption(menuItemId, str);
}

void CMainDlg::DeletePoint(CPoint** point)
{
	delete *point;
	*point = NULL;
}