#include<iostream>
#include "../exp1/LexicalAnalyzer.cpp"
#include "../exp2/LL1GrammerAnalyzer.cpp" 
#include "../commonFunctions/common.cpp"
using namespace std;

enum KINDS {
	PROGRAM,ASSIGN,PUT,GET,WHILE,IF,VARIABLE
}; 

struct ASTnode{
	int kind; // 对应的种类 这里不再使用之前table里的编号了 
	vector<ASTnode*> sons;
	//以下为terminal node专属 
	int tokenIndex; //这个用于给非终结符的node赋，其它的都为-1 
	string tokenString; //node 对应的变量名/值 
};

//process PROGRAM 
ASTnode* processProgram(TreeNode* program,ASTnode* father){
	if(father = nullptr) {
		father = new ASTnode();
		father->kind = PROGRAM;
		father->sons.clear();
	}
	for(auto ele:program->sons){
		//程序推导出来的只会是 句子 或者 程序，是空就不管了
		if(ele->num == 100) //程序
		{
			processProgram(ele,father);
		}
		else if(ele->num == 101)
		{
			father->sons.push_back(processSentence(ele),father);
		}
	}
	return father;
}

//process SENTENCE
void processSentence(TreeNode* sentence,ASTnode* father){
	//传入的parse tree的句子，然后把句子下的具体句子给father
	int num = sentence->sons[0]->num; //看第一个儿子的num 
	if(num == 104){
		//赋值语句
		ASTnode* assignNode = new ASTnode();
		father->sons.push_back(assignNode);
		processAssign(sentence->sons[0],assignNode);
	}
	else if(num == 112){
		//PUT
		ASTnode* putNode = new ASTnode();
		father->sons.push_back(putNode);
		processPut(sentence->sons[0],putNode);
	}
	else if(num == 113){
		//GET
		ASTnode* getNode = new ASTnode();
		father->sons.push_back(getNode);
		processGet(sentence->sons[0],getNode);
	}
	else if(num == 114){
		//WHILE
		ASTnode* whileNode = new ASTnode();
		father->sons.push_back(whileNode);
		processWhile(sentence->sons[0],whileNode);
	}
	else if(num == 115){
		//IF
		ASTnode* ifNode = new ASTnode();
		father->sons.push_back(ifNode);
		processIf(sentence->sons[0],ifNode);
	}
	else if(num == 102){
		//定义左公因式打头，这个比较麻烦 
	}
	else{
		cout<<"unexpected"<<endl;
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

void processExpression(TreeNode* exp, ASTnode* father){
	//<表达式> -> <一元运算符><数>
    //<表达式> -> <数><可选二元运算部分>
    
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
	
	ASTnode* ASTroot = processProgram(grammerResult.grammerTree);
	
	
	return 0;
}

