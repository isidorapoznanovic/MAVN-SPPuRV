#include <ostream>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <numeric>
#include <stack>
#include <string>

#include "IR.h"
using namespace std;
Variables variables;
Instructions instructions;
LabelsFunctions labfunc;
int p = 1;
vector<Variable*> registerVariables;

vector<vector<int>> interferenceGraph;

stack<Variable*> simplificationStack;

map<int, string> g_output_globl;
map<int, string> g_output_data;
map<int, string> g_output_text;

string g_output_str;

void Instruction::mySort()
{
	m_in.sort(compare);
	m_out.sort(compare);
}

ReturnValueVariable Instruction::getItUse()
{
	ReturnValueVariable p;
	p.itb = m_use.begin();
	p.ite = m_use.end();
	p.size = m_use.size();
	return p;
}
ReturnValueVariable Instruction::getItDef()
{
	ReturnValueVariable p;
	p.itb = m_def.begin();
	p.ite = m_def.end();
	p.size = m_def.size();
	return p;
}
ReturnValueVariable Instruction::getItIn()
{
	ReturnValueVariable p;
	p.itb = m_in.begin();
	p.ite = m_in.end();
	p.size = m_in.size();
	return p;
}
ReturnValueVariable Instruction::getItOut()
{
	ReturnValueVariable p;
	p.itb = m_out.begin();
	p.ite = m_out.end();
	p.size = m_out.size();
	return p;
}
ReturnValueIns Instruction::getItSucc()
{
	ReturnValueIns p;
	p.itb = m_succ.begin();
	p.ite = m_succ.end();
	p.size = m_succ.size();
	return p;
}

bool compare(Variable* a, Variable* b)
{
	if (a->getName().compare(b->getName()) < 0)
	{
		return true;
	}
	return false;
}

void createRegisterVariables() 
{
	variables.sort(compare);
	for (auto it = variables.begin(); it != variables.end(); it++) 
	{
		if ((*it)->geType() == REG_VAR) 
		{
			registerVariables.push_back(*it);
		}
	}
}

void createVariablesLablesandFunctions(LexicalAnalysis& lex) 
{
	TokenList list = lex.getTokenList();
	Token currenToken;
	for (auto it = list.begin(); it != list.end(); it++)
	{
		switch ((*it).getType())
		{
		case T_MEM:
		{
			it++;
			string variableName = (*it).getValue();

			it++;
			int variableValue = stoi((*it).getValue());
			if (variableName[0] != 'm')
			{
				throw runtime_error("Memory variable " + variableName + " was not declared properly!");
			}
			else
			{
				bool exist = false;
				for (auto itt = variables.begin(); itt != variables.end(); itt++)
				{

					//cout << "printisha" << endl;
					if ((*itt)->getName().compare(variableName) == 0)
					{
						exist = true;

					}
				}
				if (!exist)
				{
					Variable* temp = new Variable(MEM_VAR, variableName, p, no_assign, variableValue);
					variables.push_back(temp);
				}
				else
				{
					throw runtime_error("Memory variable " + variableName + " already exist!");
				}
				
			}
			break;
		}
		case T_REG:
		{
			it++;
			string variableName = (*it).getValue();

			if (variableName[0] != 'r')
			{
				throw runtime_error("Register variable " + variableName + " was not declared properly!");
			}
			else
			{
				bool exist = false;
				for (auto itt = variables.begin(); itt != variables.end(); itt++)
				{
					if ((*itt)->getName().compare(variableName) == 0)
					{
						exist = true;
					}
				}
				if (!exist)
				{
					Variable* temp = new Variable(REG_VAR, variableName, p, no_assign, 0);
					variables.push_back(temp);			//EMPLACE BACK treba
				}
				else
				{
					throw runtime_error("Register variable " + variableName + " already exist!");
				}
				
			}
			break;
		}
		case T_FUNC:
		{
			it++;
			string functionName = (*it).getValue();

			if (functionName[0] < 'a' && functionName[0] > 'z')
			{
				throw runtime_error("Function " + functionName + " was not declared properly!");
			}
			else
			{
				bool exist = false;
				for (auto itt = variables.begin(); itt != variables.end(); itt++)
				{
					if ((*itt)->getName().compare(functionName) == 0)
					{
						exist = true;
					}
				}
				if (!exist)
				{
					LabelFunction* temp = new LabelFunction(p, FUNCTION, functionName);
					labfunc.push_back(temp);			//EMPLACE BACK treba
				}
				else
				{
					throw runtime_error("Function " + functionName + " already exist!");
				}
				
			}
			break;
		}
		case T_ID:
		{
			string labelName = (*it).getValue();
			it++;
			if ((*it).getType() != T_COL)
			{
				p++;
				continue;
			}
			else
			{
				bool exist = false;
				for (auto itt = variables.begin(); itt != variables.end(); itt++)
				{
					if ((*itt)->getName().compare(labelName) == 0)
					{
						exist = true;
					}
				}
				if (!exist)
				{
					LabelFunction* temp = new LabelFunction(p, FUNCTION, labelName);
					labfunc.push_back(temp);			//EMPLACE BACK treba
					p++;
				}
				else
				{
					throw runtime_error("Label " + labelName + " already exist!");
				}
				
			}
			break;
		}
		case T_SEMI_COL:
		{
			p++;
			break;
		}
		case T_COL:
		{
			p++;
			break;
		}
		//default:
			//cout << "printisha" << endl;
			//(*it).printTokenInfo();
		}
		//(*it).printTokenInfo();
	}
	createRegisterVariables();
}


