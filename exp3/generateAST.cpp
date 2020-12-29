#include<iostream>
#include "../exp1/LexicalAnalyzer.cpp"
#include "../exp2/LL1GrammerAnalyzer.cpp" 
#include "../commonFunctions/common.cpp"
#include <algorithm>
using namespace std;

enum KINDS {
	PROGRAM,ASSIGN,PUT,GET,WHILE,IF,VARIABLE,NOT,NUMBER,
	ADD,SUB,BITAND,BITOR,EQUAL,NEQUAL,GT,LESS,GTOEQUAL,LESSOREQUAL,AND,OR, //二元运算符 
	INT,VARIABLES,IFPART,ELSEPART,WHILEPART,
	BOOL
	
};

map<int,string> trans_kinds;

void init_kinds(){
	trans_kinds[PROGRAM]="PROGRAM"; 
	trans_kinds[ASSIGN] = "ASSIGN"; trans_kinds[PUT] = "PUT";trans_kinds[GET] = "GET";trans_kinds[WHILE] = "WHILE";
	trans_kinds[IF] = "IF";trans_kinds[VARIABLE] = "VARIABLE";trans_kinds[NOT] = "NOT";trans_kinds[NUMBER] = "NUMBER";
	trans_kinds[ADD] = "ADD";trans_kinds[SUB] = "SUB";trans_kinds[BITAND] = "BITAND";trans_kinds[BITOR] = "BITOR";trans_kinds[EQUAL] = "EQUAL";
	trans_kinds[NEQUAL] = "NEQUAL";trans_kinds[GT] = "GT";trans_kinds[LESS] = "LESS";trans_kinds[GTOEQUAL] = "GTOEQUAL";trans_kinds[LESSOREQUAL] = "LESSOREQUAL";
	trans_kinds[AND] = "AND";trans_kinds[OR] = "OR";trans_kinds[INT] ="INT";trans_kinds[VARIABLES] ="VARIABLES";
	trans_kinds[IFPART] ="IFPART";trans_kinds[ELSEPART] ="ELSEPART";trans_kinds[WHILEPART] ="WHILEPART";
	trans_kinds[BOOL] ="BOOL";
	
}


void reverseParseTree(TreeNode* root){
	reverse(root->sons.begin(),root->sons.end());
	for(auto x:root->sons){
		reverseParseTree(x); 
	}
}


struct ASTnode{
	int kind; // 对应的种类 这里不再使用之前table里的编号了 
	vector<ASTnode*> sons;
	//以下为terminal node专属 
	int tokenIndex; //这个用于给非终结符的node赋，其它的都为-1 
	string tokenString; //node 对应的变量名/值 
};

void printAST(ASTnode* root){
	//采用层序遍历打出来看看
	queue<pair<ASTnode*,int> > q;
	q.push(make_pair(root,1));
	int curLevel = 1;
	cout<<"第"<<curLevel<<"层 :" <<endl; 
	while(!q.empty()){
		pair<ASTnode*,int> frontPair = q.front();
		q.pop();
		if(frontPair.second > curLevel){
			cout<<endl;
			curLevel++;
			cout<<"第"<<curLevel<<"层 :" <<endl; 
		}
		ASTnode* frontNode = frontPair.first;
		
		if(frontNode->sons.size() == 0){
			if(frontNode->kind == VARIABLE || frontNode->kind == NUMBER)
				cout<<frontNode->tokenString<<"   ";
			else cout<<trans_kinds[frontNode->kind]<<"   ";
		}else{
			cout<<trans_kinds[frontNode->kind]<<"->";
			for(auto x:frontNode->sons){
				if(x->kind == VARIABLE || x->kind == NUMBER)
					cout<<x->tokenString<<" ";
				else cout<<trans_kinds[x->kind]<<" ";
				q.push(make_pair(x,curLevel+1));
			}
		}
		
		cout<<endl;
	}
}

