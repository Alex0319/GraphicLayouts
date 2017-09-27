#include "stdafx.h"
#include "Shape.h"

int Shape::layoutNumber = 0;

Shape::Shape()
{
	InitShapeProperties();
}

Shape::Shape(std::vector<POINT> vertexes)
{
	this->vertexes = vertexes;
	startVertexes = vertexes;
	InitShapeProperties();
	CalculateDistancesBetweenVertexes();
}

Shape::~Shape()
{
	region.DeleteObject();
	layoutNumber--;
	vertexDistances.clear();
	ClearVector(&vertexes);
	ClearVector(&points);
	ClearVector(&startVertexes);
}

void Shape::InitShapeProperties()
{
	shapeLayoutNumber = layoutNumber++;
	switch (rand() % 5)
	{
		case 0: color = RED;
			break;
		case 1: color = YELLOW;
			break;
		case 2: color = ORANGE;
			break;
		case 3: color = GREEN;
			break;
		case 4: color = BLUE;
			break;
	}
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
	Offset(&points, offset);
	Offset(&vertexes, offset);
	startVertexes = vertexes;
	region.OffsetRgn(offset);
	shapeCenterPoint = GetShapeRect().CenterPoint();
}

void Shape::Offset(std::vector<POINT>* vectorPtr, POINT offset)
{
	for (auto i = 0; i < vectorPtr->size(); i++)
	{
		(*vectorPtr)[i].x = (*vectorPtr)[i].x + offset.x;
		(*vectorPtr)[i].y = (*vectorPtr)[i].y + offset.y;
	}
}

void Shape::ZoomCoordinates(std::vector<POINT>* points, float zoom)
{
	for (int i = 0; i < points->size(); i++)
	{
		(*points)[i].x = (*points)[i].x * zoom + shapeCenterPoint.x * (1 - zoom);
		(*points)[i].y = (*points)[i].y * zoom + shapeCenterPoint.y * (1 - zoom);
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

void Shape::ChangeZoom(float changeValue)
{
	if (((zoom * changeValue) <= 4) && ((zoom * changeValue) >= 0.25))
	{
		zoom *= changeValue;
		ZoomCoordinates(&vertexes, changeValue);
		ZoomCoordinates(&startVertexes, changeValue);
		for (int i = 0; i < vertexDistances.size(); i++)
		{
			vertexDistances[i] *= changeValue;
			vertexDistances[i] *= changeValue;
		}
		CalculatePoints(vertexes);
		region.DeleteObject();
		region.CreatePolygonRgn(&points[0], points.size(), ALTERNATE);
	}
}

void Shape::TurnCoordinates(POINT point)
{
	const int height = 5;
	if (point.x > height || point.x < -height)
		point.x %= height;

	float angle = asin((float)(point.x) / height);
	if (angle < 0.01 && angle > -0.01)
	{
		vertexes = startVertexes;
		CalculatePoints(vertexes);
	}
	else
	{
		ClearVector(&points);
		for (auto i = 0; i < vertexes.size(); i++)
		{
			POINT newPoint;
			newPoint.x = shapeCenterPoint.x + (vertexes[i].x - shapeCenterPoint.x)  * cos(angle) - (vertexes[i].y - shapeCenterPoint.y) * sin(angle);
			newPoint.y = shapeCenterPoint.y + (vertexes[i].x - shapeCenterPoint.x) * sin(angle) + (vertexes[i].y - shapeCenterPoint.y) * cos(angle);
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

void Shape::Draw(CDC* dc, std::vector<Shape*> shapes, CRgn* drawingAreaRgn, int penWidth)
{
	bool drawingState = true, currentDrawingState = true;
	CPen pen(PS_SOLID, penWidth, color);
	CPen* oldPen = dc->SelectObject(&pen);
	
	dc->MoveTo(points[0]);
	for (auto i = 1; i < points.size(); i++)
	{
		for (auto j = 0; j < shapes.size(); j++)
		{
			if ((shapes[j]->GetLayoutNumber() > shapeLayoutNumber && shapes[j]->CheckPointInRegion(points[i])) || !drawingAreaRgn->PtInRegion(points[i]))
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
	dc->SelectObject(oldPen);
	pen.DeleteObject();
}
