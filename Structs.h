#include <stdlib.h>  
#include<string.h>
#include<functional>

struct PrimaryList;
struct SecondaryList;
struct TertiaryList;
struct PrimaryNode;
struct SecondaryNode;
struct TertiaryNode;


#define DllExport __declspec(dllexport)
int vegGid = 0;
int	plantGid = 0;
extern "C" {
	DllExport struct PrimaryNode
	{
		char classId;
		char className[8];
		PrimaryNode* nextVegClass;
		SecondaryList* vegInfos;
	};
}

DllExport
struct SecondaryNode
{
	int vegId;
	char vegName[20];
	char nutrition[50];
	SecondaryNode* nextVegInfo;
	TertiaryList* vegs;
};
DllExport
struct TertiaryNode
{
	int id;
	int area;
	float weight;
	char year[5];
	TertiaryNode* nextVeg;
};
struct TertiaryList
{
	TertiaryNode* head;
	int count = 0;
	void init() {
		head = MallocTertiaryNode();
		count = 0;
	}
	TertiaryNode* Where(std::function<bool(TertiaryNode*)> lambda) {
		TertiaryNode* current = head;
		while (true)
		{
			current = current->nextVeg;
			if (current == nullptr)
			{
				return nullptr;
			}
			else if (lambda(current))
			{
				return current;
			}
		}
		return current;
	}
	void Remove(int pos) {
		count--;
		if (pos == 0)
		{
			TertiaryNode* del = head->nextVeg;
			head->nextVeg = head->nextVeg->nextVeg;
			free(del);
			return;
		}

		TertiaryNode* prev = ElementAt(pos - 1);
		TertiaryNode* del = prev->nextVeg;
		prev->nextVeg = prev->nextVeg->nextVeg;
		free(del);
	}
	void Add(int area, float weight, const char* year) {
		TertiaryNode* current = this->head;
		while (true)
		{
			if (current->nextVeg == nullptr)
			{
				current->nextVeg = MallocTertiaryNode(plantGid+1, area, weight, year);
				current->nextVeg->nextVeg = nullptr;
				plantGid++;
				break;
			}
			current = current->nextVeg;
		}
		count++;
	}

	TertiaryNode* ElementAt(int pos) {
		TertiaryNode* current = head;
		while (true)
		{
			current = current->nextVeg;
			if (pos == 0)
			{
				break;
			}
			pos--;
		}
		return current;
	}



	TertiaryNode* MallocTertiaryNode(int id, int area, float weight, const char* year) {
		TertiaryNode* node = (TertiaryNode*)malloc(sizeof(TertiaryNode));
		node->area = area;
		node->id = id;
		node->weight = weight;
		node->nextVeg = nullptr;
		int len = strlen(year);
		for (size_t i = 0; i < len + 1; i++)
		{
			node->year[i] = year[i];
		}
		return node;
	}
	TertiaryNode* MallocTertiaryNode() {
		TertiaryNode* node = (TertiaryNode*)malloc(sizeof(TertiaryNode)*2);
		node->nextVeg = nullptr;
		return node;
	}
};

struct SecondaryList
{
	SecondaryNode* head;
	int count = 0;
	void init() {
		head = MallocSecondaryNode();
		count = 0;
	}

	void Add(const char* vegName, const char* nutrtion) {
		SecondaryNode* current = this->head;
		while (true)
		{
			if (current->nextVegInfo == nullptr)
			{
				current->nextVegInfo = MallocSecondaryNode(vegGid+1, vegName, nutrtion);
				current->nextVegInfo->vegs = (TertiaryList*)malloc(sizeof(TertiaryList) * 2);
				current->nextVegInfo->vegs->init();
				current->nextVegInfo->nextVegInfo = nullptr;
				vegGid++;
				break;
			}
			current = current->nextVegInfo;
		}
		count++;
	}

