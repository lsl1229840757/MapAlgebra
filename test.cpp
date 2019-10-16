#include<stdio.h>
#include "MA.h"
#include "BmpData.h"
int main(){
	const char * srcFileName="C:\\Users\\Administrator\\Desktop\\test.bmp";
	const char * destFileName="C:\\Users\\Administrator\\Desktop\\result2.bmp";
	//BmpData bmpData = readBmp(fileName);
	//bool a = bmpOverlay(fileName,fileName);
	//bmpReverse(srcFileName, destFileName);
	bmp256To32(srcFileName, destFileName);
	printf("a");
}