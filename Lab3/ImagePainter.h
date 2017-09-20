#pragma once
class ImagePainter
{
	public:
		ImagePainter();
		~ImagePainter();
		void LoadImageFromResourceWithTransparency(UINT, COLORREF);
		void DrawTransparentBitmap(CDC* pDC, int xStart, int yStart, int xSource = 0, int ySource = 0);
		bool IsPointInImageRegion(CPoint);
	private:
		CBitmap image, imageMask;
		BITMAP imageInfo;
		CRgn imageRegion;

		bool LoadBitmapFromPNG(UINT uResourceID);
		void PrepareMask(COLORREF clrpTransColor, int iTransPixelX = 0, int iTransPixelY = 0);
};

