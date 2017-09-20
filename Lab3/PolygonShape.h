#pragma once
#include "Shape.h"
class PolygonShape : public Shape
{
	public:
		PolygonShape();
		PolygonShape(std::vector<POINT>);
		~PolygonShape();
	protected:
		void CalculatePoints(std::vector<POINT>);
		virtual std::vector<POINT> ÑoordinateAdjustment(POINT*, POINT*, int);
		std::vector<POINT> GetLinePoints(POINT, POINT, int = 0);
	private:
		bool PolygonShape::CheckDistance(int, int);
};

