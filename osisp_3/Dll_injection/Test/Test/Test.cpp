// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>


void printHelloWorld(){
	printf("Hello World");
}

int _tmain(int argc, _TCHAR* argv[])
{
	char* string_test = new char[50];
	strcpy(string_test, "some_text");
    while (1)
    {
		printf(string_test);
		printf("\n\r");
		Sleep(1000);
    }
    return 0;
}