void printRegisterVariables()
{
	cout << "------REGISTER VARIABLES------" << endl;
	for (auto it = registerVariables.begin(); it != registerVariables.end(); it++)
	{
		cout << *(*it) << endl;
	}
}

void printVariablesLablesandFunctions()
{
	/*
	cout << "------REGISTER VARIABLES------" << endl;
	for (auto it = registerVariables.begin(); it != registerVariables.end(); it++)
	{
		cout << *(*it) << endl;
	}
	*/
	cout << "----------------------------------------------" << endl;
	cout << "---------------- ALL VARIABLES----------------" << endl;
	auto itv = variables.begin();
	for (itv; itv != variables.end(); itv++)
	{
		cout << *(*itv) << endl;
	}
	cout << "----------------------------------------------" << endl;
	cout << "-------------LABELS AND FUNCTIONS-------------" << endl;
	for (auto it = labfunc.begin(); it != labfunc.end(); it++)
	{
		cout << *(*it) << endl;
	}
}

Variable* findVariable(Token t)
{
	Variable* temp = nullptr;
	for (auto it = variables.begin(); it != variables.end(); it++) 
	{
		if (t.getValue().compare((*it)->getName()) == 0)
		{
			temp = *it;
		}
	}
	if (temp == nullptr) 
	{
		throw runtime_error("Variable " + t.getValue() + " does not exist!");
	}
	return temp;
}

LabelFunction* findLabFunc(Token t)
{
	LabelFunction* temp = nullptr;
	for (auto it = labfunc.begin(); it != labfunc.end(); it++)
	{
		if (t.getValue().compare((*it)->getName()) == 0)
		{
			temp = *it;
		}
	}
	if (temp == nullptr)
	{
		throw runtime_error("Label/Function " + t.getValue() + " does not exist!");
	}
	return temp;
}

