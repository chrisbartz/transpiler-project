#include "parser.h"

using namespace std;

parser::parser()
{
}

parser::parser(int debg, vector<token> tokns) : debug(debg), tokens(tokns)
{
}

node * parser::run()
{
	verbose = 0;
	if (debug) cout << "\nparser: DEBUG MODE" << endl;
	if (debug && verbose) cout << "\nparser: VERBOSE MODE" << endl;

	cout << "\nparser: start parsing tokens" << endl;
	node * rootNode;
	tokenIterator = tokens.begin();

	rootNode = program();

	cout << "parser: finished parsing tokens" << endl;
	return rootNode;
}

node* parser::program() {
	sectionName("program()");
	node* newNode = new node();
	newNode->setNodeType("program");
	newNode->setA(vars());
	newNode->setB(block());
	return newNode;
}

node* parser::block() {
	sectionName("block()");
	node* newNode = new node();
	newNode->setNodeType("block");
	checkFor(BEGIN);
	newNode->setA(vars());
	checkFor(CO);
	newNode->setB(stats());
	checkFor(END);
	return newNode;
}

node* parser::vars() {
	sectionName("vars()");
	node* newNode = new node();
	newNode->setNodeType("vars");
	if (checkForEmpty(VOID)) {
		checkFor(ID);
		newNode->setToken(returnToken);
	}
	if (lookAhead(ID))
		newNode->setA(mvars());
	//if (debug && verbose) cout << "xxxxxxxxxxx: printing node: " << newNode->getNodeType() << " (" << getTokenSymbol(newNode->getToken()->getTokenType()) << ":" << newNode->getToken()->getTokenDesc() << ":" << newNode->getToken()->getLineNumber() << ")" << endl;
	return newNode;
}

node* parser::mvars() {
	sectionName("mvars()");
	node* newNode = new node();
	newNode->setNodeType("mvars");
	if (checkForEmpty(ID))
		newNode->setToken(returnToken);
	if (lookAhead(ID)) {
		newNode->setA(mvars());
	}
	return newNode;
}

node* parser::expr() {
	sectionName("expr()");
	node* newNode = new node();
	newNode->setNodeType("expr");
	newNode->setA(m());
	if (checkForEmpty(PL)) {
		newNode->setToken(returnToken);
		if (lookAhead(MN) + lookAhead(OP) + lookAhead(ID) + lookAhead(INT)) {
			newNode->setB(expr());
		}
	}
	return newNode;
}

node* parser::m() {
	sectionName("m()");
	node* newNode = new node();
	newNode->setNodeType("m");
	newNode->setA(t());
	if (checkForEmpty(MN)) {
		newNode->setToken(returnToken);
		newNode->setB(m());
	}
	return newNode;
}

node* parser::t() {
	sectionName("t()");
	node* newNode = new node();
	newNode->setNodeType("t");
	newNode->setA(f());
	if (checkForEmpty(TI)) {
		newNode->setToken(returnToken);
		newNode->setB(t());
	} else if (checkForEmpty(DI)) {
		newNode->setToken(returnToken);
		newNode->setB(t());
	}
	return newNode;
}

node* parser::f() {
	sectionName("f()");
	node* newNode = new node();
	newNode->setNodeType("f");
	if (checkForEmpty(MN)) {
		newNode->setToken(returnToken);
		newNode->setA(f());
	} else {
		newNode->setA(r());
	}
	return newNode;
}

node* parser::r() {
	sectionName("r()");
	node* newNode = new node();
	newNode->setNodeType("r");
	if (checkForEmpty(OP)) {
		newNode->setToken(returnToken);
		newNode->setA(expr());
		checkFor(CP);
	}
	else if (checkForEmpty(ID)) {
		newNode->setToken(returnToken);
	}
	else {
		checkFor(INT);
		newNode->setToken(returnToken);
	}
		
	return newNode;
}

node* parser::stats() {
	sectionName("stats()");
	node* newNode = new node();
	newNode->setNodeType("stats");
	newNode->setA(stat());
	checkFor(CO);
	newNode->setToken(returnToken);
	newNode->setB(mstat());
	return newNode;
}

