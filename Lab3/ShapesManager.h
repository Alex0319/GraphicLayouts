#pragma once

#ifndef __shape_h__
	#include "Shape.h"
	#include "PolygonShape.h"
	#include "AnimationShape.h"
#endif // !__shape_h__

#include "ImagePainter.h"

class ShapesManager
{
	public:
		ShapesManager();
		ShapesManager(int);
		~ShapesManager();
		void Draw(CDC* dc, CRgn*);
		void AddShape(std::vector<POINT>);
		void CreateAnimationShape(std::vector<POINT>);
		void ChangeSelectedShapePosition(POINT);
		void ChangeAnimationShapeDrawing(CWnd*);
		void DrawTurnIcon(CDC*);
		bool CheckSelectedShape(POINT);
		bool ChangeSelectedShapeZoom(float);
	private:
		std::vector<Shape*> shapes;
		Shape* selectedShape = NULL;
		ImagePainter* imagePainter = NULL;
		AnimationShape* animationShape = NULL;

		enum Actions
		{
			MOTION,
			ROTATION,
			NOACTION
		} shapeState;
		bool ChangeShapeState(Shape*, POINT);
		void ClearShapesVector();
};

