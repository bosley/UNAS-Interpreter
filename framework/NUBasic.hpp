#ifndef NUBASIC_HEADER_GUARD
#define NUBASIC_HEADER_GUARD

#include<map>
#include<queue>
#include<stack>
#include<string>
#include<vector>
#include<iomanip>
#include<sstream>
#include<fstream>
#include<iostream>
#include<algorithm>

#define STR_VAL 0
#define INT_VAL 1
#define FLT_VAL 2

#define ITOS( x ) dynamic_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

struct insertItem
{
	int lineNum;
	std::string line;
};

struct dataItems
{
	std::vector<char> ops;
	std::vector<std::string> items;
};

struct nubVariable
{
	std::string name;	 // Name of the variable
	char dtype;		 // Data type
	int ival;		 // Int value
	std::string sval;	 // String value
	float fval;		 // Float value
	int mi;			 // Memory index
	std::string translation; // Unas text designation
	std::string ftranslation;	

	void operator= (const nubVariable &n)
	{
		name 	= n.name;
		dtype 	= n.dtype;
		ival 	= n.ival;
		sval	= n.sval;
		fval 	= n.fval;
		mi	= n.mi;
		translation  = n.translation;
		ftranslation = n.ftranslation;		 
	}
};

class NUBasic
{

public:
	NUBasic();

	// Turn debug on
	void toggleDebug();

	// Evaluate memory usage
	void doMemoryEvaluation(char*);					

	// Feed the variable assignment machine	
	void assignVariable(std::string);

	// Print something
	void doPrint(std::string);

	// Goto statement
	void doGoto(int);

	// For loop
	void doForLoop(std::string, int);
	void doFLNext(std::string);

	// If statement
	void doIf(std::string, std::string, std::string, int);

	// If statement
	void doRead(std::string);

	// Perform end
	void doEnd();

private:
	/*
		Data
	*/
	int cmi;		// Current memory index
	int uspi;		// Unas stack pointer index
	int stringIndex;	// Keep track of string count
	int floatIndex;		// Keep track of floats
	int segIndex;		// For making code segments
	int forLoopIndex;	// For keeping track of for loop regs
	int fljmpIndex;		// Keep track of for loop labels
	int currentInputLine;	// What line we are operating with
	bool nDebug;		// Debug state
	bool pState;

	std::stack<int>	flis;	// For Loop index stack - loop register control
	std::vector<std::string> fllabels;	

	char mops[7];		// Operators

	std::vector<int> outputFileLT;			// For translating insertion points (IFs GOTOs, etc)
	std::vector<std::string> outputFile;		// Final product
	std::vector<struct nubVariable> variables;	// Variable list
	std::vector<std::string> stringList;
	std::vector<std::string> floatList;
	std::vector<std::string> reserved;		
	std::vector<std::string> supportCode;		// Extra code that we may need to add 
	std::vector<struct insertItem> lateInserts;
	/*
		Methods
	*/
	// Support Methods
	void display(std::string);					// Tell the user something!
	std::string stripChar(char, std::string);			// Remove a char 
	std::vector<std::string> splitString(char, std::string);	// Split a string
	int analyzeDataType(std::string);				// Tell us what something is
	std::string postfix(std::string);				// Convert expression to postfix
	void insertLineAtIndex(int, std::string);			// Add line at index to outputFile

	void loadSupportingFiles(char);				// Called to load unas support code

	// Primary Methods
	void constructString(std::string, std::string, bool);		// Create a new string!
	void constructInteger(std::string, std::string, bool);		// Create a new int!
	void constructFloat(std::string, std::string, bool);		// Create a new float!
	struct dataItems explodeStatement(std::string);			// Pick apart the statement
	void setVariable(char, std::string, std::string);		// Does all memory storage/ var updating
};



#endif
