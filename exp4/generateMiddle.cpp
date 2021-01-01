#include "../exp3/SemanticAnalyzer.cpp"
#include<string>

enum quadType{
	QASSIGNV,QASSIGNC,QADD,QSUB,QGOTO,QIFEQUAL,QIFNOTEQUAL,QIFGT,QIFGTOREQUAL,QIFLESS,
	QIFLESSOREQUAL,QFUNC,
	QBITAND,QBITOR,QAND,QOR,QPUT,QGET,
	LABEL//LABEL表示声明变量 
};




struct Quad{
	int type;
	string op1; //variable1 
	string op2; //variable2
	string ans; // variable target
	//编写构造函数 
	Quad(int type,string op1,string op2,string ans){
		this->type = type;
		this->op1 = op1;
		this->op2 = op2;
		this->ans = ans;
	}
};

int tempVarCnt;
map<int,string> string_quad; 


void generateMiddleInit(){
	tempVarCnt = 0;
	
	
	string_quad[QASSIGNV] = "li";
	string_quad[QASSIGNC] = "move";
	string_quad[QADD] = "add";
	string_quad[QSUB] = "sub";
	string_quad[QGOTO] = "goto";
	string_quad[QIFEQUAL] = "beq";
	string_quad[QIFNOTEQUAL] = "bne";
	string_quad[QIFGT] = "bgt";
	string_quad[QIFGTOREQUAL] = "bge";
	string_quad[QIFLESS] = "ble";
	string_quad[QIFLESSOREQUAL]= "blt";
	string_quad[QFUNC] = "CALL";
	string_quad[QBITAND] = "and";
	string_quad[QBITOR] = "or";
	string_quad[QPUT] = "jal put";
	string_quad[QGET] = "jal get"; 
	string_quad[LABEL] = "label";
}

//声明
vector<Quad*> processInt_quad(ASTnode* root);
vector<Quad*> processExp_quad(ASTnode* root);
vector<Quad*> processAssign_quad(ASTnode* root);



void showQuadList(vector<Quad*> list){
	for(auto x:list){
		cout<<string_quad[x->type]<<" "<<x->ans<<" "<<x->op1<<" "<<x->op2<<endl;
	}
}


vector<Quad*> processExp_quad(ASTnode* root){
	vector<Quad*> res;
	res.clear();
	if(root == nullptr) return res;
	//为了统一性，就算root是一个常量，我们也先把它给一个变量
	//如果是变量，那么把它赋给它自己 
	Quad* label = nullptr;
	Quad* item = nullptr;
	
	switch(root->kind){
		case NUMBER:
			label = new Quad(LABEL,"_","_","t"+to_string(tempVarCnt));
			item = new Quad(QASSIGNC,root->tokenString,"_","t"+to_string(tempVarCnt));
			tempVarCnt++;
			res.push_back(label);
			res.push_back(item);
			break;
		case VARIABLE:
			item = new Quad(QASSIGNV,root->tokenString,"_",root->tokenString);
			res.push_back(item);
			break;
		case ADD:
			label = new Quad(LABEL,"_","_","t"+to_string(tempVarCnt));
			item = new Quad(QADD,root->sons[0]->tokenString,root->sons[1]->tokenString,"t"+to_string(tempVarCnt));
			tempVarCnt++;
			res.push_back(label);
			res.push_back(item);
			break;
		case SUB:
			label = new Quad(LABEL,"_","_","t"+to_string(tempVarCnt));
			item = new Quad(QSUB,root->sons[0]->tokenString,root->sons[1]->tokenString,"t"+to_string(tempVarCnt));
			tempVarCnt++;
			res.push_back(label);
			res.push_back(item);
			break;
		case QBITAND:
			label = new Quad(LABEL,"_","_","t"+to_string(tempVarCnt));
			item = new Quad(QBITAND,root->sons[0]->tokenString,root->sons[1]->tokenString,"t"+to_string(tempVarCnt));
			tempVarCnt++;
			res.push_back(label);
			res.push_back(item);
			break;
		case QBITOR:
			label = new Quad(LABEL,"_","_","t"+to_string(tempVarCnt));
			item = new Quad(QBITOR,root->sons[0]->tokenString,root->sons[1]->tokenString,"t"+to_string(tempVarCnt));
			tempVarCnt++;
			res.push_back(label);
			res.push_back(item);
			break;
		case QAND:
			label = new Quad(LABEL,"_","_","t"+to_string(tempVarCnt));
			item = new Quad(QAND,root->sons[0]->tokenString,root->sons[1]->tokenString,"t"+to_string(tempVarCnt));
			tempVarCnt++;
			res.push_back(label);
			res.push_back(item);
			break;
		case QOR:
			label = new Quad(LABEL,"_","_","t"+to_string(tempVarCnt));
			item = new Quad(QOR,root->sons[0]->tokenString,root->sons[1]->tokenString,"t"+to_string(tempVarCnt));
			tempVarCnt++;
			res.push_back(label);
			res.push_back(item);
			break;
		default:
			cout<<"unexpected"<<endl;
	}
	return res;
} 

vector<Quad*> processInt_quad(ASTnode* root){
	vector<Quad*> rightPart;rightPart.clear();
	if(root->sons.size() == 2)
	{
		//说明是定义赋值性语句
		rightPart = processExp_quad(root->sons[1]);
		//把rightpart里面的最后一个的临时变量赋给变量
		int len = rightPart.size(); //好像只会是1 
		string tempv = rightPart[len-1]->ans;
		for(auto v:root->sons[0]->sons){
			rightPart.push_back(new Quad(LABEL,"_","_",v->tokenString));
			rightPart.push_back(new Quad(QASSIGNV,tempv,"_",v->tokenString));
		} 
	}
	else{
		for(auto v:root->sons[0]->sons){
			rightPart.push_back(new Quad(LABEL,"_","_",v->tokenString));
		}
	}
	return rightPart;
}

vector<Quad*> processAssign_quad(ASTnode* root){
	vector<Quad*> rightPart = processExp_quad(root->sons[1]);
	//把rightpart里面的最后一个的临时变量赋给变量
	//既然是执行assign,那么就不用再label了
	rightPart.push_back(new Quad(QASSIGNV,rightPart[0]->ans,"_",root->sons[0]->tokenString));
	return rightPart; 
	
}
	


vector<Quad*> generateMiddle(ASTnode* root){
	generateMiddleInit();
	vector<Quad*> res;
	vector<Quad*> temp;
	tempVarCnt = 0;
	for(auto son : root->sons){
		switch(son->kind){
			case INT:
				temp = processInt_quad(son);
				for(auto x:temp) res.push_back(x);
				break;
			case ASSIGN:
				temp = processAssign_quad(son);
				for(auto x:temp) res.push_back(x);
				break;
			case PUT:
				res.push_back(new Quad(QPUT,"_","_","_"));
				break;
			case GET:
				res.push_back(new Quad(QGET,"_","_","_"));
				break;
			default:
				cout<<"unexpected!"<<endl;
		}
	}
	return res;
	
} 

