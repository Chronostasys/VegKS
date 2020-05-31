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
		PrimaryNode* prevVegClass;
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
	SecondaryNode* prevVegInfo;
};
DllExport
struct TertiaryNode
{
	int id;
	int area;
	float weight;
	char year[5];
	TertiaryNode* nextVeg;
	TertiaryNode* prevVeg;
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
	TertiaryNode* While(std::function<bool(TertiaryNode*)> lambda) {
		TertiaryNode* current2 = head;
		while (true)
		{
			current2 = current2->nextVeg;
			if (current2 == nullptr)
			{
				break;
			}
			if (!lambda(current2))
			{
				if (current2->nextVeg==nullptr)
				{
					current2->prevVeg->nextVeg = nullptr;
					break;
				}
				current2->prevVeg->nextVeg = current2->nextVeg;
				current2->nextVeg->prevVeg = current2->prevVeg;
			}
		}
		return head->nextVeg;
	}
	void Remove(int pos) {
		count--;
		if (pos == 0)
		{
			TertiaryNode* del = head->nextVeg;
			head->nextVeg = head->nextVeg->nextVeg;
			head->nextVeg->prevVeg = head;
			free(del);
			return;
		}

		TertiaryNode* prev = ElementAt(pos - 1);
		TertiaryNode* del = prev->nextVeg;
		prev->nextVeg = prev->nextVeg->nextVeg;
		prev->nextVeg->prevVeg = prev;
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
				current->nextVeg->prevVeg = current;
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
				current->nextVegInfo->prevVegInfo = current;
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
				current->nextVegInfo->prevVegInfo = current;
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
	SecondaryNode* While(std::function<bool(SecondaryNode*)> lambda) {
		SecondaryNode* current1 = head;
		while (true)
		{
			current1 = current1->nextVegInfo;
			if (current1==nullptr)
			{
				break;
			}
			if (!lambda(current1))
			{
				if (current1->nextVegInfo==nullptr)
				{
					current1->prevVegInfo->nextVegInfo = nullptr;
					break;
				}
				else
				{
					current1->nextVegInfo->prevVegInfo = current1->prevVegInfo;
					current1->prevVegInfo->nextVegInfo = current1->nextVegInfo;
				}
			}
		}
		return head->nextVegInfo;
	}

	void Remove(int pos) {
		count--;
		if (pos == 0)
		{
			SecondaryNode* del = head->nextVegInfo;
			head->nextVegInfo = head->nextVegInfo->nextVegInfo;
			head->nextVegInfo->prevVegInfo = head;
			free(del);
			return;
		}

		SecondaryNode* prev = ElementAt(pos - 1);
		SecondaryNode* del = prev->nextVegInfo;
		prev->nextVegInfo = prev->nextVegInfo->nextVegInfo;
		prev->prevVegInfo = prev;
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
				current->nextVegClass->prevVegClass = current;
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
	PrimaryNode* While(std::function<bool(PrimaryNode*)> lambda) {
		PrimaryNode* current = head;
		while (true)
		{
			current = current->nextVegClass;
			if (current == nullptr)
			{
				break;
			}
			if (!lambda(current))
			{
				if (current->nextVegClass==nullptr)
				{
					current->prevVegClass->nextVegClass = nullptr;
					break;
				}
				else
				{
					current->prevVegClass->nextVegClass = current->nextVegClass;
					current->nextVegClass->prevVegClass = current->prevVegClass;
				}
			}
		}
		return head->nextVegClass;
	}
	void Remove(int pos) {
		count--;
		if (pos == 0)
		{
			PrimaryNode* del = head->nextVegClass;
			head->nextVegClass = head->nextVegClass->nextVegClass;
			head->nextVegClass->prevVegClass = head;
			free(del);
			return;
		}

		PrimaryNode* prev = ElementAt(pos - 1);
		PrimaryNode* del = prev->nextVegClass;
		prev->nextVegClass = prev->nextVegClass->nextVegClass;
		prev->nextVegClass->prevVegClass = prev;
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