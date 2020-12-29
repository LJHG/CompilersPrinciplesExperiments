#include<iostream>
#include "../exp1/LexicalAnalyzer.cpp"
#include "../exp2/LL1GrammerAnalyzer.cpp" 
#include "../commonFunctions/common.cpp"
#include <algorithm>
using namespace std;

enum KINDS {
	PROGRAM,ASSIGN,PUT,GET,WHILE,IF,VARIABLE,NOT,NUMBER,
	ADD,SUB,BITAND,BITOR,EQUAL,NEQUAL,GT,LESS,GTOEQUAL,LESSOREQUAL,AND,OR, //��Ԫ����� 
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
	int kind; // ��Ӧ������ ���ﲻ��ʹ��֮ǰtable��ı���� 
	vector<ASTnode*> sons;
	//����Ϊterminal nodeר�� 
	int tokenIndex; //������ڸ����ս����node���������Ķ�Ϊ-1 
	string tokenString; //node ��Ӧ�ı�����/ֵ 
};

void printAST(ASTnode* root){
	//���ò���������������
	queue<pair<ASTnode*,int> > q;
	q.push(make_pair(root,1));
	int curLevel = 1;
	cout<<"��"<<curLevel<<"�� :" <<endl; 
	while(!q.empty()){
		pair<ASTnode*,int> frontPair = q.front();
		q.pop();
		if(frontPair.second > curLevel){
			cout<<endl;
			curLevel++;
			cout<<"��"<<curLevel<<"�� :" <<endl; 
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

//��ǰ���� 
void processExpression(TreeNode* exp, ASTnode* father);
void processAssign(TreeNode* assign, ASTnode* father);
void processAddV(TreeNode* addv,ASTnode* father);
void processVariables(TreeNode* leftcommon,ASTnode* father);
void processElse(TreeNode* elsepart, ASTnode* father);
void processIf(TreeNode* ifSentence, ASTnode* father);
void processSentence(TreeNode* sentence,ASTnode* father);
ASTnode* processProgram(TreeNode* program,ASTnode* father);

void processExpression(TreeNode* exp, ASTnode* father){
	//<���ʽ> -> <һԪ�����><��>
    //<���ʽ> -> <��><��ѡ��Ԫ���㲿��>
    
	if(exp->sons[0]->number == 109 ){
		//һԪ�������ͷ(��ʵ���ǣ�) 
		ASTnode* notSymble = new ASTnode();
		notSymble->kind = NOT;
		notSymble->sons.clear();
		notSymble->tokenIndex = exp->sons[0]->sons[0]->tokenIndex;
		
		//��(���� or ����) 
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
		//����ͷ
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
		 	//�����ѡ��Ԫ���������Ϊ��
			father->sons.push_back(num);
		 }
		 else{
		 	//�ж�Ԫ�����
		 	//������һ��num�ڵ� 
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
			
			//����operand�ڵ�
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

void processAddV(TreeNode* addv,ASTnode* father)
{
	// <�ӱ���> -> <,><����><�ӱ���>
    // <�ӱ���> -> ��
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
	//�ҳ����ж���ı������ӽ�father
	// <��������ʽ> -> <int> <����> <�ӱ���>
    ASTnode* v = new ASTnode();
    v->kind = VARIABLE;
    v->tokenIndex = leftcommon->sons[1]->tokenIndex;
    v->tokenString = leftcommon->sons[1]->tokenString;
    father->sons.push_back(v); 
    processAddV(leftcommon->sons[2],father);
}

void processElse(TreeNode* elsepart, ASTnode* father){
	//<else����> -> <else> <{> <����> <{> <;>
	//<else����> -> ��
	if(elsepart->sons.size() != 0){
		processProgram(elsepart->sons[2],father);
	}
}

void processIf(TreeNode* ifSentence, ASTnode* father){
	//��if��������push��father��
	// <�������> -> <if> <(> <���ʽ> <)> <{> <����> <}> <;> <else����>
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
	//<������> -> <put> <(> <����> <�ӱ���> <)> <;>
	ASTnode* v = new ASTnode();
	v->kind = VARIABLE;
	v->tokenIndex = putSentence->sons[2]->tokenIndex;
	v->tokenString = putSentence->sons[2]->tokenString;
	father->sons.push_back(v);
	processAddV(putSentence->sons[3],father);
}

void processGet(TreeNode* getSentence, ASTnode* father){
	//<�������> -> <get> <(> <����> <�ӱ���> <)> <;>
	ASTnode* v = new ASTnode();
	v->kind = VARIABLE;
	v->tokenIndex = getSentence->sons[2]->tokenIndex;
	v->tokenString = getSentence->sons[2]->tokenString;
	father->sons.push_back(v);
	processAddV(getSentence->sons[3],father);
}

void processWhile(TreeNode* whileSentence, ASTnode* father){
	//<ѭ�����> -> <while> <(> <���ʽ> <)> <{> <����> <}> <;>
	processExpression(whileSentence->sons[2],father);
	ASTnode* mainbody = new ASTnode();
	mainbody->kind = WHILEPART;
	father->sons.push_back(mainbody);
	processProgram(whileSentence->sons[5],mainbody);
}

//process SENTENCE
void processSentence(TreeNode* sentence,ASTnode* father){
	//�����parse tree�ľ��ӣ�Ȼ��Ѿ����µľ�����Ӹ�father
	int num = sentence->sons[0]->number; //����һ�����ӵ�num 
	if(num == 104){
		//��ֵ���
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
		//��������ʽ��ͷ������Ƚ��鷳
		//�����������
		//<����> -> <��������ʽ><����ɸ�ֵ ><;>
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
			//�������ɸ�ֵ��Ϊ��
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
		//�����Ƶ�������ֻ���� ���� ���� �����ǿվͲ�����
		if(ele->number == 100) //����
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
	
	//֮ǰ���﷨�����sons�ǵ��Ŵ�ģ��������������������
	reverseParseTree(grammerResult.grammerTree);
	
	init_kinds();
	ASTnode* ASTroot = processProgram(grammerResult.grammerTree,nullptr);
	cout<<endl<<endl;
	cout<<"**************�����ӡAST******************"<<endl; 
	printAST(ASTroot);
	
	return 0;
}

