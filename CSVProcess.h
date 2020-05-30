#pragma once
#pragma warning(push)
#pragma warning(disable: 4996)
#include "Structs.h"
#include <string>
#include <codecvt>
#include <stdio.h>

void ReadSecondaryCsv(PrimaryList* plist);
void ReadStrings(char* str, char** classes, int* i);
void ReadTertiaryCSV(PrimaryList* plist);
void SavePrimaryCsv(PrimaryList* plist);
void SaveStrings(char* str, char** classes, int n);
void SaveSecondaryCsv(PrimaryList* plist);
void SaveTertiaryCsv(PrimaryList* plist);


//utf-8 padding开头
char* padding = (char*)malloc(sizeof(char) * 100);
char* head2 = (char*)malloc(sizeof(char) * 100);
char* head3 = (char*)malloc(sizeof(char) * 100);

void readCsv(PrimaryList* plist) {
	FILE* f = fopen("C:/Users/17697/Desktop/蔬菜种类信息表1.csv", "r");
	fflush(f);
	char** classes = (char**)malloc(sizeof(char*) * 100);
	char** classIds = (char**)malloc(sizeof(char*) * 100);
	char* del = (char*)malloc(sizeof(char) * 100);
	//去掉utf-8开头的编码
	fgets(del, 4, f);
	strcpy(padding, del);
	fgets(del, 1000, f);
	int i1 = 0;
	ReadStrings(del, classIds, &i1);
	char* str = (char*)malloc(sizeof(char) * 100);
	fflush(f);
	fgets(str, 1000, f);
	int i = 0;
	ReadStrings(str, classes, &i);
	for (size_t j = 0; j < i; j++)
	{
		plist->Add(classIds[j][0], classes[j]);
	}
	//for (size_t j = 0; j < i; j++)
	//{
	//	free(classes[j]);
	//	free(classIds[j]);
	//}
	free(classes);
	free(classIds);
	free(del);
	free(str);
	fclose(f);
	ReadSecondaryCsv(plist);
	ReadTertiaryCSV(plist);
}
/// <summary>
/// 把csv字符串分解成独立的元素
/// </summary>
/// <param name="str">源csv的字符串</param>
/// <param name="classes">以字符串格式读取出来的元素</param>
/// <param name="i">读出来的元素个数</param>
void ReadStrings(char* str, char** classes, int* i) {
	char* temp = (char*)malloc(sizeof(char) * 100);
	*i = 0;
	int i1 = 0;
	int i2 = 0;
	while (true)
	{
		if (str[i1] == ',')
		{
			i1++;
			temp[i2] = '\0';
			i2 = 0;
			int tt = *i;

			classes[tt] = (char*)malloc(sizeof(char) * 100);
			strcpy(classes[tt], temp);
			(*i)++;
			continue;
		}
		if (str[i1] == '\n')
		{
			temp[i2] = '\0';
			classes[*i] = (char*)malloc(sizeof(char) * 100);
			strcpy(classes[*i], temp);
			(*i)++;
			break;
		}
		temp[i2] = str[i1];
		i1++;
		i2++;
	}
	free(temp);
}
/// <summary>
/// 
/// </summary>
/// <param name="str"></param>
/// <param name="classes"></param>
/// <param name="n">classes 长度</param>
void SaveStrings(char* str, char** classes, int n) {
	char* temp = (char*)malloc(sizeof(char) * 1000);
	temp[0] = '\0';
	for (int i = 0; i < n; i++)
	{
		if (i!=0)
		{
			temp = strcat(temp, ",");
		}
		temp = strcat(temp, classes[i]);

	}
	temp = strcat(temp, "\n");
	strcpy(str, temp);
	free(temp);
}

void ReadSecondaryCsv(PrimaryList* plist) {
	FILE* f = fopen("C:/Users/17697/Desktop/蔬菜基本信息表1.csv", "r");
	fflush(f);
	char* del = (char*)malloc(sizeof(char) * 100);
	//去掉utf-8开头的编码
	fgets(del, 4, f);
	fgets(del, 1000, f);
	strcpy(head2, del);
	char* str = (char*)malloc(sizeof(char) * 100);
	fflush(f);
	char* classes[100];
	char* end;
	while (true)
	{
		end = fgets(str, 1000, f);
		if (end == nullptr)
		{
			break;
		}
		//free(end);
		int i = 0;
		ReadStrings(str, classes, &i);
		std::function<bool(PrimaryNode*)> func = [&](PrimaryNode* node) {
			return node->classId == classes[2][0];
		};
		PrimaryNode* node = plist->Where(func);
		node->vegInfos->Add(atoi(classes[0]) ,classes[1], classes[3]);
		for (size_t j = 0; j < i; j++)
		{
			free(classes[j]);
		}
		//free(end);
	}
	free(end);
	//free(classes);
	free(str);
	free(del);
	fclose(f);
}

void ReadTertiaryCSV(PrimaryList* plist) {
	FILE* f = fopen("C:/Users/17697/Desktop/菜农种植信息表1.csv", "r");
	fflush(f);
	char* del = (char*)malloc(sizeof(char) * 100);
	//去掉utf-8开头的编码
	fgets(del, 4, f);
	fgets(del, 1000, f);
	strcpy(head3, del);
	char* str = (char*)malloc(sizeof(char) * 100);
	fflush(f);
	char* classes[100];
	char* end;
	while (true)
	{
		end = fgets(str, 100, f);
		if (end == nullptr)
		{
			break;
		}
		//free(end);
		int i = 0;
		ReadStrings(str, classes, &i);
		auto secfunc = [&](SecondaryNode* node) {
			return node->vegId == atoi(classes[1]);
		};
		std::function<bool(PrimaryNode*)> func = [&](PrimaryNode* node) {
			auto re = node->vegInfos->Where(secfunc);
			return re != nullptr;
		};
		PrimaryNode* node = plist->Where(func);
		SecondaryNode* secNode = node->vegInfos->Where(secfunc);
		secNode->vegs->Add(atoi(classes[2]), atof(classes[3]), classes[4]);
		for (size_t j = 0; j < i; j++)
		{
			free(classes[j]);
		}
		//free(end);
	}
	free(end);
	//free(classes);
	free(str);
	free(del);
	fclose(f);
}

