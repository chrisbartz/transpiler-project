#include <iostream>
#include <fstream>
#include <string>
#include "node.h"
#include "parser.h"
#include "token.h"
#include "tokenTyp.h"
#include "varStackAdapter.h"

using namespace std;

#ifndef TREETRAVERSE_H
#define TREETRAVERSE_H
class treeTraverse
{
private:
	node* rootNode; //root of the parse tree
	int indents; //counter for indentation
	int verbose; //prints many traces
	int debug; //prints debugging traces
	int global = 1; //indicator for global variable definitions
	string fileName;
	varStackAdapter* gsa = new varStackAdapter(); //holds global variable names
	varStackAdapter* vsa = new varStackAdapter(); //holds local variable names within scope
	varStackAdapter* vsaa = new varStackAdapter(); //holds all local variable names regardless of scope
	int varCount = 0; //for local scoping
	int globalVarCount = 0; //for global scope
	int blockNum = 0; //for assigning var names in target language - each new block will increment this number
	int currentBlockNum = 0; //for assigning var names in target language - keeps track of current block number
	int tmpNum = 0; //for assigning unique temporary variables
	int labelNum = 0; //for assigning unique labels
	ofstream outFile;

	//utility methods
	void traverse(node*); //recursive descent into the parse tree
	void print(node*, string); //prints values of the node
	void targetPrint(string); //prints values to target file
	void targetPrintLn(string); //prints values to target file with newline
	void traverseA(node*,string);
	void traverseB(node*, string);
	void traverseC(node*, string);
	void traverseD(node*, string);
	void initializeVariables();
	void error(node*);
	string createTmp();
	string createLabel();

public:
	treeTraverse(); //constructor
	treeTraverse(node*,string,int); //constructor

	void run(); //method to start execution

	~treeTraverse();
};
#endif