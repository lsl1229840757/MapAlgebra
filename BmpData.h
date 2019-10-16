#pragma once
#include<Windows.h>
#include<stdio.h>
class BmpData
{
public:
	int matrixWidth;
	int matrixHeight;
	int bmpWidth;
	int bmpHeight;
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;
	unsigned char ** data;
	unsigned char colorTable[256*4];
	BmpData(void);
	~BmpData(void);
	BmpData(const BmpData& bmpData);
	BmpData(int width, int height);
};

