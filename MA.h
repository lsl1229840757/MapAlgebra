#ifndef MA_H
#define MA_H
#include "BmpData.h"
//反色处理：255-当前颜色
bool bmpReverse(const char* srcBmpName, const char* destBmpName);
// 叠置处理
bool bmpOverlay(const char* srcBmpName,double a, double b, const char* destBmpName);
// 聚焦函数——平滑处理
bool bmpSmooth(const char* srcBmpName, const char* destBmpName);
//   读取文件
BmpData& readBmp(const char* srcBmpName);
// 写文件
bool writeBmp(const char* destBmpName, BmpData& bmpData);
// 256色转32位
bool bmp256To32(const char* srcBmpName, const char* destBmpName);

#endif