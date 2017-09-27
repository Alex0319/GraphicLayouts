#include "stdafx.h"
#include "ImagePainter.h"


ImagePainter::ImagePainter()
{
}


ImagePainter::~ImagePainter()
{
	image.DeleteObject();
	imageMask.DeleteObject();
}

void ImagePainter::LoadImageFromResourceWithTransparency(UINT uResourceID, COLORREF clrpTransColor)
{
	if (LoadBitmapFromPNG(uResourceID)) {
		PrepareMask(clrpTransColor);
		imageRegion.CreateRectRgn(0,0, imageInfo.bmWidth, imageInfo.bmHeight);
	}
}

bool ImagePainter::LoadBitmapFromPNG(UINT uResourceID)
{
	bool bRet = false;

	HINSTANCE hModuleInstance = AfxGetInstanceHandle();
	HRSRC hResourceHandle = ::FindResource(hModuleInstance, MAKEINTRESOURCE(uResourceID), L"PNG");
	if (0 == hResourceHandle)
	{
		return bRet;
	}

	DWORD nImageSize = ::SizeofResource(hModuleInstance, hResourceHandle);
	if (0 == nImageSize)
	{
		return bRet;
	}

	HGLOBAL hResourceInstance = ::LoadResource(hModuleInstance, hResourceHandle);
	if (0 == hResourceInstance)
	{
		return bRet;
	}

	const void* pResourceData = ::LockResource(hResourceInstance);
	if (0 == pResourceData)
	{
		FreeResource(hResourceInstance);
		return bRet;
	}

	HGLOBAL hBuffer = ::GlobalAlloc(GMEM_MOVEABLE, nImageSize);
	if (0 == hBuffer)
	{
		FreeResource(hResourceInstance);
		return bRet;
	}

	void* pBuffer = ::GlobalLock(hBuffer);
	if (0 != pBuffer)
	{
		CopyMemory(pBuffer, pResourceData, nImageSize);
		IStream* pStream = 0;
		if (S_OK == ::CreateStreamOnHGlobal(hBuffer, FALSE, &pStream))
		{
			CImage ImageFromResource;
			ImageFromResource.Load(pStream);
			pStream->Release();
			image.Attach(ImageFromResource.Detach());
			bRet = true;
		}
		::GlobalUnlock(hBuffer);
	}
	::GlobalFree(hBuffer);

	UnlockResource(hResourceInstance);
	FreeResource(hResourceInstance);

	return bRet;
}

void ImagePainter::PrepareMask(COLORREF clrpTransColor, int iTransPixelX, int iTransPixelY)
{
	image.GetObject(sizeof(BITMAP), &imageInfo);

	imageMask.DeleteObject();
	imageMask.CreateBitmap(imageInfo.bmWidth, imageInfo.bmHeight, 1, 1, NULL);

	CDC hdcSrc, hdcDst;

	hdcSrc.CreateCompatibleDC(NULL);
	hdcDst.CreateCompatibleDC(NULL);

	CBitmap* hbmSrcT = (CBitmap*)hdcSrc.SelectObject(&image);
	CBitmap* hbmDstT = (CBitmap*)hdcDst.SelectObject(&imageMask);

	COLORREF clrTrans;
	if (clrpTransColor == NULL)
	{
		clrTrans = hdcSrc.GetPixel(iTransPixelX, iTransPixelY);
	}
	else
	{
		clrTrans = clrpTransColor;
	}

	COLORREF clrSaveBk = hdcSrc.SetBkColor(clrTrans);

	hdcDst.BitBlt(0, 0, imageInfo.bmWidth, imageInfo.bmHeight, &hdcSrc, 0, 0, SRCINVERT);

	COLORREF clrSaveDstText = hdcSrc.SetTextColor(RGB(255, 255, 255));
	hdcSrc.SetBkColor(RGB(0, 0, 0));

	hdcSrc.BitBlt(0, 0, imageInfo.bmWidth, imageInfo.bmHeight, &hdcDst, 0, 0, SRCAND);

	hdcDst.SetTextColor(clrSaveDstText);

	hdcSrc.SetBkColor(clrSaveBk);
	hdcSrc.SelectObject(hbmSrcT);
	hdcDst.SelectObject(hbmDstT);

	hdcSrc.DeleteDC();
	hdcDst.DeleteDC();
}


void ImagePainter::DrawTransparentBitmap(CDC* pDC, int xStart, int yStart, int xSource,	int ySource)
{
	CDC hdcMem, pTmpDC;
	hdcMem.CreateCompatibleDC(NULL);
	pTmpDC.CreateCompatibleDC(pDC);

	CBitmap* hbmT = hdcMem.SelectObject(&imageMask);
	CBitmap* oldBitmap = pTmpDC.SelectObject(&image);

	pDC->BitBlt(0, 0, imageInfo.bmWidth, imageInfo.bmHeight, &hdcMem, xSource, ySource, SRCAND);
	pDC->BitBlt(0, 0, imageInfo.bmWidth, imageInfo.bmHeight, &pTmpDC, xSource, ySource, SRCPAINT);

	hdcMem.SelectObject(hbmT);
	pTmpDC.SelectObject(oldBitmap);
	hdcMem.DeleteDC();
	pTmpDC.DeleteDC();
	CRect rect;
	imageRegion.GetRgnBox(&rect);
	imageRegion.OffsetRgn(xStart - rect.left, yStart - rect.top);
}

bool ImagePainter::IsPointInImageRegion(CPoint point)
{
	return imageRegion.PtInRegion(point);
}

BITMAP ImagePainter::GetImageInfo()
{
	return imageInfo;
}
