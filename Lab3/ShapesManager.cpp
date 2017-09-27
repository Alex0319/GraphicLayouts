#include "stdafx.h"
#include "ShapesManager.h"

ShapesManager::ShapesManager()
{
	srand(time(0));
}

ShapesManager::ShapesManager(int uResourceId)
{
	srand(time(0));
	shapeState = NOACTION;
	imagePainter = new ImagePainter();
	imagePainter->LoadImageFromResourceWithTransparency(uResourceId, RGB(0, 0, 0));
}

ShapesManager::~ShapesManager()
{
	ClearShapesVector();
	if(imagePainter != NULL)
		delete imagePainter;
}

void ShapesManager::Draw(CDC* dc, CRgn* drawingAreaRegion)
{
	if(selectedShape != NULL)
		selectedShape->Draw(dc, shapes, drawingAreaRegion, 3);
	for (auto it = shapes.begin(); it != shapes.end(); it++)
	{
		if ((*it) != selectedShape) 
			(*it) -> Draw(dc, shapes, drawingAreaRegion);
	}
	DrawTurnIcon(dc);
}

void ShapesManager::AddShape(std::vector<POINT> vertex)
{
	shapes.push_back(new PolygonShape(vertex));
}

void ShapesManager::CreateAnimationShape(std::vector<POINT> vertex)
{
	if (animationShape == NULL)
	{
		animationShape = new AnimationShape(vertex);
		shapes.push_back(animationShape);
	}
}

void ShapesManager::ChangeAnimationShapeDrawing(CWnd* cWnd)
{
	if(!animationShape->ChangeDrawingState(cWnd));
		selectedShape = NULL;
}

bool ShapesManager::CheckSelectedShape(POINT point)
{
	int maxLayout = 0;
	Shape* prevSelectedShape = selectedShape;
	selectedShape = NULL;
	for (auto it = shapes.begin(); it != shapes.end(); it++)
	{
		int currentLayout = (*it)->GetLayoutNumber();
		if ((*it)->CheckPointInRegion(point) && currentLayout >= maxLayout)
		{
			selectedShape = (*it);
			maxLayout = currentLayout;
		}
	}
	return ChangeShapeState(prevSelectedShape, point);
}

bool ShapesManager::ChangeShapeState(Shape *prevSelectedShape, POINT point)
{
	if (prevSelectedShape != NULL && imagePainter->IsPointInImageRegion(point))
	{
		selectedShape = prevSelectedShape;
		shapeState = ROTATION;
		return true;
	}
	else
	{
		if (selectedShape != NULL)
		{
			shapeState = MOTION;
			return true;
		}
		shapeState = NOACTION;
		return false;
	}
}

void ShapesManager::ChangeSelectedShapePosition(POINT point)
{
	switch (shapeState)
	{
		case MOTION: selectedShape->OffsetCoordinates(point);
			break;
		case ROTATION: selectedShape->TurnCoordinates(point);
			break;
	}
}

bool ShapesManager::ChangeSelectedShapeZoom(float zoom)
{
	if (selectedShape != NULL)
	{
		selectedShape->ChangeZoom(zoom);
		return true;
	}
	return false;
}

void ShapesManager::DrawTurnIcon(CDC* dc)
{
	if (selectedShape != NULL) {
		CDC imageDC;
		CBitmap bitmap;
		CRect imageRect = selectedShape->GetShapeRect();
		BITMAP imageInfo = imagePainter->GetImageInfo();
		CRect rect(0, 0, imageInfo.bmWidth, imageInfo.bmHeight);
		imageDC.CreateCompatibleDC(dc);
		bitmap.CreateCompatibleBitmap(dc, imageInfo.bmWidth, imageInfo.bmHeight);
		CBitmap* oldBitmap = imageDC.SelectObject(&bitmap);
		imageDC.FillRect(rect, WHITE_BRUSH);

		imagePainter->DrawTransparentBitmap(&imageDC, imageRect.left + (imageRect.Width() - imageInfo.bmWidth) / 2, imageRect.top - imageInfo.bmHeight - 5);
		dc->BitBlt(imageRect.left + (imageRect.Width() - imageInfo.bmWidth) / 2, imageRect.top - imageInfo.bmHeight - 5, imageInfo.bmWidth, imageInfo.bmHeight, &imageDC, 0, 0, SRCCOPY);

		imageDC.SelectObject(oldBitmap);
		bitmap.DeleteObject();
		imageDC.DeleteDC();
	}
}

void ShapesManager::ClearShapesVector()
{
	for (auto it = shapes.begin(); it != shapes.end(); it++)
		if(*it != animationShape)
			delete *it;
	if(animationShape != NULL)
		delete animationShape;
	if (!shapes.empty())
		std::vector<Shape*>().swap(shapes);
}
