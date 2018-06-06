#include "treeTraverse.h"


treeTraverse::treeTraverse()
{
}

treeTraverse::treeTraverse(node* rtNode, string fileNam, int debg) : rootNode(rtNode), debug(debg), fileName(fileNam)
{
}

void treeTraverse::run()
{
	verbose = 1;
	if (debug) cout << "treeTraverse: DEBUG MODE" << endl;
	if (debug && verbose) cout << "treeTraverse: VERBOSE MODE" << endl;

	outFile.open(fileName + ".asm");

	cout << "treeTraverse: starting tree traversal" << endl;
	node* currentNode = rootNode;
	indents = 0;

	if (debug && verbose) cout << "treeTraverse: checking rootNode" << endl;
	if (currentNode == nullptr) {
		cout << "treeTraverse: root of tree is empty!" << endl;
		return;
	}
	gsa->setIdentifier("gsa");
	vsa->setIdentifier("vsa");
	vsaa->setIdentifier("vsaa");

	cout << "treeTraverse: starting traversal" << endl;
	traverse(currentNode);

	outFile.close();
	cout << "treeTraverse: code generated target file successfully: " << fileName + ".asm" << endl;
}

void treeTraverse::traverse(node * currentNode)
{
	if ("" == currentNode->getNodeType()) {
		if (debug && verbose) cout << "treeTraverse: empty node found. Returning" << endl;
		return;
	}

	indents++;
	string pad = "";

	//if (debug && verbose) cout << "treeTraverse: creating pad characters" << endl;
	for (int i = 0; i < 2*indents; i++)
		pad += " ";

	if (debug && verbose) cout << pad << "treeTraverse: traverse()" << endl;

	//if (debug && verbose) cout << "treeTraverse: checking currentNode" << endl;
	if (currentNode == nullptr) {
		cout << pad << "treeTraverse: Something very wrong happened.  This node is null and was not handled properly." << endl;
		return;
	}

	if (debug && verbose) print(currentNode, pad);

	//check for local scoping 
	int lastVarCount = 0;

	if ("program" == currentNode->getNodeType()) {
		traverseA(currentNode, pad);
		traverseB(currentNode, pad);

		targetPrintLn("STOP");
		initializeVariables();
	}

	else if ("block" == currentNode->getNodeType()) {
		global = 0;
		int thisBlockNum = ++blockNum;
		if (debug && verbose) cout << pad << "treeTraverse: detected block - resetting varCount to 0" << endl;
		if (debug && verbose) cout << pad << "treeTraverse: designating this blockNum as : " << thisBlockNum << endl;
		lastVarCount = varCount;
		varCount = 0;

		currentBlockNum = thisBlockNum;
		traverseA(currentNode, pad);
		currentBlockNum = thisBlockNum;
		traverseB(currentNode, pad);
		currentBlockNum = thisBlockNum;

		//check for local scoping on return
		if (debug && verbose) cout << pad << "treeTraverse: popping " << varCount << " variables off the vsa" << endl;

		for (int i = 0; i < varCount; i++) {
			vsa->pop();
			targetPrintLn("POP");
		}

		if (debug && verbose) cout << pad << "treeTraverse: exiting block " << currentBlockNum << " - resetting varCount to last value: " << lastVarCount << endl;
		varCount = lastVarCount;
	}

	else if ("vars" == currentNode->getNodeType() || "mvars" == currentNode->getNodeType()) {
		if (ID == currentNode->getToken()->getTokenType()) {
			//if in a declaration node
			if (global) {
				if (debug && verbose) cout << pad << "treeTraverse: detected global ID - pushing to gsa: " << currentNode->getToken()->getIdentifierName() << endl;
				gsa->push(currentNode->getToken()->getIdentifierName(), globalVarCount++);
				if (debug && verbose) cout << pad << "treeTraverse: global varCount: " << globalVarCount << " - " << gsa->list() << endl;
			}
			else {
				if (debug && verbose) cout << pad << "treeTraverse: detected ID - pushing to vsa: " << currentNode->getToken()->getIdentifierName() << endl;
				vsa->push(currentNode->getToken()->getIdentifierName() + to_string(currentBlockNum), varCount++);
				vsaa->push(currentNode->getToken()->getIdentifierName() + to_string(currentBlockNum), varCount);
				if (debug && verbose) cout << pad << "treeTraverse: varCount: " << varCount << " - " << vsa->list() << endl;
			}
		}
		traverseA(currentNode, pad);
	}

	else if ("expr" == currentNode->getNodeType()) {
		if (PL == currentNode->getToken()->getTokenType()) {
			traverseA(currentNode, pad);
			string tmpVar = createTmp();
			targetPrintLn("STORE " + tmpVar);
			traverseB(currentNode, pad);
			targetPrintLn("ADD " + tmpVar);
		}
		else {
			traverseA(currentNode, pad);
		}
	}

	else if ("m" == currentNode->getNodeType()) {
		if (MN == currentNode->getToken()->getTokenType()) {
			traverseB(currentNode, pad);
			string tmpVar = createTmp();
			targetPrintLn("STORE " + tmpVar);
			traverseA(currentNode, pad);
			targetPrintLn("SUB " + tmpVar);
		} 
		else {
			traverseA(currentNode, pad);
		}
	}

	else if ("t" == currentNode->getNodeType()) {
		if (TI == currentNode->getToken()->getTokenType()) {
			traverseA(currentNode, pad);
			string tmpVar = createTmp();
			targetPrintLn("STORE " + tmpVar);
			traverseB(currentNode, pad);
			targetPrintLn("MULT " + tmpVar);
		}
		else if (DI == currentNode->getToken()->getTokenType()) {
			traverseB(currentNode, pad);
			string tmpVar = createTmp();
			targetPrintLn("STORE " + tmpVar);
			traverseA(currentNode, pad);
			targetPrintLn("DIV " + tmpVar);
		}
		else {
			traverseA(currentNode, pad);
		}
	}

	else if ("f" == currentNode->getNodeType()) {
		if (currentNode->getToken()->getTokenType() == MN) {
			traverseA(currentNode, pad);
			targetPrintLn("MULT -1");
		}
		else {
			traverseA(currentNode, pad);
		}
	}
	
	else if ("r" == currentNode->getNodeType()) {
		int returnValue = 0;
		if (currentNode->getToken()->getTokenType() == INT) {
			targetPrintLn("LOAD " + currentNode->getToken()->getIdentifierName());
		}
		else if (currentNode->getToken()->getTokenType() == ID) {
			if (debug && verbose) cout << pad << "treeTraverse: detected ID - finding in vsa: " << currentNode->getToken()->getIdentifierName() << endl;
			int depth = 0;
			if (depth = vsa->find(currentNode->getToken()->getIdentifierName() + to_string(currentBlockNum), varCount) >= 0) {
				//if found in local scope
				if (debug && verbose) cout << pad << "treeTraverse: detected LOCAL scope ID: " << currentNode->getToken()->getIdentifierName() + to_string(currentBlockNum) << endl;
				targetPrintLn("STACKR " + to_string(depth));
			}
			else if (gsa->find(currentNode->getToken()->getIdentifierName()) >= 0) {
				//if found in global scope
				if (debug && verbose) cout << pad << "treeTraverse: detected GLOBAL scope ID: " << currentNode->getToken()->getIdentifierName() << endl;
				targetPrintLn("LOAD " + currentNode->getToken()->getIdentifierName());
			}
			else {
				//if not found then error
				if (debug && verbose) cout << pad << "treeTraverse: global varCount: " << globalVarCount << " - " << gsa->list() << endl;
				if (debug && verbose) cout << pad << "treeTraverse: local varCount: " << varCount << " - " << vsa->list() << endl;

				cout << pad << "treeTraverse: detected unknown ID: " << currentNode->getToken()->getIdentifierName() << " Exiting!" << endl;
				exit(1);
			}
			if (debug && verbose) cout << pad << "treeTraverse: varCount: " << varCount << " " << vsa->list() << endl;
		}
		else { //must be a expr
			traverseA(currentNode, pad);
		}
	}
	
	else if ("stats" == currentNode->getNodeType()) {
		traverseA(currentNode, pad);
		traverseB(currentNode, pad);
	}
	
	else if ("mstat" == currentNode->getNodeType()) {
		traverseA(currentNode, pad);
		traverseB(currentNode, pad);
	}
	
	else if ("stat" == currentNode->getNodeType()) {
		traverseA(currentNode, pad);
	}
	
	else if ("in" == currentNode->getNodeType()) {
		if (debug && verbose) cout << "treeTraverse: looking for var: " << currentNode->getToken()->getIdentifierName() << endl;
		if (debug && verbose) cout << "treeTraverse: global varCount: " << globalVarCount << " - " << gsa->list() << endl;
		if (debug && verbose) cout << "treeTraverse: local varCount: " << varCount << " - " << vsa->list() << endl;
		if (vsa->find(currentNode->getToken()->getIdentifierName() + to_string(currentBlockNum), varCount) >= 0) {
			targetPrintLn("READ " + currentNode->getToken()->getIdentifierName() + to_string(currentBlockNum));
			cout << "Push to stack 1 " << endl;
			targetPrintLn("PUSH " + currentNode->getToken()->getIdentifierName() + to_string(currentBlockNum));
		}
		else if (gsa->find(currentNode->getToken()->getIdentifierName()) >= 0) {
			targetPrintLn("READ " + currentNode->getToken()->getIdentifierName());
		}
		
	}
	
	else if ("out" == currentNode->getNodeType()) {
		traverseA(currentNode, pad);
		string tmpVar = createTmp();
		targetPrintLn("STORE " + tmpVar);
		targetPrintLn("WRITE " + tmpVar);
	}
	
	else if ("iff" == currentNode->getNodeType()) {
		string ro = parser::getTokenSymbol(currentNode->getToken()->getTokenType());
		string ro1 = "";
		if (currentNode->getB() != 00000000) {
			ro1 = currentNode->getB()->getNodeType();
		}
		cout << "ro: " << ro << endl;
		cout << "ro1: " << ro1 << endl;
		traverseC(currentNode, pad);
		string tmpVar = createTmp();
		targetPrintLn("STORE " + tmpVar);
		traverseA(currentNode, pad);
		targetPrintLn("SUB " + tmpVar);
		string lblVar = createLabel();

		if (ro == ">=>")
			targetPrintLn("BRZNEG " + lblVar);
		else if (ro == "<=<")
			targetPrintLn("BRZPOS " + lblVar);
		else if (ro1 == "GEGE"/*ro == ">=> ="*/)
			targetPrintLn("BRNEG " + lblVar);
		else if (ro1 == "LELE"/*ro == "<=< ="*/)
			targetPrintLn("BRPOS " + lblVar);
		else if (ro == "!!") {
			targetPrintLn("BRZ " + lblVar);
		}
		else if (ro1 == "EQEQ"/*ro == "=="*/) {
			targetPrintLn("BRNEG " + lblVar);
			targetPrintLn("BRPOS " + lblVar);
		}
		else {
			error(currentNode);
		}

		traverseD(currentNode, pad);

		targetPrintLn(lblVar + ": NOOP");
	}
	
	else if ("loop" == currentNode->getNodeType()) {
		string ro = parser::getTokenSymbol(currentNode->getToken()->getTokenType());
		string ro1 = "";
		if (currentNode->getB() != 00000000) {
			ro1 = currentNode->getB()->getNodeType();
			if (debug) cout << "node type: " << ro1 << endl;
		}

		
		traverseC(currentNode, pad);
		string tmpVar = createTmp();
		targetPrintLn("STORE " + tmpVar);
		string lblVar1 = createLabel();
		targetPrintLn(lblVar1 + ": NOOP");
		traverseA(currentNode, pad);
		targetPrintLn("SUB " + tmpVar);
		string lblVar2 = createLabel();

		if (ro == ">=>")
			targetPrintLn("BRZNEG " + lblVar2);
		else if (ro == "<=<")
			targetPrintLn("BRZPOS " + lblVar2);
		else if (ro1 == "GEGE"/*ro == ">=> ="*/)
			targetPrintLn("BRNEG " + lblVar2);
		else if (ro1 == "LELE"/*ro == "<=< ="*/)
			targetPrintLn("BRPOS " + lblVar2);
		else if (ro == "!!") {
			targetPrintLn("BRZ " + lblVar2);
		}
		else if (ro1 == "EQEQ"/*ro == "=="*/) {
			targetPrintLn("BRNEG " + lblVar2);
			targetPrintLn("BRPOS " + lblVar2);
		}
		else {
			error(currentNode);
		}

		traverseD(currentNode, pad);

		targetPrintLn("BR " + lblVar1);
		targetPrintLn(lblVar2 + ": NOOP");
	}
	
	else if ("assign" == currentNode->getNodeType()) {
		node* curNode = currentNode;
		traverseA(currentNode, pad);
		if (curNode->getToken()->getTokenType() == ID) {
			if (debug && verbose) cout << pad << "treeTraverse: detected ID - finding in g/vsa: " << currentNode->getToken()->getIdentifierName() << endl;
			int depth = 0;
			if (depth = vsa->find(currentNode->getToken()->getIdentifierName() + to_string(currentBlockNum), varCount) >= 0) {
				//if found in local scope
				if (debug && verbose) cout << pad << "treeTraverse: detected LOCAL scope ID: " << currentNode->getToken()->getIdentifierName() << to_string(currentBlockNum) << endl;
				targetPrintLn("STACKW " + to_string(depth));
			}
			else if (depth = gsa->find(currentNode->getToken()->getIdentifierName()) >= 0) {
				//if found in global scope
				if (debug && verbose) cout << pad << "treeTraverse: detected GLOBAL scope ID: " << currentNode->getToken()->getIdentifierName() << endl;
				targetPrintLn("STORE " + currentNode->getToken()->getIdentifierName());
			}
			else {
				//if not found then error
				cout << pad << "treeTraverse: detected unknown ID: " << currentNode->getToken()->getIdentifierName() << " Exiting!" << endl;
				exit(1);
			}
			if (debug && verbose) cout << pad << "treeTraverse: varCount: " << varCount << " " << vsa->list() << endl;
		}
	}
	
	/*else if ("ro" == currentNode->getNodeType()) {

	}*/
	
	else {
		error(currentNode);
	}

	indents--;
	if (debug) cout << pad << "treeTraverse: returning to parent node" << endl;
}

