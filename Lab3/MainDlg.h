
// MainDlg.h : ���� ���������
//

#pragma once
#include "ShapesManager.h"
#include "ImagePainter.h"
#include "DrawingAreaManager.h"

// ���������� ���� CMainDlg
class CMainDlg : public CDialogEx
{
	// ��������
	public:
		CMainDlg(CWnd* pParent = NULL);	// ����������� �����������

	// ������ ����������� ����
	#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_LAB3_DIALOG };
	#endif

		protected:
		virtual void DoDataExchange(CDataExchange* pDX);	// ��������� DDX/DDV


	// ����������
	protected:
		HICON m_hIcon;

		// ��������� ������� ����� ���������
		virtual BOOL OnInitDialog();
	//	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
		afx_msg void OnPaint();
		BOOL CMainDlg::PreTranslateMessage(MSG*);
		//	afx_msg HCURSOR OnQueryDragIcon();
		DECLARE_MESSAGE_MAP()
	public:
	//	afx_msg void OnStnClickedDrawingArea();
		afx_msg void OnAbout();
	private:
		CStatic m_PictureControl;

		bool isNothingSelect = true;
		ShapesManager* shapesManager;
		DrawingAreaManager* drawingAreaManager;
		std::vector<POINT> vertexes;
		CPoint *currentPoint, *prevPoint;
		CString menuAnimationCaption;

		void Draw();
		void DeletePoint(CPoint**);
		CString ChangeMenuItemCaption(int, CString);
		CString CMainDlg::ChangeMenuItemCaption(int, int);

	public:
		afx_msg void OnDestroy();
		afx_msg void OnAddShape();
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//		afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
		afx_msg void OnAddAnimationShape();
};
