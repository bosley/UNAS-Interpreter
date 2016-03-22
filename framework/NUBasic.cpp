/*
The MIT License (MIT)

Copyright (c) 2016 Josh Bosley

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "NUBasic.hpp"

/*
#
#	Public Access
#
*/

NUBasic::NUBasic()
{
	// Set defaults
	cmi  = 0;
	uspi = 0;
	segIndex = 0;
	floatIndex = 1;
	nDebug = false;
	pState = false;
	stringIndex = 1;
	fljmpIndex = 0;
	forLoopIndex = 40;
	strcpy(mops,"+-*/^%");
	currentInputLine = 0;	
	outputFileLT.push_back(0);

	reserved.push_back("FOO"); 	// I made a promise to myself that
	reserved.push_back("foo"); 	// if I had the power, I would NEVER
	reserved.push_back("BAR"); 	// allow the use of these words within
	reserved.push_back("bar"); 	// a programming language.
	reserved.push_back("FU") ; 	// This is me, keeping that promise 	
	reserved.push_back("fu") ;
	reserved.push_back("fubar");
	reserved.push_back("FUBAR");
	reserved.push_back("foobar");	// Will eventually make something
	reserved.push_back("FOOBAR");	// to generate all possible fubars

	// Let the user know that we are getting started
	std::cout << "\t BASIC UNAS - Josh Bosley\n " << std::endl;

	// Add main def stuff for output file
	outputFile.push_back(".init main\n.segment\t32768\n\n\nmain:");

	outputFileLT[currentInputLine] += 4;
}

// Toggle debug mode, and alert user
void NUBasic::toggleDebug()
{
	std::string s;
	(nDebug) ? nDebug = false : nDebug = true;
	(nDebug) ? s = "ON" : s = "OFF";
	std::cout << "\t\t[DEBUG " << s << "]" << std::endl;

	std::cout << "\tTODO: \n\t" 
		  << "[X]  Figure out how to constructFloat, as of now, no float vars can be made. \n\t"
		  << "[X]. Come up with a way to analyze an input of math ops and convert equation to unas\n\t"
		  << "[X]. Create PRINT functionality \n\t"
		  << "\t[X]. CHANGE STRING FUNC TO INCLUDE USE OF + \n\t"
		  << "\t[X]. CHANGE PRINT FUNC TO INCLUDE USE OF + \n\t"
		  << "[X]. Ensure users can make comments in their code \n\t"
		  << "[X]. IF statements \n\t"
		  << "\n\n\t. [CURRENT OBJECTIVE] LATE INSERTS, MAPPING CODE TO CORRECT PLACE IN UNAS CODE \n \n\t"
		  << "7. READ in user info \n\t"
		  << "[X]. FOR loops \n\t"
		  << "[X]. GOTO \n\t"
		  << "\n\n\t[A]. SAME MAPPING ISSUE AS IN IF STATEMENTS \n\n\t"
		  << std::endl;
}

// Estimate memory usage 
void NUBasic::doMemoryEvaluation(char *file)
{
	/*
		Read through the file and build and estimation for 
		memory usage. This is done prior to parsing to map
		variable locations in memory
	*/

	std::ifstream infile(file);
	std::string line, token = "";
	while (std::getline(infile, line))
	{
		for(int i = 0; i < line.length(); i++)		
			if(line[i] == ' ')
			{						// Add memory size based on key word
									// Have to allocate 4 for how I do the print command
				if(token == "LET" || token == "READ" || token == "PRINT") 
					uspi += 4;
				else if (token == "FOR")		// Storing 8
					uspi += 8;
				token = "";
				i = line.length();			// Once we find the word, stop scanning
			}
			else
				token += line[i];			// Add current char to token
	}

	std::ostringstream sp;
    	sp << uspi;
	outputFile.push_back("\n\tsub sp, sp, $" + sp.str() + "\t\t# Make room in the stack for program variables\n");
	
	if(nDebug)
	{
		std::cout << "\t\t[MEMORY EVALUATION]" << std::endl;
		std::cout << "\t[Unas Stack Pointer Index [uspi] = " << uspi << "]" << std::endl;
	}
	infile.close();
}

// Retrieve an set a variable
void NUBasic::assignVariable(std::string input)
{
	currentInputLine++;			// Only ever called from yac
	outputFileLT.push_back(0);

	if(nDebug)
		display("\n\n\tMethod : assignVariable(" + stripChar('\n', input) + ")");

	// Split the string in half, and doctor up the sides 
	std::vector<std::string> mySides( splitString('=', input) );
	std::string rhs = stripChar('\n', mySides[1]);
	std::string lhs = stripChar('\n', mySides[0]);
	lhs = stripChar(' ', lhs);

	// We need to know if we are dealing with any strings or not
	bool isString = false;
	for(int i = 0; i < rhs.length(); i++)
		if(rhs[i] == '"')
			isString = true;
	
	/*
		At this point, we want to go through the rhs
		and tell if there are any operations happening.
		We need to make sure that we don't mistake an
		op that is part of the string for an actual op
	*/
	bool inString = false, hasOps = false;
	for(int i = 0; (i < rhs.length() && !hasOps); i++)
	{
		if(isString)					
		{
			if(rhs[i] == '"')		// Toggle if we are scanning within "s
				(!inString) ? inString = true : inString = false;
			else
				if(!inString)		// If we aren't in "s, scan for ops
					for(int j = 0; j  < 6 ; j++)
						if(rhs[i] == mops[j])
							hasOps = true;
		}
		else	
		{
			for(int j = 0; j  < 6 ; j++)	// If the data isn't a string
				if(rhs[i] == mops[j])	// We need to test each item
					hasOps = true;
		}
	}

	// Make sure they aren't trying to use reserved words
	if(!pState)	// Allow the print statement to use reserved words :D
		for(int i = 0; i < reserved.size(); i++)
			if(lhs == reserved[i])
			{
				display(lhs + " is a reserved word. Can not use.\n");
				exit(1);
			}

	
	/*
		Now that we have all the information we need about the data, 
		we can begin to put it all together! 
	*/

	if(isString)
		constructString(lhs, rhs, hasOps);			// Create a new string variable
	else
	{
		switch(analyzeDataType(rhs))				// Not a string? Well what is it!
		{
		case STR_VAL:
			constructString(lhs, rhs, hasOps);		// It could be a string in disguise!
			break;
		case INT_VAL:
			constructInteger(lhs, rhs, hasOps);		// Create a new integer variable
			break;
		case FLT_VAL:
			constructFloat(lhs, rhs, hasOps);		// Create a new float variable
			break;
		default:
			display("\nError: Unknown data type! - Analyze data type\n");	// Exit because something horrible happened
			exit(1);
			break;
		}
	}
}

