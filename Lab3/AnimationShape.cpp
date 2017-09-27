#include "stdafx.h"
#include "AnimationShape.h"

AnimationShape::AnimationShape()
{
}

AnimationShape::AnimationShape(std::vector<POINT> vertexes) : PolygonShape(vertexes)
{
}


AnimationShape::~AnimationShape()
{
	finishEvent.SetEvent();
	WaitForSingleObject(m_pThread, INFINITE);
}

void AnimationShape::Draw(CDC *dc, std::vector<Shape*> shapes, CRgn *region, int penWidth)
{	
	if (isDrawingShape && repaintCount > 0)
	{
		InterlockedDecrement(&repaintCount);
		mutex.Lock();

		CBitmap *bitmap = NULL;
		CRect shapeRect = GetShapeRect(), rect;
		GetClientRect(hWnd, &rect);
		if ((bitmap = dc->GetCurrentBitmap()) != nullptr)
		{
			if (shapeRect.right > rect.right)
				isRightMotion = false;
			if (shapeRect.left < 0)
				isRightMotion = true;
			OffsetCoordinates({ isRightMotion ? 5 : -5 , 0 });
			PolygonShape::Draw(dc, shapes, region, 2);
		}

		mutex.Unlock();
	}
}

bool AnimationShape::ChangeDrawingState(CWnd* cWnd)
{
	isDrawingShape = !isDrawingShape;
	if (isDrawingShape)
	{
		if (m_pThread == NULL)
		{
			HWND hWnd = cWnd->GetSafeHwnd();
			if(hWnd != NULL)
				m_pThread = AfxBeginThread(StartThread<AnimationShape>, new ThreadData<AnimationShape>(hWnd, this));
		}
	}
	cEvent.SetEvent();
	return isDrawingShape;
}

bool AnimationShape::CheckPointInRegion(POINT point)
{
	return isDrawingShape && region.PtInRegion(point);
}

UINT AnimationShape::ThreadOffsetShape(HWND hWnd)
{
	bool isDrawing = false, currentDrawingState;
	CBitmap *bitmap = NULL;
	CWnd* cWnd = CWnd::FromHandle(hWnd);
	while (WaitForSingleObject(finishEvent, 10) != WAIT_OBJECT_0)
	{
		if (WaitForSingleObject(cEvent, 0) == WAIT_OBJECT_0)
			isDrawing = !isDrawing;
		if (currentDrawingState = isDrawing) {
			mutex.Lock();
			CRect rect;
			this->hWnd = hWnd;
			GetClientRect(hWnd, &rect);
			InterlockedIncrement(&repaintCount);
			InvalidateRect(hWnd, &rect, false);
			mutex.Unlock();
		}
		currentDrawingState = false;
	}
	if(bitmap != NULL)
		bitmap->DeleteObject();
	return 0;
}

template<class T>
UINT _cdecl AnimationShape::StartThread(LPVOID pParam)
{
	ThreadData<T>* thData = (ThreadData<T>*)pParam;
	
	UINT result = thData->instance->ThreadOffsetShape(thData->hWnd);
	delete thData;
	return result;
}