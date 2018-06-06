#include <iostream>
#include <vector>
#include "token.h"
#include "tokenTyp.h"
#include "node.h"

#ifndef PARSER_H
#define PARSER_H
class parser
{
private:
	int debug;
	int verbose;
	vector<token> tokens;
	//token* currentToken;
	node* currentNode;
	node* returnNode;
	token* returnToken;
	vector<token>::iterator tokenIterator;
	node* program();
	node* block();
	node* vars();
	node* mvars();
	node* expr();
	node* m();
	node* t();
	node* f();
	node* r();
	node* stats();
	node* mstat();
	node* stat();
	node* in();
	node* out();
	node* iff();
	node* loop();
	node* assign();
	node* ro();
	void error(tokenTyp);
	node* checkFor(tokenTyp);
	int checkForEmpty(tokenTyp);
	int lookAhead(tokenTyp);
	void sectionName(string);
public:
	parser();
	parser(int,vector<token>);
	node * run();
	static string getTokenSymbol(tokenTyp);
	//token nextToken();
	~parser();
};
#endif
