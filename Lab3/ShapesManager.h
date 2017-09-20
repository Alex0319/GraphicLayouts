#pragma once
#include "Shape.h"
#include "ImagePainter.h"

class ShapesManager
{
	public:
		ShapesManager();
		ShapesManager(int);
		~ShapesManager();
		void Draw(CDC* dc);
		void AddShape(std::vector<POINT>);
		bool CheckShapeSelect(POINT);
		void ChangeSelectShapePosition(POINT);
		void DrawTurnIcon(CDC*);
	private:
		std::vector<Shape*> shapes;
		Shape* selectedShape = NULL;
		ImagePainter* imagePainter = NULL;

		enum Actions
		{
			MOTION,
			ROTATION,
			NOACTION
		} shapeState;
		bool ChangeShapeState(Shape*, POINT);
		void ClearShapesVector();
};

