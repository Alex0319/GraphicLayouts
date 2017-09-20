
// MainDlg.h : файл заголовка
//

#pragma once
#include "ShapesManager.h"
#include "ImagePainter.h"

// диалоговое окно CMainDlg
class CMainDlg : public CDialogEx
{
// Создание
public:
	CMainDlg(CWnd* pParent = NULL);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LAB3_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
//	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
//	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnStnClickedDrawingArea();
	afx_msg void OnAbout();
private:
	CStatic m_PictureControl;

	bool isNothingSelect = true;
	ShapesManager* shapesManager;
	std::vector<POINT> vertexes;
	CPoint *currentPoint, *prevPoint;

	void Draw();
	void DeletePoint(CPoint**);
	void ChangeMenuItemCaption(int menuItemId, int stringId);

public:
	afx_msg void OnDestroy();
	afx_msg void OnAddShape();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
