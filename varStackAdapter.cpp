#include "varStackAdapter.h"



varStackAdapter::varStackAdapter()
{
}

void varStackAdapter::push(string var, int depth)
{
	if (find(var, depth) < 0)
		stack.push_back(var);
	else
		error(var);
}

void varStackAdapter::pop()
{
	stack.pop_back();
}

int varStackAdapter::find(string var)
{
	int i = 1;
	for (vector<string>::reverse_iterator stackIterator = stack.rbegin(); stackIterator != stack.rend(); ++stackIterator, i++) {
		if (*stackIterator == var) {
			if (verbose) cout << identifier << ": depth of global variable " << var << " found : " << i - 1 << endl;
			return i - 1; //returns variable depth
		}
	}
	if (verbose) cout << identifier << ": depth of global variable " << var << " NOT found : " << i - 1 << endl;
	return -1; //returns var not found
}

int varStackAdapter::find(string var, int depth)
{ 
	int i = 1;
	for (vector<string>::reverse_iterator stackIterator = stack.rbegin(); stackIterator != stack.rend(); ++stackIterator, i++) {
		if (*stackIterator == var && i > ((int)stack.size() - depth)) {
			if (verbose) cout << identifier << ": depth of local variable " << var << " found : " << i - 1 << endl;
			return i - 1; //returns variable depth
		}
	}
	if (verbose) cout << identifier << ": depth of local variable " << var << " NOT found : " << i - 1 << endl;
	return -1; //returns var not found
}

string varStackAdapter::list()
{
	string returnString = "";

	for (vector<string>::iterator stackIterator = stack.begin(); stackIterator != stack.end(); ++stackIterator) {
		returnString += *stackIterator + " ";
	}

	return returnString;
}

string varStackAdapter::list(int depth)
{
	int i = 1;
	string returnString = "";

	for (vector<string>::iterator stackIterator = stack.begin(); stackIterator != stack.end(); ++stackIterator, i++) {
		if (i > ((int)stack.size() - depth))
			returnString += *stackIterator + " ";
	}

	return returnString;
}

void varStackAdapter::error(string var)
{
	cout << "varStackAdapter: error while pushing " << var << " to stack.  Var " << var << " is already declared in this scope.  Exiting!" << endl;
	exit(1);
}

vector<string> varStackAdapter::getStack()
{
	return stack;
}

void varStackAdapter::setIdentifier(string id)
{
	identifier = id;
}


varStackAdapter::~varStackAdapter()
{
}
