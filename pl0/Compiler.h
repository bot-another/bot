#ifndef COMPILER_H
#define COMPILER_H
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
enum symtype
{
	SYM_NULL,
	SYM_IDENTIFIER,//id_name
	SYM_NUMBER, //number
	SYM_PLUS,//op +
	SYM_MINUS,//op -
	SYM_TIMES,//op *
	SYM_SLASH, //op /
	SYM_ODD,  //key,op  odd
	SYM_EQU,  //op  =
	SYM_NEQ,  //op  <>
	SYM_LES,   //op <
	SYM_LEQ,  //op <=
	SYM_GTR,  //op >
	SYM_GEQ,  //op >=
	SYM_LPAREN,  //(
	SYM_RPAREN,  //)
	SYM_COMMA,   //,
	SYM_SEMICOLON,  //;
	SYM_PERIOD,   //.
	SYM_DOUBLE_MARKS, //"
	SYM_BECOMES, //op :=
	SYM_LEFTBRACKET,//[
	SYM_RIGHTBRACKET, //]
	SYM_COLON,//:
    SYM_BEGIN,  //key
	SYM_END,  //key
	SYM_IF,   //key
	SYM_THEN,//key
	SYM_WHILE,//key
	SYM_DO,//key
	SYM_CALL,//key
	SYM_CONST,//key
	SYM_VAR,//key
	SYM_PROCEDURE,//key
	SYM_PRINT,//key
	SYM_SCANF,//key
	SYM_ELSE,//key
	SYM_FOR,//key
	SYM_BREAK,//key
	SYM_SETJMP,//key
	SYM_LONGJMP,//key
	SYM_RETURN,//key
};

enum idtype
{
	ID_CONSTANT, ID_VARIABLE, ID_PROCEDURE, ID_VARARRAY
};

enum opcode
{
	LIT, OPR, LOD, STO, CAL, INT, JMP, JPC, LEA, LODA, STOA, POP
};

enum oprcode
{
	OPR_RET, OPR_NEG, OPR_ADD, OPR_MIN,
	OPR_MUL, OPR_DIV, OPR_ODD, OPR_EQU,
	OPR_NEQ, OPR_LES, OPR_LEQ, OPR_GTR,
	OPR_GEQ
}; //NEG '-'号, OPR对应操作


typedef struct
{
	int f; // function code
	int l; // level
	int a; // displacement address
} instruction;
#define MAXINS   12
std::string mnemonic[MAXINS] =
{
	"LIT", "OPR", "LOD", "STO", "CAL", "INT", "JMP", "JPC", "LEA", "LODA", "STOA", "POP"
};