void SavePrimaryCsv(PrimaryList* plist)
{
	FILE* f = fopen("C:/Users/17697/Desktop/蔬菜种类信息表1.csv", "w+");
	fflush(f);
	//char* dest = nullptr;
	PrimaryNode* node = plist->head;
	char* temp[10][10];
	int n = 0;
	while (true)
	{
		if (node->nextVegClass!=nullptr)
		{
			node = node->nextVegClass;
		}
		else
		{
			break;
		}
		temp[0][n] = (char*)malloc(sizeof(char) * 3);
		temp[0][n][0] = node->classId;
		temp[0][n][1] = '\0';
		temp[1][n] = (char*)malloc(sizeof(char) * 3);
		temp[1][n] = node->className;
		n++;
	}
	char* dest = (char*)malloc(sizeof(char) * 100);
	for (size_t i = 0; i < 2; i++)
	{
		SaveStrings(dest, temp[i], n);
		if (i==0)
		{
			char* padtemp = (char*)malloc(sizeof(char) * 100);
			strcpy(padtemp, padding);
			strcpy(dest, strcat(padding, dest));
			strcpy(padding, padtemp);
		}
		fputs(dest, f);
		fflush(f);
	}
	fclose(f);
	SaveSecondaryCsv(plist);
	SaveTertiaryCsv(plist);
}
void SaveSecondaryCsv(PrimaryList* plist)
{
	FILE* f = fopen("C:/Users/17697/Desktop/蔬菜基本信息表1.csv", "w+");
	fflush(f);
	//char* dest = nullptr;
	PrimaryNode* node = plist->head;
	char* temp[10][10];
	int n = 0;
	while (true)
	{
		if (node->nextVegClass != nullptr)
		{
			node = node->nextVegClass;
		}
		else
		{
			break;
		}
		SecondaryNode* secnode = node->vegInfos->head;
		while (true)
		{
			if (secnode->nextVegInfo != nullptr)
			{
				secnode = secnode->nextVegInfo;
			}
			else
			{
				break;
			}
			temp[n][0] = (char*)malloc(sizeof(char) * 100);
			itoa(secnode->vegId, temp[n][0],10);
			temp[n][1] = (char*)malloc(sizeof(char) * 100);
			temp[n][1] = secnode->vegName;
			temp[n][2] = (char*)malloc(sizeof(char) * 100);
			temp[n][2][0]= node->classId;
			temp[n][2][1] = '\0';
			temp[n][3] = (char*)malloc(sizeof(char) * 100);
			temp[n][3] = secnode->nutrition;
			n++;
		}
	}
	char* padtemp = (char*)malloc(sizeof(char) * 100);
	char* dest = (char*)malloc(sizeof(char) * 100);
	strcpy(padtemp, padding);
	strcpy(dest, strcat(padding, head2));
	strcpy(padding, padtemp);
	fputs(dest, f);
	fflush(f);

	for (size_t i = 0; i < n; i++)
	{
		SaveStrings(dest, temp[i], 4);
		fputs(dest, f);
		fflush(f);
	}
	fclose(f);
}

void SaveTertiaryCsv(PrimaryList* plist)
{
	FILE* f = fopen("C:/Users/17697/Desktop/菜农种植信息表1.csv", "w+");
	fflush(f);
	//char* dest = nullptr;
	PrimaryNode* node = plist->head;
	char* temp[10][10];
	int n = 0;
	while (true)
	{
		if (node->nextVegClass != nullptr)
		{
			node = node->nextVegClass;
		}
		else
		{
			break;
		}
		SecondaryNode* secnode = node->vegInfos->head;
		while (true)
		{
			if (secnode->nextVegInfo != nullptr)
			{
				secnode = secnode->nextVegInfo;
			}
			else
			{
				break;
			}
			TertiaryNode* ternode = secnode->vegs->head;
			while (true)
			{
				if (ternode->nextVeg != nullptr)
				{
					ternode = ternode->nextVeg;
				}
				else
				{
					break;
				}
				temp[n][0] = (char*)malloc(sizeof(char) * 100);
				itoa(ternode->id, temp[n][0], 10);
				temp[n][1] = (char*)malloc(sizeof(char) * 100);
				itoa(secnode->vegId, temp[n][1], 10);
				temp[n][2] = (char*)malloc(sizeof(char) * 100);
				itoa(ternode->area, temp[n][2], 10);
				temp[n][3] = (char*)malloc(sizeof(char) * 100);
				gcvt(ternode->weight, 10, temp[n][3]);
				temp[n][4] = (char*)malloc(sizeof(char) * 100);
				strcpy(temp[n][4], ternode->year);
				n++;
			}
		}
	}
	char* padtemp = (char*)malloc(sizeof(char) * 100);
	char* dest = (char*)malloc(sizeof(char) * 100);
	strcpy(padtemp, padding);
	strcpy(dest, strcat(padding, head3));
	strcpy(padding, padtemp);
	fputs(dest, f);
	fflush(f);

	for (size_t i = 0; i < n; i++)
	{
		SaveStrings(dest, temp[i], 5);
		fputs(dest, f);
		fflush(f);
	}
	fclose(f);
}