node* parser::mstat() {
	sectionName("mstat()");
	node* newNode = new node();
	newNode->setNodeType("mstat");
	if (lookAhead(INPUT) + lookAhead(OUTPUT) + lookAhead(BEGIN) + lookAhead(IF) + lookAhead(FOR) + lookAhead(ID)) {
		newNode->setA(stat());
		checkFor(CO);
		newNode->setToken(returnToken);
		newNode->setB(mstat());
	}
	return newNode;
}

node* parser::stat()
{
	sectionName("stat()");
	node* newNode = new node();
	newNode->setNodeType("stat");

	if (lookAhead(INPUT)) {
		newNode->setA(in());
	}
	else if (lookAhead(OUTPUT)) {
		newNode->setA(out());
	}
	else if (lookAhead(BEGIN)) {
		newNode->setA(block());
	}
	else if (lookAhead(IF)) {
		newNode->setA(iff());
	}
	else if (lookAhead(FOR)) {
		newNode->setA(loop());
	}
	else if (lookAhead(ID))
		newNode->setA(assign());
	else
		error(tokenIterator->getTokenType());
	return newNode;
}

node* parser::in() {
	sectionName("in()");
	node* newNode = new node();
	newNode->setNodeType("in");
	checkFor(INPUT);
	checkFor(ID);
	newNode->setToken(returnToken);
	return newNode;
}

node* parser::out() {
	sectionName("out()");
	node* newNode = new node();
	newNode->setNodeType("out");
	checkFor(OUTPUT);
	newNode->setToken(returnToken);
	newNode->setA(expr());
	return newNode;
}

node* parser::iff() {
	sectionName("iff()");
	node* newNode = new node();
	newNode->setNodeType("iff");
	checkFor(IF);
	newNode->setToken(returnToken);
	checkFor(OP);
	newNode->setA(expr());
	newNode->setB(ro());
	newNode->setToken(newNode->getB()->getToken());
	newNode->setC(expr());
	checkFor(CP);
	newNode->setD(block());
	return newNode;
}

node* parser::loop() {
	sectionName("loop()");
	node* newNode = new node();
	newNode->setNodeType("loop");
	checkFor(FOR);
	newNode->setToken(returnToken);
	checkFor(OP);
	newNode->setA(expr());
	newNode->setB(ro());
	newNode->setToken(newNode->getB()->getToken());
	newNode->setC(expr());
	checkFor(CP);
	newNode->setD(block());
	return newNode;
}

node* parser::assign() {
	sectionName("assign()");
	node* newNode = new node();
	newNode->setNodeType("assign");
	checkFor(ID);
	newNode->setToken(returnToken);
	checkFor(EQ);
	newNode->setA(expr());
	return newNode;
}

node* parser::ro() {
	sectionName("ro()");
	if (checkForEmpty(GEG)) {
		node* newNode = new node();
		newNode->setToken(returnToken);
		if (checkForEmpty(EQ)) {
			newNode->setNodeType("GEGE");
			newNode->setToken(returnToken);
			if (debug && verbose) cout << "GEGE" << endl;
		}
		else {
			newNode->setNodeType("GEG");
			if (debug && verbose) cout << "GEG" << endl;
		}
		return newNode;
	} 
	else if (checkForEmpty(LEL) ) {
		node* newNode = new node();
		if (checkForEmpty(EQ)) {
			newNode->setNodeType("LELE");
			newNode->setToken(returnToken);
			if (debug && verbose) cout << "LELE" << endl;
		}
		else {
			node* newNode = new node();
			newNode->setNodeType("LEL");
			newNode->setToken(returnToken);
			if (debug && verbose) cout << "LEL" << endl;
		}
		return newNode;
	}
	else if (checkForEmpty(NN)) {
		node* newNode = new node();
		newNode->setNodeType("NN");
		newNode->setToken(returnToken);
		return newNode;
	}
	else if (checkForEmpty(EQ)) {
		node* newNode = new node();
		if (checkForEmpty(EQ)) {
			newNode->setNodeType("EQEQ");
			newNode->setToken(returnToken);
		}
		else {
			newNode->setNodeType("EQ");
			newNode->setToken(returnToken);
		}
		return newNode;
	}
	return new node();
}

