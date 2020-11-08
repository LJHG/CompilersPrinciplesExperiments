#include<iostream>
#include<string>
#include<vector> 
#include<map>
#include<set>
#include<unordered_set>
using namespace std;

struct production{
	int left; //左部是一个编号，对应一个非终结符 
	vector<int> right; //右部是一个编号序列 
	production(int left,vector<int> right){
		this->left = left;
		this->right = right;
	} 
};

vector<production> productions; //文法 
unordered_set<int> nonTerminal;  //非终结符的集合
unordered_set<int> terminal;    //终结符的集合
map<string,int> table;   //符号表  <符号,编号> 
map<int,string> id_string_table; //符号翻译表 


//没什么x用的函数，可以把两行变一行 
void addProduction(int left,vector<int> right){
	production p(left,right);
	productions.push_back(p);
}
 
void initTable(){
	//现在根据一个特定的文法来初始化table 和 TNT试试
	table["X"] = 1; id_string_table[1] = "X";
	table["Y"] = 2; id_string_table[2] = "Y";
	table["Z"] = 3; id_string_table[3] = "Z";
	table["a"] = 4; id_string_table[4] = "a";
	table["c"] = 5; id_string_table[5] = "c";
	table["d"] = 6; id_string_table[6] = "d";
}

void initTNT(){
	//初始化nonterminal和terminal 
	nonTerminal.insert(1);nonTerminal.insert(2);nonTerminal.insert(3);
	terminal.insert(4);terminal.insert(5);terminal.insert(6);	
}

void initProductions(){
	int left;
	vector<int> right;
	//纯手打文法，没办法啊 
	///////////////////
	left = 3;
	right.clear(); right.push_back(6);
	addProduction(left,right);
	///////////////////
	left = 3;
	right.clear();right.push_back(1);right.push_back(2);right.push_back(3);
	addProduction(left,right);
	///////////////////
	left = 2;
	right.clear();right.push_back(5);
	addProduction(left,right);
	///////////////////
	left = 2;
	right.clear(); //右部为空，什么都不push 
	addProduction(left,right);
	///////////////////
	left = 1;
	right.clear();right.push_back(2);
	addProduction(left,right);
	//////////////////
	left = 1;
	right.clear();right.push_back(4);
	addProduction(left,right);
	///////////////////////// 
} 

void printProductions(){
	//用编号打印一遍 
	for(auto production:productions){
		cout<<production.left<<" -> ";
		for(auto icon:production.right)
		{
			cout<<icon;
		}
		cout<<endl;
	}
	cout<<"*********************"<<endl;
	//用字符串的形式打印一遍
	for(auto production:productions){
		cout<<id_string_table[production.left]<<" -> ";
		for(auto icon:production.right)
		{
			cout<<id_string_table[icon];
		}
		cout<<endl;
	}
	cout<<endl;
}

//获得非终结符的First集 
map<int,vector<int> > getFirst(){
	
	
}

//获得非终结符的Follow集 
map<int,vector<int> > getFollow(){
	
}

//获得所有的NULLABLE的非终结符 
unordered_set<int> getNullable(){
	unordered_set<int> nullable;
	while(1){
		int len1 = nullable.size();
		for(auto production:productions){
			int left = production.left;
			vector<int> right = production.right;
			if(right.size() == 0){
				nullable.insert(left);
			}
			int allNullable = 1; 
			for(auto icon:right){
				if(nullable.count(icon) == 0)
				{
					allNullable = 0;
					break;
				}
			}
			if(allNullable){
				nullable.insert(left);
			}
		}
		int len2 = nullable.size();
		if(len2 == len1){
			break;
		}
	}
	return nullable;
}




int main(){
	
	vector<int> tokens; int tokenPos = 0;
	//init
	initTable();
	initTNT();
	initProductions();
	//
	printProductions();
	//测试Nullable集合
	cout<<"nullable集合如下所示:"<<endl;
	unordered_set<int> nullable =getNullable();
	for(auto x:nullable){
		cout<<id_string_table[x]<<" ";
	}
	cout<<endl;
	
	
	return 0;
}
