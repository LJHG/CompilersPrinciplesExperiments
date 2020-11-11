#include<iostream>
#include<string>
#include<vector> 
#include<map>
#include<set>
#include<unordered_set>
#include<unordered_map>
#include<stack>
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

unordered_set<int> analyze_table[100][100]; //������(����Ϊʲô����������д��ȫ�ֱ������ˣ���Ϊ����)  analyze_table[i][j] ��ʾ���ս��i�����ս��jʱ��ѡ�� 


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
	table["E"] = 1; id_string_table[1] = "E";
	table["E'"]= 2; id_string_table[2] = "E'";
	table["T"] = 3; id_string_table[3] = "T";
	table["T'"] = 4; id_string_table[4] = "T'";
	table["F"] = 5; id_string_table[5] = "F";
	table["+"] = 6; id_string_table[6] = "+";
	table["*"] = 7; id_string_table[7] = "*";
	table["id"] = 8; id_string_table[8] = "id";
	table["("] = 9; id_string_table[9] = "(";
	table[")"] = 10; id_string_table[10] = ")";
	table["$"] = 11; id_string_table[11] = "$"; //��ֹ�����Ҹо�����Ҫ�ӽ�������Ȼ����� 
	 
}

void initTNT(){
	//��ʼ��nonterminal��terminal
	vector<int> nt = {1,2,3,4,5};
	vector<int> t = {6,7,8,9,10,11};
	for(int x:nt) nonTerminal.insert(x);
	for(int x:t) terminal.insert(x);
}

void initProductions(){
	int left;
	vector<int> right;
	//���ִ��ķ���û�취�� 
	///////////////////
	left = 1;
	right.clear(); right.push_back(3);right.push_back(2);
	addProduction(left,right);
	///////////////////
	left = 2;
	right.clear(); right.push_back(6);right.push_back(3);right.push_back(2);
	addProduction(left,right);
	///////////////////
	left = 2;
	right.clear();
	addProduction(left,right);
	///////////////////
	left = 3;
	right.clear(); right.push_back(5);right.push_back(4);
	addProduction(left,right);
	///////////////////
	left = 4;
	right.clear(); right.push_back(7);right.push_back(5);right.push_back(4);
	addProduction(left,right);
	///////////////////
	left = 4;
	right.clear();
	addProduction(left,right);
	///////////////////
	left = 5;
	right.clear(); right.push_back(9);right.push_back(1);right.push_back(10);
	addProduction(left,right);
	///////////////////
	left = 5;
	right.clear(); right.push_back(8);
	addProduction(left,right);
} 


void printProduction(int productionIndex){
	cout<<id_string_table[productions[productionIndex].left]<<" -> ";
	for(auto x:productions[productionIndex].right){
		cout<<id_string_table[x]; 
	}
	cout<<endl; 
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
	follow[1].insert(11);
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
					//follow[icon] ��= temp  �ۣ�����֮ǰд���ˣ����˰���Ĵ�
					for(auto x:temp){
						follow[icon].insert(x);
					}
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

//���������� 
void createAnalyzeTable(unordered_map<int,unordered_set<int>> firstS){
	for(int i=0;i<100;i++)
		for(int j=0;j<100;j++)
			analyze_table[i][j].clear();
	
	for(auto x:firstS){
		int productionIndex = x.first;
		int NT = productions[productionIndex].left;
		for(auto a:x.second){
			int T = a;
			analyze_table[NT][T].insert(productionIndex);
		}
	}
}

void printAnalyzeTable(){
	//�����ʽ������̫���� 
	int initial = 9;
	int spaceNum;
	for(int t:terminal){
		spaceNum = initial-id_string_table[t].size();
		while(spaceNum--) cout<<" ";
		cout<<id_string_table[t];
	}
	
	cout<<endl; 
	// space num ---- initial���� 
	for(int nt:nonTerminal){
		cout<<id_string_table[nt];
		spaceNum = initial - id_string_table[nt].size();
		while(spaceNum--) cout<<" ";
		for(int t:terminal){
			spaceNum = initial;
			for(int candidate:analyze_table[nt][t]){
				cout<<candidate<<" ";spaceNum-=2;
			}
			while(spaceNum--) cout<<" ";
		}
		cout<<endl;
	}
	cout<<endl; 
}



void printStack(stack<int> s){
	
}




int main(){
	
	
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
	//����analyze_table ������
	createAnalyzeTable(firstS);
	printAnalyzeTable();
	
	//���ݷ���������﷨����
	vector<int> tokens={8,6,8,7,8,11}; int tokenPos = 0; int tokenLen = tokens.size(); //{8,6,8,7,8}
	stack<int> s;
	s.push(11);//push������ 
	s.push(1); //push��ʼ���� 
	while(!s.empty()){
		//��ӡ��ǰջ��Ԫ�� 
		stack<int> temp = s;
		cout<<"ջ��Ԫ��: ";
		while(!temp.empty()){
			cout<<id_string_table[temp.top()]<<" ";
			temp.pop();
		}
		cout<<"ջ��";
		cout<<"    ";
		//��ӡ��ǰ���ڴ����token
		cout<<"����: ";
		for(int i=tokenPos;i<tokenLen;i++){
			cout<<id_string_table[tokens[i]];
		} 
		 
		int x  = s.top();
		if(isTerminal(x))
		{
			if(x == tokens[tokenPos]){
			tokenPos++;
			s.pop();
			}else{
				cout<<"��������"<<endl;
				break;
			}
			cout<<endl;
		}
		else{
			s.pop();
			unordered_set<int> productionIndexs = analyze_table[x][tokens[tokenPos]]; //���
			//��Ȼ��Indexs�����ǽ�����ֻ��һ������������,������͵�һ������
			vector<int> right;
			int index = -1;
			for(int productionIndex:productionIndexs){
				index = productionIndex;
				right = productions[productionIndex].right;
			}
			cout<<"   Ӧ���ķ���";
			cout<<id_string_table[productions[index].left]<<" -> ";
			for(auto x:productions[index].right) cout<<id_string_table[x];
			cout<<endl; 
			
			for(int i=right.size()-1;i>=0;i--)
				s.push(right[i]);
		}
	}
	cout<<"�﷨�����������"<<endl;
	 
	return 0;
}