void NUBasic::doPrint(std::string val)
{
	currentInputLine++;			// Only ever called from yac
	outputFileLT.push_back(0);
	pState = true;				// Allow the use of reserved variable usage
	int vindex = 0;
	std::string hyp = ("foo = " + val);	// Assign the current item to the foo var
	bool found = false;
	assignVariable(hyp);		
	for(int i = 0; i < variables.size() && !found; i++)
		if(variables[i].name == "foo")			// Locate the var
		{						// Load the var memdex to r10
			vindex = i;
			found = true;
			switch(analyzeDataType("foo"))
			{
			case STR_VAL:
				outputFile.push_back("\n\tldw r50, $" + ITOS(variables[i].mi) + "(sp)\t\t# ldw for print\n");
				outputFile.push_back("\tcall _printstr\n");
				break;
			case INT_VAL:
				outputFile.push_back("\n\tldw r59, $" + ITOS(variables[i].mi) + "(sp)\t\t# ldw for print\n");
				outputFile.push_back("\tcall _printint\n");
				break;
			case FLT_VAL:
				outputFile.push_back("\n\tldf f59, $" + ITOS(variables[i].mi) + "(sp)\t\t# ldf for print\n");
				outputFile.push_back("\tcall _printfloat\n");
				break;
			default:
				display("\nError: Unknown data type!\n");	// Exit because something horrible happened
				exit(1);
				break;
			}
		}
	if(!found)
	{
		display("\n\tAn internal issue has occured within the doPrint function\n");
		exit(1);
	}
	pState = false;
	
	// Purge the foo!
	variables.erase(variables.begin() + vindex);

	outputFileLT[currentInputLine] += 6;	// Reminder of how many lines
}

void NUBasic::doGoto(int index)
{
	currentInputLine++;			// Only ever called from yac
	outputFileLT.push_back(0);
	outputFile.push_back("\n");
	// Make the segment to jump to, and insert into file 
	std::string s = ("\tJABGT" + ITOS(segIndex) + ":\t\t\t# Top of a goto statemnt\n");
	insertLineAtIndex(index, s);

	outputFile.push_back("\n");
	// Add the jmp statment that is the goto entry point	
	outputFile.push_back("\tjmp JABGT" + ITOS(segIndex) + "\t\t# Bottom of a goto statement\n");
	segIndex++;

	outputFileLT[currentInputLine] += 4;
}

void NUBasic::doForLoop(std::string statement, int hs)
{
	currentInputLine++;			// Only ever called from yac
	outputFileLT.push_back(0);
	if(nDebug)
		display("doForLoop(" + statement + ")\n");

	fljmpIndex++;	
	if(fljmpIndex == 10)
	{
		display("\n\n\tError: Too many nested for loops\n\n");
		exit(1);
	}

	std::vector<std::string> items = splitString('=', statement);	// Split by =
	std::string var = stripChar(' ', items[0]);			// Grab var name
	items.erase(items.begin());
	items = splitString(' ', items[0]);				// Split remaining items

	int ifrom = 0, ito = ifrom, istep = 1;				// From, to, and assume step is 1

	std::stringstream iss(items[1]);				// Set start
	iss >> ifrom;
	if(iss.fail())
	{
		std::cout << "Failed to convert int in for loop\n" << std::endl;
		exit(1);
	}

	std::stringstream itss(items[3]);				// Set end 
	itss >> ito;
	if(itss.fail())
	{
		std::cout << "Failed to convert int in for loop\n" << std::endl;
		exit(1);
	}

	if(1 == hs)							// If there is a STEP, get its num
	{								// And overwrite the 1
		std::stringstream ss(items[5]);
		ss >> istep;
	}

	std::string fltop = ("\nFLT" + ITOS(fljmpIndex) + ":\n");	

	int tcntrflt = 0;
	bool isafltop = false;
	for(int i = 0; i < fllabels.size() && !isafltop; i++)
		if(fllabels[i] == fltop)
		{
			tcntrflt++;
			fljmpIndex += tcntrflt;
			fltop = ("\nFLT" + ITOS(fljmpIndex) + ":\n");
		}
	fllabels.push_back(fltop);

	setVariable('i', var, ITOS(ifrom)); 					// Attempt to create the variable in the for loop
	setVariable('i', ("_FLVAR_STEP_NUM_" + ITOS(fljmpIndex)), ITOS(istep));	// Create var to keep teack of step

  	std::string regLine = ("\tlda r" + ITOS(forLoopIndex) + ", $" + ITOS(ito) + "\t\t# Load FOR LOOP limit [" + ITOS(ito) + "]\n");
									// This is how we can call correct top when loop is nested
	outputFile.push_back(regLine);					
	outputFile.push_back(fltop);					// Write lines	

	flis.push(forLoopIndex);					// Add the current float index
	forLoopIndex++;

	outputFileLT[currentInputLine] += 4;
}