//提前声明 
void processExpression(TreeNode* exp, ASTnode* father);
void processAssign(TreeNode* assign, ASTnode* father);
void processAddV(TreeNode* addv,ASTnode* father);
void processVariables(TreeNode* leftcommon,ASTnode* father);
void processElse(TreeNode* elsepart, ASTnode* father);
void processIf(TreeNode* ifSentence, ASTnode* father);
void processSentence(TreeNode* sentence,ASTnode* father);
ASTnode* processProgram(TreeNode* program,ASTnode* father);

void processExpression(TreeNode* exp, ASTnode* father){
	//<表达式> -> <一元运算符><数>
    //<表达式> -> <数><可选二元运算部分>
    
	if(exp->sons[0]->number == 109 ){
		//一元运算符打头(其实就是！) 
		ASTnode* notSymble = new ASTnode();
		notSymble->kind = NOT;
		notSymble->sons.clear();
		notSymble->tokenIndex = exp->sons[0]->sons[0]->tokenIndex;
		
		//数(变量 or 常量) 
		ASTnode* num = new ASTnode();
		if(exp->sons[1]->sons[0]->number == 66){
			num->kind = NUMBER;
		}
		else if(exp->sons[1]->sons[0]->number == 77){
			num -> kind = VARIABLE;
		}
		else{
			cout<<"unexpected"<<endl;
		}
		
		num->sons.clear();
		num->tokenIndex = exp->sons[1]->sons[0]->tokenIndex;
		num->tokenString = exp->sons[1]->sons[0]->tokenString;
		
		father->sons.push_back(notSymble);
		notSymble->sons.push_back(num);
		 
	}
	else if(exp->sons[0]->number == 111){
		//数打头
		ASTnode* num = new ASTnode();
		if(exp->sons[0]->sons[0]->number == 66){
			num->kind = NUMBER;
		}
		else if(exp->sons[0]->sons[0]->number == 77){
			num -> kind = VARIABLE;
		}
		else{
			cout<<"unexpected"<<endl;
		}
		num->sons.clear();
		num->tokenIndex = exp->sons[0]->sons[0]->tokenIndex;
		num->tokenString = exp->sons[0]->sons[0]->tokenString;
		
		 if(exp->sons[1]->sons.size() == 0){
		 	//如果可选二元运算符部分为空
			father->sons.push_back(num);
		 }
		 else{
		 	//有二元运算符
		 	//构建另一个num节点 
			ASTnode* num2 = new ASTnode();
			if(exp->sons[1]->sons[1]->sons[0]->number == 66){
				num2->kind = NUMBER;
			}
			else if(exp->sons[1]->sons[1]->sons[0]->number == 77){
				num2-> kind = VARIABLE;
			}
			else{
				cout<<"unexpected"<<endl;
			}
			num2->tokenIndex = exp->sons[1]->sons[1]->sons[0]->tokenIndex;
			num2->tokenString = exp->sons[1]->sons[1]->sons[0]->tokenString;
			
			//构建operand节点
			int openum = exp->sons[1]->sons[0]->sons[0]->number;
			ASTnode* operand = new ASTnode();
			operand->sons.clear();
			operand->tokenIndex = exp->sons[1]->sons[0]->sons[0]->tokenIndex;
			operand->tokenString = exp->sons[1]->sons[0]->sons[0]->tokenString;
			switch(openum){
				case 8:
					operand->kind = GT;
					break;
				case 9:
					operand->kind = LESS;
					break;
				case 11:
					operand->kind = GTOEQUAL;
					break;
				case 12:
					operand->kind = LESSOREQUAL;
					break;
				case 13:
					operand->kind = EQUAL;
					break;
				case 15:
					operand->kind = NEQUAL;
					break;
				case 16:
					operand->kind = AND;
					break;
				case 17:
					operand->kind = OR;
					break;
				case 21:
					operand->kind = ADD;
					break;
				case 22:
					operand->kind = SUB;
					break;
				case 25:
					operand->kind = BITOR;
					break;
				case 26:
					operand->kind = BITAND;
					break;
				default:
					operand->kind = -1;
					break;	
			}
			father->sons.push_back(operand);
			operand->sons.push_back(num);
			operand->sons.push_back(num2);		 	
		 }
	}
}


