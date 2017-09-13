#pragma once
#include <vector>

class Shape
{
	public:
		Shape();
		Shape(std::vector<LPPOINT>);
		~Shape();
		virtual void Draw(CPaintDC, std::vector<Shape*>) = 0;
	protected:
		std::vector<LPPOINT> vertexes;
		int shapeLayoutNumber = 0;

		int GetLayoutNumber();
	private:
		static int layoutNumber;
};