typedef struct
{
	std::string name;
	int   kind;
	int   value;
	std::vector<int> dimension;
	short level;
	short address;
}comtab;//kind, value, level, address
typedef struct
{
	std::string name;
	int level;
}key;//kind, value, level, address
bool operator<(key A, key B){
	return A.level < B.level;
}
key key_zero{"+", -1};
class LexicalAnalysis{
	private:
		std::string line;
	public:
		std::ifstream file;
	public:
		std::string id;
		int number;
		char ch;
		int sym;
		size_t chi;
		bool save_sym = false;//仅在else中使用, 一步回溯
		int ssym;//仅在else中使用
		bool is_for = false;
		std::map<std::string, int> w_sym = 
		{
			{"", SYM_NULL}, {"begin", SYM_BEGIN}, {"call", SYM_CALL}, 
			{"const", SYM_CONST}, {"do", SYM_DO}, {"end", SYM_END}, 
			{"if", SYM_IF}, {"odd", SYM_ODD}, {"procedure", SYM_PROCEDURE},
			{"then", SYM_THEN}, {"var", SYM_VAR}, {"while", SYM_WHILE},{"print", SYM_PRINT},
			{"scanf", SYM_SCANF}, {"else", SYM_ELSE}, {"for", SYM_FOR},
			{"break", SYM_BREAK}, {"setjmp",SYM_SETJMP}, {"longjmp", SYM_LONGJMP}, {"return",SYM_RETURN},
		};
		std::map<char, int> c_sym = 
		{
			{'\0', SYM_NULL}, {'+', SYM_PLUS}, {'-', SYM_MINUS}, 
			{'*', SYM_TIMES}, {'/', SYM_SLASH}, {'(', SYM_LPAREN}, 
			{')', SYM_RPAREN}, {'=', SYM_EQU}, {',', SYM_COMMA}, 
			{'.', SYM_PERIOD}, {';', SYM_SEMICOLON}, {'"', SYM_DOUBLE_MARKS},
			{'[', SYM_LEFTBRACKET}, {']', SYM_RIGHTBRACKET}, {':', SYM_COLON}
		};
	public:
		LexicalAnalysis(std::string& name){
			file.open(name.c_str(), std::ios::in);
			if(!file.is_open()){
				std::cout<<"File open failed"<<std::endl;
				exit(0);
			}
			ch = '\n';
			chi = 0;
		}
		~LexicalAnalysis(){
			file.close();
		}
		bool get_line(){
			chi = 0;
			if(file>>line)return true;
			else return false;
		}
		bool getch(){
			if(chi < line.size()){
				ch = line[chi++];
				return true;
			}
			else{
				if(get_line()){
					ch = '\n';
					return true;
				}
			}
			ch = '\0';
			return false;
		}
		bool get_sym(){
			if(save_sym){
				save_sym = false;
				sym = ssym;
				return true;
			}
			while(ch == '\n')getch();
			std::string buff;
			if(isalpha(ch)){
				do{
					buff += ch;
					getch();
				}while(isalpha(ch) || isdigit(ch));
			
				if(w_sym[buff]){
					sym = w_sym[buff];
				}
				else{
					sym = SYM_IDENTIFIER;
					id = buff;
				}
			}
			else if(isdigit(ch)){
				sym = SYM_NUMBER;
				number = 0;
				do{
					number = number * 10 + ch - '0';//possible error
					getch();
				}while(isdigit(ch));
			}
			else if(ch == ':'){
				getch();
				if(ch == '='){
					sym = SYM_BECOMES;
					getch();
				}
				else{
					sym = SYM_COLON;
					//possible error
				}
			}
			else if(ch == '>'){
				getch();
				if(ch == '='){
					sym = SYM_GEQ;
					getch();
				}
				else{
					sym = SYM_GTR;
				}
			}
			else if(ch == '<'){
				getch();
				if(ch == '='){
					sym = SYM_LEQ;
					getch();
				}
				else if(ch == '>'){
					sym = SYM_NEQ;
					getch();
				}
				else{
					sym = SYM_LES;
				}
			}
			else if(ch == '\0')return false;
			else{
				if(c_sym[ch]){
					sym = c_sym[ch];
					getch();
				}
				else{
					printf("Fatal Error: Unknown character.\n");
					exit(1);
				}
			}
			return true;
		}
};
comtab zero{"main", ID_PROCEDURE, 0, std::vector<int>(), -1, 0};
class GrammarAnalysis : public LexicalAnalysis{
	public:
		std::vector<instruction>code;
		GrammarAnalysis(std::string& name):LexicalAnalysis(name){
			key a{"main", 0};
			table[key_zero]["main"] = zero;
			table[a]["*"] = comtab();
			table[a]["*"].name = '+';
		}
	private:
		std::map<key, std::map<std::string, comtab>>table;
		std::set<int>declbegsys{SYM_CONST, SYM_VAR, SYM_PROCEDURE, SYM_NULL};
		std::set<int>relset{SYM_EQU, SYM_NEQ, SYM_LES, SYM_LEQ, SYM_GTR, SYM_GEQ, SYM_NULL};
		int break_arr = -1;
		int level = 0;//层
		int value = 0;//var 初始化;
		int dx = 3;
		std::string name = "main";
		std::vector<int>dim;
		void GeneratesInstruction(int x, int y, int z){
			instruction A{x, y, z};
			code.push_back(A);
		}
		void print(int from, int to){
			putchar('\n');
			for(int i = from; i < to; i++){
				printf("%5d %s\t%d\t%d\n", i, mnemonic[code[i].f].c_str(), code[i].l, code[i].a);
			}
			putchar('\n');
		}
		comtab* position(){
			key a{name, level};
			if(is_for){
				if(table[a].count("_" + id) != 0){
					return &table[a]["_" + id];
				}
			}
			while(a.level > -1 && table[a].count(id) == 0){
				a.level--;
				a.name = table[a]["*"].name;
			}
			if(a.level != -1){
				return &table[a][id];
			}
			else{
				return nullptr;
			}
		}
		void AddTable(int kind, bool isparameter = false, std::string ID = "", comtab* p = nullptr){
			if(!isparameter){
				key a{name, level};
				if(table[a].count(id) == 0){
					comtab A;
					int size = 1;
					A.kind = kind;
					A.level = level;
					A.name = id;
					key c{id, level + 1};
					switch (kind)
					{
					case ID_CONSTANT:
						A.value = number;
						break;
					case ID_VARIABLE:
						A.address = dx++;
						A.value = value;
						break;
					case ID_PROCEDURE:
						table[c]["*"] = comtab();
						table[c]["*"].name = name;
						A.value = 0;
						break;
					case ID_VARARRAY:
						A.address = dx;
						A.dimension = dim;
						for(int i = 0; i < (int)dim.size(); i++){
							size *= dim[i];
						}
						dx += size;
						A.value = size;
						break;
					default:
						break;
					}
					table[a][id] = A;
				}
				else{
					std::cout<<"Duplicate definition"<<std::endl;
					exit(0);
				}
			}
			else{
				if(p != nullptr){
					key a{ID, level + 1};
					table[a][p->name] = *p;
				}
				else{
					key a{name, level};
					table[a][ID].value = kind;
				}
			}
		}
		void constdeclaration(){
			if (sym == SYM_IDENTIFIER){
				get_sym();
				if (sym == SYM_EQU || sym == SYM_BECOMES)
				{
					if (sym == SYM_BECOMES){
						std::cout<<"Found ':=' when expecting '='."<<std::endl;
						exit(0);
					}
					get_sym();
					if (sym == SYM_NUMBER){
						AddTable(ID_CONSTANT);
						get_sym();
					}
					else{
						std::cout<<"There must be a number to follow '='."<<std::endl;
						exit(0);
					}
				}
				else{
					std::cout<<"There must be an '=' to follow the identifier."<<std::endl;
					exit(0);
				}
			} 
			else{
				std::cout<<"There must be an identifier to follow 'const', 'var', or 'procedure'."<<std::endl;
			}
		}
		void vardeclaration(void){
			if (sym == SYM_IDENTIFIER){
				get_sym();
				if(sym == SYM_BECOMES){
					get_sym();
					if(sym == SYM_NUMBER){
						value = number;
						AddTable(ID_VARIABLE);
						get_sym();
					}
					else{
						std::cout<<"Missing number"<<std::endl;
						exit(0);
					}
				}
				else{
					if(sym != SYM_LEFTBRACKET){
						value = 0;
						AddTable(ID_VARIABLE);
					}
					else{
						std::vector<int>dimension;
						do{
							get_sym();
							if(sym == SYM_NUMBER){
								dimension.push_back(number);
							}
							else{
								std::cout<<"Missing array dimension information."<<std::endl;
								exit(0);
							}
							get_sym();
							if(sym == SYM_RIGHTBRACKET){
								get_sym();
							}
							else{
								std::cout<<"Missing right bracket."<<std::endl;
								exit(0);
							}
						}while(sym == SYM_LEFTBRACKET);
						dim = dimension;
						AddTable(ID_VARARRAY);
					}
				}
			}
			else{
				std::cout<<"There must be an identifier to follow 'const', 'var', or 'procedure'"<<std::endl; // There must be an identifier to follow 'const', 'var', or 'procedure'.
				exit(0);
			}
		} // vardeclaration
		void parameter(std::string& ID){
			std::vector<comtab> parameter;
			comtab A;
			int arr = 0;
			do{
				get_sym();
				if(sym == SYM_RPAREN)return;
				if(sym == SYM_VAR){
					get_sym();
				}
				else{
					std::cout<<"Missing 'var'."<<std::endl;
					exit(0);
				}
				if (sym == SYM_IDENTIFIER){
					get_sym();
					if(sym != SYM_LEFTBRACKET){
						A.name = id;
						A.kind = ID_VARIABLE;
						A.level = level + 1;
						parameter.push_back(A);
						arr -= 1;
					}
					else{
						std::vector<int>dimension;
						do{
							get_sym();
							if(sym == SYM_NUMBER){
								dimension.push_back(number);
							}
							else{
								std::cout<<"Missing array dimension information."<<std::endl;
								exit(0);
							}
							get_sym();
							if(sym == SYM_RIGHTBRACKET){
								get_sym();
							}
							else{
								std::cout<<"Missing right bracket."<<std::endl;
								exit(0);
							}
						}while(sym == SYM_LEFTBRACKET);
						A.name = id;
						A.level = level + 1;
						A.dimension = dimension;
						A.kind = ID_VARARRAY;
						int size = 1;
						for(size_t j = 0; j < A.dimension.size(); j++){
							size = size * A.dimension[j];
						}
						A.value = size;
						arr -= size;
						parameter.push_back(A);
					}
				}
				else{
					std::cout<<"There must be an identifier to follow 'const', 'var', or 'procedure'"<<std::endl; // There must be an identifier to follow 'const', 'var', or 'procedure'.
					exit(0);
				}
			}while(sym == SYM_COMMA);
			AddTable(-arr, true, ID, nullptr);
			for(size_t i = 0; i < parameter.size(); i++){
				parameter[i].address = arr;
				if(parameter[i].kind == ID_VARARRAY){
					arr += parameter[i].value;
				}
				else{
					arr += 1;
				}
				AddTable(parameter[i].kind, true, ID, &parameter[i]);
			}
			
		}
		//传参
		void transmit_parameters(){
			comtab *a = position();
			int i = 0;
			get_sym();
			if(sym == SYM_LPAREN){
				get_sym();
			}
			else{
				std::cout<<"Missing left parenthesis."<<std::endl;
				exit(0);
			}
			if(sym == SYM_RPAREN){
				get_sym();
				if(a->value != 0){
					std::cout<<"Missing parameters."<<std::endl;
					exit(0);
				}
				return;
			}
			comtab *paramters;
			do{
				if(sym == SYM_COMMA)get_sym();
				if(sym == SYM_IDENTIFIER){
					paramters = position();
					if(paramters->kind == ID_VARARRAY)i += (paramters->value - 1);
				}
				expression();
				i += 1;
			}while(sym == SYM_COMMA);
			if(i != a->value){
				std::cout<<"The number of parameters is wrong."<<std::endl;
				exit(0);
			}
			if(sym == SYM_RPAREN){
				get_sym();
			}
			else{
				std::cout<<"Missing right parenthesis."<<std::endl;
				exit(0);
			}
		}
		void factor(){
			if(sym == SYM_IDENTIFIER){
				comtab* a;
				a = position();
				if(a == nullptr){
					std::cout<<"Undeclared identifier."<<std::endl;
					exit(0);
				}
				switch (a->kind){
				case ID_CONSTANT:
					GeneratesInstruction(LIT, 0, a->value);
					break;
				case ID_VARIABLE:
					GeneratesInstruction(LOD, level - a->level, a->address);
					break;
				case ID_PROCEDURE:
					transmit_parameters();
					GeneratesInstruction(CAL, level - a->level, a->address);
					GeneratesInstruction(POP, 1, a->value);
					break;
				case ID_VARARRAY:
					array_analysis();
					GeneratesInstruction(LODA, level - a->level, a->address);
					break;
				default:
					break;
				}
				if(a->kind != ID_VARARRAY && a->kind != ID_PROCEDURE){
					int b, c;
					get_sym();////////////////
				}
			}
			else if(sym == SYM_NUMBER){
				GeneratesInstruction(LIT, 0, number);
				get_sym();
			}
			else if(sym == SYM_LPAREN){
				get_sym();
				expression();
				if(sym == SYM_RPAREN){
					get_sym();
				}
				else{
					std::cout<<"Missing ')'."<<std::endl;
					exit(0);
				}
			}else if(sym == SYM_MINUS){
				get_sym();
				factor();
				GeneratesInstruction(OPR, 0, OPR_NEG);
			}
			else if(sym == SYM_SETJMP){
				get_sym();
				if(sym == SYM_LPAREN){
					get_sym();
				}
				else{
					std::cout<<"Missing left parenthesis."<<std::endl;
					exit(0);
				}
				if(sym == SYM_IDENTIFIER){
					comtab* a = position();
					if(a && a->kind == ID_VARARRAY && a->value >= 4 && a->dimension.size() == 1){
						get_sym();
					}
					else{
						std::cout<<"Set_jmp: miss jmp_buf or jmp_buf is too small or jmp_buf is not one dimensional array"<<std::endl;
						exit(0);
					}
					GeneratesInstruction(LEA, level - a->level, a->address);
					GeneratesInstruction(CAL, -2, 0);
					GeneratesInstruction(LIT, 0, 3);
					GeneratesInstruction(LODA, level - a->level, a->address);
					if(sym == SYM_RPAREN){
						get_sym();
					}
					else{
						std::cout<<"Missing right parenthesis."<<std::endl;
						exit(0);
					}
				}
				else{
					std::cout<<"Setjmp miss jmp_buf."<<std::endl;
					exit(0);
				}
			}
			else{
				std::cout<<"The symbol can not be as the beginning of an expression."<<std::endl;
				exit(0);
			}
		}
		void array_analysis(){
			comtab* a = position();
			if(a == nullptr){
				std::cout<<"Undeclared identifier."<<std::endl;
				exit(0);
			}
			get_sym();
			if(sym != SYM_LEFTBRACKET){
				for(int i = 0; i < a->value; i++){
					GeneratesInstruction(LIT, 0, i);
					GeneratesInstruction(LODA, level - a->level, a->address);
				}
				code.pop_back();
				return;
			}
			GeneratesInstruction(LIT, 0, 0);
			int l = 1;
			do{
				if(sym == SYM_LEFTBRACKET){
					get_sym();
					expression();
				}
				else{
					std::cout<<"Missing left bracket."<<std::endl;
					exit(0);
				}
				GeneratesInstruction(OPR, 0, OPR_ADD);
				if(l < (int)a->dimension.size()){
					GeneratesInstruction(LIT, 0, a->dimension[l++]);
					GeneratesInstruction(OPR, 0, OPR_MUL);
				}
				if(sym == SYM_RIGHTBRACKET){
					get_sym();
				}
				else{
					std::cout<<"Missing closing bracket."<<std::endl;
					exit(0);
				}
			}while(sym == SYM_LEFTBRACKET);
			if(l != (int)a->dimension.size()){
				std::cout<<"Missing array dimension information."<<std::endl;
				exit(0);
			}
			// GeneratesInstruction(LODA, 0, 0);			
		}
		void term(){
			factor();
			int op = 0;
			while(sym == SYM_TIMES || sym == SYM_SLASH){
				op = sym;
				get_sym();
				factor();
				if(op == SYM_TIMES){
					GeneratesInstruction(OPR, 0, OPR_MUL);
				}
				else{
					GeneratesInstruction(OPR, 0, OPR_DIV);
				}
			}
		}
		void expression(bool assignment_expression = false){
			if (sym == SYM_IDENTIFIER){
				comtab* a = position();
				factor();
				if (sym == SYM_BECOMES){//赋值表达式
					if (a == nullptr){
						std::cout << "Undeclared identifier." << std::endl;
						exit(0);
					}
					get_sym();
					if (a->kind == ID_VARIABLE){
						code.pop_back();
						expression();
						GeneratesInstruction(STO, level - a->level, a->address);
					}
					else if(a->kind == ID_VARARRAY){
						code.pop_back();
						expression();
						GeneratesInstruction(STOA, level - a->level, a->address);
					}
					else{
						std::cout << "Procedure isn't assigned." << std::endl;
						exit(0);
					}
				}
				else{//普通表达式
				    if(assignment_expression){
						std::cout<<"Expression error, this must be assignment_expression."<<std::endl;
						exit(0);
					}
					int op = 0;
					while (sym == SYM_TIMES || sym == SYM_SLASH){
						op = sym;
						get_sym();
						factor();
						if (op == SYM_TIMES){
							GeneratesInstruction(OPR, 0, OPR_MUL);
						}
						else{
							GeneratesInstruction(OPR, 0, OPR_DIV);
						}
					}
					while (sym == SYM_PLUS || sym == SYM_MINUS){
						op = sym;
						get_sym();
						term();
						if (op == SYM_PLUS) {
							GeneratesInstruction(OPR, 0, OPR_ADD);
						}
						else {
							GeneratesInstruction(OPR, 0, OPR_MIN);
						}
					}
				}
			}
			else{
				int op;
				term();
				while (sym == SYM_PLUS || sym == SYM_MINUS){
					op = sym;
					get_sym();
					term();
					if (op == SYM_PLUS) {
						GeneratesInstruction(OPR, 0, OPR_ADD);
					}
					else {
						GeneratesInstruction(OPR, 0, OPR_MIN);
					}
				}
			}
		}
		void condition(){
			int op;
			if(sym == SYM_ODD){
				get_sym();
				expression();
				GeneratesInstruction(OPR, 0, OPR_ODD);
			}
			else{
				expression();
				if(relset.count(sym) == 0){
					std::cout<<"Relative operators expected."<<std::endl;
					exit(0);
				}
				else{
					op = sym;
					get_sym();
					expression();
					switch (op)
					{
					case SYM_EQU:
						GeneratesInstruction(OPR, 0, OPR_EQU);
						break;
					case SYM_NEQ:
						GeneratesInstruction(OPR, 0, OPR_NEQ);
						break;
					case SYM_LES:
						GeneratesInstruction(OPR, 0, OPR_LES);
						break;
					case SYM_GEQ:
						GeneratesInstruction(OPR, 0, OPR_GEQ);
						break;
					case SYM_GTR:
						GeneratesInstruction(OPR, 0, OPR_GTR);
						break;
					case SYM_LEQ:
						GeneratesInstruction(OPR, 0, OPR_LEQ);
						break;
					default:
						break;
					}
				}
			}
		}
		void statement(){
		    if(sym == SYM_SEMICOLON){
				return;
			}
			else if(sym == SYM_IDENTIFIER || sym == SYM_SETJMP){
				expression();
				GeneratesInstruction(POP, 0, 1);
			}
			else if(sym == SYM_CALL){
				get_sym();
				if(sym != SYM_IDENTIFIER){
					std::cout<<"There must be an identifier to follow the 'call'."<<std::endl;
					exit(0);
				}
				else{
					comtab* a;
					a = position();
					if(a == nullptr){
						std::cout<<"Undeclared identifier."<<std::endl;
						exit(0);
					}
					if(a->kind != ID_PROCEDURE){
						std::cout<<"A constant or variable can not be called."<<std::endl;
						exit(0);
					}
					if(a->value != 0){
						std::cout<<"The procedure can not be called, because it has parameters"<<std::endl;
						exit(0);
					}
					GeneratesInstruction(CAL, level - a->level, a->address);
					GeneratesInstruction(POP, 0, 1);			
					get_sym();
				}
			}
			else if(sym == SYM_IF){
				get_sym();
				condition();
				if(sym == SYM_THEN){
					get_sym();
				}
				else{
					std::cout<<"'then' expected."<<std::endl;
					exit(0);
				}
				int cx0 = code.size();
				GeneratesInstruction(JPC, 0, 0);
				statement();
				GeneratesInstruction(JMP, 0, 0);
				int cx1 = code.size() - 1;
				code[cx0].a = code.size();
				if(sym == SYM_SEMICOLON){
					get_sym();
					if(sym == SYM_ELSE){
						get_sym();
						statement();
					}
					else{
						save_sym = true;//无else, 回溯
						ssym = sym;
						sym = SYM_SEMICOLON;
					}
				}
				else{
					std::cout<<"';' or 'else' expected."<<std::endl;
					exit(0);
				}
				code[cx1].a = code.size();
			}
			else if(sym == SYM_BEGIN){
				get_sym();
				statement();
				while(sym == SYM_SEMICOLON){
					get_sym();
					if(sym == SYM_END)break;
					statement();
				}
				if(sym == SYM_END){
					get_sym();
				}
				else{
					std::cout<<"';' or 'end' expected."<<std::endl;
					exit(0);
				}
			}
			else if(sym == SYM_WHILE){
				int save_break_arr = break_arr;
				int cx0 = code.size();
				get_sym();
				condition();
				int cx1 = code.size();
				GeneratesInstruction(JPC, 0, 0);
				if(sym == SYM_DO){
					get_sym();
				}
				else{
					std::cout<<"'do' expected."<<std::endl;
					exit(0);
				}
				statement();
				GeneratesInstruction(JMP, 0, cx0);
				code[cx1].a = code.size();
				if(break_arr != save_break_arr){
					code[break_arr].a = code.size();
					break_arr = save_break_arr;
				}
			}
			else if(sym == SYM_FOR){
				int save_break_arr = break_arr;
				bool save_is_for = is_for;
				is_for = true;
				int save_dx = dx;
				bool NEW = false;
				get_sym();
				if(sym == SYM_LPAREN){
					get_sym();
				}
				else{
					std::cout<<"Missing left parenthesis."<<std::endl;
					exit(0);
				}
				if(sym == SYM_VAR){
					NEW = true;
					get_sym();
				}
				else{
					NEW = false;
				}
				key a{name, level};
				std::string ID ="";
				if(sym == SYM_IDENTIFIER){
					if(NEW){
						ID = id;
						GeneratesInstruction(LIT, 0, 0);
						table[a]["_" + id] = comtab();
						table[a]["_" + id].kind = ID_VARIABLE;
						table[a]["_" + id].level = level;
						table[a]["_" + id].address = dx++;
						table[a]["_" + id].name = id;
					}
					expression(true);
					GeneratesInstruction(POP, 0, 1);
					if(sym == SYM_SEMICOLON){
						get_sym();
					}
					else{
						std::cout<<"Missing ',' or ';'."<<std::endl;
						exit(0);
					}
				}
				else if(sym == SYM_SEMICOLON){
						get_sym();
					}
				else{
					std::cout<<"Missing ',' or ';'."<<std::endl;
					exit(0);
				}
				int cx = code.size();
				if(sym == SYM_ODD || sym == SYM_IDENTIFIER || sym == SYM_NUMBER || sym == SYM_LPAREN){
					condition();
					if(sym == SYM_SEMICOLON){
						get_sym();
					}
					else{
						std::cout<<"Missing ',' or ';'."<<std::endl;
						exit(0);
					}
				}
				else if(sym == SYM_SEMICOLON){
						GeneratesInstruction(LIT, 0, 1);
						get_sym();
					}
				else{
					std::cout<<"Missing ',' or ';'."<<std::endl;
					exit(0);
				}
				int cx0 = code.size();
				GeneratesInstruction(JPC, 0, 0);
				GeneratesInstruction(JMP, 0, 0);
				if(sym == SYM_IDENTIFIER){
					expression(true);
					GeneratesInstruction(POP, 0, 1);
					GeneratesInstruction(JMP, 0, cx);
					if(sym == SYM_RPAREN){
						get_sym();
					}
					else{
						std::cout<<"Missing right parenthesis."<<std::endl;
						exit(0);
					}
				}
				else if(sym == SYM_RPAREN){
						GeneratesInstruction(JMP, 0, cx);
						get_sym();
					}
				else{
					std::cout<<"'For' missing ')'."<<std::endl;
					exit(0);
				}
				code[cx0 + 1].a = code.size();
				statement();
				GeneratesInstruction(JMP, 0, cx0 + 2);
				code[cx0].a = code.size();
				is_for = save_is_for;
				dx = save_dx;
				if(NEW)table[a].erase("_" + ID);
				if(break_arr != save_break_arr){
					code[break_arr].a = code.size();
					break_arr = save_break_arr;
				}
			}
			else if(sym == SYM_BREAK){
				break_arr = code.size();
				GeneratesInstruction(JMP, 0, 0);
				get_sym();
			}
			else if(sym == SYM_LONGJMP){
				get_sym();
				if(sym == SYM_LPAREN){
					get_sym();
				}
				else{
					std::cout<<"Missing left parenthesis."<<std::endl;
					exit(0);
				}
				if(sym == SYM_IDENTIFIER){
					comtab* a = position();
					if(a && a->kind == ID_VARARRAY && a->value >= 4 && a->dimension.size() == 1){
						get_sym();
					}
					else{
						std::cout<<"Long_jmp: miss jmp_buf or jmp_buf is too small or jmp_buf is not one dimensional array"<<std::endl;
						exit(0);
					}
					GeneratesInstruction(LEA, level - a->level, a->address);
					if(sym == SYM_COMMA){
						get_sym();
					}
					else{
						std::cout<<"Miss ','."<<std::endl;
						exit(0);
					}
					expression();
					GeneratesInstruction(CAL, -2, 1);
					if(sym == SYM_RPAREN){
						get_sym();
					}
					else{
						std::cout<<"Miss right parenthesis."<<std::endl;
						exit(0);
					}
				}
			}
			else if(sym == SYM_PRINT){
				get_sym();
				if(sym == SYM_LPAREN){
					get_sym();
					if(sym == SYM_DOUBLE_MARKS){
						std::string str = "";
						while(ch != '"'){
							if(ch !=  '\n'){
								str += ch;
							}
							else{
								str += ' ';
							}
							getch();
						}
						str += '\n';
						for(int i = (int)str.size() - 1; i >= 0; i--){
							GeneratesInstruction(LIT, 0, (int)str[i]);
						}
						GeneratesInstruction(CAL, -1, 0);
						ch = '\n';
						get_sym();
						if(sym == SYM_RPAREN){
							get_sym();
						}
						else{
							std::cout<<"Missing right parenthesis."<<std::endl;
							exit(0);
						}

					}
					else if(sym == SYM_IDENTIFIER || sym == SYM_NUMBER || sym == SYM_LPAREN){
						int n = 0;
						do{
							expression();
							if(sym == SYM_COMMA)get_sym();
							n++;
						}while(sym == SYM_IDENTIFIER || sym == SYM_NUMBER);
						if(sym == SYM_RPAREN){
							get_sym();
						}
						else{
							std::cout<<"Missing right parenthesis."<<std::endl;
							exit(0);
						}
						GeneratesInstruction(LIT, 0, n);
						GeneratesInstruction(CAL, -1, 1);
					}
					else{
						std::cout<<"print error"<<std::endl;
						exit(0);
					}
				}
				else{
					std::cout<<"Missing left parenthesis."<<std::endl;
					exit(0);
				}
			}
			else if(sym == SYM_SCANF){
				get_sym();
				if(sym == SYM_LPAREN){
					get_sym();
					if(sym == SYM_IDENTIFIER){
						int n = 0;
						do{
							comtab* a = position();
							if(a == nullptr){
								std::cout<<"Undeclared identifier."<<std::endl;
								exit(0);
							}
							if(a->kind == ID_VARIABLE){
								GeneratesInstruction(LEA, level - a->level, a->address);
								get_sym();
							}
							else if(a->kind == ID_VARARRAY){
								array_analysis();
								GeneratesInstruction(LEA, level - a->level, a->address);
								GeneratesInstruction(OPR, 0, OPR_ADD);
							}
							if(sym == SYM_COMMA){
								get_sym();
							}
							n++;
						}while(sym == SYM_IDENTIFIER);
						if(sym == SYM_RPAREN){
							get_sym();
						}
						else{
							std::cout<<"Missing right parenthesis."<<std::endl;
							exit(0);
						}
						GeneratesInstruction(LIT, 0, n);
						GeneratesInstruction(CAL, -1, 2);
					}
					else{
						std::cout<<"scanf error"<<std::endl;
						exit(0);
					}
				}
				else{
					std::cout<<"Missing left parenthesis."<<std::endl;
					exit(0);
				}
			}
			else if(sym == SYM_RETURN){
				get_sym();
				if(sym == SYM_SEMICOLON){
					GeneratesInstruction(LIT, 0, 0);
					GeneratesInstruction(OPR, 0, OPR_RET);
				}
				else{
					expression();
					GeneratesInstruction(OPR, 0, OPR_RET);
				}
			}
			else{
				std::cout<<"The symbol can not be as the beginning of a statement."<<std::endl;
				exit(0);
			}
		}
	public:
		void block(){
			dx = 3;
			int block_dx = dx;
			comtab* mk = &table[{table[{name, level}]["*"].name, level - 1}][name];
			if(level == 0){
				mk->address = code.size();
				GeneratesInstruction(JMP, 0, 0);
			}
			do{
				if(sym == SYM_CONST){
					get_sym();
					do{
						constdeclaration();
						while(sym == SYM_COMMA){
							get_sym();
							constdeclaration();
						}
						if(sym == SYM_SEMICOLON){
							get_sym();
						}
						else{
							std::cout<<"Missing ',' or ';'."<<std::endl;
							exit(0);
						}
					}while(sym == SYM_IDENTIFIER);
				}
				if(sym == SYM_VAR){
					get_sym();
					do{
						vardeclaration();
						while (sym == SYM_COMMA)
						{
							get_sym();
							vardeclaration();
						}
						if (sym == SYM_SEMICOLON)
						{
							get_sym();
						}
						else{
							std::cout<<"Missing ',' or ';'."<<std::endl;
							exit(0);
						}
					}while (sym == SYM_IDENTIFIER);
				}
				block_dx = dx;
				std::string ID;
				while(sym == SYM_PROCEDURE){
					get_sym();
					if(sym == SYM_IDENTIFIER){
						ID = id;
						AddTable(ID_PROCEDURE);
						get_sym();
					}
					else{
						std::cout<<"There must be an identifier to follow 'const', 'var', or 'procedure'."<<std::endl;
						exit(0);
					}
					if(sym == SYM_LPAREN){
						parameter(ID);
						if(sym == SYM_RPAREN){
							get_sym();
						}
						else{
							std::cout<<"Missing right parenthesis."<<std::endl;
							exit(0);
						}
					}
					if(sym == SYM_SEMICOLON){
						get_sym();
					}
					else{
						std::cout<<"Missing ',' or ';'."<<std::endl;
					}
					level++;
					std::string father = name;
					name = ID;
					block();
					name = father;
					level--;
					if(sym == SYM_SEMICOLON){
						get_sym();
					}
					else{
						std::cout<<"Missing ',' or ';'."<<std::endl;
						exit(0);
					}
					dx = block_dx;
				}
			}while(declbegsys.count(sym) == 1);
			if(level == 0)code[mk->address].a = code.size();
			mk->address = code.size();
			int cx0 = code.size();
			GeneratesInstruction(INT, 0, 3);
			std::vector<comtab> ver;
			for(auto k : table[{name, level}]){
				if((k.second.kind == ID_VARIABLE || k.second.kind == ID_VARARRAY) && k.second.address > 0){
					ver.push_back(k.second);
				}
			}
			auto com = [](comtab A, comtab B){
				return A.address < B.address;
			};
			std::sort(ver.begin(), ver.end(), com);
			for(auto i : ver){
				if(i.kind == ID_VARIABLE)GeneratesInstruction(LIT, 0, i.value);
				else{
					for(int j = 0; j < i.value; j++){
						GeneratesInstruction(LIT, 0, 0);
					}
				}
			}
			statement();
			GeneratesInstruction(LIT, 0, 0);
			GeneratesInstruction(OPR, 0, OPR_RET);
			std::cout<<"procedure "<<name<<":"<<std::endl;
			print(cx0, code.size());
		}
		void compile(){
			get_sym();
			block();
			if(sym == SYM_PERIOD){
				std::cout<<"Finish compiling."<<std::endl;
			}
			else{
				std::cout<<"'.' expected."<<std::endl;
			}
		}
};
#endif