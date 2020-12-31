#include<iostream>
#include "symbolTable.cpp"
#include "generateAST.cpp"
using namespace std;

struct errorInfo{
	string errorDetail;
	int errorIndex;
	bool hasError;
	int val; //for getNodeVal function
};

//�������� 
errorInfo analyzeProgram(ASTnode* root, int curScope);
errorInfo analyzeAssign(ASTnode* root, int curScope);
errorInfo getNodeVal(ASTnode* root); 
errorInfo analyzeInt(ASTnode* root,int curScope);
errorInfo analyzeBool(ASTnode* root,int curScope);

//enum quadType{
//	QASSIGNV,QASSIGNC,QADD,QMINUS,QGOTO,QIFEQUAL,QIFNOTEQUAL,QIFGT,QIFGTOREQUAL,QIFLESS,
//	QIFLESSOREQUAL,QFUNC
//};
//
//struct Quad{
//	int type;
//	int op1;
//	int op2;
//	int ans;
//};

errorInfo getNodeVal(ASTnode* root){
	errorInfo e;
	e.hasError = 0;
	if(root->kind == NUMBER){
		e.val = atoi(root->tokenString.c_str());
		return e;
	}
	if(root->kind == VARIABLE){
		if( !hasVariable(root->tokenString)){
			e.hasError = 1;
			e.errorDetail = "����δ����";
			e.errorIndex = root->tokenIndex;
			return e;
		}
		if(hasVariable(root->tokenString) && hasNoValue(root->tokenString)){
			e.hasError = 1;
			e.errorDetail = "����δ��ֵ";
			e.errorIndex = root->tokenIndex;
			return e;
		}
		e.val = getVal(root->tokenString).val;
		return e;
	}
	if(root->kind == NOT){
		errorInfo e = getNodeVal(root->sons[0]);
		e.val = !e.val;
		return e;
	}
	errorInfo e1 = getNodeVal(root->sons[0]);
	errorInfo e2 = getNodeVal(root->sons[1]);
	e.hasError = e1.hasError || e2.hasError;
	if(e1.hasError) return e1;
	if(e2.hasError) return e2;
	switch(root->kind){
		case ADD:
			e.val = e1.val + e2.val;
			break;
		case SUB:
			e.val = e1.val - e2.val;
			break;
		case BITAND:
			e.val = e1.val & e2.val;
			break;
		case BITOR:
			e.val = e1.val | e2.val;
			break;
		case EQUAL:
			e.val = (e1.val == e2.val);
			break;
		case NEQUAL:
			e.val = !(e1.val == e2.val);
			break;
		case GT:
			e.val = (e1.val > e2.val);
			break;
		case LESS:
			e.val = (e1.val < e2.val);
			break;
		case GTOEQUAL:
			e.val = (e1.val > e2.val) || (e1.val == e2.val);
			break;
		case LESSOREQUAL:
			e.val = (e1.val < e2.val) || (e1.val == e2.val);
			break;
		case AND:
			e.val = e1.val && e2.val;
			break;
		case OR:
			e.val = e1.val || e2.val;
			break;
		default:
			cout<<"unexpected"<<endl;
	}
	return e;
}

errorInfo analyzeAssign(ASTnode* root, int curScope){
	//���ȿ������Ƿ񱻶�����
	errorInfo e;
	e.hasError = 0;
	if(!hasVariable(root->sons[0]->tokenString)){
		e.hasError = 1;
		e.errorDetail = "����"+root->sons[0]->tokenString+"δ����";
		e.errorIndex = root->sons[0]->tokenIndex;
		return e;
	}
	//�ٿ��ǲ��ǲ����ķǷ���ֵ
	if(root->sons[0]->kind == BOOL){
		errorInfo valInfo = getNodeVal(root->sons[1]);
		if(valInfo.hasError) return valInfo;
		if( !(valInfo.val==0 ||  valInfo.val==1) ){
			e.hasError = 1;
			e.errorDetail = "���������ķǷ���ֵ!";
			e.errorIndex = root->sons[0]->tokenIndex;
			return e;
		}
 	}
	 
	errorInfo ee = getNodeVal(root->sons[1]);
	if(ee.hasError) return ee;
	
	//�����û���⣬entertable
	enterTable(-1,root->sons[0]->tokenString,ee.val,curScope);	
	return e;
	 
}

errorInfo analyzeInt(ASTnode* root,int curScope){
	//��Ҫ���ǲ��Ƿ������ض���
	errorInfo e;
	e.hasError = 0;
	int val = -1;
	if(root->sons.size() > 1)
	{
		//˵���Ƕ��帳ֵ����
		errorInfo e2 = getNodeVal(root->sons[1]);
		if(e2.hasError) return e2;
		val = e2.val;
	}
	for(auto x:root->sons[0]->sons){
		if(hasVariable(x->tokenString)){
			e.hasError = 1;
			e.errorIndex = x->tokenIndex;
			e.errorDetail = "����"+x->tokenString+"�������ض���";
			return e; 
		}else{
			enterTable(1,x->tokenString,val,curScope);
		}
	}
	 
	return e;
}

errorInfo analyzeBool(ASTnode* root,int curScope){
	//��Ҫ���ǲ��Ƿ������ض���
	errorInfo e;
	e.hasError = 0;
	int val = -1;
	if(root->sons.size() > 1)
	{
		//˵���Ƕ��帳ֵ����
		val = getNodeVal(root->sons[1]).val;
		if( !(val == 0 || val == 1)){
			e.errorDetail = "�Ƿ��Ĳ���������ֵ";
			e.errorIndex = root->sons[0]->sons[0]->tokenIndex;
			e.hasError = 1;
			return e;
		}
	}
	for(auto x:root->sons[0]->sons){
		if(hasVariable(x->tokenString)){
			e.hasError = 1;
			e.errorIndex = x->tokenIndex;
			e.errorDetail = "����"+x->tokenString+"�������ض���";
			return e; 
		}else{
			enterTable(1,x->tokenString,val,curScope);
		}
	}
	return e;
}


errorInfo analyzeProgram(ASTnode* root, int curScope){
	errorInfo e;
	e.hasError = 0;
	for(auto son:root->sons){
		switch(son->kind){
			case ASSIGN:
				e = analyzeAssign(son,curScope);
				break;
				//if(e.hasError = 1) return e;
			case INT:
				e = analyzeInt(son,curScope);
				break;
			case BOOL:
				e = analyzeBool(son,curScope);
				break;
			default:
				cout<<"unexpected1"<<endl;
		}
	}
	return e;
}


errorInfo SemanticAnalyze(ASTnode* root){
	int curScope = 1; 
	return analyzeProgram(root, 1);
}