/*
	If all goes well, at this point we can ref the end when we find a NEXT VAR 
	We can eval the forLoopIndex on top of the stack, and comp the VAR's ival to the forLoopIndex
	to determine if we should jump to top, or continue on. If we continue on, pop it off the stack.
*/
void NUBasic::doFLNext(std::string var)
{
	currentInputLine++;			// Only ever called from yac
	outputFileLT.push_back(0);

	var = stripChar('\n', var);

	if(nDebug)
		display("doFLNext(" + var + ")\n");

	// LOAD THE FOR LOOP VAR
	int vindex = -1;
	for(int i = 0; i < variables.size() && vindex == -1; i++)
		if(variables[i].name == var)
			vindex = i;
	if(vindex == -1)
	{
		display("\nError: NEXT " + var + " <- Var not found\n");
		exit(1);
	}

	// LOAD THE VAR FOR NEXT
	int niIndex = -1;
	for(int i = 0; i < variables.size() && niIndex == -1; i++)
		if( variables[i].name == ("_FLVAR_STEP_NUM_" + ITOS(fljmpIndex)))
			niIndex = i;
	if(niIndex == -1)
	{
		display("\nError: STEP " + ("_FLVAR_STEP_NUM_" + ITOS(fljmpIndex)) + " <- Var not found\n");
		exit(1);
	}

	// WRITE OUT THE LDA COMMANDS TO LOAD VAR, AND NEXT
        outputFile.push_back("\n\tldw r7, $" + ITOS(variables[vindex].mi) +
                                "(sp)\t\t# Load " + var + " to test for loop condition\n");
	outputFile.push_back("\tldw r8, $" + ITOS(variables[niIndex].mi) + 
				"(sp)\t\t# Load " + var + " to test for loop condition\n");

	int flindex = flis.top();					// Grab the current for loop var
	int flreg = flindex - 40;

	outputFile.push_back("\n\t\t\t\t# Test loop conditions");
	outputFile.push_back("\n\tadd r7, r7, r8\t\t# Add step\n");	// Perform addition on for loop var
	outputFile.push_back("\tstw r7, $" + ITOS(variables[vindex].mi) + "(sp)\t\t# Store back in VAR location\n");	
	outputFile.push_back("\tble r7, r" + ITOS(flindex) + ", FLT" + ITOS(fljmpIndex) + 
				"\t# Test if for loop conditions are met by gt\n");
	
	forLoopIndex--;							// -1 reg use
	flis.pop();				
	fljmpIndex--;							// +1 for loop branch tops
}

void NUBasic::doIf(std::string slhs, std::string op, std::string srhs, int line)
{
	currentInputLine++;			// Only ever called from yac
	outputFileLT.push_back(1);
	outputFile.push_back("\n");

	if(nDebug)
		display("doIf(" + op + ")");

	if(line > outputFile.size() || line < 0)
	{
		display("Requested line jump indexing error: " + ITOS(line) + "\n");
		exit(1);
	}

	// Test rhs, and lhs to determine if they are variables or not
	int rhs = 0, lhs = 0;
	bool rhsiv = false, lhsiv = false;
	std::stringstream ssrhs(srhs);
	std::stringstream sslhs(slhs);
	ssrhs >> rhs;	
	sslhs >> lhs;	
	
	// If they are vars, their index in var list will be indicated via the corresponding int rhs || lhs
	// If they are an int variable, they will also be stored in the correspoinding int rhs || lhs

	// If rhs failed, get var index, and load
	if(ssrhs.fail())
	{
		rhsiv = true;	
		bool found = false;
		std::vector<std::string> items = splitString(' ', srhs);
		for(int i = 0; i < variables.size() && !found; i++)
			if(variables[i].name == items[0])
			{
				found = true;
				rhs = i;
			}
		if(!found)
		{
			display("Could not locate rhs variable : " + items[0] + " for IF statement.\n");
			exit(1);
		}
		outputFile.push_back("\tldw r6, $" + ITOS(variables[rhs].mi) + "(sp)\t # Load rhs var from mem for IF\n"); // Put var loc
	}
	else
		outputFile.push_back("\tldw r6, $" + ITOS(rhs) + "\t# Load rhs int from mem for IF\n");	// Put raw int

	// If lhs failed, get the var index
	if(sslhs.fail())
	{
		lhsiv = true;	
		bool found = false;
		std::vector<std::string> items = splitString(' ', slhs);
		for(int i = 0; i < variables.size() && !found; i++)
			if(variables[i].name == items[0])
			{
				found = true;
				lhs = i;
			}
		if(!found)
		{
			display("Could not locate lhs variable : " + items[0] + " for IF statement.\n");
			exit(1);
		}
		outputFile.push_back("\tldw r5, $" + ITOS(variables[lhs].mi) + "(sp)\t # Load lhs var from mem for IF\n"); // Put var loc
	}
	else
		outputFile.push_back("\tldw r5, $" + ITOS(lhs) + "\t# Load lhs int from mem for IF\n");	// Put raw int

	// Now we need to make an entry point in outputFile for the top of the IF, in the case that it fires	
	std::string jmppnt = ("\n\tJABIF" + ITOS(segIndex) + ":\t\t\t# Top of an IF statemnt\n"); // Piggybacks off of GOTO's segment index scheme
	insertLineAtIndex(line, jmppnt);

	// It is now time to determine what the op we are using is to build the correspoinding code
	op = stripChar(' ', op);	// Do this, 
	op = stripChar('\n', op);    // and this just to be safe

	if(">" == op)
		outputFile.push_back("\tbgt r5, r6, JABIF" + ITOS(segIndex) + "\n");
	if("<" == op)
		outputFile.push_back("\tblt r5, r6, JABIF" + ITOS(segIndex) + "\n");
	if(">=" == op)
		outputFile.push_back("\tbge r5, r6, JABIF" + ITOS(segIndex) + "\n");
	if("<=" == op)
		outputFile.push_back("\tble r5, r6, JABIF" + ITOS(segIndex) + "\n");
	if("==" == op)
		outputFile.push_back("\tbeq r5, r6, JABIF" + ITOS(segIndex) + "\n");

	// INC the GOTO/IF segIndex
	segIndex++;
	outputFileLT[currentInputLine] += 2;
}

