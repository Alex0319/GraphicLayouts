#pragma once

#include "PolygonShape.h"

class AnimationShape : public PolygonShape
{
	public:
		AnimationShape();
		AnimationShape(std::vector<POINT>);
		~AnimationShape();

		bool ChangeDrawingState(CWnd*);
		bool CheckPointInRegion(POINT);
		void Draw(CDC*, std::vector<Shape*>, CRgn*, int = 1);

	private:
		bool isDrawingShape = false, isRightMotion = true;
		CWinThread *m_pThread = NULL;
		CEvent cEvent, finishEvent;
		CMutex mutex;
		HWND hWnd;
		volatile LONG repaintCount = 0;

		UINT ThreadOffsetShape(HWND);

		template<class T>
		static UINT _cdecl StartThread(LPVOID);

		template <typename T>
		struct ThreadData
		{
			HWND hWnd;
			T* instance;

			ThreadData(HWND hwnd, T* inst) : hWnd(hwnd), instance(inst) {}
		};
};