#include <string>
#include "token.h"

#ifndef NODE_H
#define NODE_H
class node
{
private: 
	token* tokn;
	node* a;
	node* b;
	node* c;
	node* d;
	string nodeType;
public:
	node();
	node(token*);
	void setToken(token*);
	void setA(node*);
	void setB(node*);
	void setC(node*);
	void setD(node*);
	void setNodeType(string);
	token* getToken();
	node* getA();
	node* getB();
	node* getC();
	node* getD();
	string getNodeType();
	~node();
};
#endif
