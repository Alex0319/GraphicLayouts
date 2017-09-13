#include "stdafx.h"
#include "ShapesManager.h"


ShapesManager::ShapesManager()
{
}


ShapesManager::~ShapesManager()
{
	ClearShapesVector();
}

void ShapesManager::Draw(CPaintDC* dc)
{
}

void ShapesManager::AddShape(std::vector<LPPOINT> vertex)
{
}

void ShapesManager::ClearShapesVector()
{
	for (auto it = shapes.begin(); it != shapes.end(); it++)
		delete *it;
	if (!shapes.empty())
		std::vector<Shape*>().swap(shapes);
}
