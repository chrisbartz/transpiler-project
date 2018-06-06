#include "scanner.h"

using namespace std;

scanner::scanner() : debug(0)
{
}

scanner::scanner(int debg) : debug(debg)
{
}

int scanner::run(testScanner* ts)
{
	int verbose = 0;

	cout << "scanner: starting " << endl;

	if (debug && verbose)
		cout << "scanner: VERBOSE MODE" << endl;

	//get map for finite automaton - should be from text file but I should 
	//have also included the source spreadsheet for reference
	getDfaMap();
	char c = 0;
	int lineNumber = -1;
	vector<token> tokens; //this is the token vector we will return when finished
	token currentToken = token(NUL,"NULL",-1);
	if (debug) cout << "\nscanner: begin scanning for tokens" << endl;

	while (c != EOF) {
		currentToken = token(NUL, "NULL", -1);
		c = 0;
		c = ts->getNextChar();
		lineNumber = ts->getLineNumber();

		if (c == '\0')
			break;

		if (isValidCharacter(c)) {
			identifier += c;  //keep track of identifier names to be placed in the token description
			if (identifier == " ") identifier = ""; //trim identifier names
			if (debug && verbose)
				cout << c;
			//printToken(dfaProcess(c,lineNumber));
			currentToken = dfaProcess(c, lineNumber,verbose);  //use the finite automaton map to process the text input
			if (currentToken.getTokenType() != NUL) {
				if (currentToken.getTokenType() != NXT) {  //NXT tokens are a signal to get the next character
					tokens.push_back(currentToken);
					printToken(currentToken);
				}
			}
			else {
				//token(NUL, "error", -1)
				cout << "scanner: unknown token on line " << lineNumber << endl;
				cout << "scanner: exiting!" << endl;
				break;
			}
		}
		else
			cout << "scanner: invalid char: " << (int)c << endl;
	}

	ostringstream oss;
	oss << "EoF" << "_" << tokenID++;

	//Finish with EOF Token
	if (debug)
		printToken(token(EoF, oss.str(), lineNumber));
	tokens.push_back(token(EoF, oss.str(), lineNumber));

	if (debug && verbose) cout << endl;
	cout << "scanner: end scanning for tokens" << endl;

	if (debug && verbose) {
		//test the tokens are inserted into vector successfully
		cout << "scanner: Printing tokens from vector:" << endl;
		vector<token>::iterator tokenIterator = tokens.begin();
		while (tokenIterator != tokens.end()) {
			printToken(*tokenIterator);
			tokenIterator++;
		}
	}
		
	//return token vector to parent class
	ts->setTokenVector(tokens);

	return 1;
}

//reads input from a text file and creates a 2 dimension array[lineNumber][elementNumber] to process tokens
void scanner::getDfaMap()
{
	int verbose = 0;

	if (debug) {
		cout << "getDfaMap: starting" << endl;
	}

	

	ifstream inFile;
	string fileName = "dfamap.txt";  //the source spreadsheet should be "DFA Table.xlsx"
	inFile.open(fileName.c_str());

	string validCharsString = "";

	getline(inFile, validCharsString);

	if (debug && verbose) {
		cout << validCharsString << endl;
	}

	{
		int i = 0;
		stringstream ssin(validCharsString);
		while (ssin.good()) {  //read in the first line of the dfamap.txt file which contains the valid characters
			string data;
			ssin >> data;

			if (data.compare("WS") == 0) //had to represent whitespace some other way than a space
				data = " ";

			validChars[i++] = data.c_str()[0];
		}
	}

	if (debug && verbose) {
		for (int i = 0; i < 81; i++)
			cout << validChars[i];
		cout << endl;
	}

	if (debug && !verbose) cout << "getDfaMap: read contents of validChars" << endl;

	if (debug && verbose) {
		cout << "getDfaMap: contents of validChars" << endl;
		for (int k = 0; k < cols; k++)
			cout << validChars[k] << ",";
		cout << endl << endl;
	}

	int lineNum = 0;
	//int dfaTest[rows][cols];
	while (!inFile.eof()) {  //start reading in rows for finite automaton logic
		int i = 0;
		string line;
		getline(inFile, line);

		stringstream ssin(line);
		//if (debug) cout << endl;
		while (ssin.good()) {
			string data;
			ssin >> data;
			if (!data.compare("|"))
				break;

			//this puts a lot of stuff in the screen
			/*if (debug && verbose) {
			int num = atoi(data.c_str());
			cout << num << ",";
			}*/

			dfaMap[lineNum][i++] = atoi(data.c_str()); //store fa logic in 2d array
		}
		lineNum++;
	}

	if (debug && !verbose)
		cout << "getDfaMap: read contents of dfaMap" << endl;
	if (debug && verbose) {
		cout << "getDfaMap: contents of dfaMap" << endl;
		for (int j = 0; j < rows; j++) {
			for (int k = 0; k < cols; k++)
				cout << dfaMap[j][k] << ",";
			cout << endl;
		}
		cout << endl;
	}

}

