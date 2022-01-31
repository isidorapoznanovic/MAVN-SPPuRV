#include <iostream>
#include <iomanip>

#include "SyntaxAnalysis.h"

using namespace std;


SyntaxAnalysis::SyntaxAnalysis(LexicalAnalysis& lex)
	: lexicalAnalysis(lex), errorFound(false), tokenIterator(lexicalAnalysis.getTokenList().begin())
{
}


bool SyntaxAnalysis::Do()
{
	currentToken = getNextToken();
	
	//TO DO: Call function for the starting non-terminal symbol
	Q();

	return !errorFound;
}


void SyntaxAnalysis::printSyntaxError(Token token)
{
	cout << "Syntax error! Token: " << token.getValue() << " unexpected" << endl;
}

/**
void SyntaxAnalysis::printTokenInfo(Token token)
{
	//cout << setw(20) << left << token.tokenTypeToString();
	cout << setw(25) << right << token.getValue() << endl;
}
*/

void SyntaxAnalysis::eat(TokenType t)
{
	if (errorFound == false)
	{
		if (currentToken.getType() == t)
		{
			cout << currentToken.getValue() << endl;
			if (t != T_END_OF_FILE)
				currentToken = getNextToken();
		}
		else
		{
			printSyntaxError(currentToken);
			errorFound = true;
		}
	}
}


Token SyntaxAnalysis::getNextToken()
{
	if (tokenIterator == lexicalAnalysis.getTokenList().end())
		throw runtime_error("End of input file reached");
	return *tokenIterator++;
}


void SyntaxAnalysis::Q()
{
	if (errorFound == false)
	{
		S();
		eat(T_SEMI_COL);
		L();
	}
}


void SyntaxAnalysis::S()
{	
	if (errorFound == false)
	{
		if (currentToken.getType() == T_MEM)
		{
			//cout << currentToken.getValue() << endl;
			
			eat(T_MEM);
			eat(T_M_ID);
			eat(T_NUM);
		}
		else
		{
			if (currentToken.getType() == T_REG)
			{
				//cout << currentToken.getValue() << endl;
				eat(T_REG);
				eat(T_R_ID);
			}
			else 
			{
				if (currentToken.getType() == T_FUNC) 
				{
					eat(T_FUNC);
					eat(T_ID);
				}
				else
				{
					if (currentToken.getType() == T_ID) 
					{
						eat(T_ID);
						eat(T_COL);
						E();
					}
					else 
					{
						E();
						//printSyntaxError(currentToken);
						//errorFound = true;
					}
				}
			}
		}
	}
}


void SyntaxAnalysis::L()
{
	if (errorFound == false)
	{
		if (currentToken.getType() == T_END_OF_FILE)
		{
			//cout << currentToken.getValue() << endl;
			eat(T_END_OF_FILE);
		}
		else
		{
			Q();
		}
	}

	
}


void SyntaxAnalysis::E()
{
	if (errorFound == false)
	{
		switch (currentToken.getType())
		{
		case T_ADD:
			eat(T_ADD);
			if (currentToken.getType() == T_R_ID) 
			{
				eat(T_R_ID);
				eat(T_COMMA);
				eat(T_R_ID);
				eat(T_COMMA);
				eat(T_R_ID);
			}
			else 
			{
				printSyntaxError(currentToken);
				errorFound = true;
			}
			break;
		case T_ADDI:
			eat(T_ADDI);
			if (currentToken.getType() == T_R_ID) 
			{
				eat(T_R_ID);
				eat(T_COMMA);
				eat(T_R_ID);
				eat(T_COMMA);
				eat(T_NUM);
			}
			else 
			{
				printSyntaxError(currentToken);
				errorFound = true;
			}
			break;
		case T_LA:
			eat(T_LA);
			if (currentToken.getType() == T_R_ID) 
			{
				eat(T_R_ID);
				eat(T_COMMA);
				eat(T_M_ID);
			}
			else 
			{
				printSyntaxError(currentToken);
				errorFound = true;
			}
			break;
		case T_LW:
			eat(T_LW);
			if (currentToken.getType() == T_R_ID) 
			{
				eat(T_R_ID);
				eat(T_COMMA);
				eat(T_NUM);
				eat(T_L_PARENT);
				eat(T_R_ID);
				eat(T_R_PARENT);
			}
			else 
			{
				printSyntaxError(currentToken);
				errorFound = true;
			}
			break;
		case T_SW:
			eat(T_SW);
			if (currentToken.getType() == T_R_ID) 
			{
				eat(T_R_ID);
				eat(T_COMMA);
				eat(T_NUM);
				eat(T_L_PARENT);
				eat(T_R_ID);
				eat(T_R_PARENT);
			}
			else 
			{
				printSyntaxError(currentToken);
				errorFound = true;
			}
			break;
		case T_B:
			eat(T_B);
			if (currentToken.getType() == T_ID) 
			{
				eat(T_ID);
			}
			else 
			{
				printSyntaxError(currentToken);
				errorFound = true;
			}
			break;
		case T_BLTZ:
			eat(T_BLTZ);
			if (currentToken.getType() == T_R_ID) 
			{
				eat(T_R_ID);
				eat(T_COMMA);
				eat(T_ID);
			}
			else 
			{
				printSyntaxError(currentToken);
				errorFound = true;
			}
			break;
		case T_NOT:
			eat(T_NOT);
			if (currentToken.getType() == T_R_ID) 
			{
				eat(T_R_ID);
				eat(T_COMMA);
				eat(T_R_ID);
			}
			else 
			{
				printSyntaxError(currentToken);
				errorFound = true;
			}
			break;
		case T_JR:
			eat(T_JR);
			if (currentToken.getType() == T_R_ID) 
			{
				eat(T_R_ID);
			}
			else 
			{
				printSyntaxError(currentToken);
				errorFound = true;
			}
			break;
		case T_SRA:
			eat(T_SRA);
			if (currentToken.getType() == T_R_ID) 
			{
				eat(T_R_ID);
				eat(T_COMMA);
				eat(T_R_ID);
				eat(T_COMMA);
				eat(T_NUM);
			}
			else 
			{
				printSyntaxError(currentToken);
				errorFound = true;
			}
			break;
		default:
			printSyntaxError(currentToken);
			errorFound = true;
		}
	}
}
