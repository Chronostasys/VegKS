// VegDatabaseCpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include "CSVProcess.h"
#include <string.h>

struct Heads
{
    PrimaryNode* pnode;
    SecondaryNode* snode;
    TertiaryNode* tnode;
};


bool contains(const char* str, const char* substr) {
    int len = strlen(str);
    int sublen = strlen(substr);
    if (sublen==0)
    {
        return true;
    }
    for (int i = 0; i <= len - sublen; i++)
    {
        for (int j = 0; j < sublen; j++)
        {
            if (str[i + j] != substr[j]) {
                break;
            }
            if (j==sublen-1)
            {
                return true;
            }
        }
    }
    return false;
}

extern "C"{
    DllExport PrimaryNode* GetPrimary() {
        auto plist = PrimaryList();
        readCsv(&plist);
        return plist.head->nextVegClass;
    }
    DllExport PrimaryNode* NextPrimary(PrimaryNode* node) {
        return node->nextVegClass;
    }
    DllExport char GetPrimaryId(PrimaryNode* node) {
        return node->classId;
    }
    DllExport char* GetPrimaryName(PrimaryNode* node) {
        return node->className;
    }
    DllExport SecondaryNode* GetSecondary(PrimaryNode* node) {
        return node->vegInfos->head->nextVegInfo;
    }
    DllExport SecondaryNode* NextSecondary(SecondaryNode* node) {
        return node->nextVegInfo;
    }
    DllExport int GetSecondaryId(SecondaryNode* node) {
        return node->vegId;
    }
    DllExport char* GetSecondaryName(SecondaryNode* node) {
        return node->vegName;
    }
    DllExport char* GetSecondaryNutrition(SecondaryNode* node) {
        return node->nutrition;
    }
    DllExport TertiaryNode* GetTertiary(SecondaryNode* node) {
        return node->vegs->head->nextVeg;
    }
    DllExport TertiaryNode* NextTertiary(TertiaryNode* node) {
        return node->nextVeg;
    }
    DllExport int GetTertiaryId(TertiaryNode* node) {
        return node->id;
    }
    DllExport int GetTertiaryArea(TertiaryNode* node) {
        return node->area;
    }
    DllExport char* GetTertiaryYear(TertiaryNode* node) {
        return node->year;
    }
    DllExport float GetTertiaryWeight(TertiaryNode* node) {
        return node->weight;
    }
    DllExport Heads* Search(char* key, PrimaryNode* p) {
        //FILE* f = fopen("string.txt", "r");
        //char a1[100];
        //fgets(a1, 100, f);
        //fflush(f);
        //fclose(f);

        FILE* f = fopen("string.txt", "w+");
        fputs(key, f);
        fflush(f);
        fclose(f);
        //key = toUTF8(key);
        auto headT = TertiaryNode();
        auto currentT = &headT;
        auto headS = SecondaryNode();
        auto currentS = &headS;
        auto plist1 = PrimaryList();
        plist1.head->nextVegClass = p;
        p->prevVegClass = plist1.head;
        std::function<bool(TertiaryNode*)> func3 = [&](TertiaryNode* node) {
            char a[100];
            auto b = contains(itoa(node->area, a, 10), key)
                || contains(itoa(node->weight, a, 10), key)
                || contains(gcvt(node->area, 10, a), key)
                || contains(node->year, key);
            if (b)
            {
                currentT->nextVeg = node;
                currentT = currentT->nextVeg;
            }
            return b;
        };
        std::function<bool(SecondaryNode*)> func2 = [&](SecondaryNode* node) {
            char a[100];
            auto inner = node->vegs->While(func3);
            auto b = contains(node->nutrition, key)
                || contains(node->vegName, key)
                || contains(itoa(node->vegId, a, 10), key);
            if (b)
            {
                currentS->nextVegInfo = node;
                currentS = currentS->nextVegInfo;
            }
            return b;
        };
        std::function<bool(PrimaryNode*)> func1 = [&](PrimaryNode* node) {
            auto inner = node->vegInfos->While(func2);
            return node->classId == key[0]
                || contains(node->className, key);
        };
        Heads* heads = (Heads*)malloc(100);
        heads->pnode = plist1.While(func1);
        currentS->nextVegInfo = nullptr;
        currentT->nextVeg = nullptr;
        heads->snode = headS.nextVegInfo;
        heads->tnode = headT.nextVeg;
        return heads;
    }
    DllExport PrimaryNode* GetPSearch(Heads* h) {
        return h->pnode;
    }
    DllExport SecondaryNode* GetSSearch(Heads* h) {
        return h->snode;
    }
    DllExport TertiaryNode* GetTSearch(Heads* h) {
        return h->tnode;
    }
    DllExport PrimaryNode* EditS(PrimaryNode* p, int vegId, char* vegName, char* nutrition, char classid) {
        auto plist1 = PrimaryList();
        plist1.head->nextVegClass = p;
        p->prevVegClass = plist1.head;
        SecondaryNode* snode;
        plist1.While([&](PrimaryNode* p) {
            p->vegInfos->While([&](SecondaryNode* s) {
                if (s->vegId == vegId) {
                    snode = s;
                    return false;
                }
                return true;
            });
            return true;
        });
        auto node = plist1.Where([&](PrimaryNode* p) {
            return p->classId == classid;
        });
        node->vegInfos->Add(vegId, vegName, nutrition);
        
        SavePrimaryCsv(&plist1);
        return p;
    }
    DllExport PrimaryNode* EditT(PrimaryNode* p, int id, int area, float weight, char* year) {
        auto plist1 = PrimaryList();
        plist1.head->nextVegClass = p;
        p->prevVegClass = plist1.head;
        SecondaryNode* snode;
        TertiaryNode* tnode;
        bool found = false;
        plist1.While([&](PrimaryNode* p) {
            p->vegInfos->While([&](SecondaryNode* s) {
                s->vegs->While([&](TertiaryNode* t) {
                    if (t->id==id)
                    {
                        tnode = t;
                        found = true;
                        return false;
                    }
                    return true;
                });
                if (found)
                {
                    snode = s;
                    found = false;
                }
                return true;
            });
            return true;
        });
        snode->vegs->Add(id, area, weight, year);

        SavePrimaryCsv(&plist1);
        return p;
    }
    DllExport PrimaryNode* EditP(PrimaryNode* p, char id, char* name) {
        auto plist1 = PrimaryList();
        plist1.head->nextVegClass = p;
        p->prevVegClass = plist1.head;
        auto node = plist1.Where([&](PrimaryNode* p) {
            return p->classId == id;
            });
        strcpy(node->className, name);
        SavePrimaryCsv(&plist1);
        return p;
    }
    DllExport PrimaryNode* DelP(PrimaryNode* p, char id) {
        auto plist1 = PrimaryList();
        plist1.head->nextVegClass = p;
        p->prevVegClass = plist1.head;
        plist1.While([&](PrimaryNode* p) {
                return p->classId != id;
            });
        SavePrimaryCsv(&plist1);
        return p;
    }
    DllExport PrimaryNode* DelT(PrimaryNode* p, int id) {
        auto plist1 = PrimaryList();
        plist1.head->nextVegClass = p;
        p->prevVegClass = plist1.head;
        plist1.While([&](PrimaryNode* p) {
            p->vegInfos->While([&](SecondaryNode* s) {
                s->vegs->While([&](TertiaryNode* t) {
                    if (t->id == id)
                    {
                        return false;
                    }
                    return true;
                    });
                return true;
                });
            return true;
            });

        SavePrimaryCsv(&plist1);
        return p;
    }
    DllExport PrimaryNode* DelS(PrimaryNode* p, int vegId) {
        auto plist1 = PrimaryList();
        plist1.head->nextVegClass = p;
        p->prevVegClass = plist1.head;
        plist1.While([&](PrimaryNode* p) {
            p->vegInfos->While([&](SecondaryNode* s) {
                if (s->vegId == vegId) {
                    return false;
                }
                return true;
                });
            return true;
            });

        SavePrimaryCsv(&plist1);
        return p;
    }
    DllExport PrimaryNode* AddP(PrimaryNode* p, char id, char* name) {
        auto plist1 = PrimaryList();
        plist1.head->nextVegClass = p;
        plist1.Add(id, name);
        SavePrimaryCsv(&plist1);
        return p;
    }
    DllExport PrimaryNode* AddS(PrimaryNode* p, int vegId, char* vegName, char* nutrition, char classid) {
        auto plist1 = PrimaryList();
        plist1.head->nextVegClass = p;
        p->prevVegClass = plist1.head;
        auto node = plist1.Where([&](PrimaryNode* p) {
            return p->classId == classid;
        });
        node->vegInfos->Add(vegId, vegName, nutrition);

        SavePrimaryCsv(&plist1);
        return p;
    }
    DllExport PrimaryNode* AddT(PrimaryNode* p, int id, int area, float weight, char* year, int vegId) {
        auto plist1 = PrimaryList();
        plist1.head->nextVegClass = p;
        p->prevVegClass = plist1.head;
        SecondaryNode* snode;
        plist1.While([&](PrimaryNode* p) {
            p->vegInfos->While([&](SecondaryNode* s) {
                if (s->vegId==vegId)
                {
                    snode = s;
                }
                return true;
            });
            return true;
            });
        snode->vegs->Add(id, area, weight, year);

        SavePrimaryCsv(&plist1);
        return p;
    }
}
int main()
{

    auto plist = PrimaryList();
    readCsv(&plist);
    //plist.Add('1', "aaaaa");
    //auto e = plist.ElementAt(0);
    //e->vegInfos->Add("111", "222");
    //auto e1 = plist.ElementAt(0)->vegInfos->ElementAt(0);
    //e1->vegs->Add(1, 1, "2222");
    //auto e2 = plist.ElementAt(0)->vegInfos->ElementAt(0)->vegs->ElementAt(0);
    //plist.Remove(0);
    //auto b = 0;
    //printf_s("aaa");
    //SavePrimaryCsv(&plist);
    char a[10] = "µ°°×ÖÊ";
    FILE* f = fopen("string.txt", "r");
    char a1[100];
    fgets(a1, 100, f);
    fflush(f);
    fclose(f);
    auto l2 = Search(a, plist.head->nextVegClass);
    auto l1 = GetPSearch(l2);
}




// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
