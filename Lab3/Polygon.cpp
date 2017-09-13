#include "stdafx.h"
#include "Polygon.h"


Polygon::Polygon()
{
}

Polygon::Polygon(std::vector<LPPOINT> vertexes) : Shape(vertexes)
{
}

Polygon::~Polygon()
{
}

void Polygon::Draw(CPaintDC dc, std::vector<Shape*> shapes)
{
}

