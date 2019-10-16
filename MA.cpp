#include "stdafx.h"

#include "MA.h"
#include<Windows.h>
#include<stdio.h>


bool BmpReverse(const char* SrcBmpName, const char* DestBmpName)
{
	// 1. ��Դ�ļ�
	FILE *file = fopen(SrcBmpName, "rb");
	if (file == NULL) 
	{
		printf("file not found!");
		return false;
	}

	// 2. ��ȡλͼ������Ϣ
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

	// 3. ��ȡλͼ��������
	unsigned char * LineBuf = new unsigned char[MtxWidth]; // �������л���

	unsigned char ** MtxBuf = new unsigned char *[MtxHeight]; // д���ݾ��󻺳�
	for (int i = 0; i<MtxHeight; i++)
	{
		MtxBuf[i] = new unsigned char[MtxWidth]; 
		for (int j=0; j<MtxWidth; j++)
		{
			MtxBuf[i][j] = 0xFF;
		}
	}


	// 4. ��ɫ����
	for (int i=0; i<MtxHeight; i++)
	{
		fread(LineBuf, sizeof(unsigned char), MtxWidth, file);
		for (int j=0; j<MtxWidth; j++)
		{
			if (LineBuf[j] == 0xFF) continue;
			MtxBuf[i][j] = 255 - LineBuf[j];
		}
	}
	// 5. �½����λͼ�ļ�
	FILE *file1 = fopen(DestBmpName, "wb");
	if (file == NULL) 
	{
		printf("file not found!");
		return false;
	}
	fwrite(&BmpFileHeader, sizeof(BITMAPFILEHEADER), 1, file1);
	fwrite(&BmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file1);
	fwrite(ClrTab, 1024, 1, file1);

	// 6. д����
	for (int i = 0; i<MtxHeight; i++)
	{
		fwrite(MtxBuf[i], 1, MtxWidth, file1); 
	}

	// 7. �����ڴ桢�ر��ļ�
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