void NUBasic::doRead(std::string statement)
{
	currentInputLine++;
	outputFileLT.push_back(0);

	statement = stripChar('\n', statement);
	statement = stripChar(' ', statement);
	if(nDebug)
		display("doRead(" + statement + ")");

	int vindex;
	bool found = false;
	for(int i = 0; i < variables.size() && !found; i++)
		if(variables[i].name == statement)
		{
			found = true;
			vindex = i;
		}
	if(!found)
	{
		display("Error: Reading in unassigned variable! - Variable must be assigned prior to read.");
		exit(1);
	}

	outputFile.push_back("\n");
	switch(analyzeDataType(statement))
	{
		case STR_VAL:
		{
                        outputFile.push_back("\tcall _readch\t\t\n");
                        outputFile.push_back("\tstw r2, $" + ITOS(variables[vindex].mi) + "(sp)\n");
			break;
		}
		case INT_VAL:
		{
			outputFile.push_back("\tcall _geti\n");
			outputFile.push_back("\tstw r2, $" + ITOS(variables[vindex].mi) + "(sp)\n");
			break;	
		}
		case FLT_VAL:
		{
			outputFile.push_back("\tcall _getf\n");
			outputFile.push_back("\tstf f2, $" + ITOS(variables[vindex].mi) + "(sp)\n");
			break;
		}
	}

        outputFileLT[currentInputLine] += 3;	
}

void NUBasic::doEnd()
{
	/*
		Write out .s file, and simulator file
	*/
	
	// Add in the late inserts
	if(lateInserts.size() > 0)
		for(int i = 0; i < lateInserts.size(); i++)
			outputFile.insert(outputFile.begin() + lateInserts[i].lineNum, lateInserts[i].line);

	outputFile.push_back("\n\texit\n");

	// Append the support code to the main output file
	if(supportCode.size() > 0)
		outputFile.insert( outputFile.end(), supportCode.begin(), supportCode.end() );

	// Write simulator file	
	std::ofstream simFile("simulator.config");
	simFile << "UNAS2\nCORES 1\n0x0010 /colossus/projects2/unas/working/unas/simulator/subprogs/stdio" << std::endl;
	simFile.close();

	// Write output file
	std::ofstream file("nub.s");
	for(int i = 0; i < outputFile.size(); i++)	//	Write out assembly file 
		file << outputFile[i];
	file << "\n\n";
	
	// Write out strings and floats
	for(int i = 0; i < stringList.size(); i++)	//	Write out strings
		file << stringList[i];
	file << "\n\n";
	
	for(int i = 0; i < floatList.size(); i++)	//	Write out floats
		file << floatList[i];
	file.close();
	
	std::cout << std::endl;
}

/*
#
#	Private Access
#
*/
void NUBasic::display(std::string msg)
{
	std::cout << msg << std::endl;		// Display a message to the user
}

std::string NUBasic::stripChar(char c, std::string str)
{						// Take something out of a string
	std::string t = "";
	for(int i = 0; i < str.length(); i++)
	{
		if(str[i] != c)
			t += str[i];
	}
	return t;
}

/*
		Split a string by a char, and return a list of strings
*/
std::vector<std::string> NUBasic::splitString(char c, std::string str)
{
	std::vector<std::string> ret;		// Create return item	
	std::string token;			
	std::istringstream ss(str);		
	while(std::getline(ss, token, c)) 	// Get each token
		ret.push_back(token);		// Add token to return item
	return ret;				// Return the return item
}

/*
		Here we check each item in the data to judge what type we want to set the var.
		If any to-int conversion fails, a variable is assumed, so the vars are checked
		to determine var type. All items are scanned with floats taking priority to ints,
		and string val being returned if a string type is found.
*/
int NUBasic::analyzeDataType(std::string data)
{
	if(nDebug)
		display("analyzeDataType(" + data + ")");

	// Some things we need to keep track of
	int dataType = -1;
	bool setAsFloat = false;

	// Explode the data so we can test each individual item		
	struct dataItems statementInformation = explodeStatement(data);
	for(int i = 0; i < statementInformation.items.size(); i++)
	{
		int anInt;				
		std::stringstream ss(statementInformation.items[i]);
		ss >> anInt;									// If item can be converted, its an int or float
		if(ss.fail())
		{
			for(int q = 0; q < variables.size(); q++)				// Check the vars to see what its data type is
				if(variables[q].name == statementInformation.items[i])
					if(variables[q].dtype == 's')				// its a string
						return STR_VAL;
					else if(variables[q].dtype == 'i' && !setAsFloat)	// its an int
						dataType = INT_VAL;
					else if(variables[q].dtype == 'f')			// its a float
						setAsFloat = true;
		}
		else
		{
			bool dec = false;				// Check to see if the value contains a decimal
			for(int k = 0; k < statementInformation.items[i].length() && !dec; k++)
				if(statementInformation.items[i][k] == '.')
					dec = true;
			if(dec)
				setAsFloat = true;					// If it has one, its a float
			else
				dataType = INT_VAL;					// If not, its an int
		}
	}

	if(setAsFloat)
		dataType = FLT_VAL;
	return dataType;
}

