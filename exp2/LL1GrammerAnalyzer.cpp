#include<iostream>
#include<string>
#include<vector> 
#include<map>
#include<set>
#include<unordered_set>
#include<unordered_map>
#include<stack>
#include <fstream>
#include<queue>
using namespace std;

struct production{
	int left; //����һ����ţ���Ӧһ�����ս�� 
	vector<int> right; //�Ҳ���һ��������� 
	production(int left,vector<int> right){
		this->left = left;
		this->right = right;
	}
};

struct TreeNode{
	int number; // ��Ӧ���
	vector<TreeNode*> sons; 
};

struct grammerAnalyzeResult{
	//�﷨�������صĽ��һ�����﷨����һ���Ǵ����λ��
	TreeNode* grammerTree;
	int tokenWrongIndex; 
};

vector<production> productions; //�ķ� 
unordered_set<int> nonTerminal;  //���ս���ļ���
unordered_set<int> terminal;    //�ս���ļ���
map<string,int> table;   //���ű�  <����,���> 
map<int,string> id_string_table; //���ŷ���� 

unordered_set<int> analyze_table[200][200]; //������(����Ϊʲô����������д��ȫ�ֱ������ˣ���Ϊ����)  analyze_table[i][j] ��ʾ���ս��i�����ս��jʱ��ѡ�� 


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
	//�Ȱ�֮ǰ�ʷ��������ս��������
	table["int"] = 1; id_string_table[1] = "int";
	table["if"] = 2; id_string_table[2] = "if";
	table["else"] = 3; id_string_table[3] = "else";
	table["get"] = 4; id_string_table[4] = "get";
	table["put"] = 5; id_string_table[5] = "put";
	table["("] = 6; id_string_table[6] = "(";
	table[")"] = 7; id_string_table[7] = ")";
	table[">"] = 8; id_string_table[8] = ">";
	table["<"] = 9; id_string_table[9] = "<";
	table["="] = 10; id_string_table[10] = "=";
	table[">="] = 11; id_string_table[11] = ">=";
	table["<="] = 12; id_string_table[12] = "<=";
	table["=="] = 13; id_string_table[13] = "==";
	table["!"] = 14; id_string_table[14] = "!";
	table["!="] = 15; id_string_table[15] = "!=";
	table["&&"] = 16; id_string_table[16] = "&&";
	table["||"] = 17; id_string_table[17] = "||";
	table["while"] = 18; id_string_table[18] = "while";
	table[";"] = 19; id_string_table[19] = ";";
	table[","] = 20; id_string_table[20] = ",";
	table["+"] = 21; id_string_table[21] = "+";
	table["-"] = 22; id_string_table[22] = "-";
	table["{"] = 23; id_string_table[23] = "{";
	table["}"] = 24; id_string_table[24] = "}";
	table["|"] = 25; id_string_table[25] = "|";
	table["����"] = 66; id_string_table[66] = "����";
	table["����"] = 77; id_string_table[77] = "����";
	
	table["$"] = 200; id_string_table[200] = "$";
	
	//���ս���ҾͶ�����100�������
	table["����"] = 100; id_string_table[100] = "����";
	table["����"] = 101; id_string_table[101] = "����";
	table["��������ʽ"] = 102; id_string_table[102] = "��������ʽ";
	table["����ɸ�ֵ"] = 103; id_string_table[103] = "����ɸ�ֵ";
	table["��ֵ���"] = 104; id_string_table[104] = "��ֵ���";
	table["�ӱ���"] = 105; id_string_table[105] = "�ӱ���";
	table["���ʽ"] = 106; id_string_table[106] = "���ʽ";
	table["���ʽ�м䲿��"] = 107; id_string_table[107] = "���ʽ�м䲿��";
	table["һԪ���ʽ�м䲿��"] = 108; id_string_table[108] = "һԪ���ʽ�м䲿��";
	table["��Ԫ���ʽ�м䲿��"] = 109; id_string_table[109] = "��Ԫ���ʽ�м䲿��";
	table["��Ԫ�����"] = 110; id_string_table[110] = "��Ԫ�����";
	table["��"] = 111; id_string_table[111] = "��"; 
	table["�������"] = 112; id_string_table[112] = "�������";
	table["������"] = 113; id_string_table[113] = "������";
	table["ѭ�����"] = 114; id_string_table[114] = "ѭ�����"; 
	table["�������"] = 115; id_string_table[115] = "�������";
	table["else����"] = 116; id_string_table[116] = "else����";
}

