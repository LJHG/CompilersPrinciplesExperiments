#include<iostream>
#include<stack>
#include<map>
#include<string>
using namespace std;

struct tableItem{
	int type; //1 for int, 0 for bool, -1 for only assign value
	string name;
	stack<pair<int,int>> vals; //<val,scope>
	int dscope; //变量被定义时候的scope 
};

struct tableInfo{
	bool hasError;
	string errorInfo;
	int val;
};

map<string,tableItem*> symbolTable;

tableInfo enterTable(int type,string name,int val,int scope){
	tableInfo info;
	info.hasError = 0;
	//定义形插表 
	if(type == 0 || type == 1)
	{
		if(symbolTable.count(name) == 0){
			tableItem* item = new tableItem();
			item->name = name; 
			item->type = type;
			if(val != -1) //如果val不为-1，则需要赋值 
				item->vals.push(make_pair(val,scope));
			item->dscope = scope;
			symbolTable[name] = item;
		}
		else{
			info.hasError = 1;
			info.errorInfo = "发生了重定义!";
		}
	}
	//重新赋值 
	else{
		if(symbolTable.count(name) == 0){
			info.hasError = 1;
			info.errorInfo = "变量未定义!";
		}
		else if(symbolTable[name]->type == 0 && !(val ==0 || val==1)){
			info.hasError = 1;
			info.errorInfo = "非法的布尔变量赋值!";
		}
		else{
			if(symbolTable[name]->vals.size() >0 && symbolTable[name]->vals.top().second == scope){
				//如果在同一作用域就是重新赋值 
				symbolTable[name]->vals.pop();
			}
			symbolTable[name]->vals.push(make_pair(val,scope));
		}
		
	}
	
	return info;	
}

tableInfo getVal(string name){
	tableInfo info;
	info.hasError = 0;
	info.val = -999999;
	if(symbolTable.count(name) == 0){
		info.hasError = 1;
		info.errorInfo = "变量未定义!";
	}else{
		if(symbolTable[name]->vals.size() == 0){
			info.hasError = 1;
			info.errorInfo = "变量未赋值!";
		}
		info.val = symbolTable[name]->vals.top().first;
	}
	return info;
}

void deleteVal(string name,int scope){
	if(scope == symbolTable[name]->dscope && symbolTable[name]->vals.size() == 1){
		//如果当前scope就是dscope，而且里面只有一个元素
		//直接删掉
		symbolTable.erase(name);
		cout<<"删完了"<<endl;
	}
	else{
		symbolTable[name]->vals.pop();
	}	
}

bool hasVariable(string name){
	if(symbolTable.count(name) == 0) return false;
	return true;
}

bool hasNoValue(string name){
	if(symbolTable[name]->vals.size() == 0) return true;
	return false;
}





