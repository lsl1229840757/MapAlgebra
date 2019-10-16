#include "BmpData.h"

BmpData::BmpData(const BmpData& bmpData){
	this->matrixWidth = bmpData.matrixWidth;
	this->matrixHeight = bmpData.matrixHeight;
	this->bmpHeight = bmpData.bmpHeight;
	this->bmpWidth = bmpData.bmpWidth;
	this->fileHeader = bmpData.fileHeader;
	this->infoHeader = bmpData.infoHeader;
	//��������
	for(int i=0;i<1024;i++){
		this->colorTable[i] = bmpData.colorTable[i];
	}
	//��������
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
	data = new unsigned char*[height]; //ָ����ָ���ָ��
	for(unsigned y=0;y<height;y++){
		data[y] = new unsigned char[width];
	}
}

BmpData::~BmpData(void)
{
	//��������
	for(int i=0;i<matrixHeight;i++){
		delete[] data[i];
		data[i] = NULL;
	}
	delete[] data;
	data = NULL;
}
