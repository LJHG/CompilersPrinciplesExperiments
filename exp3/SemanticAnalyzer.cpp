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

//事先声明 
errorInfo analyzeProgram(ASTnode* root, int curScope);
errorInfo analyzeAssign(ASTnode* root, int curScope);
errorInfo getNodeVal(ASTnode* root); 
errorInfo analyzeInt(ASTnode* root,int curScope);
errorInfo analyzeBool(ASTnode* root,int curScope);
errorInfo analyzePut(ASTnode* root,int curScope);
errorInfo analyzeGet(ASTnode* root,int curScope);
errorInfo analyzeIf(ASTnode* root,int curScope);
errorInfo analyzeWhile(ASTnode* root,int curScope);


errorInfo checkVariable(ASTnode* v){
	//check 一个变量是否存在未定义或是未赋值的错误
	errorInfo e;
	e.hasError = 0;
	string name = v->tokenString;
	if(!hasVariable(name)){
		e.hasError = 1;
		e.errorDetail = "变量"+name+"未定义";
		e.errorIndex = v->tokenIndex;
		return e;
	}
	if(hasNoValue(name)){
		e.hasError = 1;
		e.errorDetail = "变量"+name+"未赋值";
		e.errorIndex = v->tokenIndex;
		return e;
	}
	return e;
	
}

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
			e.errorDetail = "变量未定义";
			e.errorIndex = root->tokenIndex;
			return e;
		}
		if(hasVariable(root->tokenString) && hasNoValue(root->tokenString)){
			e.hasError = 1;
			e.errorDetail = "变量未赋值";
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
	//首先看变量是否被定义了
	errorInfo e;
	e.hasError = 0;
	if(!hasVariable(root->sons[0]->tokenString)){
		e.hasError = 1;
		e.errorDetail = "变量"+root->sons[0]->tokenString+"未定义";
		e.errorIndex = root->sons[0]->tokenIndex;
		return e;
	}
	//再看是不是布尔的非法赋值
	if(root->sons[0]->kind == BOOL){
		errorInfo valInfo = getNodeVal(root->sons[1]);
		if(valInfo.hasError) return valInfo;
		if( !(valInfo.val==0 ||  valInfo.val==1) ){
			e.hasError = 1;
			e.errorDetail = "布尔变量的非法赋值!";
			e.errorIndex = root->sons[0]->tokenIndex;
			return e;
		}
 	}
	 
	errorInfo ee = getNodeVal(root->sons[1]);
	if(ee.hasError) return ee;
	
	//如果都没问题，entertable
	enterTable(-1,root->sons[0]->tokenString,ee.val,curScope);	
	return e;
	 
}

errorInfo analyzeInt(ASTnode* root,int curScope){
	//主要看是不是发生了重定义
	errorInfo e;
	e.hasError = 0;
	int val = -1;
	if(root->sons.size() > 1)
	{
		//说明是定义赋值类型
		errorInfo e2 = getNodeVal(root->sons[1]);
		if(e2.hasError) return e2;
		val = e2.val;
	}
	for(auto x:root->sons[0]->sons){
		if(hasVariable(x->tokenString)){
			e.hasError = 1;
			e.errorIndex = x->tokenIndex;
			e.errorDetail = "变量"+x->tokenString+"发生了重定义";
			return e; 
		}else{
			enterTable(1,x->tokenString,val,curScope);
		}
	}
	 
	return e;
}

errorInfo analyzeBool(ASTnode* root,int curScope){
	//主要看是不是发生了重定义
	errorInfo e;
	e.hasError = 0;
	int val = -1;
	if(root->sons.size() > 1)
	{
		//说明是定义赋值类型
		val = getNodeVal(root->sons[1]).val;
		if( !(val == 0 || val == 1)){
			e.errorDetail = "非法的布尔变量赋值";
			e.errorIndex = root->sons[0]->sons[0]->tokenIndex;
			e.hasError = 1;
			return e;
		}
	}
	for(auto x:root->sons[0]->sons){
		if(hasVariable(x->tokenString)){
			e.hasError = 1;
			e.errorIndex = x->tokenIndex;
			e.errorDetail = "变量"+x->tokenString+"发生了重定义";
			return e; 
		}else{
			enterTable(1,x->tokenString,val,curScope);
		}
	}
	return e;
}

errorInfo analyzePut(ASTnode* root,int curScope){
	//主要看变量是否定义以及是否有值 
	errorInfo e;
	e.hasError = 0;
	for(auto x:root->sons){
		if(!hasVariable(x->tokenString)){
			e.hasError = 1;
			e.errorDetail = "变量"+x->tokenString+"未定义"; 
			e.errorIndex = x->tokenIndex;
			return e;
		}
		if(hasNoValue(x->tokenString)){
			e.hasError = 1;
			e.errorDetail = "变量"+x->tokenString+"未赋值"; 
			e.errorIndex = x->tokenIndex;
			return e;
		}
	} 
} 


errorInfo analyzeGet(ASTnode* root,int curScope){
	//主要看变量是否定义以及是否有值 
	errorInfo e;
	e.hasError = 0;
	for(auto x:root->sons){
		if(!hasVariable(x->tokenString)){
			e.hasError = 1;
			e.errorDetail = "变量"+x->tokenString+"未定义"; 
			e.errorIndex = x->tokenIndex;
			return e;
		}
	}
}

errorInfo analyzeIf(ASTnode* root,int curScope){
	errorInfo e;
	e.hasError = 0;
	for(auto x : root->sons[0]->sons){
		if(x->kind == VARIABLE)
		{
			errorInfo temp = checkVariable(x);
			if(temp.hasError) return temp;
		}
		
	}
	errorInfo ifParts = analyzeProgram(root->sons[1],curScope+1);
	if(ifParts.hasError) return ifParts;
	if(root->sons.size() == 3){
		errorInfo elseParts = analyzeProgram(root->sons[2],curScope+1);
		if(elseParts.hasError) return elseParts;
	}
	return e;
}


errorInfo analyzeWhile(ASTnode* root,int curScope){
	errorInfo e;
	e.hasError = 0;
	for(auto x : root->sons[0]->sons){
		if(x->kind == VARIABLE)
		{
			errorInfo temp = checkVariable(x);
			if(temp.hasError) return temp;
		}
		
	}
	errorInfo bodyParts = analyzeProgram(root->sons[1],curScope+1);
	if(bodyParts.hasError) return bodyParts;
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
			case INT:
				e = analyzeInt(son,curScope);
				break;
			case BOOL:
				e = analyzeBool(son,curScope);
				break;
			case PUT:
				e = analyzePut(son,curScope);
				break;
			case GET:
				e = analyzeGet(son,curScope);
				break;
			case IF:
				e = analyzeIf(son,curScope);
				break;
			case WHILE:
				e = analyzeWhile(son,curScope);
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