void initTNT(){
	//��ʼ��nonterminal��terminal
	vector<int> t = {200,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,66,77};
	vector<int> nt = {100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116};
	for(int x:nt) nonTerminal.insert(x);
	for(int x:t) terminal.insert(x);
}

void initProductions(){
	//�����Զ������� 
	int left;
	vector<int> right;
	//���ִ��ķ�
	///////////////////
	left = 100;
	right.clear(); right.push_back(101);right.push_back(100);
	addProduction(left,right);
	///////////////////
	left = 100;
	right.clear();
	addProduction(left,right);
	///////////////////
	left = 101;
	right.clear(); right.push_back(102);right.push_back(103); right.push_back(19);
	addProduction(left,right);
	///////////////////
	left = 101;
	right.clear(); right.push_back(104);
	addProduction(left,right);
	///////////////////
	left = 101;
	right.clear(); right.push_back(112);
	addProduction(left,right);
	///////////////////
	left = 101;
	right.clear(); right.push_back(113);
	addProduction(left,right);
	///////////////////
	left = 101;
	right.clear(); right.push_back(114);
	addProduction(left,right);
	///////////////////
	left = 101;
	right.clear(); right.push_back(115);
	addProduction(left,right);
	///////////////////
	left = 102;
	right.clear(); right.push_back(1);right.push_back(77);right.push_back(105);
	addProduction(left,right);
	///////////////////
	left = 103;
	right.clear(); right.push_back(10);right.push_back(106);
	addProduction(left,right);
	///////////////////
	left = 103;
	right.clear();
	addProduction(left,right);
	///////////////////
	left = 105;
	right.clear(); right.push_back(20);right.push_back(77);right.push_back(105);
	addProduction(left,right);
	///////////////////
	left = 105;
	right.clear();
	addProduction(left,right);
	///////////////////
	left = 104;
	right.clear(); right.push_back(77);right.push_back(10);right.push_back(106);right.push_back(19);
	addProduction(left,right);
	///////////////////
	left = 106;
	right.clear(); right.push_back(66);
	addProduction(left,right);
	///////////////////
	left = 106;
	right.clear(); right.push_back(6);right.push_back(107);right.push_back(7); 
	addProduction(left,right);
	///////////////////
	left = 107;
	right.clear(); right.push_back(108);
	addProduction(left,right);
	///////////////////
	left = 107;
	right.clear(); right.push_back(109);
	addProduction(left,right);
	///////////////////
	left = 108;
	right.clear();right.push_back(14);right.push_back(111);
	addProduction(left,right);
	///////////////////
	left = 109;
	right.clear();right.push_back(111);right.push_back(110);right.push_back(111);
	addProduction(left,right);
	///////////////////
	left = 110;
	right.clear(); right.push_back(21);
	addProduction(left,right);
	///////////////////
	left = 110;
	right.clear(); right.push_back(22);
	addProduction(left,right);
	///////////////////
	left = 110;
	right.clear(); right.push_back(13);
	addProduction(left,right);
	///////////////////
	left = 110;
	right.clear(); right.push_back(8);
	addProduction(left,right);
	///////////////////
	left = 110;
	right.clear(); right.push_back(9);
	addProduction(left,right);
	///////////////////
	left = 110;
	right.clear(); right.push_back(15);
	addProduction(left,right);
	///////////////////
	left = 110;
	right.clear(); right.push_back(11);
	addProduction(left,right);
	///////////////////
	left = 110;
	right.clear(); right.push_back(12);
	addProduction(left,right);
	///////////////////
	left = 110;
	right.clear(); right.push_back(16);
	addProduction(left,right);
	///////////////////
	left = 110;
	right.clear(); right.push_back(17);
	addProduction(left,right);
	///////////////////
	left = 111;
	right.clear(); right.push_back(66);
	addProduction(left,right);
	///////////////////
	left = 111;
	right.clear(); right.push_back(77);
	addProduction(left,right);
	///////////////////
	left = 112;
	right.clear(); right.push_back(4);right.push_back(6);right.push_back(77);right.push_back(105);right.push_back(7);right.push_back(19);
	addProduction(left,right);
	///////////////////
	left = 113;
	right.clear(); right.push_back(5);right.push_back(6);right.push_back(77);right.push_back(105);right.push_back(7);right.push_back(19);
	addProduction(left,right);
	///////////////////
	left = 114;
	right.clear(); right.push_back(18);right.push_back(106);right.push_back(23);right.push_back(100);right.push_back(24);right.push_back(19);
	addProduction(left,right);
	///////////////////
	left = 115;
	right.clear(); right.push_back(2);right.push_back(106);right.push_back(23);right.push_back(100);right.push_back(24);right.push_back(19);right.push_back(116);
	addProduction(left,right);
	///////////////////
	left = 116;
	right.clear(); right.push_back(3);right.push_back(23);right.push_back(100);right.push_back(24);right.push_back(19);
	addProduction(left,right);
	///////////////////
	left = 116;
	right.clear();
	addProduction(left,right);
	
} 


