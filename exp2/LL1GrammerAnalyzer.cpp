#include<iostream>
#include<string>
#include<vector> 
#include<map>
using namespace std;

struct production{
	string left; //�� 
	string right; //�Ҳ�
	production(string left,string right){
		this->left = left;
		this->right = right;
	} 
};


//��÷��ս����First�� 
map<string,int> getFirst(vector<production> productions){
	
}

//��÷��ս����Follow�� 
map<string,int> getFollow(vector<production> productions){
	
}

//������е�NULLABLE�ķ��ս�� 
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

//ûʲôx�õ�һ��������������дһ�� 
void addProduction(string left,string right,vector<production>& productions)
{
	production p(left,right); productions.push_back(p); 
} 


int main(){
	
	vector<int> tokens; int tokenPos = 0;
	vector<production> productions;
	//�����ķ� ���ֹ�����***************************************** 
	addProduction("Z","d",productions);
	addProduction("Z","XYZ",productions);
	addProduction("Y","c",productions);
	addProduction("Z","@",productions); //�������д�Ĺ���@�ǿղ���ʽ 
	addProduction("X","Y",productions);
	addProduction("X","a",productions);
	//************************************************************
	for(auto x:productions){
		cout<<x.left<<" "<<x.right<<endl;
	}
	
	return 0;
}
