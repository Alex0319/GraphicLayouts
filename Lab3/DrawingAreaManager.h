#pragma once
class DrawingAreaManager
{
	public:
		DrawingAreaManager();
		DrawingAreaManager(CRect*);
		~DrawingAreaManager();
		CRgn* GetDrawingAreaRegion();
		bool IsSelectedDrawingArea(CPoint*);
		void ChangeCursorPosition(CPoint*);
		bool CheckDrawingAreaSelection(CPoint*);
		void DeselectDrawingArea();
		void DrawArea(CDC*);
	private:
		const int borderIndent = 5;

		CRgn region, borderRgn;
		CRect rect;
		CPoint startPoint;
		HCURSOR oldCursor = NULL;
		bool isDrawingAreaSelected = false;
		bool IsPointOnAreaBorder(int, int);
		void ChangeDrawingAreaRegions(CRect*);
		void DrawingAreaManager::SwapRectBounds(long*, long*);
};

