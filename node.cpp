#include "node.h"



node::node() : tokn(new token()), a(nullptr), b(nullptr), c(nullptr), d(nullptr), nodeType("")
{	
}

node::node(token* tkn) : tokn(tkn), a(nullptr), b(nullptr), c(nullptr), d(nullptr), nodeType("")
{
}

void node::setToken(token* tkn)
{
	tokn = tkn;
}

void node::setA(node * aa)
{
	a = aa;
}

void node::setB(node * bb)
{
	b = bb;
}

void node::setC(node * cc)
{
	c = cc;
}

void node::setD(node * dd)
{ 
	d = dd;
}

void node::setNodeType(string str)
{
	nodeType = str;
}

token* node::getToken()
{
	return tokn;
}

node * node::getA()
{
	return a;
}

node * node::getB()
{
	return b;
}

node * node::getC()
{
	return c;
}

node * node::getD()
{
	return d;
}

string node::getNodeType()
{
	return nodeType;
}

node::~node()
{
}
