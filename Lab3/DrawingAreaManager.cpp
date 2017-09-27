#include "stdafx.h"
#include "DrawingAreaManager.h"


DrawingAreaManager::DrawingAreaManager()
{
}

DrawingAreaManager::DrawingAreaManager(CRect* rect)
{
	const int parentWindowBorderIndent = borderIndent * 2;
	this->rect.SetRect(rect->left + parentWindowBorderIndent, rect->top + parentWindowBorderIndent, rect->right - parentWindowBorderIndent, rect->bottom - parentWindowBorderIndent);
	ChangeDrawingAreaRegions(&this->rect);
}

DrawingAreaManager::~DrawingAreaManager()
{
	region.DeleteObject();
	borderRgn.DeleteObject();
}

void DrawingAreaManager::ChangeDrawingAreaRegions(CRect *rect)
{
	CRgn internalRgn;
	borderRgn.DeleteObject();
	borderRgn.CreateRectRgn(rect->left - borderIndent, rect->top - borderIndent, rect->right + borderIndent, rect->bottom + borderIndent);
	internalRgn.CreateRectRgn(rect->left + borderIndent, rect->top + borderIndent, rect->right - borderIndent, rect->bottom - borderIndent);
	borderRgn.CombineRgn(&borderRgn, &internalRgn, RGN_XOR);
	internalRgn.DeleteObject();

	region.DeleteObject();
	region.CreateRectRgn(rect->left, rect->top, rect->right, rect->bottom);
}

CRgn* DrawingAreaManager::GetDrawingAreaRegion()
{
	return &region;
}

void DrawingAreaManager::ChangeCursorPosition(CPoint* point)
{
	HCURSOR hNewCursor = NULL;
	if(borderRgn.PtInRegion(*point))
		if (IsPointOnAreaBorder(point->x, rect.left) || IsPointOnAreaBorder(point->x, rect.right))
			hNewCursor = LoadCursorW(NULL, IDC_SIZEWE);
		else 
			if (IsPointOnAreaBorder(point->y, rect.top) || IsPointOnAreaBorder(point->y, rect.bottom))
				hNewCursor = LoadCursorW(NULL, IDC_SIZENS);
	if (hNewCursor != NULL) 
		oldCursor = SetCursor(hNewCursor);
	else
	{
		if (oldCursor != NULL) {
			hNewCursor = SetCursor(oldCursor);
			DeleteObject(hNewCursor);
			oldCursor = NULL;
		}
	}
}

bool DrawingAreaManager::IsPointOnAreaBorder(int pointCoordinate, int border)
{
	return pointCoordinate >= border - borderIndent && pointCoordinate <= border + borderIndent;
}

bool DrawingAreaManager::IsSelectedDrawingArea(CPoint* point)
{
	if(isDrawingAreaSelected = borderRgn.PtInRegion(*point))
		startPoint = *point;
	return isDrawingAreaSelected;
}

void DrawingAreaManager::SwapRectBounds(long* first, long* second)
{
	if (*first >= *second)
	{
		int temp = *first;
		*first = *second;
		*second = temp;
	}
}

bool DrawingAreaManager::CheckDrawingAreaSelection(CPoint* point)
{
	if (isDrawingAreaSelected)
	{
		CRect currentRect = rect;
		if (IsPointOnAreaBorder(startPoint.x, rect.left))
			currentRect.SetRect(point->x, rect.top, rect.right, rect.bottom);
		if (IsPointOnAreaBorder(startPoint.x, rect.right))
			currentRect.SetRect(rect.left, rect.top, point->x, rect.bottom);
		if (IsPointOnAreaBorder(startPoint.y, rect.top))
			currentRect.SetRect(rect.left, point->y, rect.right, rect.bottom);
		if (IsPointOnAreaBorder(startPoint.y, rect.bottom))
			currentRect.SetRect(rect.left, rect.top, rect.right, point->y);
		SwapRectBounds(&currentRect.left, &currentRect.right);
		SwapRectBounds(&currentRect.top, &currentRect.bottom);
		rect.SetRect(currentRect.TopLeft(), currentRect.BottomRight());
		startPoint = *point;
		ChangeDrawingAreaRegions(&rect);
	}
	return isDrawingAreaSelected;
}

void DrawingAreaManager::DeselectDrawingArea()
{
	isDrawingAreaSelected = false;
}

void DrawingAreaManager::DrawArea(CDC *dc)
{
	CPen pen(PS_SOLID, 2, RGB(0,0,0));
	CPen* oldPen = dc->SelectObject(&pen);
	dc->Rectangle(&rect);
	dc->SelectObject(oldPen);
	pen.DeleteObject();
}