/*
	Takes in an expression, and performs shuntyard 
	Returns the expression in postfix form
*/
std::string NUBasic::postfix(std::string exp)
{
        std::stack<char> op;                                    // Stack for postfix conversion
        std::string res = "";
        std::map<char,std::pair<int,int> > ops;                 // Map for symbol handling priority
        ops['^'] = std::make_pair(4,1); // E
        ops['*'] = std::make_pair(3,0); // M
        ops['/'] = std::make_pair(3,0); // D
        ops['+'] = std::make_pair(2,0); // A
        ops['-'] = std::make_pair(2,0); // S

        for(int i = 0; i < exp.size(); i++)                     // Go through string L to R
        {
                if(exp[i] == '(')                               // Op
                        op.push(exp[i]);                        // Add to stack
                else if(exp[i] == ')')                          // Cp
                {
                        while(op.top() != '(')
                        {
                                res += op.top();                // Add to return string
                                op.pop();
                        }
                        op.pop();
                }
                else if(ops.find(exp[i]) != ops.end())          // Find op
                {
                        while(!op.empty() &&
                                ((ops[exp[i]].second == 0 &&
                                  ops[op.top()].first == ops[exp[i]].first) ||
                                  ops[exp[i]].first < ops[op.top()].first))
                        {
                                res += op.top();                // Add op to ret string
				op.pop();
                        }
                        op.push(exp[i]);
                }
                else
                        res += exp[i];                          // Add item to ret string
        }
        while(!op.empty())                                      // Add remaining ops
        {
                res += op.top();
                op.pop();
        }
        return res;                                             // Retrun the postfix item
}

/*
	Put a line at a specific line number within the main function
	in the input file
*/
void NUBasic::insertLineAtIndex(int index, std::string line)
{
	int newIndex = 2;
	for(int i = 0; i < index-2; i++)
	{
		std::cout << "INPUT LINE [" << i << "] TRANSLATED TO [" << outputFileLT[i] << "] LINES\n";
		newIndex += (outputFileLT[i]+1);
	}

	std::cout << "LINE 653: CALCULATE INSERTION POINT FOR insertLineAtIndex\n";

	std::cout << "\n\t INSERT AT LINE [" << newIndex << "] IN ASM FILE \n";

	struct insertItem m;
	m.lineNum = newIndex;
	m.line = line;
	lateInserts.push_back(m);

	currentInputLine++;
}

void NUBasic::loadSupportingFiles(char id)
{
	if(id == 'p')
	{
		// Load the power code into supporting files
		std::string line;
  		std::ifstream sup ("framework/support/power");
  		if(sup.is_open())
  		{
    			while(std::getline(sup, line))
    			{
				supportCode.push_back(line);
				supportCode.push_back("\n");
    			}
		}
		else
		{
			display("Unable to load support file : framework/support/power");
			exit(1);
		}
		sup.close();
	}
/*
	if(id == 'o')
	{
				ROOM FOR EXPANSION, COULD EASILY ADD OTHER SUPPORT FILES
				TO EXPAND THE ABILITY OF THE LANGUAGE
	}
*/
}


// END OF THE PRIVATE HELPER METHODS

/*
	Given some input, create a string
*/
void NUBasic::constructString(std::string lhs, std::string rhs, bool hasOps)
{
	if(nDebug)					// If debug mode is on decide to what to say
	{						// to the user
		std::string s;			
		(hasOps) ? s = "YES" : s = "NO";	// If it has a +, say so
		display("constructString(" + lhs + ", " + rhs + ", " + s + ")");
	}

	if(hasOps)
	{
		/*
			 Create a new string from multiple parts. Assuming operation is +, 
			 as all others do not pertain to basic string operations.
			 Finds value of variable if input is a var, or adds the strings
		*/
		std::string newString = "";
		struct dataItems sInfo = explodeStatement(rhs);			// Get items in statement

		for(int i = 0; i < sInfo.items.size(); i++)
		{
			bool isString = false;
			std::string cItem = sInfo.items[i];
			for(int j = 0; j < cItem.length(); j++)			// See if the statement item is a string
				if(cItem[j] == '"')				
					isString = true;

			if(isString)
			{
				std::string t = stripChar('"', sInfo.items[i]);		// Trim, and add the string
				t = stripChar('\n', t);
				newString += t;
			}
			else
			{
				for(int k = 0; k < variables.size(); k++)
					if(stripChar(' ', sInfo.items[i]) == variables[k].name)
					{
						if(variables[k].dtype == 's')
						{
							newString += stripChar('\n', variables[k].sval);
						}
						else if(variables[k].dtype == 'i')
						{
							std::ostringstream ss;
							ss << variables[i].ival;
							std::string s(ss.str());
							newString += s;
						}
						else if(variables[k].dtype == 'f')
						{
							std::ostringstream ss;
							ss << variables[i].fval;
							std::string s(ss.str());
							newString += s;
						}
					}
			}
		}	
		setVariable('s', lhs, newString);
	}
	else						// If there is no string operations, create the string variable
	{
		setVariable('s', lhs, rhs);		
	}
}


