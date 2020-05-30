// VegDatabaseCpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include "CSVProcess.h"
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
    SavePrimaryCsv(&plist);
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
