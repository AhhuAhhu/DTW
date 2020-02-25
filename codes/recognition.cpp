#include "stdafx.h"
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;

const int MAXNFRAME = 300; // max number of frames 一段音频最多允许300帧，即约3000ms
const int DVECTOR = 39; // dimension of a frame 每一帧用一个39维MFCC向量表示

#include "binaryRead.h"
#include "recognition.h"

float TemplateVectors[10][MAXNFRAME + 1][DVECTOR + 1]; // 存储10个template的帧数据
int NFTemplate[10]; // number of frames in each template vector 存储10个template各自的帧数

void main(int argc, char *argv[])
{
	if (1 == argc)
	{
		cout << "USAGE:\t TestFileList \n";
		exit(-1);
	}

	string TestFileList = argv[1]; // test list 测试数据列表

								   /*----------------读取模板数据---------------------*/
	string TemplateFolder = "template";
	for (int i = 0; i<10; i++)
	{
		ostringstream number;
		number << i;
		string template_file = TemplateFolder + '\\' + number.str() + "_template.mfc";
		NFTemplate[i] = ReadHtk(template_file, TemplateVectors[i]);
	}

	/*----------------读取测试数据并分别与模板进行匹配---------------*/
	string *fileInfos = new string[100];
	int nfiles = get_strs_from_file(TestFileList, fileInfos); // 读取测试文件列表，存入fileInfos矩阵
	float TestVector[MAXNFRAME + 1][DVECTOR + 1]; // 存储test的帧数据
	int NFTest = 0; // 存储test的帧数
	for (int j = 1; j <= nfiles; j++)
	{
		float re = 0;
		int k = 1;
		float result = 1e+38f; //表示最大的数 
		NFTest = ReadHtk(fileInfos[j], TestVector);
		for (int i = 0; i<10; i++)
		{// 将测试数据依次与10个模板进行匹配，记录匹配距离最小的那一个
			re = Run_DTW(TemplateVectors[i], NFTemplate[i], TestVector, NFTest);
			if (result > re)
			{
				result = re;
				k = i;
			}
		}
		cout << fileInfos[j] << '\t' << k << endl;
	}
	delete[] fileInfos;
}