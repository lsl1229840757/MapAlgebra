#include "MA.h"
#include<Windows.h>
#include<stdio.h>

bool bmpReverse(const char* srcBmpName, const char* destBmpName){
	BmpData bmpData = readBmp(srcBmpName);
	//��ֵ
	for(int i=0;i<bmpData.matrixHeight;i++){
		for(int j=0;j<bmpData.matrixWidth;j++){
			//if (bmpData.data[i][j] == 0xFF) continue;
			bmpData.data[i][j] = 255-bmpData.data[i][j];
		}
	}
	//��ʼд����
	writeBmp(destBmpName, bmpData);
	return true;
}

bool bmp256To32(const char* srcBmpName, const char* destBmpName){
	BmpData bmp256Data = readBmp(srcBmpName);
	//��ʱ�þ�������
	BmpData bmpData = readBmp(srcBmpName);
	unsigned **Bmp32Mtx = new unsigned *[bmp256Data.bmpHeight];
	//������Ҫ����һ��colorTable
	unsigned* clrTable = new unsigned[256];
	for(int i=0;i<1024;i+=4){
		unsigned char* bufChar = new unsigned char[4];
		for(int j=0;j<4;j++){
			bufChar[j] = bmp256Data.colorTable[i+j];
		}
		clrTable[i/4] = *(unsigned*)bufChar;
	}
	for(int i = 0; i<bmp256Data.bmpHeight;i++){
		Bmp32Mtx[i] = new unsigned [bmp256Data.bmpWidth];
		for(int j = 0;j<bmp256Data.bmpWidth;j++){
			Bmp32Mtx[i][j] = clrTable[bmp256Data.data[i][j]];
		}
	}
	// ��ʼд����
	// �½����λͼ�ļ�
	FILE *outBmp = fopen(destBmpName, "wb");
	if (outBmp == NULL) 
	{
		printf("file not found!");
		return false;
	}
	// ��ȡλͼ������Ϣ
	BITMAPFILEHEADER BmpFileHeader = bmp256Data.fileHeader;
	BITMAPINFOHEADER BmpInfoHeader = bmp256Data.infoHeader;
	BmpFileHeader.bfOffBits = 54;
	BmpFileHeader.bfSize = sizeof(unsigned)*(bmp256Data.bmpHeight*bmp256Data.bmpWidth)+54;
	fwrite(&BmpFileHeader, sizeof(BITMAPFILEHEADER), 1, outBmp);
	BmpInfoHeader.biBitCount = 32;
	fwrite(&BmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, outBmp);	
		// д����
	for (int i = 0; i<bmp256Data.bmpHeight; i++)
	{
		fwrite(Bmp32Mtx[i], sizeof(unsigned), bmp256Data.bmpWidth,outBmp); 
	}
	// �����ڴ桢�ر��ļ�
	fclose(outBmp);

	for(int i=0;i<bmp256Data.bmpHeight;i++){
		delete[] Bmp32Mtx[i];
		Bmp32Mtx[i] = NULL;
	}
	delete []Bmp32Mtx; 
	Bmp32Mtx = NULL;
	return true;
}



bool bmpSmooth(const char* scrBmpName, const char* destBmpName){
	// TODO
	return false;
}


bool bmpOverlay(const char* srcBmpName,double a, double b, const char* destBmpName){
	// TODO
	return false;
}



// ����bmp�ļ�
BmpData& readBmp(const char* srcBmpName){
	//1.��Դ�ļ�
	FILE* file = fopen(srcBmpName, "rb");
	if (file == NULL) 
	{
		printf("file not found!");
		BmpData b;
		return b;
	}
	//1.1��ȡ�ļ�ͷ
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;
	unsigned char colorTable[256*4];
	fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, file);
	fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, file);
	//�߶ȺͿ��
	int bmpWidth = infoHeader.biWidth;
	int bmpHeight = infoHeader.biHeight;
	//�Լ�����������
	BmpData* bmpData = new BmpData(bmpWidth, bmpHeight);
	fread(bmpData->colorTable, 1024, 1, file);
	bmpData->fileHeader = fileHeader;
	bmpData->infoHeader = infoHeader;
	bmpData->bmpHeight = bmpHeight;
	bmpData->bmpWidth = bmpWidth;
	// ��Ϊint���ĸ��ֽڣ�����������Ҫ��4�ı���
	bmpData->matrixWidth = (bmpWidth+3)/4*4;
	bmpData->matrixHeight = (bmpHeight+3)/4*4;
	//��ȡλͼ����
	for(unsigned y=0;y<bmpHeight;y++){
		fread(bmpData->data[y], sizeof(char), bmpWidth, file);
	}
	return *bmpData;
}

// дbmp�ļ�
bool writeBmp(const char* destBmpName, BmpData& bmpData){
	// 5. �½����λͼ�ļ�
	FILE *outFile = fopen(destBmpName, "wb");
	if (outFile == NULL) 
	{
		printf("file not found!");
		return false;
	}
	fwrite(&bmpData.fileHeader, sizeof(BITMAPFILEHEADER), 1, outFile);
	fwrite(&bmpData.infoHeader, sizeof(BITMAPINFOHEADER), 1, outFile);
	fwrite(bmpData.colorTable, 1024, 1, outFile);
	// 6. д����
	for (int i = 0; i<bmpData.matrixHeight; i++)
	{
		fwrite(bmpData.data[i], 1, bmpData.matrixWidth, outFile); 
	}
}