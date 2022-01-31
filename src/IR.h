#ifndef __IR__
#define __IR__

#include "LexicalAnalysis.h"
#include <list>
using namespace std;


//class LabelFunction;

class Instruction;
/**
 * This class represents one variable from program code.
 */
class Variable
{
public:
	/**
	* Constructors
	*/
	Variable() : m_type(NO_TYPE), m_name(""), m_position(-1), m_assignment(no_assign), m_value(0), m_pos_ig_mat(-1) {}
	Variable(std::string name, int pos) : m_type(NO_TYPE), m_name(name), m_position(pos), m_assignment(no_assign), m_value(0), m_pos_ig_mat(-1) {}
	Variable(VariableType t, string n, int p, Regs assgn, int v) : m_type(t), m_name(n), m_position(p), m_assignment(assgn), m_value(v), m_pos_ig_mat(-1) {}
	/**
	* Setters
	*/
	void setPosition(int p) { m_position = p; }
	void seType(VariableType t) { m_type = t; }
	void setName(string n) { m_name = n; }
	void setAssignment(Regs a) { m_assignment = a; }
	void setValue(int v) { m_value = v; }
	void setPosIgM(int v) { m_pos_ig_mat = v; }
		/**
		* Getters
		*/
		int getPosition() { return m_position; }
		VariableType geType() { return m_type; }
		string getName() { return m_name; }
		Regs getAssignment() { return m_assignment; }
		int getValue() { return m_value; }
		int getPosIgM() { return m_pos_ig_mat; }
		/**
		* Operator overloading
		*/
		friend ostream& operator<<(ostream& out, const Variable& v)
		{
			const string type[] = { "MEM_VAR", "REG_VAR", "NO_TYPE" };
			const string assgn[] = { "no_assign", "t0", "t1", "t2", "t3" };
			out << "----------------------------------------------" << endl;
			out << "Type: " << type[v.m_type] << endl;
			out << "Name: " << v.m_name << endl;
			out << "Position: " << v.m_position << endl;
			out << "Assignment: " << assgn[v.m_assignment] << endl;
			out << "Value: " << v.m_value << endl;
			out << "Position in ig matrix: " << v.m_pos_ig_mat << endl << endl;

			return out;
		}
private:
	VariableType m_type;
	std::string m_name;
	int m_position;
	Regs m_assignment;
	int m_value;
	int m_pos_ig_mat;
	
};


/**
 * This type represents list of variables from program code.
 */
typedef std::list<Variable*> Variables;

struct ReturnValueVariable
{
	Variables::iterator itb;
	Variables::iterator ite;
	int size;
};

struct ReturnValueIns
{
	std::list<Instruction*>::iterator itb;
	std::list<Instruction*>::iterator ite;
	int size;
};

/**
 * This class represents one label or function in program code.
 */
class LabelFunction
{
public:
	/**
	* Constructors
	*/
	LabelFunction() : m_position(-1), m_type(LF_NO_TYPE), m_name("") {}
	LabelFunction(string n, int p) : m_position(p), m_type(LF_NO_TYPE), m_name(n) {}
	LabelFunction(int p, LabelFunctionType type, string n) : m_position(p), m_type(type), m_name(n) {}
	/**
	* Setters
	*/
	void setPosition(int p) { m_position = p; }
	void seType(LabelFunctionType t) { m_type = t; }
	void setName(string n) { m_name = n; }
	/**
	* Getters
	*/
	int getPosition() { return m_position; }
	LabelFunctionType geType() { return m_type; }
	string getName() { return m_name; }
	/**
	* Operator overloading
	*/
	friend ostream& operator<<(ostream& out, const LabelFunction& lf)
	{
		const string type[] = { "FUNCTION", "LABEL", "LF_NO_TYPE" };
		out << "----------------------------------------------" << endl;
		out << "Type: " << type[lf.m_type] << endl;
		out << "Name: " << lf.m_name << endl;
		out << "Position: " << lf.m_position << endl << endl;

		return out;
	}
private:
	int m_position;
	LabelFunctionType m_type;
	string m_name;
};


/**
 * This type represents list of labels and functions from program code.
 */
typedef list<LabelFunction*> LabelsFunctions;



/**
 * This class represents one instruction in program code.
 */
class Instruction
{
public:
	/**
	* Constructors
	*/
	Instruction () : m_position(0), m_type(I_NO_TYPE) {}
	Instruction (int pos, InstructionType type, Variables& dst, Variables& src) : m_position(pos), m_type(type), m_dst(dst), m_src(src) {}
	Instruction (int p, InstructionType t) : m_position(p), m_type(t) {}
	/**
	* Setters
	*/
	void setInstructioName(string n) { m_instructioName = n; }
	/**
	* Getters
	*/
	int getPosition() { return m_position; }
	InstructionType geType() { return m_type; }
	string getinstructioName() { return m_instructioName; }

	Variable* takeSrc()
	{
		auto it = m_src.begin();
		Variable* p = *it;
		m_src.erase(it);
		return p;
	}

