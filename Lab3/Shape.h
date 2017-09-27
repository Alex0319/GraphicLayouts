#pragma once
#include <vector>

class Shape
{
	public:
		Shape();
		Shape(std::vector<POINT>);
		~Shape();
		virtual void Draw(CDC*, std::vector<Shape*>, CRgn*, int = 1);
		virtual bool CheckPointInRegion(POINT);
		void OffsetCoordinates(POINT);
		void TurnCoordinates(POINT);
		void ChangeZoom(float);
		int GetLayoutNumber();
		CRect GetShapeRect();
	protected:
		std::vector<POINT> vertexes;
		std::vector<POINT> startVertexes;
		std::vector<POINT> points;
		std::vector<int> vertexDistances;
		CRgn region;
		CPoint shapeCenterPoint;
		int shapeLayoutNumber = 0;
		float zoom = 1;

		virtual void CalculatePoints(std::vector<POINT>) = 0;
		virtual std::vector<POINT> ÑoordinateAdjustment(POINT*, POINT*, int) = 0;
		void ClearVector(std::vector<POINT>*);
		int GetDistance(POINT, POINT);
	private:
		static int layoutNumber;
//		float angle = 0;

		enum ShapeColors
		{
			RED = RGB(230, 92, 40),
			YELLOW = RGB(248, 248, 5),
			ORANGE = RGB(255, 163, 77),
			GREEN = RGB(0, 204, 102),
			BLUE = RGB(0, 128, 192),
		};

		ShapeColors color;
		void Offset(std::vector<POINT>*, POINT);
		void ZoomCoordinates(std::vector<POINT>*, float);
		void CalculateDistancesBetweenVertexes();
		void InitShapeProperties();
};

