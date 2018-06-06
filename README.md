// readme : P4 Back End - Code Gen + Storage
// Christopher Bartz
// cyb01b
// CS4280 E02

This is implemented using Local Scoping Option.

This P4 directory has a couple of subdirectories: 
asm: i put asm test files in this folder for reference
dat: the test programs in the language to be compiled
source: source code files - use make command to compile

P1: I am using a table driven scanner and it is stored in the dat folder where I expect you 
may be doing your testing since all of the programs to be compiled are here.

P3/P4: local 

Then state anything important:  I implemented true local scoping - globals will work inside of local scopes but a 
local scope like this will not work for y.  I thought it was supposed to be C - like so that is what I implemented:

VOID x
  BEGIN 
    VOID y
      BEGIN
        INPUT y
        OUTPUT y
...

Quick Start: cd to the source folder. Execute "make" command to build.  mv "comp" to ../dat folder.  
dfamap.txt needs to be present for the scanner to work.  The "comp" command will assume .datE02 file suffixes.
Compile programs by executing "comp test1" for test1.datE02 and it will create an .asm file to execute in 
VirtMach (also included in dat folder).

DfaTable.xlsx is in source folder and contains my finite automaton lookup table.

If you want to change the level of information output on the screen, you can go into the treeTraverse.cpp, 
scanner.cpp, parser.cpp, and varStackAdapter.h files and change a value called "verbose" to 1 or 0 to turn 
verbose output on or off.  Turning DEBUG to 0 in main.cpp should quiet almost all extra screen output.

I tested with all test scripts provided and know that the last 2 tests: test11 and test12 will fail.  Test11
will fail compillation due to my local scoping design. Test12 will compile but fail in the VirtMach exectuion.

