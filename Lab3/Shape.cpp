#include "stdafx.h"
#include "Shape.h"

int Shape::layoutNumber = 0;

Shape::Shape()
{
	shapeLayoutNumber = layoutNumber++;
}

Shape::Shape(std::vector<POINT> vertexes)
{
	this->vertexes = vertexes;
	startVertexes = vertexes;
	shapeLayoutNumber = layoutNumber++;
	CalculateDistancesBetweenVertexes();
}

Shape::~Shape()
{
	DeleteObject(region);
	layoutNumber--;
	vertexDistances.clear();
	ClearVector(&vertexes);
	ClearVector(&points);
	ClearVector(&startVertexes);
}

void Shape::ClearVector(std::vector<POINT>* vector)
{
	vector->clear();
	if (!vector->empty())
		std::vector<POINT>().swap(*vector);
}

int Shape::GetLayoutNumber()
{
	return shapeLayoutNumber;
}

CRect Shape::GetShapeRect()
{
	CRect rect;
	
	region.GetRgnBox(&rect);
	return rect;
}

bool Shape::CheckPointInRegion(POINT point)
{
	return region.PtInRegion(point);
}

void Shape::OffsetCoordinates(POINT offset)
{
	centerX = -1;
	Offset(&points, offset);
	Offset(&vertexes, offset);
	startVertexes = vertexes;
	region.OffsetRgn(offset);
}

void Shape::Offset(std::vector<POINT>* vectorPtr, POINT offset)
{
	for (auto i = 0; i < vectorPtr->size(); i++)
	{
		(*vectorPtr)[i].x = (*vectorPtr)[i].x + offset.x;
		(*vectorPtr)[i].y = (*vectorPtr)[i].y + offset.y;
	}
}

void Shape::CalculateDistancesBetweenVertexes()
{
	for (int i = 0; i < vertexes.size(); i++)
		vertexDistances.push_back(GetDistance(vertexes[i], i + 1 < vertexes.size() ? vertexes[i + 1] : vertexes[0]));
}

int Shape::GetDistance(POINT x1, POINT x2)
{
	return pow(x2.x - x1.x, 2) + pow(x2.y - x1.y, 2);
}

void Shape::TurnCoordinates(POINT point)
{
	const int height = 5;
	if (point.x > height || point.x < -height)
		point.x %= height;

	float angle = asin((float)(point.x) / height);
	if (angle < 0.0001 && angle > -0.0001)
	{
		vertexes = startVertexes;
		CalculatePoints(vertexes);
	}
	else
	{
		CRect rect = GetShapeRect();
		if (centerX <= 0)
		{
			centerX = rect.left + rect.Width() / 2;
			centerY = rect.top + rect.Height() / 2;
		}
		ClearVector(&points);
		for (auto i = 0; i < vertexes.size(); i++)
		{
			POINT newPoint;
			newPoint.x = centerX + (vertexes[i].x - centerX)  * cos(angle) - (vertexes[i].y - centerY) * sin(angle);
			newPoint.y = centerY + (vertexes[i].x - centerX) * sin(angle) + (vertexes[i].y - centerY) * cos(angle);
			if (i != 0)
			{
				std::vector<POINT> linePoints = ÑoordinateAdjustment(&vertexes[i - 1], &newPoint, vertexDistances[i - 1]);
				points.insert(points.end(), linePoints.begin(), linePoints.end());
			}
			vertexes[i] = newPoint;
		}
		std::vector<POINT> linePoints = ÑoordinateAdjustment(&vertexes[vertexes.size() - 1], &vertexes[0], vertexDistances[vertexDistances.size() - 1]);
		points.insert(points.end(), linePoints.begin(), linePoints.end());
	}
	region.DeleteObject();
	region.CreatePolygonRgn(&points[0], points.size(), ALTERNATE);
}

void Shape::Draw(CDC* dc, std::vector<Shape*> shapes)
{
	bool drawingState = true, currentDrawingState = true;
	dc->MoveTo(points[0]);
	for (auto i = 1; i < points.size(); i++)
	{
		for (auto j = 0; j < shapes.size(); j++)
		{
			if (shapes[j]->GetLayoutNumber() > shapeLayoutNumber && shapes[j]->CheckPointInRegion(points[i]))
			{
				if (i % 3 == 0)
				{
					currentDrawingState = !currentDrawingState;
				}
				drawingState = currentDrawingState;
				break;
			}
		}
		if (drawingState)
		{
			dc->LineTo(points[i]);
		}
		dc->MoveTo(points[i]);
		drawingState = true;
	}
	dc->LineTo(points[0]);
}
