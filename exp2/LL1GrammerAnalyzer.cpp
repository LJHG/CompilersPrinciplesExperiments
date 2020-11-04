#include<iostream>
#include<string>
#include<vector> 
#include<map>
using namespace std;

struct production{
	string left; //左部 
	string right; //右部
	production(string left,string right){
		this->left = left;
		this->right = right;
	} 
};


//获得非终结符的First集 
map<string,int> getFirst(vector<production> productions){
	
}

//获得非终结符的Follow集 
map<string,int> getFollow(vector<production> productions){
	
}

//获得所有的NULLABLE的非终结符 
set<string> getNullable(vector<production> productions){
	set<string> nullable;
	while(1){
		int len1 = set.size();
		for(auto production:productions){
			string left = production.left;
			string right = productoin.right;
			if(right == "@"){
				nullable.insert(left);
			}
			
		}
	}
	
}

//没什么x用的一个函数，可以少写一行 
void addProduction(string left,string right,vector<production>& productions)
{
	production p(left,right); productions.push_back(p); 
} 


int main(){
	
	vector<int> tokens; int tokenPos = 0;
	vector<production> productions;
	//构建文法 纯手工输入***************************************** 
	addProduction("Z","d",productions);
	addProduction("Z","XYZ",productions);
	addProduction("Y","c",productions);
	addProduction("Z","@",productions); //这里随便写的哈，@是空产生式 
	addProduction("X","Y",productions);
	addProduction("X","a",productions);
	//************************************************************
	for(auto x:productions){
		cout<<x.left<<" "<<x.right<<endl;
	}
	
	return 0;
}