/*
	Given some input, create an integer
*/
void NUBasic::constructInteger(std::string lhs, std::string rhs, bool hasOps)
{
	if(nDebug)
	{
		std::string s;
		(hasOps) ? s = "YES" : s = "NO";
		display("constructInteger(" + lhs + ", " + rhs + ", " + s + ")");
	}

	if(hasOps)
	{
		bool flag;					// Add some ws around operations
		std::string n = "";				// This is done to ensure readability after 
		for(int i = 0; i < rhs.length(); i++)		// input is passed through post fix
		{
			flag = false;
			for(int j = 0; j < 7 && !flag; j++)
			{
				if(mops[j] == rhs[i])		// If find op
				{
					flag = true;		// Say so
					n += " ";		// Add some ws
					n += rhs[i];
					n += " ";
				}
			}
			if(!flag)
				n += rhs[i];			// If not an op, ad in the char
		}
		rhs = postfix(n);				// Convert to post fix
	}
	setVariable('i', lhs, rhs);				// Send to be created
}

/*
	Given some input, create a float
*/
void NUBasic::constructFloat(std::string lhs, std::string rhs, bool hasOps)
{
	if(nDebug)
	{
		std::string s;
		(hasOps) ? s = "YES" : s = "NO";
		display("constructFloat(" + lhs + ", " + rhs + ", " + s + ")");
	}

	if(hasOps)
	{
		bool flag;					// Add some ws around operations
		std::string n = "";				// This is done to ensure readability after 
		for(int i = 0; i < rhs.length(); i++)		// input is passed through post fix
		{
			flag = false;
			for(int j = 0; j < 7 && !flag; j++)
			{
				if(mops[j] == rhs[i])		// If find op
				{
					flag = true;		// Say so
					n += " ";		// Add some ws
					n += rhs[i];
					n += " ";
				}
			}
			if(!flag)
				n += rhs[i];			// If not an op, ad in the char
		}
		rhs = postfix(n);				// Convert to post fix
	}
	setVariable('f', lhs, rhs);		
}

/*
	Takes in a statement and divides it into parts based on
	if any mathmatical operations are taking place  
*/
struct dataItems NUBasic::explodeStatement(std::string statement)
{
	if(nDebug)
		display("explodeStatement(" + statement + ")");

	struct dataItems ret;					// The return item	
	std::vector<char> ops;					// Op list
	std::vector<std::string> items;				// Tokens
	std::string token = "";

	bool isString = false;					// Stest if its a string
	for(int i = 0; (i < statement.length() && !isString); i++)
		if(statement[i] == '"')
			isString = false;

        bool inString = false;
	
        for(int i = 0; i < statement.length(); i++)
        {
        	if(statement[i] == '"')  	             	// Toggle if we are scanning within "s
		{
                	(!inString) ? inString = true : inString = false;
			token += statement[i];
			if(!inString && token != "")		// Add the string item to the items list	
			{
				items.push_back(token);
				token = "";
			}
    		}
	      	else
		{	
                    	if(!inString)           		// If we aren't in "s, scan for ops
                     	{
				bool isOp = false;
				for(int j = 0; j  < 6 ; j++)
				{
                                        if(statement[i] == mops[j])
                                        {
						ops.push_back(mops[j]);	
						isOp = true;			// Signal its an op
						j = 6;					// Quit searching for matching op

						items.push_back( 
							stripChar(' ', token) 
								);		// Since we found an op, we can add the token
						token = "";
					}
				}

				if(!isOp)
				{
					token += statement[i];
				}
			}
			else
			{
				token += statement[i];
			}
		}
    	}
	items.push_back( stripChar(' ', token) );		// Add the last item
	ret.items = items;					// Copy data, and return
	ret.ops = ops;
	return ret;
}

