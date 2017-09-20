#include "stdafx.h"
#include "PolygonShape.h"


PolygonShape::PolygonShape()
{
}

PolygonShape::PolygonShape(std::vector<POINT> vertexes) : Shape(vertexes)
{
	CalculatePoints(vertexes);
	region.CreatePolygonRgn(&points[0], points.size(), ALTERNATE);
}

PolygonShape::~PolygonShape()
{
}

void PolygonShape::CalculatePoints(std::vector<POINT> vertexes)
{
	ClearVector(&points);
	for (auto i = 0; i < vertexes.size(); i++)
	{
		std::vector<POINT> linePoints = (i + 1 != vertexes.size() ? GetLinePoints(vertexes[i], vertexes[i + 1]) : GetLinePoints(vertexes[i], vertexes[0]));
		points.insert(points.end(), linePoints.begin(), linePoints.end());
	}
}

std::vector<POINT> PolygonShape::ÑoordinateAdjustment(POINT *a, POINT *b, int prevDistance)
{
	std::vector<POINT> linePoints = GetLinePoints(*a, *b, prevDistance);
	*a = linePoints[0];
	*b = linePoints[linePoints.size() - 1];
	return linePoints;
}

bool PolygonShape::CheckDistance(int newDistance, int prevDistance)
{
	if (prevDistance != 0)
		return newDistance <= prevDistance;
	return true;
}

std::vector<POINT> PolygonShape::GetLinePoints(POINT a, POINT b, int prevDistance)
{
	std::vector<POINT> points;
	int dx = abs(b.x - a.x);
	int dy = abs(b.y - a.y);
	int sx = b.x >= a.x ? 1 : -1;
	int sy = b.y >= a.y ? 1 : -1;
	if (dy <= dx) 
	{
		int d = (dy << 1) - dx;
		int d1 = dy << 1;
		int d2 = (dy - dx) << 1;
		points.push_back(a);
		for (int x = a.x + sx, y = a.y, i = 1; i <= dx && CheckDistance(GetDistance(points[0], points[points.size() - 1]), prevDistance); i++, x += sx) {
			if (d > 0) {
				d += d2; y += sy;
			}
			else
				d += d1;
			points.push_back({ x, y });
		}
	}
	else 
	{
		int d = (dx << 1) - dy;
		int d1 = dx << 1;
		int d2 = (dx - dy) << 1;
		points.push_back(a);
		for (int x = a.x, y = a.y + sy, i = 1; i <= dy && CheckDistance(GetDistance(points[0], points[points.size() - 1]), prevDistance); i++, y += sy) {
			if (d > 0) {
				d += d2; x += sx;
			}
			else
				d += d1;
			points.push_back({ x, y });
		}
	}
	return points;
}