void treeTraverse::print(node * currentNode, string pad)
{
	//if (debug && verbose) cout << "treeTraverse: print node" << endl;

	token* tkn = currentNode->getToken();

	if (verbose) cout << pad << "treeTraverse: printing node: (" << currentNode->getNodeType() << ") token: (" << parser::getTokenSymbol(tkn->getTokenType()) << ":" << tkn->getTokenDesc() << ":" << tkn->getLineNumber() << ") children (A:B:C:D) : (" << currentNode->getA() << ":" << currentNode->getB() << ":" << currentNode->getC() << ":" << currentNode->getD() << ")" << endl;
	return;
}

void treeTraverse::targetPrint(string str)
{
	if (debug && verbose) cout << "targetPrint: Writing (" << str << ") to target" << endl;
	outFile << str;
}

void treeTraverse::targetPrintLn(string str)
{
	if (debug && verbose) cout << "targetPrint: Writing (" << str << ") to target" << endl;
	outFile << str << endl;
}

void treeTraverse::traverseA(node * currentNode, string pad)
{
	if (currentNode->getA() != 00000000) {
		if (debug) cout << pad << "treeTraverse: traversing to child node A" << endl;
		traverse(currentNode->getA());
	}
}

void treeTraverse::traverseB(node * currentNode, string pad)
{
	if (currentNode->getB() != 00000000) {
		if (debug) cout << pad << "treeTraverse: traversing to child node B" << endl;
		traverse(currentNode->getB());
	}
}

