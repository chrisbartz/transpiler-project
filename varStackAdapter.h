#include <iostream>
#include <vector>
#include <string>

#pragma once

using namespace std;

class varStackAdapter
{
private:
	vector<string> stack;
	int verbose = 1;
	string identifier;
public:
	varStackAdapter();
	void push(string, int);
	void pop();
	int find(string);
	int find(string,int);
	string list();
	string list(int);
	void error(string);
	vector<string> getStack();
	void setIdentifier(string);
	~varStackAdapter();
};

