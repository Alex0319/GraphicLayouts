#pragma once
#include "Shape.h"

class ShapesManager
{
	public:
		ShapesManager();
		~ShapesManager();
		void Draw(CPaintDC* dc);
		void AddShape(std::vector<LPPOINT>);
	private:
		std::vector<Shape*> shapes;

		void ClearShapesVector();
};