void treeTraverse::traverseC(node * currentNode, string pad)
{
	if (currentNode->getC() != 00000000) {
		if (debug) cout << pad << "treeTraverse: traversing to child node C" << endl;
		traverse(currentNode->getC());
	}
}

void treeTraverse::traverseD(node * currentNode, string pad)
{
	if (currentNode->getD() != 00000000) {
		if (debug) cout << pad << "treeTraverse: traversing to child node D" << endl;
		traverse(currentNode->getD());
	}
}

void treeTraverse::initializeVariables()
{
	//initialize all locals
	if (debug) cout << "treeTraverse: initializing local variables" << endl;
	vector<string> vstack = vsaa->getStack();
	for (vector<string>::iterator stackIterator = vstack.begin(); stackIterator != vstack.end(); ++stackIterator) {
		targetPrintLn(*stackIterator + " 0");
	}
	
	//initialize all globals
	if (debug) cout << "treeTraverse: initializing global variables" << endl;
	vector<string> gstack = gsa->getStack();
	for (vector<string>::iterator stackIterator = gstack.begin(); stackIterator != gstack.end(); ++stackIterator) {
		targetPrintLn(*stackIterator + " 0");
	}
}

void treeTraverse::error(node* currentNode)
{
	cout << "treeTraverse: ERROR during traversal!" << endl << "ERROR on node: ";
	print(currentNode,0);
	cout << "\nExiting!";

	exit(0);
}

string treeTraverse::createTmp()
{
	vsaa->push("TMP" + to_string(++tmpNum),0);
	return "TMP" + to_string(tmpNum);
}

string treeTraverse::createLabel()
{
	return "LABEL" + to_string(++labelNum);
}

treeTraverse::~treeTraverse()
{
}