//checks for valid characters
int scanner::isValidCharacter(char c)
{
	for (int i = 0; i < cols; i++) {
		if (c == validChars[i])
			return 1;
	}
	if (c == '$')
		return 1;
	if (isspace(c))
		return 1;
	//cout << "isNotValid: " << c << endl;
	return 0;
}

//maps a valid character to the column in the dfamap.txt
int scanner::getCharacterIndex(char c)
{
	for (int i = 0; i < cols; i++) {
		if (c == validChars[i])
			return i;
	}
	return 0;
}

//processes a valid character and sets a "state" in FA when applying logic
token scanner::dfaProcess(char c, int lineNumber, int verbose)
{
	
	if (dfaInComment) {
		if (isspace(c))
			dfaInComment = 0;
	}
	else if (c == '$') {
		dfaInComment = 1;
		dfaString = "";
	}
	else if (!isspace(c)) {
		dfaMapValue = dfaMapTraversal(c, verbose);
		return dfaTokenDecode(dfaMapValue, lineNumber);
	}
	else {
		dfaMapValue = dfaMapTraversal(' ', verbose);
		return dfaTokenDecode(dfaMapValue, lineNumber);
	}



	dfaString += c;
	return token(NXT, "next", lineNumber);
}

//applies FA logic to get next state
int scanner::dfaMapTraversal(char c, int verbose)
{
	int charIndex = getCharacterIndex(c);
	int value = dfaMap[dfaMapValue][charIndex];
	if (debug && verbose) cout << "|" << dfaMapValue << "," << charIndex << "," << value << "|";
	return value;
}

//assigns a token when FA logic arrives at an endpoint
token scanner::dfaTokenAssign(int dfaTokenNum, int lineNum)
{
	dfaString = "";

	//I hate doing this, but couldn't get these values to referece correctly from the tokenTyp.h file
	string tokenText[] = { "BEGIN", "END", "IF", "FOR", "VOID", "VAR", "RETURN", "INPUT", "OUTPUT", "EQ", "GEG", "LEL", "NN", "CO", "PL", "MN", "TI", "DI", "PE", "OP", "CP", "CM", "OF", "CF", "SC", "OS", "CS", "ID", "INT", "EoF", "NUL", "NXT" };

	int i = getTokenIndex(dfaTokenNum);

	ostringstream oss;

	//tokens with ID or INT tokenTypes get a modified ID_IDNAME_TOKENNUM token description to preserve the values within the token
	if (i == ID || i == INT) {
		oss << tokenText[i] << "_" << identifier.substr(0,identifier.size() - 1) << "_" << tokenID++;
		token newToken((tokenTyp)i, oss.str(), lineNum, identifier.substr(0, identifier.size() - 1));
		dfaMapValue = 0;
		identifier = "";
		return newToken;
	}
	else { //other tokens only get the INST_TOKENNUM description
		oss << tokenText[i] << "_" << tokenID++;
		token newToken((tokenTyp)i, oss.str(), lineNum);
		dfaMapValue = 0;
		identifier = "";
		return newToken;
	}
}

//these values corespond to the token assignment values in the FA logic
int scanner::getTokenIndex(int dfaTokenNum)
{
	//string tokenSymbols[] = { "BEGIN", "END", "IF", "FOR", "VOID", "VAR", "RETURN", "INPUT", "OUTPUT", "=", ">=>", "<=<", "!!", ":", "+", "-", "*", "/", ".", "(", ")", ",", "{", "}", ";", "[", "]", "ID", "INT", "EoF", "NULL", "NXT" };
	int tokenValues[] = { 1000,1007,1011,1009,1026,1025,1022,1012,1019,1008,1010,1013,1015,1003,1021,1014,1024,1006,1020,1017,1004,1002,1016,1001,1023,1018,1005,1027,1028,1029 };

	for (int i = 0; i < sizeof(tokenValues) / sizeof(tokenValues[0]); i++) {
		if (dfaTokenNum == tokenValues[i])
			return i;
	}
	return -1;
}

//prints the token triplet data in this format (TOKEN_TYPE,TOKEN_DESC,LINE_NUM)
void scanner::printToken(token tkn)
{
	string tokenSymbols[] = { "BEGIN", "END", "IF", "FOR", "VOID", "VAR", "RETURN", "INPUT", "OUTPUT", "=", ">=>", "<=<", "!!", ":", "+", "-", "*", "/", ".", "(", ")", ",", "{", "}", ";", "[", "]", "ID", "INT", "EoF", "NULL", "NXT" };

	if (tkn.getTokenDesc().compare("next") != 0) {
		if (debug) cout << " Token generated: (" <<
			tokenSymbols[tkn.getTokenType()] << "," <<
			tkn.getTokenDesc() << "," <<
			tkn.getLineNumber() << ")";
	}
	if (debug) cout << endl;
}

//decides when to assign a token and when to get the next character
token scanner::dfaTokenDecode(int dfaMapValue, int lineNumber)
{
	if (dfaMapValue > 999) {
		//cout << " " << dfaMapValue;
		return dfaTokenAssign(dfaMapValue, lineNumber);
	}
	else {
		//cout << " " << dfaMapValue;
		return token(NXT, "next", lineNumber);
	}
}


scanner::~scanner()
{
}
