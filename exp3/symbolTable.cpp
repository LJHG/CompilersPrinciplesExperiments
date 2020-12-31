#include<iostream>
#include<stack>
#include<map>
#include<string>
using namespace std;

struct tableItem{
	int type; //1 for int, 0 for bool, -1 for only assign value
	string name;
	stack<pair<int,int>> vals; //<val,scope>
	int dscope; //����������ʱ���scope 
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
	//�����β�� 
	if(type == 0 || type == 1)
	{
		if(symbolTable.count(name) == 0){
			tableItem* item = new tableItem();
			item->name = name; 
			item->type = type;
			if(val != -1) //���val��Ϊ-1������Ҫ��ֵ 
				item->vals.push(make_pair(val,scope));
			item->dscope = scope;
			symbolTable[name] = item;
		}
		else{
			info.hasError = 1;
			info.errorInfo = "�������ض���!";
		}
	}
	//���¸�ֵ 
	else{
		if(symbolTable.count(name) == 0){
			info.hasError = 1;
			info.errorInfo = "����δ����!";
		}
		else if(symbolTable[name]->type == 0 && !(val ==0 || val==1)){
			info.hasError = 1;
			info.errorInfo = "�Ƿ��Ĳ���������ֵ!";
		}
		else{
			if(symbolTable[name]->vals.size() >0 && symbolTable[name]->vals.top().second == scope){
				//�����ͬһ������������¸�ֵ 
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
		info.errorInfo = "����δ����!";
	}else{
		if(symbolTable[name]->vals.size() == 0){
			info.hasError = 1;
			info.errorInfo = "����δ��ֵ!";
		}
		info.val = symbolTable[name]->vals.top().first;
	}
	return info;
}

void deleteVal(string name,int scope){
	if(scope == symbolTable[name]->dscope && symbolTable[name]->vals.size() == 1){
		//�����ǰscope����dscope����������ֻ��һ��Ԫ��
		//ֱ��ɾ��
		symbolTable.erase(name);
		cout<<"ɾ����"<<endl;
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