void createInstructionList(LexicalAnalysis& lex)
{
	TokenList list = lex.getTokenList();
	Token currenToken;
	p = 1;

	for (auto it = list.begin(); it != list.end(); it++)
	{
		switch ((*it).getType())
		{
		case T_ADD:
		{
			Instruction* i = new Instruction(p, I_ADD);
			it++;
			i->pushDst(findVariable(*it));

			advance(it, 2);
			i->pushSrc(findVariable(*it));

			advance(it, 2);
			i->pushSrc(findVariable(*it));

			i->setInstructioName("add    ~d, ~s, ~s\n");
			instructions.push_back(i);
			break;
		}
		case T_ADDI:
		{
			Instruction* i = new Instruction(p, I_ADDI);
			it++;
			i->pushDst(findVariable(*it));

			advance(it, 2);
			i->pushSrc(findVariable(*it));

			advance(it, 2);
			string num = (*it).getValue();

			i->setInstructioName("addi   ~d, ~s, " + num + "\n");
			instructions.push_back(i);
			break;
		}
		case T_LA:
		{
			Instruction* i = new Instruction(p, I_LA);
			it++;
			i->pushDst(findVariable(*it));
			advance(it, 2);
			i->pushSrc(findVariable(*it));

			i->setInstructioName("la     ~d, ~s\n");
			instructions.push_back(i);
			break;
		}
		case T_LW:
		{
			Instruction* i = new Instruction(p, I_LW);
			it++;
			i->pushDst(findVariable(*it));

			advance(it, 2);
			string num = (*it).getValue();

			advance(it, 2);
			i->pushSrc(findVariable(*it));

			i->setInstructioName("lw     ~d, " + num + "(~s)\n");
			instructions.push_back(i);
			break;
		}
		case T_SW:
		{
			Instruction* i = new Instruction(p, I_SW);
			it++;
			i->pushSrc(findVariable(*it));

			advance(it, 2);
			string num = (*it).getValue();

			advance(it, 2);
			i->pushSrc(findVariable(*it));

			i->setInstructioName("sw     ~s, " + num + "(~s)\n");
			instructions.push_back(i);
			break;
		}
		case T_B:
		{
			Instruction* i = new Instruction(p, I_B);
			it++;
			i->pushLabFunc(findLabFunc(*it));

			i->setInstructioName("b      ~d\n");
			instructions.push_back(i);
			break;
		}
		case T_BLTZ:
		{
			Instruction* i = new Instruction(p, I_BLTZ);
			it++;
			i->pushSrc(findVariable(*it));

			advance(it, 2);
			i->pushLabFunc(findLabFunc(*it));

			i->setInstructioName("bltz   ~s, ~d\n");
			instructions.push_back(i);
			break;
		}
		case T_NOT:
		{
			Instruction* i = new Instruction(p, I_NOT);
			it++;
			i->pushDst(findVariable(*it));

			advance(it, 2);
			i->pushSrc(findVariable(*it));

			i->setInstructioName("not    ~d, ~s\n");
			instructions.push_back(i);
			break;
		}
		case T_JR:
		{

			Instruction* i = new Instruction(p, I_JR);
			it++;
			i->pushLabFunc(findLabFunc(*it));

			i->setInstructioName("jr     ~d\n");
			instructions.push_back(i);
			break;
		}
		case T_SRA:
		{
			Instruction* i = new Instruction(p, I_SRA);
			it++;
			i->pushDst(findVariable(*it));

			advance(it, 2);
			i->pushSrc(findVariable(*it));

			advance(it, 2);
			string num = (*it).getValue();

			i->setInstructioName("sra    ~d, ~s, " + num + "\n");
			instructions.push_back(i);
			break;
		}
		case T_SEMI_COL:
			p++;
			break;
		case T_COL:
			p++;
			break;
		//default:
			
		}
	}
	fillSuccAndPred();
	fillUseAndDef();
}
Instructions::iterator findInstruction(LabelFunction* t)
{
	int pos = t->getPosition();
	pos++;
	for (auto it = instructions.begin(); it != instructions.end(); it++)
	{
		if ((*it)->getPosition() == pos)
			return it;
	}
}
void fillSuccAndPred()
{
	auto it = instructions.begin();
	auto it_next = instructions.begin();

	for (it; it != instructions.end(); it++)
	{
		if ((*it)->geType() == I_BLTZ || (*it)->geType() == I_B)
		{
			it_next = findInstruction((*it)->getLabFunc());

			(*it)->pushSucc(*it_next);
			(*it_next)->pushPred(*it);

			it_next = it;
			it_next++;
			if (it_next != instructions.end())
			{
				(*it)->pushSucc(*it_next);
				(*it_next)->pushPred(*it);
			}
		}
		else
		{
			it_next = it;
			it_next++;
			if (it_next != instructions.end())
			{
				(*it)->pushSucc(*it_next);
				(*it_next)->pushPred(*it);
			}
		}
	}
}
void fillUseAndDef()
{
	auto it = instructions.begin();
	for (it; it != instructions.end(); it++)
	{
		Variables temp = (*it)->getSrc();
		auto it_t = temp.begin();
		for (it_t; it_t != temp.end(); it_t++)
			if ((*it_t)->geType() == REG_VAR)
				(*it)->pushUse(*it_t);

		Variables temp2 = (*it)->getDst();
		auto it_t2 = temp2.begin();
		for (it_t2; it_t2 != temp2.end(); it_t2++)
			if ((*it_t2)->geType() == REG_VAR)
				(*it)->pushDef(*it_t2);
	}
}


void printInstructions()
{
	for (auto it = instructions.begin(); it != instructions.end(); it++)
		cout << *(*it) << endl;
}

