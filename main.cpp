// main.cpp : P4 Back End - Code Gen + Storage
// Christopher Bartz
// cyb01b
// CS4280 E02

#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include "testScanner.h"
#include "token.h"
#include "parser.h"
#include "treeTraverse.h"

//setting debug here will set for all classes in project
#define DEBUG 1

using namespace std;

int main(int argc, char* argv[])
{
	//setting verbose only sets for this class/file
	int verbose = 0;
	cout << "Christopher Bartz\ncyb01b\nCS4280 E02\nP4 Back End - Code Gen + Storage\n" << endl;
	string tmpFileName = "";
	string fileName = "";
	string shortFileName = "";

	if (DEBUG) cout << "main: running in DEBUG MODE\n" << endl;
	if (DEBUG && verbose) cout << "main: running in VERBOSE MODE\n" << endl;
	
	//check command line args
	if (argc < 2) {
		tmpFileName = "tmpFile.datE02";
		fileName = tmpFileName;
		ofstream tmpFile(tmpFileName.c_str());
		cout << "Enter input.  Terminate with EOF character:" << endl;
		
		char c;
		while (cin.get(c)) {
			tmpFile << c;
		}
		tmpFile.close();
		//cout << "Filename missing!\nSyntax: " << argv[0] << " fileName" << endl;
	} else {
		if (DEBUG) {
			cout << "argc:" << argc << endl;
			cout << "argv[1]:" << argv[1] << endl;
		}

		shortFileName = (string)argv[1];
		fileName = (string) argv[1] + ".datE02";
		if (DEBUG) cout << "fileName:" << fileName << endl;
	}
	
	if (fileName.compare("") != 0) {
		testScanner scanController(fileName, DEBUG);
		if (scanController.run()) {
			cout << "\nmain: scanController finished successfully: \n" << fileName << " generated" << endl;

			cout << "main: retreiving tokens from scanController" << endl;
			vector<token> tokens = scanController.getTokenVector();

			//This just didnt work like I wanted it to so I ended up making it work by passing the token vector around

			//for (int i = 0; i < 5; i++) {
			//token newToken = scanController.nextToken();
			//	cout << i /*<< " : " << newToken.getTokenDesc() */<< endl;
			//}
			
			if (DEBUG && verbose) {
				for (vector<token>::iterator tokenIterator = tokens.begin(); tokenIterator != tokens.end(); ++tokenIterator) {
					cout << tokenIterator->getTokenDesc() << endl;
				}
				cout << endl;
			}
				
			parser parsr(DEBUG,tokens);
			node* rootNode = parsr.run();  //in theory should build parse tree and return the root node

			if (rootNode != nullptr) {
				if (verbose) cout << "xxxxxxmainxxxxx: printing root node: " << rootNode->getNodeType() << " (" << parser::getTokenSymbol(rootNode->getToken()->getTokenType()) << ":" << rootNode->getToken()->getTokenDesc() << ":" << rootNode->getToken()->getLineNumber() << ")" << endl;

				cout << "\nmain: start treeTraverse\n" << endl;
				treeTraverse tt(rootNode,shortFileName,DEBUG);  //in theory should traverse tree and print 
				tt.run();
				cout << "main: finish treeTraverse successfully\n" << endl;
			}
			
		} else {
			cout << "\nmain: scanController finished with errors: " << fileName << endl;
		}
	}

	if (!DEBUG && tmpFileName.compare("") != 0) {
		remove(tmpFileName.c_str());
		cout << "main: Temporary file: " << tmpFileName << " has been deleted" << endl;
	}
	
	return 0;
}