//error function prints error message and exits
void parser::error(tokenTyp expected)
{
	sectionName("error()");
	cout << "parser: ERROR during parsing!  Unexpected token (" << getTokenSymbol(tokenIterator->getTokenType()) << ":" << tokenIterator->getTokenDesc() << ") at line: " << tokenIterator->getLineNumber();
	cout << " Expected: (" << getTokenSymbol(expected) << ")" << endl << "Exiting!";

	exit(0);
}

//getTokenSymbol translates tokenType (int) to the character(s) representation
string parser::getTokenSymbol(tokenTyp tokentype)
{
	string tokenText[] = { "BEGIN", "END", "IF", "FOR", "VOID", "VAR", "RETURN", "INPUT", "OUTPUT", "EQ", "GEG", "LEL", "NN", "CO", "PL", "MN", "TI", "DI", "PE", "OP", "CP", "CM", "OF", "CF", "SC", "OS", "CS", "ID", "INT", "EoF", "NUL", "NXT" };
	string tokenSymbols[] = { "BEGIN", "END", "IF", "FOR", "VOID", "VAR", "RETURN", "INPUT", "OUTPUT", "=", ">=>", "<=<", "!!", ":", "+", "-", "*", "/", ".", "(", ")", ",", "{", "}", ";", "[", "]", "ID", "INT", "EoF", "NULL", "NXT" };
	return tokenSymbols[tokentype];
}

//checkFor will inspect the next token and if it matches, consumes it - 
//(creates the node and returns it); if no match then throws error
node* parser::checkFor(tokenTyp tokentype)
{
	returnToken = new token();
	if (debug && verbose) sectionName("checkFor(" + getTokenSymbol(tokentype) + ")");
	if (tokenIterator->getTokenType() == tokentype) {
		if (debug && verbose) cout << "Consumed: (" << getTokenSymbol(tokenIterator->getTokenType()) << ":" << tokenIterator->getTokenDesc() << ":" << tokenIterator->getLineNumber() << ")" << endl;
		node* newNode = new node(&*tokenIterator);
		newNode->setNodeType(getTokenSymbol(tokentype));
		returnToken = &*tokenIterator;
		tokenIterator++;
		return newNode;
	}
	else
		error(tokentype);
	return nullptr;
}

//checkForEmpty will look for a specified tokenType and if present, consumes it - 
//creating node and passing back via returnNode and returns 1; if not present then returns 0
int parser::checkForEmpty(tokenTyp tokentype)
{
	returnToken = new token();
	returnNode = new node();
	if (debug && verbose) sectionName("checkForEmpty(" + getTokenSymbol(tokentype) + ")");
	if (tokenIterator->getTokenType() == tokentype) {
		if (debug && verbose) cout << "Consumed: (" << getTokenSymbol(tokenIterator->getTokenType()) << ":" << tokenIterator->getTokenDesc() << ":" << tokenIterator->getLineNumber() << ")" << endl;
		token tkn = *tokenIterator;
		node newNode(&tkn);
		newNode.setNodeType(getTokenSymbol(tokentype));
		returnNode = &newNode;
		returnToken = &*tokenIterator;
		tokenIterator++;
		return 1;
	}
	return 0;
}

//lookAhead will inspect the next token without consuming it; returns 1 if match, 0 if no match
int parser::lookAhead(tokenTyp tokentype) {
	if (debug && verbose) sectionName("lookAhead(" + getTokenSymbol(tokentype) + ")");
	if (tokenIterator->getTokenType() == tokentype) {
		return 1;
	}
	else {
		return 0;
	}
}

//prints the debugging section titles
void parser::sectionName(string str)
{
	if (debug) cout << str << endl;
}

parser::~parser()
{
}
