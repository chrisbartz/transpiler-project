#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "token.h"

using namespace std;

#ifndef _TESTSCANNER_H
#define _TESTSCANNER_H

class testScanner
{
private:
	string fileName;
	int debug;
	char * charPtr;
	string allData[100];
	int allDataRow = 0;
	int allDataSize = 0;
	vector<token> tokens;
public:
	testScanner();
	testScanner(string fileName, int debug);
	int run();
	char getNextChar();
	int getLineNumber();
	void setTokenVector(vector<token> tokensIn);
	vector<token> getTokenVector();
	token nextToken();
	~testScanner();
};

#endif

