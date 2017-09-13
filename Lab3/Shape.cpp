#include "stdafx.h"
#include "Shape.h"

int Shape::layoutNumber = 0;

Shape::Shape()
{
	shapeLayoutNumber = layoutNumber++;
}

Shape::Shape(std::vector<LPPOINT> vertexes)
{
	this->vertexes = vertexes;
	shapeLayoutNumber = layoutNumber++;
}


Shape::~Shape()
{
	layoutNumber--;
}

int Shape::GetLayoutNumber()
{
	return shapeLayoutNumber;
}
