#include<iostream>
#include "../exp1/LexicalAnalyzer.cpp"
#include "../exp2/LL1GrammerAnalyzer.cpp" 
#include "../commonFunctions/common.cpp"
using namespace std;

enum KINDS {
	PROGRAM,ASSIGN,PUT,GET,WHILE,IF,VARIABLE
}; 

struct ASTnode{
	int kind; // ��Ӧ������ ���ﲻ��ʹ��֮ǰtable��ı���� 
	vector<ASTnode*> sons;
	//����Ϊterminal nodeר�� 
	int tokenIndex; //������ڸ����ս����node���������Ķ�Ϊ-1 
	string tokenString; //node ��Ӧ�ı�����/ֵ 
};

//process PROGRAM 
ASTnode* processProgram(TreeNode* program,ASTnode* father){
	if(father = nullptr) {
		father = new ASTnode();
		father->kind = PROGRAM;
		father->sons.clear();
	}
	for(auto ele:program->sons){
		//�����Ƶ�������ֻ���� ���� ���� �����ǿվͲ�����
		if(ele->num == 100) //����
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
	//�����parse tree�ľ��ӣ�Ȼ��Ѿ����µľ�����Ӹ�father
	int num = sentence->sons[0]->num; //����һ�����ӵ�num 
	if(num == 104){
		//��ֵ���
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
		//��������ʽ��ͷ������Ƚ��鷳 
	}
	else{
		cout<<"unexpected"<<endl;
	}
}

void processAssign(TreeNode* assign, ASTnode* father){
	// <��ֵ���> -> <����> <=> <���ʽ> <;>
		
	//��������astnode
	ASTnode* variable = new ASTnode();
	variable->kind =  VARIABLE;
	variable->sons.clear();
	variable->tokenIndex = assign->sons[0]->tokenIndex;
	variable->tokenString = assign->sons[0]->tokenString;
	father->sons.push_back(variable);
	//����ʽ
	processExpression(assign->sons[2],father);
}

void processExpression(TreeNode* exp, ASTnode* father){
	//<���ʽ> -> <һԪ�����><��>
    //<���ʽ> -> <��><��ѡ��Ԫ���㲿��>
    
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
	tokenNums.push_back(200); //push��ֹ��
	tokenStrings.push_back("$");
	grammerAnalyzeResult grammerResult  = grammerAnalyze(tokenNums,tokenStrings);
	cout<<endl<<endl;
	if(grammerResult.tokenWrongIndex != -1){
		int index = grammerResult.tokenWrongIndex;
		cout<<"�﷨�����������󣬴����token��indexΪ"<<index<<"����Ӧ���ַ���Ϊ "<<id_string_table[tokenNums[index]]<<endl;
		cout<<"λ���ڵ�"<<""<<lexicalResult.tokens[index].rowNum<<"��"<<"����"<<""<<lexicalResult.tokens[index].colNum<<"��"<<endl;
		markWrongPos("test.txt",lexicalResult.tokens[index].rowNum,lexicalResult.tokens[index].colNum);
	}
	else{
		//���﷨�����������
		cout<<"�������﷨��������: "<<endl;
		printGrammerTree(grammerResult.grammerTree);
	}
	
	ASTnode* ASTroot = processProgram(grammerResult.grammerTree);
	
	
	return 0;
}

