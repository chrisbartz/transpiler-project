#include <iostream>
#include <fstream>
#include <array>
#include <sstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>
#include "testScanner.h"
#include "token.h"
#include "tokenTyp.h"
using namespace std;

#ifndef _SCANNER_H
#define _SCANNER_H
class scanner
{
private:
	//char validChars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ=><!:+-*/.(),{};[] abcdefghijklmnopqrstuvwxyz0123456789";
	//{'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','=','>','<','!',':','+','-','*','/','.','(',')',',','{','}',';','[',']','WS','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','0','1','2','3','4','5','6','7','8','9'};
	int rows = 62;
	int cols = 81;
	int debug;
	int dfaMap[62][81];
	char validChars[81];
	int dfaMapValue = 0;
	int dfaInComment = 0;
	string dfaString = "";
	int tokenID = 0;
	string identifier;
public:
	scanner();
	scanner(int debg);
	int run(testScanner* ts);
	void getDfaMap();
	int isValidCharacter(char c);
	int getCharacterIndex(char c);
	token dfaProcess(char c, int lineNumber, int verbose);
	int dfaMapTraversal(char c, int verbose);
	token dfaTokenAssign(int dfaTokenNum, int lineNum);
	int getTokenIndex(int dfaTokenNum);
	void printToken(token tkn);
	token dfaTokenDecode(int dfaMapValue, int lineNumber);
	~scanner();
};

#endif