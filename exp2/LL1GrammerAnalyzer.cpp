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

unordered_set<int> analyze_table[100][100]; //分析表(至于为什么分析表我又写在全局变量里了，因为方便)  analyze_table[i][j] 表示非终结符i遇到终结符j时的选择 


//没什么x用的函数，可以把两行变一行 
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
	//现在根据一个特定的文法来初始化table 和 TNT试试
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
	table["$"] = 11; id_string_table[11] = "$"; //终止符，我感觉还是要加进来，不然很奇怪 
	 
}

void initTNT(){
	//初始化nonterminal和terminal
	vector<int> nt = {1,2,3,4,5};
	vector<int> t = {6,7,8,9,10,11};
	for(int x:nt) nonTerminal.insert(x);
	for(int x:t) terminal.insert(x);
}

void initProductions(){
	int left;
	vector<int> right;
	//纯手打文法，没办法啊 
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

//获得非终结符的First集 
unordered_map<int,unordered_set<int> > getFirst(unordered_set<int> nullable){
	//计算first集需要传入nullable(要不要把nullable搞成全局变量呢？ 还没想好)
	unordered_map<int,unordered_set<int>> first;
	while(1){
		//先计算first集的大小
		int size1 = 0;
		for(auto x:first){
			size1 += x.second.size();
		}
		//更新first集
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
						//first[left] = first[left] 并 first[icon] 
						for(auto x:first[icon]){
							first[left].insert(x); 
						}
						if(nullable.count(icon) == 0){ //icon不是nullable，直接结束 
							break;
						}
					}
				}
			}
		}
		//再次计算first集大小
		int size2 = 0;
		for(auto x:first){
			size2 += x.second.size();
		}
		if(size2 == size1){
			return first;
		}
	} 
}

//获得非终结符的Follow集 
unordered_map<int,unordered_set<int> > getFollow(unordered_set<int> nullable,unordered_map<int,unordered_set<int>> first){
	unordered_map<int,unordered_set<int> > follow;
	follow[1].insert(11);
	while(1){
		//先计算follow集的大小
		int size1 = 0;
		for(auto x:follow){
			size1 += x.second.size();
		}
		//更新follow集
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
					//follow[icon] 并= temp  哇，这里之前写错了，找了半天的错
					for(auto x:temp){
						follow[icon].insert(x);
					}
					if(nullable.count(icon) == 0) //icon不是nullable
					{
						temp = first[icon]; 
					}else{
						//temp 并= first[icon]
						for(auto x:first[icon]){
							temp.insert(x);
						} 
					}
				}
			} 
		}
		//再计算follow集大小
		int size2 = 0;
		for(auto x:follow){
			size2 += x.second.size();
		}
		if(size1 == size2) return follow;
	} 
}

//获得first_s集合(针对每一个产生式的first集)
unordered_map<int,unordered_set<int>> getFirstS(unordered_set<int> nullable,unordered_map<int,unordered_set<int>> first,unordered_map<int,unordered_set<int>> follow){
	unordered_map<int,unordered_set<int>> firstS;
	int index = 0;
	for(auto production:productions){
		int left = production.left;
		vector<int> right = production.right;
		int middleReturn = 0;
		for(int icon:right){
			//icon是终结符 
			if(isTerminal(icon)){
				firstS[index].insert(icon);
				middleReturn = 1;
				break;
			}
			//icon是非终结符 
			else{
				//firstS[index] 并= first[icon] 
				for(int x:first[icon]){
					firstS[index].insert(x); 
				}
				//如果icon不是nullable 
				if(nullable.count(icon)==0){
					middleReturn = 1;
					break;
				}
			}
		}
		if(!middleReturn){
			//如果没有中途返回，firstS[index] 并= follow[left]
			for(int x:follow[left]){
				firstS[index].insert(x);
			} 
		}
		index++;
	}
	return firstS;
}

//构建分析表 
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
	//这个格式化对齐太难了 
	int initial = 9;
	int spaceNum;
	for(int t:terminal){
		spaceNum = initial-id_string_table[t].size();
		while(spaceNum--) cout<<" ";
		cout<<id_string_table[t];
	}
	
	cout<<endl; 
	// space num ---- initial个数 
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
	//测试Nullable集合
	cout<<"nullable集合如下所示:"<<endl;
	unordered_set<int> nullable =getNullable();
	for(auto x:nullable){
		cout<<id_string_table[x]<<" ";
	}
	cout<<endl;
	//测试first集合
	cout<<"first集合如下所示："<<endl;
	unordered_map<int,unordered_set<int>> first = getFirst(nullable);
	for(auto x:first){
		cout<<id_string_table[x.first]<<":"<<" ";
		for(auto a:x.second){
			cout<<id_string_table[a]<<" ";
		}
		cout<<endl;
	}
	//测试follow集合 
	cout<<"follow集合如下所示："<<endl;
	unordered_map<int,unordered_set<int>> follow = getFollow(nullable,first);
	for(auto x:follow){
		cout<<id_string_table[x.first]<<":"<<" ";
		for(auto a:x.second){
			cout<<id_string_table[a]<<" ";
		}
		cout<<endl;
	}
	//测试first_S集合
	cout<<"first_S集合如下所示："<<endl;
	unordered_map<int,unordered_set<int>> firstS = getFirstS(nullable,first,follow);
	for(auto x:firstS){
		cout<<x.first<<":"<<" ";
		for(auto a:x.second){
			cout<<id_string_table[a]<<" ";
		}
		cout<<endl;
	}
	//测试analyze_table 分析表
	createAnalyzeTable(firstS);
	printAnalyzeTable();
	
	//根据分析表进行语法分析
	vector<int> tokens={8,6,8,7,8,11}; int tokenPos = 0; int tokenLen = tokens.size(); //{8,6,8,7,8}
	stack<int> s;
	s.push(11);//push结束符 
	s.push(1); //push开始符号 
	while(!s.empty()){
		//打印当前栈内元素 
		stack<int> temp = s;
		cout<<"栈内元素: ";
		while(!temp.empty()){
			cout<<id_string_table[temp.top()]<<" ";
			temp.pop();
		}
		cout<<"栈底";
		cout<<"    ";
		//打印当前正在处理的token
		cout<<"输入: ";
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
				cout<<"发生错误"<<endl;
				break;
			}
			cout<<endl;
		}
		else{
			s.pop();
			unordered_set<int> productionIndexs = analyze_table[x][tokens[tokenPos]]; //查表
			//虽然是Indexs，但是讲道理，只有一个才是正常的,我这里就当一个算了
			vector<int> right;
			int index = -1;
			for(int productionIndex:productionIndexs){
				index = productionIndex;
				right = productions[productionIndex].right;
			}
			cout<<"   应用文法：";
			cout<<id_string_table[productions[index].left]<<" -> ";
			for(auto x:productions[index].right) cout<<id_string_table[x];
			cout<<endl; 
			
			for(int i=right.size()-1;i>=0;i--)
				s.push(right[i]);
		}
	}
	cout<<"语法分析完成无误"<<endl;
	 
	return 0;
}