	void Add(int gid,const char* vegName, const char* nutrtion) {
		SecondaryNode* current = this->head;
		while (true)
		{
			if (current->nextVegInfo == nullptr)
			{
				current->nextVegInfo = MallocSecondaryNode(gid, vegName, nutrtion);
				current->nextVegInfo->vegs = (TertiaryList*)malloc(sizeof(TertiaryList) * 2);
				current->nextVegInfo->vegs->init();
				current->nextVegInfo->nextVegInfo = nullptr;
				vegGid++;
				break;
			}
			current = current->nextVegInfo;
		}
		count++;
	}
	SecondaryNode* Where(std::function<bool(SecondaryNode*)> lambda) {
		SecondaryNode* current = head;
		while (true)
		{
			current = current->nextVegInfo;
			if (current == nullptr)
			{
				return nullptr;
			}
			else if (lambda(current))
			{
				return current;
			}
		}
		return current;
	}

	void Remove(int pos) {
		count--;
		if (pos == 0)
		{
			SecondaryNode* del = head->nextVegInfo;
			head->nextVegInfo = head->nextVegInfo->nextVegInfo;
			free(del);
			return;
		}

		SecondaryNode* prev = ElementAt(pos - 1);
		SecondaryNode* del = prev->nextVegInfo;
		prev->nextVegInfo = prev->nextVegInfo->nextVegInfo;
		free(del);
	}

	SecondaryNode* MallocSecondaryNode(int vegId, const char* vegName, const char* nutrtion) {

		SecondaryNode* node = (SecondaryNode*)malloc(sizeof(SecondaryNode)*2);
		node->vegId = vegId;
		node->nextVegInfo = nullptr;
		node->vegs = (TertiaryList*)malloc(sizeof(TertiaryList)*2);
		node->vegs->init();
		int len = strlen(vegName);
		for (size_t i = 0; i < len + 1; i++)
		{
			node->vegName[i] = vegName[i];
		}
		len = strlen(nutrtion);
		for (size_t i = 0; i < len + 1; i++)
		{
			node->nutrition[i] = nutrtion[i];
		}
		return node;
	}

	SecondaryNode* ElementAt(int pos) {
		SecondaryNode* current = head;
		while (true)
		{
			current = current->nextVegInfo;
			if (pos == 0)
			{
				break;
			}
			pos--;
		}
		return current;
	}

	SecondaryNode* MallocSecondaryNode() {

		SecondaryNode* node = (SecondaryNode*)malloc(sizeof(SecondaryNode));
		node->nextVegInfo = nullptr;
		node->vegs = nullptr;
		return node;
	}
};
struct PrimaryList
{
	PrimaryNode* head;
	int count = 0;
	PrimaryList() {
		this->head = MallocPrimaryNode('\0', nullptr);
		count = 0;
	}
	void Add(char classId, const char* className) {
		PrimaryNode* current = this->head;
		while (true)
		{
			if (current->nextVegClass == nullptr)
			{
				current->nextVegClass = MallocPrimaryNode(classId, className);
				break;
			}
			current = current->nextVegClass;
		}
		count++;
	}
	PrimaryNode* Where(std::function<bool(PrimaryNode*)> lambda) {
		PrimaryNode* current = head;
		while (true)
		{
			current = current->nextVegClass;
			if (current == nullptr)
			{
				return nullptr;
			}
			else if (lambda(current))
			{
				return current;
			}
		}
		return current;
	}
	void Remove(int pos) {
		count--;
		if (pos == 0)
		{
			PrimaryNode* del = head->nextVegClass;
			head->nextVegClass = head->nextVegClass->nextVegClass;
			free(del);
			return;
		}

		PrimaryNode* prev = ElementAt(pos - 1);
		PrimaryNode* del = prev->nextVegClass;
		prev->nextVegClass = prev->nextVegClass->nextVegClass;
		free(del);

	}
	PrimaryNode* ElementAt(int pos) {
		PrimaryNode* current = head;
		while (true)
		{
			current = current->nextVegClass;
			if (pos == 0)
			{
				break;
			}
			pos--;
		}
		return current;
	}

	PrimaryNode* MallocPrimaryNode(char classId, const char* className) {

		PrimaryNode* node = (PrimaryNode*)malloc(sizeof(PrimaryNode));
		node->classId = classId;
		node->nextVegClass = nullptr;
		node->vegInfos = (SecondaryList*)malloc(sizeof(SecondaryList));
		node->vegInfos->init();
		if (className == nullptr)
		{
			return node;
		}
		int len = strlen(className);
		for (size_t i = 0; i < len + 1; i++)
		{
			node->className[i] = className[i];
		}
		return node;
	}
};