void processAssign(TreeNode* assign, ASTnode* father){
	// <赋值语句> -> <变量> <=> <表达式> <;>
		
	//构建变量astnode
	ASTnode* variable = new ASTnode();
	variable->kind =  VARIABLE;
	variable->sons.clear();
	variable->tokenIndex = assign->sons[0]->tokenIndex;
	variable->tokenString = assign->sons[0]->tokenString;
	father->sons.push_back(variable);
	//求表达式
	processExpression(assign->sons[2],father);
}

void processAddV(TreeNode* addv,ASTnode* father)
{
	// <加变量> -> <,><变量><加变量>
    // <加变量> -> 空
	if(addv->sons.size() != 0){
		ASTnode* v = new ASTnode();
		v->kind = VARIABLE;
		v->tokenIndex = addv->sons[1]->tokenIndex;
		v->tokenString = addv->sons[1]->tokenString;
		father->sons.push_back(v);
		processAddV(addv->sons[2],father);
	}
}

void processVariables(TreeNode* leftcommon,ASTnode* father){
	//找出所有定义的变量，加进father
	// <定义左公因式> -> <int> <变量> <加变量>
    ASTnode* v = new ASTnode();
    v->kind = VARIABLE;
    v->tokenIndex = leftcommon->sons[1]->tokenIndex;
    v->tokenString = leftcommon->sons[1]->tokenString;
    father->sons.push_back(v); 
    processAddV(leftcommon->sons[2],father);
}

void processElse(TreeNode* elsepart, ASTnode* father){
	//<else部分> -> <else> <{> <程序> <{> <;>
	//<else部分> -> 空
	if(elsepart->sons.size() != 0){
		processProgram(elsepart->sons[2],father);
	}
}

void processIf(TreeNode* ifSentence, ASTnode* father){
	//把if语句解析后push到father下
	// <条件语句> -> <if> <(> <表达式> <)> <{> <程序> <}> <;> <else部分>
	processExpression(ifSentence->sons[2],father);
	ASTnode* ifpart = new ASTnode();
	ifpart->kind = IFPART;
	father->sons.push_back(ifpart);
	processProgram(ifSentence->sons[5],ifpart);
	
	ASTnode* elsepart = new ASTnode();
	elsepart->kind = ELSEPART;
	father->sons.push_back(elsepart);
	processElse(ifSentence->sons[8],elsepart);
		
}

void processPut(TreeNode* putSentence, ASTnode* father){
	//<输出语句> -> <put> <(> <变量> <加变量> <)> <;>
	ASTnode* v = new ASTnode();
	v->kind = VARIABLE;
	v->tokenIndex = putSentence->sons[2]->tokenIndex;
	v->tokenString = putSentence->sons[2]->tokenString;
	father->sons.push_back(v);
	processAddV(putSentence->sons[3],father);
}

void processGet(TreeNode* getSentence, ASTnode* father){
	//<输入语句> -> <get> <(> <变量> <加变量> <)> <;>
	ASTnode* v = new ASTnode();
	v->kind = VARIABLE;
	v->tokenIndex = getSentence->sons[2]->tokenIndex;
	v->tokenString = getSentence->sons[2]->tokenString;
	father->sons.push_back(v);
	processAddV(getSentence->sons[3],father);
}

void processWhile(TreeNode* whileSentence, ASTnode* father){
	//<循环语句> -> <while> <(> <表达式> <)> <{> <程序> <}> <;>
	processExpression(whileSentence->sons[2],father);
	ASTnode* mainbody = new ASTnode();
	mainbody->kind = WHILEPART;
	father->sons.push_back(mainbody);
	processProgram(whileSentence->sons[5],mainbody);
}