/*
	Creating a variable at some location in the stack.
	Takes in information regarding a variable and assigns it
	a memory index, and keeps track of it. 
*/
void NUBasic::setVariable(char t, std::string name, std::string value)
{
	if(nDebug)
		display("setVariable(" + name + ", " + value + ") - line 459");

	char currDataType = 'u';
	int lineCount = 0;
	struct dataItems di = explodeStatement(value);

	int lhseavi = 0;				// See if updating lhs var or making a new
	bool lhseav = false;
	for(int z = 0; z < variables.size() && !lhseav; z++)
		if(name == variables[z].name)
		{
			lhseavi = z;
			lhseav = true;
		}

	int tempCmi = 0;
	if(lhseav)
	{
		tempCmi = cmi;
		cmi = variables[lhseavi].mi;		
	}

	if(di.ops.size() == 0)
	{
		// Single item
		struct nubVariable s;					// Assign variable information
		s.name = name;						// Name	
		s.dtype = t;						// Variable type
		s.mi = cmi;						// Memory index (where it lives)
		
		int locationInVarList;
		bool existsAsVariable = false;			// See if current var has already been used
		for(int i = 0; i < variables.size(); i++)
		{
			if(variables[i].name == stripChar(' ', value))
			{
				locationInVarList = i;		// If it has, get its info
				existsAsVariable = true;
				i = variables.size();
			}
		}

		if(existsAsVariable)				// If the variable exists, then update it
		{
			outputFile.push_back("\n");
			// Need to see if lhs is a var now
			struct nubVariable n;
			n = variables[locationInVarList];
			n.name = name;	
			n.mi = cmi;
			variables.push_back(n);

			if( t != 'f')
			{
				// Need to load variable mem location to reg 10
				// At the end of this fucntion r10 is stored in the new mem location
				outputFile.push_back("\tldw r10, $" + ITOS(variables[locationInVarList].mi) + "(sp)\t\t# Load existing variable\n");	
				outputFile.push_back("\tstw r10, $" + ITOS(cmi) + "(sp)\n"); // Write to file
			}
			else
			{
				outputFile.push_back("\tldf f10, $" + ITOS(variables[locationInVarList].mi) + "(sp)\t\t# Load existing variable\n");	
				outputFile.push_back("\tstf f10, $" + ITOS(cmi) + "(sp)\n"); // Write to file
			}
			lineCount += 3;
		}

                if(!existsAsVariable && t == 's')                                            
                {
			outputFile.push_back("\n");
			std::string regLoad = "\tlda r10, $";		// Assembly code!
			value = stripChar('"', value);			
			s.sval = value;
			s.fval = 0.0;
			s.ival = 0;					// With the exception of saving some extra output information
			std::string sTrans = "text" + ITOS(stringIndex) + ": .string \"" + value  + "\"\n" ;
			regLoad += ("text" + ITOS(stringIndex) + "\n");	// Add to the lda command
                       	stringList.push_back(sTrans);			// Add the string information to a list for later output
                       	s.translation = sTrans;				// Store the string translation
                       	stringIndex++;				
			outputFile.push_back(regLoad); // Write to file
			outputFile.push_back("\tstw r10, $" + ITOS(cmi) + "(sp)\n"); // Write to file
			variables.push_back(s);		
			lineCount += 3;
		}

                if(!existsAsVariable && t == 'f')                                            
		{	
			std::string regLoad = "\tldf f10, ";		// Assembly code!
			value = stripChar('"', value);			
                       	float f;
                       	std::stringstream ss(value);			// Convert to int
                       	ss >> f;
			s.sval = "";
			s.fval = f;
			s.ival = 0;					// With the exception of saving some extra output information
			std::string fTrans = "cfloat" + ITOS(floatIndex) + ": .float " + value  + "\n" ;
			regLoad += ("cfloat" + ITOS(floatIndex) + "\n");// Add to the lda command
                        floatList.push_back(fTrans);			// Add the string information to a list for later output
                        s.ftranslation = fTrans;				// Store the string translation
                        floatIndex++;				
			outputFile.push_back(regLoad); // Write to file
			outputFile.push_back("\tstf f10, $" + ITOS(cmi) + "(sp)\n"); // Write to file
			variables.push_back(s);		
			lineCount += 2;
		}

                if(!existsAsVariable && t == 'i')                                            
		{
			std::string regLoad = "\tlda r10, $";		// Assembly code!
                       	int i;
                       	std::stringstream ss(value);			// Convert to int
                       	ss >> i;
			s.ival = i;					// Set variable data
			s.sval = "";					
			s.fval = 0.0;
			regLoad += stripChar(' ', value);		// Add its information to the register load command
			outputFile.push_back(regLoad + "\n"); // Write to file
			outputFile.push_back("\tstw r10, $" + ITOS(cmi) + "(sp)\n"); // Write to file
			variables.push_back(s);		
			lineCount += 2;
		}
	}
	else
	{
		// Multiple items - Ignore strings here, they will be all set by now. . . trust me, I'm a Doctor.

		struct nubVariable s;					// Assign variable information
		s.name = name;						// Name	
		s.dtype = t;						// Variable type
		s.mi = cmi;						// Memory index (where it lives)
		s.ival = 0;						// Init var data
		s.sval = "";					
		s.fval = 0.0;

		lineCount += 1;
		outputFile.push_back(("\t\t\t\t# Calculate: " + value + "\n") );	// Tell the user in the .s file whats about to happen

		std::string loadCommand;					// figure out the iinital load type
		(t == 'f') ? loadCommand = "# ldf " : loadCommand = "lda ";
			
		bool done = false;   			// Need to convert the postfix notation into its assembly form
		std::string tmp = "";
		std::queue<std::string> values;		// Queue to hold each value that has to be analyzed

		// Go through the postfixed value, and figure out if each item is a var, or a raw value
		tmp = "";
		done = false;	
		for(int i = 0; i < value.length(); i++)
		{		
			bool isOp = false;
			for(int j = 0; j < 7 && !isOp; j++)
				if(mops[j] == value[i])
					isOp = true;
			if(!isOp)				// Handle if it is an op or not
			{
				if(value[i] == ' ' && tmp != "")
				{
					values.push(tmp);	// Add
					tmp = "";
				}
				else
					tmp += value[i];		// Add value
			}

			if (isOp)
			{
				values.push(tmp);	// Add						
				tmp = value[i];
				values.push(tmp);							
				tmp = "";
			}
		}

		// Go through each value, and figure out if it is a variable, or not
		// If its a variable, load it from the stack into the operation, 
		// If its a raw value, lda it into the operation
		std::string currentItem;	// Strings to hold the assembly line equivelant of operation
		std::string regLoadLine;	// Register load lines (lda, or whatever)
		std::stack<int> regLocs;	// Register locations
		int currReg = 10;		// Current register to use
		
		while(!values.empty())					// Step through queue
		{
			int ival;
			bool viop = false;
			currentItem = stripChar(' ', values.front());	// Clean up the item
			currentItem = stripChar('\t', currentItem);
			currentItem = stripChar('\n', currentItem);
			if(currentItem == "")				// Ignore blank items
			{						
				values.pop();
				continue;	
			}
		
			for(int i =0; i < 7 && !viop; i++)		// See if current item is an operation
				if(mops[i] == currentItem[0])
					viop = true;

			if(viop)					// Figure out what operation it is
			{
				std::string regLoad = "\n\t";		// Create the aseembly line associated

				switch(currentItem[0])
				{
				case '^':
				{
					if(t == 'f')
					{
						display("\n\tError: ^ does not currently support floats.\n");
						exit(1);
					}

					lineCount += 6;
					loadSupportingFiles('p');		// Load code for power 

					int rhs = regLocs.top();                // Get lhs (middle) and rhs register for command
                                	regLocs.pop();
                                	currReg--;

                                	int lhs = regLocs.top();
                                	regLocs.pop();
                                	currReg--;

					regLoad += ("mov r3, r" + ITOS(lhs));
					regLoad += ("\n\tmov r4, r" + ITOS(rhs));
					regLoad += ("\n\tcall pwri\t\t# Call the power funtion");
					regLoad += ("\n\tmov r" + ITOS(currReg) + ", r5\n");

                                	regLocs.push(currReg);                  // Add the newest result
                                	currReg++;

                                	lineCount++;
                                	outputFile.push_back(regLoad);          // Add the regLoad line to output
                                	values.pop();                           // Pop current item from queue

					continue;
					break;
				}
				case '*':
				{
					if(t != 'f') 
						regLoad += "mul ";
					else
						regLoad += "mulf ";
				}
					break;
				case '+':
				{
					if(t != 'f') 
						regLoad += "add ";
					else
						regLoad += "addf ";
				}
					break;
				case '-':
				{
					if(t != 'f') 
						regLoad += "sub ";
					else
						regLoad += "subf ";
				}
					break;
				case '/':
				{
					if(t != 'f') 
						regLoad += "div ";
					else
						regLoad += "divf ";
				}
					break;
				default:
					break;
				}
		
				int rhs = regLocs.top();		// Get lhs (middle) and rhs register for command
				regLocs.pop();
				currReg--;

				int lhs = regLocs.top();
				regLocs.pop();
				currReg--;

				if(t != 'f')
					regLoad += ( "r" +  ITOS(currReg) + ", r" + ITOS(lhs) + ", r" + ITOS(rhs));
				else
					regLoad += ( "f" +  ITOS(currReg) + ", f" + ITOS(lhs) + ", f" + ITOS(rhs));
					
				regLocs.push(currReg); 			// Add the newest result
				currReg++;				
			
				lineCount += 2;;
				outputFile.push_back(regLoad);		// Add the regLoad line to output
				values.pop();				// Pop current item from queue
				continue;				// Move on to the next item in the queue
			}

			outputFile.push_back("\n");
			/*
				IF THE CURRENT ITEM IN THE QUEUE IS NOT AN OPERATOR
			*/			
			std::stringstream ss(currentItem);		// Stream the current item		
			ss >> ival;					// Try making it an int
			if(ss.fail())					// If it can't me made into an int, its a variable
			{
				bool found = false;
				for(int i = 0; i < variables.size() && !found; i++)
					if(variables[i].name == currentItem)	// If its a variable, we need to load it from its place in memory
					{
						found = true;
						if(variables[i].dtype != 'f')
						{
							regLoadLine = ("\tldw r" + ITOS(currReg) + 
								       ", $" + 
								       ITOS(variables[i].mi) + 
								       "(sp)\n");		// Add to assembly line
						}
						else
						{
							regLoadLine = ("\tldf f" + ITOS(currReg) + 
								       ", $" + 
								       ITOS(variables[i].mi) + 
								       "(sp)\n");		// Add float line to assembly line
						}
						lineCount += 2;
					}
				if(!found)					// If it is a var, but not found then we want to start the program on fire
				{
					display("Variable: " + currentItem + " does not exist.");
					exit(1);
				}
						
				outputFile.push_back(regLoadLine);		// Add assembly line to file
				regLoadLine = "";				// Reset assembly line
				regLocs.push(currReg);				// Push the current load location
				currReg++;						// Add to counter
			}
			else							// If it isn't a variable
			{							// Add the assembly operation to load item into register
				if(t != 'f')					// Standard Integer
				{
					regLoadLine = ("\n\tlda r" + ITOS(currReg) + 
						       ", $" + currentItem + "\n");
					outputFile.push_back(regLoadLine);		// Add line to output file
				}
				else
				{						// Float item	
					bool found = false;
					for(int x = 0; x < currentItem.length() && !found; x++)
						if(currentItem[x] == '.')
							found = true;
					if(!found)
						currentItem += ".0";

					std::string fTrans = "cfloat" + ITOS(floatIndex) + ": .float " + currentItem  + "\n";
					regLoadLine = ("\n\tldf f" + ITOS(currReg) +
                                                       ", " + ("cfloat" + ITOS(floatIndex)) + "\n");
                        		floatList.push_back(fTrans);                    // Add the string information to a list for later output
 		                        floatIndex++;
				}
				outputFile.push_back(regLoadLine);
				regLoadLine = "";				// Reset the line
				regLocs.push(currReg);				// Add current load location
				currReg++;					// Update counter
				lineCount +=2;
			}
			values.pop();						// Remove the item from the queue, we're done

		} // End of queue !empty loop

		int nw = regLocs.top();						// New word location. At this point, with a corretly formatted expr, rLocations will have one entry remaining
		regLocs.pop();							// Remove it
		currReg--;							// Update the counter

		outputFile.push_back("\n");

		if(t != 'f')
			outputFile.push_back("\tstw r" + ITOS(nw) + ", $" + ITOS(cmi) + "(sp)\t\t# Copy result into mem index \n");
		else
			outputFile.push_back("\tstf f" + ITOS(nw) + ", $" + ITOS(cmi) + "(sp)\t\t# Copy result into mem index \n");

		int lhseavi = 0;						// See if updating lhs var or making a new
		bool lhseav = false;
		for(int z = 0; z < variables.size() && !lhseav; z++)		// If updating a var, delete the old one
			if(name == variables[z].name)				// We are about to write it back with its 
			{							// updated information, so we don't need it anymore
				lhseavi = z;
				lhseav = true;
			}
		if(lhseav)
			variables.erase(variables.begin() + lhseavi);

		variables.push_back(s);         				// Add var to list
		lineCount += 2;
	}

	if(!lhseav)
		cmi += 4;							// Add 4 to the current memory index	
	else
	{
		cmi = tempCmi;
	}
	outputFileLT[currentInputLine] += lineCount;
}

