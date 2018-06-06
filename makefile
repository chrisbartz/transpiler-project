comp: main.o  testScanner.o  token.o  scanner.o parser.o treeTraverse.o node.o varStackAdapter.o
	g++ -o comp main.o  testScanner.o  token.o scanner.o parser.o treeTraverse.o node.o varStackAdapter.o

main.o: main.cpp 
	g++ -c -std=c++11 main.cpp

testScanner.o: testScanner.cpp testScanner.h
	g++ -c -std=c++11 testScanner.cpp

token.o: token.cpp token.h 
	g++ -c -std=c++11 token.cpp

scanner.o: scanner.cpp scanner.h
	g++ -c -std=c++11 scanner.cpp

parser.o: parser.cpp parser.h
	g++ -c -std=c++11 parser.cpp
	
treeTraverse.o: treeTraverse.cpp treeTraverse.h
	g++ -c -std=c++11 treeTraverse.cpp
	
node.o: node.cpp node.h
	g++ -c -std=c++11 node.cpp
	
varStackAdapter.o: varStackAdapter.cpp varStackAdapter.h
	g++ -c -std=c++11 varStackAdapter.cpp