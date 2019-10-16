#include "BmpData.h"

BmpData::BmpData(const BmpData& bmpData){
	this->matrixWidth = bmpData.matrixWidth;
	this->matrixHeight = bmpData.matrixHeight;
	this->bmpHeight = bmpData.bmpHeight;
	this->bmpWidth = bmpData.bmpWidth;
	this->fileHeader = bmpData.fileHeader;
	this->infoHeader = bmpData.infoHeader;
	//拷贝数组
	for(int i=0;i<1024;i++){
		this->colorTable[i] = bmpData.colorTable[i];
	}
	//拷贝数据
	this->data = new unsigned char*[bmpData.matrixHeight];
	for(int i=0;i<bmpData.matrixHeight;i++){
		this->data[i] = new unsigned char [bmpData.matrixWidth];
		for(int j=0;j<bmpData.matrixWidth;j++){
			this->data[i][j] = bmpData.data[i][j];
		}
	}
}

BmpData::BmpData(void)
{
	matrixWidth = 0;
	matrixHeight = 0;
}

BmpData::BmpData(int width, int height)
{
	this->matrixWidth = width;
	this->matrixHeight = height;
	data = new unsigned char*[height]; //指向行指针的指针
	for(unsigned y=0;y<height;y++){
		data[y] = new unsigned char[width];
	}
}

BmpData::~BmpData(void)
{
	//析构数据
	for(int i=0;i<matrixHeight;i++){
		delete[] data[i];
		data[i] = NULL;
	}
	delete[] data;
	data = NULL;
}
