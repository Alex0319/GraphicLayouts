#pragma once
#include "Shape.h"
class Polygon : public Shape
{
	public:
		Polygon();
		Polygon(std::vector<LPPOINT>);
		~Polygon();
		void Draw(CPaintDC, std::vector<Shape*>);
};