void livenessAnalysis() 
{
	
	for (int i = 0; i < instructions.size(); i++)
	{
		bool flag = true;
		for (auto it = instructions.rbegin(); it != instructions.rend(); it++)
		{
			Variables in_prv = (*it)->getIn();
			Variables out_prv = (*it)->getOut();
			
			(*it)->inClear();
			(*it)->outClear();
			ReturnValueVariable in = (*it)->getItIn();
			ReturnValueVariable out = (*it)->getItOut();
			ReturnValueIns succ = (*it)->getItSucc();
			for (auto itt = succ.itb; itt != succ.ite; itt++)
			{
				ReturnValueVariable inSucc = (*itt)->getItIn();
				
				for (auto it2 = inSucc.itb; it2 != inSucc.ite; it2++)
				{
					(*it)->pushOut(*it2);
				}
			}

			(*it)->mySort();
			(*it)->myUnique();
			
			in = (*it)->getItIn();
			out = (*it)->getItOut();
			ReturnValueVariable use = (*it)->getItUse();
			for (auto itt = use.itb; itt != use.ite; itt++)			//(*it)->in = (*it)->use;
			{
				(*it)->pushIn(*itt);
			}
				

			ReturnValueVariable def = (*it)->getItDef();
			for (auto itt = out.itb; itt != out.ite; itt++)
			{
				auto it2 = find(def.itb, def.ite, *itt);
				if (it2 != def.ite)
				{
					//prepisano iz domaceg, za vreme pisanja ovog komentara sam mogla negirati uslov da bih imala normalan if ali ok je ovako
				}
				else
				{
					(*it)->pushIn(*itt);
				}
			}

			(*it)->mySort();
			(*it)->myUnique();

			flag = (in_prv == (*it)->getIn() && out_prv == (*it)->getOut()) ? flag & true : flag & false;
		}
		if (flag) break;
	}
}

int findPosition(Variable* p)
{
	int i = 0;
	for (auto it = registerVariables.begin(); it != registerVariables.end(); it++)
	{
		if ((*it)->getName().compare(p->getName()) == 0)
		{
			return i;
		}
		i++;
	}
}

void doInterferenceGraph() 
{
	//We do not have move reg, reg instruction in this asm so we will implement just first step of algorithm for forming interference graph from vezba10.pdf 
	for (int i = 0; i < registerVariables.size(); i++)
	{
		registerVariables[i]->setPosIgM(i);
	}
	
	interferenceGraph.resize(registerVariables.size());

	for (auto it = interferenceGraph.begin(); it != interferenceGraph.end(); it++)
	{
		(*it).assign(registerVariables.size(), __EMPTY__);
	}

	for (auto it = instructions.begin(); it != instructions.end(); it++)
	{
		Variables def = (*it)->getDef();
		Variables out = (*it)->getOut();

		for (auto itd = def.begin(); itd != def.end(); itd++)
		{
			for (auto ito = out.begin(); ito != out.end(); ito++)
			{
				if ((*itd)->getName().compare((*ito)->getName()) != 0) //Za svaku instrukciju gledamo sta se definise i sta je na izlazu cvora, u matricu smetnji zapisujemo sve sto je zivo na izlazu a ne definise se tj smetnju izmedju toga i ovoga sto se definise
				{
					interferenceGraph[(*itd)->getPosIgM()][(*ito)->getPosIgM()] = __INTERFERENCE__;
					interferenceGraph[(*ito)->getPosIgM()][(*itd)->getPosIgM()] = __INTERFERENCE__;
				}
			}
		}
	}

}