//process SENTENCE
void processSentence(TreeNode* sentence,ASTnode* father){
	//传入的parse tree的句子，然后把句子下的具体句子给father
	int num = sentence->sons[0]->number; //看第一个儿子的num 
	if(num == 104){
		//赋值语句
		ASTnode* assignNode = new ASTnode();assignNode->kind = ASSIGN;
		father->sons.push_back(assignNode);
		processAssign(sentence->sons[0],assignNode);
	}
	else if(num == 113){
		//PUT
		ASTnode* putNode = new ASTnode();
		putNode->kind = PUT;
		father->sons.push_back(putNode);
		processPut(sentence->sons[0],putNode);
	}
	else if(num == 112){
		//GET
		ASTnode* getNode = new ASTnode();
		getNode->kind = GET;
		father->sons.push_back(getNode);
		processGet(sentence->sons[0],getNode);
	}
	else if(num == 114){
		//WHILE
		ASTnode* whileNode = new ASTnode();
		whileNode->kind = WHILE;
		father->sons.push_back(whileNode);
		processWhile(sentence->sons[0],whileNode);
	}
	else if(num == 115){
		//IF
		ASTnode* ifNode = new ASTnode();
		ifNode->kind = IF;
		father->sons.push_back(ifNode);
		processIf(sentence->sons[0],ifNode);
	}
	else if(num == 102){
		//定义左公因式打头，这个比较麻烦
		//变量声明语句
		//<句子> -> <定义左公因式><定义可赋值 ><;>
		ASTnode* intNode = new ASTnode();
		if(sentence->sons[0]->sons[0]->number == 1)
			intNode->kind = INT;
		else if(sentence->sons[0]->sons[0]->number == 27)
			intNode->kind = BOOL;
		father->sons.push_back(intNode);
		
		ASTnode* variables = new ASTnode();
		variables->kind = VARIABLES;
		intNode->sons.push_back(variables);
		processVariables(sentence->sons[0],variables);
		
		if(sentence->sons[1]->sons.size() != 0) 
		{
			//如果定义可赋值不为空
			processExpression(sentence->sons[1]->sons[1],intNode);
		}
		
		
	}
	else{
		cout<<"unexpected"<<endl;
	}
}
//process PROGRAM 
ASTnode* processProgram(TreeNode* program,ASTnode* father){
	if(father == nullptr) {
		father = new ASTnode();
		father->kind = PROGRAM;
		father->sons.clear();
	}
	for(auto ele:program->sons){
		//程序推导出来的只会是 句子 或者 程序，是空就不管了
		if(ele->number == 100) //程序
		{
			processProgram(ele,father);
		}
		else if(ele->number == 101)
		{
			processSentence(ele,father);
		}
		else{
			cout<<"unexpected"<<endl;
		}
	}
	return father;
}
 
int main()
{
	lexicalAnalyzeResult lexicalResult = lexicalAnalyze("test.txt");
	vector<int> tokenNums;
	vector<string> tokenStrings;
	for(auto x:lexicalResult.tokens){
		tokenNums.push_back(x.num);
		tokenStrings.push_back(x.s);
	}
	tokenNums.push_back(200); //push终止符
	tokenStrings.push_back("$");
	grammerAnalyzeResult grammerResult  = grammerAnalyze(tokenNums,tokenStrings);
	cout<<endl<<endl;
	if(grammerResult.tokenWrongIndex != -1){
		int index = grammerResult.tokenWrongIndex;
		cout<<"语法分析发生错误，错误的token的index为"<<index<<"，对应的字符串为 "<<id_string_table[tokenNums[index]]<<endl;
		cout<<"位置在第"<<""<<lexicalResult.tokens[index].rowNum<<"行"<<"，第"<<""<<lexicalResult.tokens[index].colNum<<"列"<<endl;
		markWrongPos("test.txt",lexicalResult.tokens[index].rowNum,lexicalResult.tokens[index].colNum);
	}
	else{
		//把语法树打出来看看
		cout<<"下面是语法树输出结果: "<<endl;
		printGrammerTree(grammerResult.grammerTree);
	}
	
	//之前的语法树里的sons是倒着存的，这里把它改正。。。。
	reverseParseTree(grammerResult.grammerTree);
	
	init_kinds();
	ASTnode* ASTroot = processProgram(grammerResult.grammerTree,nullptr);
	cout<<endl<<endl;
	cout<<"**************下面打印AST******************"<<endl; 
	printAST(ASTroot);
	
	return 0;
}

