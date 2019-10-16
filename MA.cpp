#include "stdafx.h"

#include "MA.h"
#include<Windows.h>
#include<stdio.h>


bool BmpReverse(const char* SrcBmpName, const char* DestBmpName)
{
	// 1. 打开源文件
	FILE *file = fopen(SrcBmpName, "rb");
	if (file == NULL) 
	{
		printf("file not found!");
		return false;
	}

	// 2. 读取位图基本信息
	BITMAPFILEHEADER BmpFileHeader;
	BITMAPINFOHEADER BmpInfoHeader;
	unsigned char ClrTab[256 * 4];

	fread(&BmpFileHeader, sizeof(BITMAPFILEHEADER), 1, file);
	fread(&BmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);
	fread(ClrTab, 1024, 1, file);

	unsigned MtxWidth, MtxHeight, BmpWidth;
	BmpWidth = BmpInfoHeader.biWidth;
	MtxWidth = (BmpWidth + 3) / 4 * 4;
	MtxHeight = BmpInfoHeader.biHeight;

	// 3. 读取位图矩阵数据
	unsigned char * LineBuf = new unsigned char[MtxWidth]; // 读数据行缓冲

	unsigned char ** MtxBuf = new unsigned char *[MtxHeight]; // 写数据矩阵缓冲
	for (int i = 0; i<MtxHeight; i++)
	{
		MtxBuf[i] = new unsigned char[MtxWidth]; 
		for (int j=0; j<MtxWidth; j++)
		{
			MtxBuf[i][j] = 0xFF;
		}
	}


	// 4. 反色处理
	for (int i=0; i<MtxHeight; i++)
	{
		fread(LineBuf, sizeof(unsigned char), MtxWidth, file);
		for (int j=0; j<MtxWidth; j++)
		{
			if (LineBuf[j] == 0xFF) continue;
			MtxBuf[i][j] = 255 - LineBuf[j];
		}
	}
	// 5. 新建结果位图文件
	FILE *file1 = fopen(DestBmpName, "wb");
	if (file == NULL) 
	{
		printf("file not found!");
		return false;
	}
	fwrite(&BmpFileHeader, sizeof(BITMAPFILEHEADER), 1, file1);
	fwrite(&BmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file1);
	fwrite(ClrTab, 1024, 1, file1);

	// 6. 写入结果
	for (int i = 0; i<MtxHeight; i++)
	{
		fwrite(MtxBuf[i], 1, MtxWidth, file1); 
	}

	// 7. 清理内存、关闭文件
	fclose(file);
	fclose(file1);
	for (int i = 0; i<MtxHeight; i++)
	{
		delete [] MtxBuf[i]; 
		MtxBuf[i] = NULL;
	}
	delete [] MtxBuf;
	MtxBuf = NULL;

	delete [] LineBuf;
	LineBuf = NULL;

	return true;
};
