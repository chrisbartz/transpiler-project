#include <string>
#include "tokenTyp.h"
using namespace std;

#ifndef _TOKEN_H
#define _TOKEN_H

class token {
private:
	tokenTyp tokenType;
	string tokenDesc;
	int lineNumber;
	string identifierName;
public:
	token();
	token(tokenTyp);
	token(enum tokenTyp, string, int);
	token(enum tokenTyp,string,int,string);
	
	enum tokenTyp getTokenType() { return tokenType; };
	string getTokenDesc() { return tokenDesc; };
	int getLineNumber() { return lineNumber; };
	string getIdentifierName() { return identifierName; };

	//string getTokenText() { return tokenText[tokenType]; };
	//string getTokenSymbol() { return tokenSymbols[tokenType]; };

	void setTokenType(enum tokenTyp tokenTp) { tokenType = tokenTp; };
	void setTokenDesc(string tokenDes) { tokenDesc = tokenDes; };
	void setLineNumber(int lineNum) { lineNumber = lineNum; };
	void setIdentifierName(string ident) { identifierName = ident; };

	~token();
};

#endif