#include <iostream>
#include <fstream>
#include <vector>
#include "testScanner.h"
#include "scanner.h"
#include "token.h"

using namespace std;

testScanner::testScanner() : fileName(""), debug(0)
{
}

testScanner::testScanner(string fileNm, int debg) : fileName(fileNm), debug(debg)
{
}

int testScanner::run()
{
	int verbose = 0;

	if (debug) cout << "\nscanController: running in DEBUG MODE" << endl;
	if (verbose) cout << "scanController: running in VERBOSE MODE" << endl;
	if (debug || verbose) cout << endl;
	
	//open file specified from command line
	ifstream inFile; 
	inFile.open(fileName.c_str());

	if (!inFile) {
		cout << "Error opening: " << fileName << endl;
		return 0;
	}
	
	string line;
	allDataSize = 0;
	while (getline(inFile,line)) {
		allData[allDataSize++] = line;
	}
	
	inFile.close();

	if (debug && verbose) cout << "scanController: read contents of data file: " << fileName << endl;
	if (debug && verbose) {
		cout << "scanController: Contents of data file: " << fileName << endl;
		for (int i = 0; i < allDataSize; i++) {
			cout << "line " << i + 1 << ": " << allData[i] << endl;
		}
		cout << "scanController: size of allData: " << allDataSize << endl;
	}
	
	charPtr = &allData[allDataRow][0];

	//run scanner on text from file or input
	scanner scanner(debug);
	scanner.run(this);

	if (debug && verbose) {
		//test the tokens are passed back to scan controller
		cout << "scanController: Printing tokens from vector:" << endl;
		vector<token>::iterator tokenIterator = tokens.begin();
		while (tokenIterator != tokens.end()) {
			scanner.printToken(*tokenIterator);
			tokenIterator++;
		}
	}



	return 1;
}

char testScanner::getNextChar()
{
	if (*charPtr != '\0') {
		return *charPtr++;
	} else {
		if (++allDataRow >= allDataSize) {
			return '\0';
		} else {
			charPtr = &allData[allDataRow][0];
			return *charPtr++;
		}
	}
}

int testScanner::getLineNumber()
{
	return allDataRow + 1;
}

void testScanner::setTokenVector(vector<token> tokensIn)
{
	tokens = tokensIn;
}

vector<token> testScanner::getTokenVector() 
{
	return tokens;
}

token testScanner::nextToken()
{
	return token();
}

testScanner::~testScanner()
{
}
