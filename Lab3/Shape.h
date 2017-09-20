#pragma once
#include <vector>

class Shape
{
	public:
		Shape();
		Shape(std::vector<POINT>);
		~Shape();
		virtual void Draw(CDC*, std::vector<Shape*>);
		bool CheckPointInRegion(POINT);
		void OffsetCoordinates(POINT);
		void TurnCoordinates(POINT);
		int GetLayoutNumber();
		CRect GetShapeRect();
protected:
		std::vector<POINT> vertexes;
		std::vector<POINT> points;
		std::vector<int> vertexDistances;
		CRgn region;
		int shapeLayoutNumber = 0;

		virtual void CalculatePoints(std::vector<POINT>) = 0;
		virtual std::vector<POINT> ÑoordinateAdjustment(POINT*, POINT*, int) = 0;
		void ClearVector(std::vector<POINT>*);
		int GetDistance(POINT, POINT);
private:
		static int layoutNumber;
		int centerX, centerY;

		void Offset(std::vector<POINT>*, POINT);
		void CalculateDistancesBetweenVertexes();
};

