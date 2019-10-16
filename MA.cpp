#include "MA.h"
#include<Windows.h>
#include<stdio.h>

bool bmpReverse(const char* srcBmpName, const char* destBmpName){
	BmpData bmpData = readBmp(srcBmpName);
	//赋值
	for(int i=0;i<bmpData.matrixHeight;i++){
		for(int j=0;j<bmpData.matrixWidth;j++){
			//if (bmpData.data[i][j] == 0xFF) continue;
			bmpData.data[i][j] = 255-bmpData.data[i][j];
		}
	}
	//开始写数据
	writeBmp(destBmpName, bmpData);
	return true;
}

bool bmp256To32(const char* srcBmpName, const char* destBmpName){
	BmpData bmp256Data = readBmp(srcBmpName);
	//暂时用矩阵来存
	BmpData bmpData = readBmp(srcBmpName);
	unsigned **Bmp32Mtx = new unsigned *[bmp256Data.bmpHeight];
	//这里需要处理一下colorTable
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
	// 开始写数据
	// 新建结果位图文件
	FILE *outBmp = fopen(destBmpName, "wb");
	if (outBmp == NULL) 
	{
		printf("file not found!");
		return false;
	}
	// 读取位图基本信息
	BITMAPFILEHEADER BmpFileHeader = bmp256Data.fileHeader;
	BITMAPINFOHEADER BmpInfoHeader = bmp256Data.infoHeader;
	BmpFileHeader.bfOffBits = 54;
	BmpFileHeader.bfSize = sizeof(unsigned)*(bmp256Data.bmpHeight*bmp256Data.bmpWidth)+54;
	fwrite(&BmpFileHeader, sizeof(BITMAPFILEHEADER), 1, outBmp);
	BmpInfoHeader.biBitCount = 32;
	fwrite(&BmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, outBmp);	
		// 写入结果
	for (int i = 0; i<bmp256Data.bmpHeight; i++)
	{
		fwrite(Bmp32Mtx[i], sizeof(unsigned), bmp256Data.bmpWidth,outBmp); 
	}
	// 清理内存、关闭文件
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



// 读入bmp文件
BmpData& readBmp(const char* srcBmpName){
	//1.打开源文件
	FILE* file = fopen(srcBmpName, "rb");
	if (file == NULL) 
	{
		printf("file not found!");
		BmpData b;
		return b;
	}
	//1.1读取文件头
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;
	unsigned char colorTable[256*4];
	fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, file);
	fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, file);
	//高度和宽度
	int bmpWidth = infoHeader.biWidth;
	int bmpHeight = infoHeader.biHeight;
	//自己来控制析构
	BmpData* bmpData = new BmpData(bmpWidth, bmpHeight);
	fread(bmpData->colorTable, 1024, 1, file);
	bmpData->fileHeader = fileHeader;
	bmpData->infoHeader = infoHeader;
	bmpData->bmpHeight = bmpHeight;
	bmpData->bmpWidth = bmpWidth;
	// 因为int是四个字节，所以这里需要是4的倍数
	bmpData->matrixWidth = (bmpWidth+3)/4*4;
	bmpData->matrixHeight = (bmpHeight+3)/4*4;
	//读取位图数据
	for(unsigned y=0;y<bmpHeight;y++){
		fread(bmpData->data[y], sizeof(char), bmpWidth, file);
	}
	return *bmpData;
}

// 写bmp文件
bool writeBmp(const char* destBmpName, BmpData& bmpData){
	// 5. 新建结果位图文件
	FILE *outFile = fopen(destBmpName, "wb");
	if (outFile == NULL) 
	{
		printf("file not found!");
		return false;
	}
	fwrite(&bmpData.fileHeader, sizeof(BITMAPFILEHEADER), 1, outFile);
	fwrite(&bmpData.infoHeader, sizeof(BITMAPINFOHEADER), 1, outFile);
	fwrite(bmpData.colorTable, 1024, 1, outFile);
	// 6. 写入结果
	for (int i = 0; i<bmpData.matrixHeight; i++)
	{
		fwrite(bmpData.data[i], 1, bmpData.matrixWidth, outFile); 
	}
}