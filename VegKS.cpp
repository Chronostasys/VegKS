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




// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
