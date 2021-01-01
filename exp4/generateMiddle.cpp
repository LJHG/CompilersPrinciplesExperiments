#include "../exp3/SemanticAnalyzer.cpp"
#include<string>
#include<queue>

enum quadType{
	QASSIGNV,QASSIGNC,QADD,QSUB,QGOTO,QIFEQUAL,QIFNOTEQUAL,QIFGT,QIFGTOREQUAL,QIFLESS,
	QIFLESSOREQUAL,QFUNC,
	QBITAND,QBITOR,QAND,QOR,QPUT,QGET,
	LABEL,//LABEL表示声明变量 
	QEND//自己加一个end 
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

struct goto_pair{
	int ifIndex; //if语句的位置（用来补上那个？） 
	int elseIndex; // else语句的位置 （用来在后面mainbody goback） 
	bool isWhile;
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
	string_quad[QEND] = "end";
}

//声明
vector<Quad*> processInt_quad(ASTnode* root);
vector<Quad*> processExp_quad(ASTnode* root);
vector<Quad*> processAssign_quad(ASTnode* root);
void processIf_quad(ASTnode* root,queue<goto_pair>& goto_indexs,queue<ASTnode*>& tobe_process,vector<Quad*>& res);
void processProgram_quad(ASTnode* root,queue<goto_pair>& goto_indexs,queue<ASTnode*>& tobe_process,vector<Quad*>& res);
void processWhile_quad(ASTnode* root,queue<goto_pair>& goto_indexs,queue<ASTnode*>& tobe_process,vector<Quad*>& res);

void showQuadList(vector<Quad*> list){
	int len = list.size();
	for(int i=0;i<len;i++){
		Quad* x = list[i];
		cout<<" 位置:"<<i<<"   "<<string_quad[x->type]<<" "<<x->ans<<" "<<x->op1<<" "<<x->op2<<endl;
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


void processIf_quad(ASTnode* root,queue<goto_pair>& goto_indexs,queue<ASTnode*>& tobe_process,vector<Quad*>& res){
	//为了方便获取index，把res当成参数传进来了
	goto_pair pp;pp.ifIndex = -1;pp.elseIndex = -1;pp.isWhile = false;
	switch(root->sons[0]->kind){
		case EQUAL:
			res.push_back(new Quad(QIFEQUAL,root->sons[0]->sons[0]->tokenString,root->sons[0]->sons[1]->tokenString,"?"));
			pp.ifIndex = res.size() - 1;
			break;
		case NEQUAL:
			res.push_back(new Quad(QIFNOTEQUAL,root->sons[0]->sons[0]->tokenString,root->sons[0]->sons[1]->tokenString,"?"));
			pp.ifIndex = res.size() - 1;
			break;
		case GT:
			res.push_back(new Quad(QIFGT,root->sons[0]->sons[0]->tokenString,root->sons[0]->sons[1]->tokenString,"?"));
			pp.ifIndex = res.size() - 1;
			break;
		case GTOEQUAL:
			res.push_back(new Quad(QIFGTOREQUAL,root->sons[0]->sons[0]->tokenString,root->sons[0]->sons[1]->tokenString,"?"));
			pp.ifIndex = res.size() - 1;
			break;
		case LESS:
			res.push_back(new Quad(QIFLESS,root->sons[0]->sons[0]->tokenString,root->sons[0]->sons[1]->tokenString,"?"));
			pp.ifIndex = res.size() - 1;
			break;
		case LESSOREQUAL:
			res.push_back(new Quad(QIFLESSOREQUAL,root->sons[0]->sons[0]->tokenString,root->sons[0]->sons[1]->tokenString,"?"));
			pp.ifIndex = res.size() - 1;
			break;
		default:
			cout<<"unexpected"<<endl;
	}
	//把mainbody给tobeprocess
	tobe_process.push(root->sons[1]);
	//else部分直接就当成程序处理
	if(root->sons.size() == 3){
		processProgram_quad(root->sons[2],goto_indexs,tobe_process,res);
		pp.elseIndex = res.size()-1; //goto到else的下面一句, 这里记录else处理完的位置 
	}
	goto_indexs.push(pp);
}

void processProgram_quad(ASTnode* root,queue<goto_pair>& goto_indexs,queue<ASTnode*>& tobe_process,vector<Quad*>& res){
	vector<Quad*> temp;
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
			case IF:
				processIf_quad(son,goto_indexs,tobe_process,res);
				break;
			case WHILE:
				processWhile_quad(son,goto_indexs,tobe_process,res);
				break;
			default:
				cout<<"unexpected!"<<endl;
		}
	}
}

void processWhile_quad(ASTnode* root,queue<goto_pair>& goto_indexs,queue<ASTnode*>& tobe_process,vector<Quad*>& res){
	goto_pair pp;pp.ifIndex = -1;pp.elseIndex = -1; //其实while根本就用不到这个elseindex
	pp.isWhile = true;
	switch(root->sons[0]->kind){
		case EQUAL:
			res.push_back(new Quad(QIFEQUAL,root->sons[0]->sons[0]->tokenString,root->sons[0]->sons[1]->tokenString,"?"));
			pp.ifIndex = res.size() - 1;  
			break;
		case NEQUAL:
			res.push_back(new Quad(QIFNOTEQUAL,root->sons[0]->sons[0]->tokenString,root->sons[0]->sons[1]->tokenString,"?"));
			pp.ifIndex = res.size() - 1;
			break;
		case GT:
			res.push_back(new Quad(QIFGT,root->sons[0]->sons[0]->tokenString,root->sons[0]->sons[1]->tokenString,"?"));
			pp.ifIndex = res.size() - 1;
			break;
		case GTOEQUAL:
			res.push_back(new Quad(QIFGTOREQUAL,root->sons[0]->sons[0]->tokenString,root->sons[0]->sons[1]->tokenString,"?"));
			pp.ifIndex = res.size() - 1;
			break;
		case LESS:
			res.push_back(new Quad(QIFLESS,root->sons[0]->sons[0]->tokenString,root->sons[0]->sons[1]->tokenString,"?"));
			pp.ifIndex = res.size() - 1;
			break;
		case LESSOREQUAL:
			res.push_back(new Quad(QIFLESSOREQUAL,root->sons[0]->sons[0]->tokenString,root->sons[0]->sons[1]->tokenString,"?"));
			pp.ifIndex = res.size() - 1;
			break;
		default:
			cout<<"unexpected"<<endl;
	}
	//把mainbody给tobeprocess
	tobe_process.push(root->sons[1]);
	goto_indexs.push(pp);
}
	

vector<Quad*> generateMiddle(ASTnode* root){
	generateMiddleInit();
	vector<Quad*> res;
	
	queue<goto_pair> goto_indexs; //当有if或者while语句 "加入到res里面时" 往里面添加一个index
	queue<ASTnode*> tobe_process; //先把if的body 或者 else的body往里面放一放 
	
	tempVarCnt = 0;
	
	processProgram_quad(root,goto_indexs,tobe_process,res);
	
	
	cout<<goto_indexs.size()<<" "<<tobe_process.size()<<endl;
	
	res.push_back(new Quad(QEND,"","",""));
	
	//所有全部弄完了之后，开始弄
	while(!tobe_process.empty()){
		ASTnode* r = tobe_process.front();
		tobe_process.pop();
		if(goto_indexs.size() == 0) cout<<"unexpected2222222222222"<<endl;
		goto_pair pp = goto_indexs.front();
		goto_indexs.pop();
		
		//先去填那个？
		res[pp.ifIndex]->ans = to_string(res.size()); //这里我假设if的body不是空的
		//然后去process if的body部分
		processProgram_quad(r,goto_indexs,tobe_process,res);
		//加一句goto
		if(pp.elseIndex != -1){
			res.push_back(new Quad(QGOTO,"_","_",to_string(pp.elseIndex+1)));
		}else{
			if(pp.isWhile){
				res.push_back(new Quad(QGOTO,"_","_",to_string(pp.ifIndex))); //while跳回自己 
			}
			else{
				res.push_back(new Quad(QGOTO,"_","_",to_string(pp.ifIndex+1)));
			}
			
		}
	}
	return res;
	
} 

