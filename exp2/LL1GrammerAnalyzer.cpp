#include<iostream>
#include<string>
#include<vector> 
#include<map>
#include<set>
#include<unordered_set>
using namespace std;

struct production{
	int left; //����һ����ţ���Ӧһ�����ս�� 
	vector<int> right; //�Ҳ���һ��������� 
	production(int left,vector<int> right){
		this->left = left;
		this->right = right;
	} 
};

vector<production> productions; //�ķ� 
unordered_set<int> nonTerminal;  //���ս���ļ���
unordered_set<int> terminal;    //�ս���ļ���
map<string,int> table;   //���ű�  <����,���> 
map<int,string> id_string_table; //���ŷ���� 


//ûʲôx�õĺ��������԰����б�һ�� 
void addProduction(int left,vector<int> right){
	production p(left,right);
	productions.push_back(p);
}
 
void initTable(){
	//���ڸ���һ���ض����ķ�����ʼ��table �� TNT����
	table["X"] = 1; id_string_table[1] = "X";
	table["Y"] = 2; id_string_table[2] = "Y";
	table["Z"] = 3; id_string_table[3] = "Z";
	table["a"] = 4; id_string_table[4] = "a";
	table["c"] = 5; id_string_table[5] = "c";
	table["d"] = 6; id_string_table[6] = "d";
}

void initTNT(){
	//��ʼ��nonterminal��terminal 
	nonTerminal.insert(1);nonTerminal.insert(2);nonTerminal.insert(3);
	terminal.insert(4);terminal.insert(5);terminal.insert(6);	
}

void initProductions(){
	int left;
	vector<int> right;
	//���ִ��ķ���û�취�� 
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
	right.clear(); //�Ҳ�Ϊ�գ�ʲô����push 
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
	//�ñ�Ŵ�ӡһ�� 
	for(auto production:productions){
		cout<<production.left<<" -> ";
		for(auto icon:production.right)
		{
			cout<<icon;
		}
		cout<<endl;
	}
	cout<<"*********************"<<endl;
	//���ַ�������ʽ��ӡһ��
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

//��÷��ս����First�� 
map<int,vector<int> > getFirst(){
	
	
}

//��÷��ս����Follow�� 
map<int,vector<int> > getFollow(){
	
}

//������е�NULLABLE�ķ��ս�� 
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
	//����Nullable����
	cout<<"nullable����������ʾ:"<<endl;
	unordered_set<int> nullable =getNullable();
	for(auto x:nullable){
		cout<<id_string_table[x]<<" ";
	}
	cout<<endl;
	
	
	return 0;
}