void printProduction(int productionIndex){
	cout<<id_string_table[productions[productionIndex].left]<<" -> ";
	for(auto x:productions[productionIndex].right){
		cout<<id_string_table[x]<<" "; 
	}
	cout<<endl; 
} 

void printProductions(){
	//�ñ�Ŵ�ӡһ�� 
	cout<<"�ķ�������ʾ��"<<endl; 
	cout<<"*********************"<<endl;
	for(auto production:productions){
		cout<<production.left<<" -> ";
		for(auto icon:production.right)
		{
			cout<<icon<<" ";
		}
		cout<<endl;
	}
	cout<<"*********************"<<endl;
	//���ַ�������ʽ��ӡһ��
	for(auto production:productions){
		cout<<id_string_table[production.left]<<" -> ";
		for(auto icon:production.right)
		{
			cout<<id_string_table[icon]<<" ";
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
	follow[100].insert(200);  //��ʼ����follow����ʼ��Ϊ��ֹ�� 
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
	for(int i=0;i<200;i++)
		for(int j=0;j<200;j++)
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
	int initial = 20;
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

bool checkAnalyzeTable(){
	//���table������û�г�ͻ�����
	for(int nt:nonTerminal){
		for(int t:terminal){
			if(analyze_table[nt][t].size() > 1)
			{
				cout<<"�����ս��"<<" "<<id_string_table[nt];
				cout<<" ����"<<"�ս�� "<<id_string_table[t]<<" ʱ";
				cout<<"������ʽ�����¼���ѡ��"<<endl;
				for(int productionIndex:analyze_table[nt][t]){
					printProduction(productionIndex);
				}
				return false; 
			}
		}
	}
	cout<<"������û����"<<endl;
	return true;
}

grammerAnalyzeResult grammerAnalyze(vector<int> tokens){
	
	grammerAnalyzeResult result;
	result.tokenWrongIndex = -1;
	result.grammerTree = nullptr;
	
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
	cout<<endl<<endl;
	
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
	cout<<endl;

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
	cout<<endl;

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
	cout<<endl; 
	
	//����analyze_table ������
	createAnalyzeTable(firstS);
	bool ATOK = checkAnalyzeTable(); //analyze_table is OK 
	//printAnalyzeTable();
	if(!ATOK) {
		result.tokenWrongIndex = -2; //��ʾ�ķ�������Ȼ�������ߵ�������
		return result; 
	}
	cout<<endl;

	//���ݷ���������﷨����
	int tokenPos = 0; int tokenLen = tokens.size(); //{8,6,8,7,8}
	stack<TreeNode*> s;
	TreeNode* endMark = new TreeNode; endMark->number = 200; //endMark->sons.clear();
	TreeNode* root = new TreeNode; root->number=100; //root->sons.clear();
	s.push(endMark);//push������ 
	s.push(root); //push��ʼ���� 
	int error = 0; 

	while(!s.empty()){
		//��ӡ��ǰջ��Ԫ�� 
		stack<TreeNode*> temp = s;
		cout<<"ջ��Ԫ��: ";
		while(!temp.empty()){
			cout<<id_string_table[temp.top()->number]<<" ";
			temp.pop();
		}
		cout<<"ջ��";
		cout<<"    ";
		//��ӡ��ǰ���ڴ����token
		cout<<"����: ";
		for(int i=tokenPos;i<tokenLen;i++){
			cout<<id_string_table[tokens[i]];
		} 
		TreeNode* curNode = s.top(); 
		int x  = s.top()->number;
		if(isTerminal(x))
		{
			if(x == tokens[tokenPos]){
			tokenPos++;
			s.pop();
			}else{
				error = 1;
				cout<<endl;
				result.tokenWrongIndex = tokenPos;
				cout<<"!!!!�������﷨����!!!!"<<endl;
				break;
			}
			cout<<endl; 
		}
		else{
			s.pop();
			unordered_set<int> productionIndexs = analyze_table[x][tokens[tokenPos]]; //���
			//��Ȼ��Indexs�����ǽ�����ֻ��һ������������,������͵�һ������
			if(productionIndexs.size() != 1){
				error = 1;
				cout<<endl;
				if(productionIndexs.size() ==0 )cout<<"!!!!�������﷨����,�������ӦΪ��!!!!"<<endl;
				if(productionIndexs.size() >1 )cout<<"!!!!�������﷨����,�������Ӧѡ�����1!!!!"<<endl;
				result.tokenWrongIndex = tokenPos;	
				break;
			}
			vector<int> right;
			int index = -1;
			for(int productionIndex:productionIndexs){
				index = productionIndex;
				right = productions[productionIndex].right;
			}
			cout<<"   Ӧ���ķ���";
			printProduction(index);
			
			for(int i=right.size()-1;i>=0;i--)
			{
				TreeNode* pushNode = new TreeNode; pushNode->number =right[i]; pushNode->sons.clear();
				s.push(pushNode);
				curNode->sons.push_back(pushNode); //����ջҪ����push��û�취��ֻ��Ҳ���Ŵ��sons�� 
			}
				
		}
	}
	if(!error) cout<<"�﷨�����������"<<endl;
	result.grammerTree = root;
	return result;
}


void printGrammerTree(TreeNode* root){
	//ע�⣬����node�����sons�ǵ��Ŵ�Ĺ�
	//���ò���������������
	queue<pair<TreeNode*,int> > q;
	q.push(make_pair(root,1));
	int curLevel = 1;
	while(!q.empty()){
		pair<TreeNode*,int> frontPair = q.front();
		q.pop();
		if(frontPair.second > curLevel){
			cout<<endl;
			curLevel++;
		}
		TreeNode* frontNode = frontPair.first;
		if(isTerminal(frontNode->number)){
			cout<<"���ڵ�"<<curLevel<<"���  "<<id_string_table[frontNode->number]<<"���ս��"; 
		}
		else{
			cout<<"���ڵ�"<<curLevel<<"���  "<<id_string_table[frontNode->number]<<"  "<<"-> ";
			for(int i=frontNode->sons.size()-1; i>=0;i--){
				cout<<id_string_table[frontNode->sons[i]->number]<<" ";
				q.push(make_pair(frontNode->sons[i],curLevel+1));
			}
		}
		cout<<endl;
	}
}

int main(){
	vector<int> tokens={2,6,77,13,66,7,23,24,19,200}; 
	grammerAnalyzeResult result  = grammerAnalyze(tokens);
	cout<<endl<<endl;
	
	if(result.tokenWrongIndex != -1){
		cout<<"�﷨������������,����λ���� index "<<result.tokenWrongIndex<<endl;
	}
	else{
		//���﷨�����������
		cout<<"�������﷨��������: "<<endl;
		printGrammerTree(result.grammerTree);
	}
	 
	 
	return 0;
}
