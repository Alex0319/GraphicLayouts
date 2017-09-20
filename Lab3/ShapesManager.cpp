#include "stdafx.h"
#include "ShapesManager.h"
#include "PolygonShape.h"


ShapesManager::ShapesManager()
{
}

ShapesManager::ShapesManager(int uResourceId)
{
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

void ShapesManager::Draw(CDC* dc)
{
	CPen *oldPen = NULL;
	CPen newPen(PS_SOLID, 3, RGB(0,0,0));
	oldPen = (*dc).SelectObject(&newPen);
	if(selectedShape != NULL)
		selectedShape->Draw(dc, shapes);
	if (oldPen != NULL)
		(*dc).SelectObject(oldPen);
	DeleteObject(newPen);
	for (auto it = shapes.begin(); it != shapes.end(); it++)
	{
		if ((*it) != selectedShape) 
			(*it) -> Draw(dc, shapes);
	}
}

void ShapesManager::AddShape(std::vector<POINT> vertex)
{
	shapes.push_back(new PolygonShape(vertex));
}

bool ShapesManager::CheckShapeSelect(POINT point)
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
	if (selectedShape == NULL && prevSelectedShape != NULL && imagePainter->IsPointInImageRegion(point))
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
		else
			shapeState = NOACTION;
		return false;
	}
}

void ShapesManager::ChangeSelectShapePosition(POINT point)
{
	switch (shapeState)
	{
		case MOTION: selectedShape->OffsetCoordinates(point);
			break;
		case ROTATION: selectedShape->TurnCoordinates(point);
			break;
	}
}

void ShapesManager::DrawTurnIcon(CDC* dc)
{
	if (selectedShape != NULL) {
		CRect rect = selectedShape->GetShapeRect();
		imagePainter->DrawTransparentBitmap(dc, rect.left + rect.Width() / 2, rect.top);
	}
}

void ShapesManager::ClearShapesVector()
{
	for (auto it = shapes.begin(); it != shapes.end(); it++)
		delete *it;
	if (!shapes.empty())
		std::vector<Shape*>().swap(shapes);
}
