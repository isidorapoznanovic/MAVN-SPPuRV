#include <iostream>
#include <exception>

#include "SyntaxAnalysis.h"
using namespace std;


int main(int argc, char** argv)
{
	string fInput, fOutput;
	if (argc == 3) 
	{
		fInput = argv[1];
		fOutput = argv[2];
	}
	else
	{
		std::cout << "You must pass input and output file to command line" << endl;
		return 7;
	}
	try
	{
		//std::string fileName = ".\\..\\examples\\" + fInput + ".mavn";
		std::string fileName = fInput + ".mavn";
		bool retVal = false;

		LexicalAnalysis lex;

		if (!lex.readInputFile(fileName))
			throw runtime_error("\nException! Failed to open input file!\n");

		lex.initialize();

		retVal = lex.Do();

		if (retVal)
		{
			std::cout << "Lexical analysis finished successfully!" << endl;
			lex.printTokens();
		}
		else
		{
			lex.printLexError();
			throw runtime_error("\nException! Lexical analysis failed!\n");
		}

		//sintaksna analiza
		std::cout << "---------------------------------------------" << endl;
		std::cout << "             Syntax analysis" << endl;
		Instructions instructions;
		SyntaxAnalysis syn(lex);

		retVal = syn.Do();

		if (retVal)
		{
			std::cout << "Syntax analysis finished successfully!" << endl;
		}
		else
		{
			throw runtime_error("\nException! Syntax analysis failed!\n");
		}

		std::cout << "----------------------------------------------" << endl << endl;
		std::cout << "                Important lists" << endl << endl;

		createVariablesLablesandFunctions(lex);

		printVariablesLablesandFunctions();

		createInstructionList(lex);

		std::cout << "----------------------------------------------" << endl << endl;
		std::cout << "     Instructions before liveness analysis" << endl << endl;
		printInstructions();

		livenessAnalysis();
		std::cout << "----------------------------------------------" << endl << endl;
		std::cout << "     Instructions after liveness analysis" << endl << endl;
		printInstructions();
		
		doInterferenceGraph();
		printInterferenceGraph();

		if (doSimplification(__REG_NUMBER__)) 
		{
			std::cout << "Simplification finished successfully!" << endl;
		}
		else
		{
			throw runtime_error("\nException! This compiler do not support spilling!\n");
		}
		doResourceAllocation();
		//printRegisterVariables();

		std::cout << "----------------------------------------------" << endl << endl;
		std::cout << "               " << fOutput << ".s" << endl << endl;
		createOutputString();
		printOutputString(fOutput);
		stlFree();

		std::cout << endl << "TESTIRANO SVE DO SAD" << endl;


	}
	catch (runtime_error e)
	{
		std::cout << e.what() << endl;
		return 1;
	}

	return 0;
}