void printInterferenceGraph()
{
	cout << "----------------------------------------------" << endl << endl;
	cout << "           Print interference graph" << endl << endl;
	for (int i = 0; i < interferenceGraph.size(); i++)
	{
		for (int j = 0; j < interferenceGraph.size(); j++)
		{
			cout << interferenceGraph[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

bool degOne(vector<int> v)
{
	bool one = true;
	for (auto it = v.begin(); it != v.end(); it++)
	{
		if (!(*it == 1 || *it == 0))
		{
			one = false;
		}
	}
	return one;
}

bool doSimplification(int regNum)
{
	vector<int> vertexDeg;
	vector<bool> vertexPushed(registerVariables.size(), false);

	vertexDeg.resize(registerVariables.size());
	int i = 0;
	for (auto it = interferenceGraph.begin(); it != interferenceGraph.end(); it++)
	{
		vertexDeg[i] = accumulate((*it).begin(), (*it).end(), 0);
		i++;
	}

	for (auto it = registerVariables.begin(); it != registerVariables.end(); it++)
		cout << (*it)->getName() << " ";
	cout << endl;
	for (i = 0; i < vertexDeg.size(); i++)
		cout << vertexDeg[i] << "  ";
	cout << endl;
	

	int max = regNum - 1;
	while (!degOne(vertexDeg))
	{
		bool found = false;
		for (i = 0; i < vertexDeg.size(); i++)
		{
			if (vertexDeg[i] == max)
			{
				vertexDeg[i] = 0;
				simplificationStack.push(registerVariables[i]);
				vertexPushed[i] = true;
				cout << registerVariables[i]->getName() << endl;
				//cout << "max je " << max << endl;
				for (int j = 0; j < interferenceGraph.size(); j++)
				{
					if (interferenceGraph[i][j] == __INTERFERENCE__)
					{
						vertexDeg[j]--;
					}
				}

				found = true;
				break;
			}
		}
		if (!found) max--;
		if (max == 0) return false;
	}

	for (i = 0; i < vertexDeg.size(); i++)
	{
		if (vertexPushed[i] == false)
		{
			simplificationStack.push(registerVariables[i]);
			cout << registerVariables[i]->getName() << endl;
		}
	}
	return true;
}

bool doResourceAllocation()
{
	bool b;

	while (!(&simplificationStack)->empty()) {
		Variable* temp = (&simplificationStack)->top();
		int color[__REG_NUMBER__] = { 0 };
		for (auto it = registerVariables.begin(); it != registerVariables.end(); it++) {
			if (interferenceGraph[findPosition(temp)][findPosition(*it)] == __INTERFERENCE__) {
				if ((*it)->getAssignment() != -1) {
					color[(*it)->getAssignment()] = 1;
				}
			}
		}

		bool s = false;
		for (int i = 0; i < __REG_NUMBER__; i++) {
			if (color[i] == 0) {
				s = true;
				temp->setAssignment((Regs)i);
			}
		}
		b = s;
		(&simplificationStack)->pop();
	}

	return b;
}

string makeInstructionString(Instruction* i)
{
	string oldInstruction = i->getinstructioName();
	string newInstruction;
	//"sw    ~s, " + num + "(~s)\n"
	
	for (auto it = oldInstruction.begin(); it != oldInstruction.end(); it++)
	{
		if ((*it) == '~') 
		{
			//newInstruction += '$';
			it++;
			if ((*it) == 'd')
			{
				if (i->geType() != I_B && i->geType() != I_BLTZ)
				{
					Variable* p = *(i->getDst().begin());

					if (p->geType() == MEM_VAR)
					{
						newInstruction += p->getName();
					}
					else
					{
						newInstruction += "$t";
						newInstruction += to_string((int)p->getAssignment());	//vrv ne radi
					}
				}
				else
				{
					LabelFunction* p = i->getLabFunc();
					newInstruction += p->getName();
				}
			}
			else
			{
				Variable* p = i->takeSrc();
				if (p->geType() == MEM_VAR)
					newInstruction += p->getName();
				else
				{
					newInstruction += "$t";
					newInstruction += to_string((int)p->getAssignment());
				}
			}
		}
		else
		{
			newInstruction += (*it);
		}
	}
	return newInstruction;
}

void createOutputString()
{
	for (auto it = labfunc.begin(); it != labfunc.end(); it++)
		if ((*it)->geType() == FUNCTION)
		{
			g_output_globl[(*it)->getPosition()] = ".globl " + (*it)->getName() + "\n";
			break;
		}

	g_output_data[0] = ".data\n";
	for (auto it = variables.begin(); it != variables.end(); it++)
		if ((*it)->geType() == MEM_VAR)
			g_output_data[(*it)->getPosition()] = (*it)->getName() + ":" + string(5, ' ') + ".word " + to_string((*it)->getValue()) + "\n";

	g_output_text[0] = ".text\n";
	for (auto it = labfunc.begin(); it != labfunc.end(); it++)
		g_output_text[(*it)->getPosition()] = (*it)->getName() + ":\n";

	for (auto it = instructions.begin(); it != instructions.end(); it++)
		g_output_text[(*it)->getPosition()] = string(4, ' ') + makeInstructionString(*it);
}

void printOutputString(string out)
{
	for (auto it = g_output_globl.begin(); it != g_output_globl.end(); it++)
		g_output_str += it->second;
	g_output_str += "\n";
	for (auto it = g_output_data.begin(); it != g_output_data.end(); it++)
		g_output_str += it->second;
	g_output_str += "\n";
	for (auto it = g_output_text.begin(); it != g_output_text.end(); it++)
		g_output_str += it->second;


	cout << g_output_str;

	//string filename = ".\\..\\examples\\" + out + ".s";
	string filename = out + ".s";
	ofstream outputFile(filename);


	outputFile << g_output_str;

	outputFile.close();

}

void printInstructioons()
{
	for (auto it = instructions.begin(); it != instructions.end(); it++)
		cout << (*it) << endl;
}

void stlFree()
{
	for (auto it = instructions.begin(); it != instructions.end(); it++)
		delete (*it);
	for (auto it = variables.begin(); it != variables.end(); it++)
		delete (*it);
	for (auto it = labfunc.begin(); it != labfunc.end(); it++)
		delete (*it);
}