	LabelFunction* getLabFunc() { return *(m_lf.begin()); }
	Variables getDst() { return m_dst; }
	Variables getSrc() { return m_src; }
	Variables getUse() { return m_use; }
	Variables getDef() { return m_def; }
	Variables getIn() { return m_in; }
	Variables getOut() { return m_out; }
	//ReturnValueVariable getItUse()
	//{
	//	ReturnValueVariable p;
	//	p.itb = m_use.begin();
	//	p.ite = m_use.end();
	//	p.size = m_use.size();
	//	return p;
	//}
	ReturnValueVariable getItUse();
	ReturnValueVariable getItDef();
	ReturnValueVariable getItIn();
	ReturnValueVariable getItOut();
	ReturnValueIns getItSucc();

	void pushDst(Variable* t) { m_dst.push_back(t); }
	void pushSrc(Variable* t) { m_src.push_back(t); }
	void pushUse(Variable* t) { m_use.push_back(t); }
	void pushDef(Variable* t) { m_def.push_back(t); }
	void pushIn(Variable* t) { m_in.push_back(t); }
	void pushOut(Variable* t) { m_out.push_back(t); }
	void pushLabFunc(LabelFunction* t) { m_lf.push_back(t); }
	void pushSucc(Instruction* t) { m_succ.push_back(t); }
	void pushPred(Instruction* t) { m_pred.push_back(t); }

	//void inErace() { m_in.; }
	//neka mooja govna
	void inClear() 
	{
		ReturnValueVariable in = getItIn();
		m_in.erase(in.itb,in.ite);
	}
	void outClear()
	{
		ReturnValueVariable in = getItOut();
		m_out.erase(in.itb, in.ite);
	}
	//lraj nekih mojih govana
	void mySort();
	void myUnique()
	{
		m_in.unique();
		m_out.unique();
	}

	friend ostream& operator<<(ostream& out, const Instruction& i)
	{
		const string type[] = { "I_NO_TYPE", "I_ADD", "I_ADDI", "I_SUB", "I_LA", "I_LI", "I_LW", "I_SW", "I_BLTZ", "I_B", "I_NOP", "I_SRA", "I_JR", "I_NOT" };
		out << "----------------------------------------------" << endl;
		out << "Type: " << type[i.m_type] << endl;
		out << "Name: " << i.m_instructioName << endl;
		out << "Position: " << i.m_position << endl;
		out << "Destinations: ";
		for (auto it = i.m_dst.begin(); it != i.m_dst.end(); it++)
			out << (*it)->getName() << " ";
		out << endl;
		out << "Sources: ";
		for (auto it = i.m_src.begin(); it != i.m_src.end(); it++)
			out << (*it)->getName() << " ";
		out << endl;
		out << "Uses: ";
		for (auto it = i.m_use.begin(); it != i.m_use.end(); it++)
			out << (*it)->getName() << " ";
		out << endl;
		out << "Defines: ";
		for (auto it = i.m_def.begin(); it != i.m_def.end(); it++)
			out << (*it)->getName() << " ";
		out << endl;
		out << "Ins: ";
		for (auto it = i.m_in.begin(); it != i.m_in.end(); it++)
			out << (*it)->getName() << " ";
		out << endl;
		out << "Outs: ";
		for (auto it = i.m_out.begin(); it != i.m_out.end(); it++)
			out << (*it)->getName() << " ";
		out << endl;
		out << "Successor position: ";
		for (auto it = i.m_succ.begin(); it != i.m_succ.end(); it++)
			out << (*it)->getPosition() << " ";
		out << endl;
		out << "Predecessor position: ";
		for (auto it = i.m_pred.begin(); it != i.m_pred.end(); it++)
			out << (*it)->getPosition() << " ";
		out << endl;

		return out;
	}

private:
	int m_position;
	InstructionType m_type;
	
	Variables m_dst;
	Variables m_src;

	Variables m_use;
	Variables m_def;
	Variables m_in;
	Variables m_out;
	std::list<Instruction*> m_succ;
	std::list<Instruction*> m_pred;

	string m_instructioName;
	LabelsFunctions m_lf;
};



/**
 * This type represents list of instructions from program code.
 */
typedef std::list<Instruction*> Instructions;




bool compare(Variable* a, Variable* b);
void createRegisterVariables();
void createVariablesLablesandFunctions(LexicalAnalysis& lex);
void printRegisterVariables();
void printVariablesLablesandFunctions();

Variable* findVariable(Token t);
LabelFunction* findLabFunc(Token t);
void createInstructionList(LexicalAnalysis& lex);
Instructions::iterator findInstruction(LabelFunction* t);
void fillSuccAndPred();
void fillUseAndDef();
void printInstructions();

void livenessAnalysis();
int findPosition(Variable* p);
void doInterferenceGraph();
void printInterferenceGraph();
bool degOne(vector<int> v);
bool doSimplification(int graphDeg);
bool doResourceAllocation();

void createOutputString();
string makeInstructionString();
void printOutputString(string f);
void printInstructioons();
void stlFree();
#endif
