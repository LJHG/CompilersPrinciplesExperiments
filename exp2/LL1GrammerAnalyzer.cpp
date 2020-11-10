#include<iostream>
#include<string>
#include<vector> 
#include<map>
#include<set>
#include<unordered_set>
#include<unordered_map>
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

bool isTerminal(int icon)
{
	return terminal.count(icon);
}

bool isNonTerminal(int icon){
	return nonTerminal.count(icon);
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

//��÷��ս����First�� 
unordered_map<int,unordered_set<int> > getFirst(unordered_set<int> nullable){
	//����first����Ҫ����nullable(Ҫ��Ҫ��nullable���ȫ�ֱ����أ� ��û���)
	unordered_map<int,unordered_set<int>> first;
	while(1){
		//�ȼ���first���Ĵ�С
		int size1 = 0;
		for(auto x:first){
			size1 += x.second.size();
		}
		//����first��
		for(auto production:productions){
			int left = production.left;
			vector<int> right = production.right;
			if(right.size() > 0){
				for(int icon:right){
					if(isTerminal(icon)){
						first[left].insert(icon);
						break;
					}
					if(isNonTerminal(icon)){
						//first[left] = first[left] �� first[icon] 
						for(auto x:first[icon]){
							first[left].insert(x); 
						}
						if(nullable.count(icon) == 0){ //icon����nullable��ֱ�ӽ��� 
							break;
						}
					}
				}
			}
		}
		//�ٴμ���first����С
		int size2 = 0;
		for(auto x:first){
			size2 += x.second.size();
		}
		if(size2 == size1){
			return first;
		}
	} 
}

//��÷��ս����Follow�� 
unordered_map<int,unordered_set<int> > getFollow(unordered_set<int> nullable,unordered_map<int,unordered_set<int>> first){
	unordered_map<int,unordered_set<int> > follow;	
	while(1){
		//�ȼ���follow���Ĵ�С
		int size1 = 0;
		for(auto x:follow){
			size1 += x.second.size();
		}
		//����follow��
		for(auto production:productions){
			int left = production.left;
			vector<int> right = production.right;
			unordered_set<int> temp = follow[left];
			int rightLen = right.size();
			for(int i=rightLen-1;i>=0;i--){
				int icon = right[i];
				if(isTerminal(icon)){
					temp.clear();
					temp.insert(icon);
				}
				else{
					follow[icon] = temp;
					if(nullable.count(icon) == 0) //icon����nullable
					{
						temp = first[icon]; 
					}else{
						//temp ��= first[icon]
						for(auto x:first[icon]){
							temp.insert(x);
						} 
					}
				}
			} 
		}
		//�ټ���follow����С
		int size2 = 0;
		for(auto x:follow){
			size2 += x.second.size();
		}
		if(size1 == size2) return follow;
	} 
}

//���first_s����(���ÿһ������ʽ��first��)
unordered_map<int,unordered_set<int>> getFirstS(unordered_set<int> nullable,unordered_map<int,unordered_set<int>> first,unordered_map<int,unordered_set<int>> follow){
	unordered_map<int,unordered_set<int>> firstS;
	int index = 0;
	for(auto production:productions){
		int left = production.left;
		vector<int> right = production.right;
		int middleReturn = 0;
		for(int icon:right){
			//icon���ս�� 
			if(isTerminal(icon)){
				firstS[index].insert(icon);
				middleReturn = 1;
				break;
			}
			//icon�Ƿ��ս�� 
			else{
				//firstS[index] ��= first[icon] 
				for(int x:first[icon]){
					firstS[index].insert(x); 
				}
				//���icon����nullable 
				if(nullable.count(icon)==0){
					middleReturn = 1;
					break;
				}
			}
		}
		if(!middleReturn){
			//���û����;���أ�firstS[index] ��= follow[left]
			for(int x:follow[left]){
				firstS[index].insert(x);
			} 
		}
		index++;
	}
	return firstS;
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
	//����first����
	cout<<"first����������ʾ��"<<endl;
	unordered_map<int,unordered_set<int>> first = getFirst(nullable);
	for(auto x:first){
		cout<<id_string_table[x.first]<<":"<<" ";
		for(auto a:x.second){
			cout<<id_string_table[a]<<" ";
		}
		cout<<endl;
	}
	//����follow���� 
	cout<<"follow����������ʾ��"<<endl;
	unordered_map<int,unordered_set<int>> follow = getFollow(nullable,first);
	for(auto x:follow){
		cout<<id_string_table[x.first]<<":"<<" ";
		for(auto a:x.second){
			cout<<id_string_table[a]<<" ";
		}
		cout<<endl;
	}
	//����first_S����
	cout<<"first_S����������ʾ��"<<endl;
	unordered_map<int,unordered_set<int>> firstS = getFirstS(nullable,first,follow);
	for(auto x:firstS){
		cout<<x.first<<":"<<" ";
		for(auto a:x.second){
			cout<<id_string_table[a]<<" ";
		}
		cout<<endl;
	}
	return 0;
}
