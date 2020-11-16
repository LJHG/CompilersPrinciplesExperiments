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
	int left; //左部是一个编号，对应一个非终结符 
	vector<int> right; //右部是一个编号序列 
	production(int left,vector<int> right){
		this->left = left;
		this->right = right;
	}
};

struct TreeNode{
	int number; // 对应编号
	vector<TreeNode*> sons; 
};

struct grammerAnalyzeResult{
	//语法分析返回的结果一个是语法树，一个是错误的位置
	TreeNode* grammerTree;
	int tokenWrongIndex; 
};

vector<production> productions; //文法 
unordered_set<int> nonTerminal;  //非终结符的集合
unordered_set<int> terminal;    //终结符的集合
map<string,int> table;   //符号表  <符号,编号> 
map<int,string> id_string_table; //符号翻译表 

unordered_set<int> analyze_table[200][200]; //分析表(至于为什么分析表我又写在全局变量里了，因为方便)  analyze_table[i][j] 表示非终结符i遇到终结符j时的选择 


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
	//先把之前词法分析的终结符抄过来
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
	table["常量"] = 66; id_string_table[66] = "常量";
	table["变量"] = 77; id_string_table[77] = "变量";
	
	table["$"] = 200; id_string_table[200] = "$";
	
	//非终结符我就定义在100开外好了
	table["程序"] = 100; id_string_table[100] = "程序";
	table["句子"] = 101; id_string_table[101] = "句子";
	table["定义左公因式"] = 102; id_string_table[102] = "定义左公因式";
	table["定义可赋值"] = 103; id_string_table[103] = "定义可赋值";
	table["赋值语句"] = 104; id_string_table[104] = "赋值语句";
	table["加变量"] = 105; id_string_table[105] = "加变量";
	table["表达式"] = 106; id_string_table[106] = "表达式";
	table["表达式中间部分"] = 107; id_string_table[107] = "表达式中间部分";
	table["一元表达式中间部分"] = 108; id_string_table[108] = "一元表达式中间部分";
	table["二元表达式中间部分"] = 109; id_string_table[109] = "二元表达式中间部分";
	table["二元运算符"] = 110; id_string_table[110] = "二元运算符";
	table["数"] = 111; id_string_table[111] = "数"; 
	table["输入语句"] = 112; id_string_table[112] = "输入语句";
	table["输出语句"] = 113; id_string_table[113] = "输出语句";
	table["循环语句"] = 114; id_string_table[114] = "循环语句"; 
	table["条件语句"] = 115; id_string_table[115] = "条件语句";
	table["else部分"] = 116; id_string_table[116] = "else部分";
}

void initTNT(){
	//初始化nonterminal和terminal
	vector<int> t = {200,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,66,77};
	vector<int> nt = {100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116};
	for(int x:nt) nonTerminal.insert(x);
	for(int x:t) terminal.insert(x);
}

void initProductions(){
	//放弃自动处理了 
	int left;
	vector<int> right;
	//纯手打文法
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
	//用编号打印一遍 
	cout<<"文法如下所示："<<endl; 
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
	//用字符串的形式打印一遍
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
	follow[100].insert(200);  //起始符的follow集初始化为终止符 
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
	//这个格式化对齐太难了 
	int initial = 20;
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

bool checkAnalyzeTable(){
	//检查table里面有没有冲突的情况
	for(int nt:nonTerminal){
		for(int t:terminal){
			if(analyze_table[nt][t].size() > 1)
			{
				cout<<"当非终结符"<<" "<<id_string_table[nt];
				cout<<" 遇上"<<"终结符 "<<id_string_table[t]<<" 时";
				cout<<"，产生式有以下几个选择："<<endl;
				for(int productionIndex:analyze_table[nt][t]){
					printProduction(productionIndex);
				}
				return false; 
			}
		}
	}
	cout<<"分析表没问题"<<endl;
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
	
	//测试Nullable集合
	cout<<"nullable集合如下所示:"<<endl;
	unordered_set<int> nullable =getNullable();
	for(auto x:nullable){
		cout<<id_string_table[x]<<" ";
	}
	cout<<endl<<endl;
	
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
	cout<<endl;

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
	cout<<endl;

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
	cout<<endl; 
	
	//测试analyze_table 分析表
	createAnalyzeTable(firstS);
	bool ATOK = checkAnalyzeTable(); //analyze_table is OK 
	//printAnalyzeTable();
	if(!ATOK) {
		result.tokenWrongIndex = -2; //表示文法错误，虽然不可能走到这里了
		return result; 
	}
	cout<<endl;

	//根据分析表进行语法分析
	int tokenPos = 0; int tokenLen = tokens.size(); //{8,6,8,7,8}
	stack<TreeNode*> s;
	TreeNode* endMark = new TreeNode; endMark->number = 200; //endMark->sons.clear();
	TreeNode* root = new TreeNode; root->number=100; //root->sons.clear();
	s.push(endMark);//push结束符 
	s.push(root); //push开始符号 
	int error = 0; 

	while(!s.empty()){
		//打印当前栈内元素 
		stack<TreeNode*> temp = s;
		cout<<"栈内元素: ";
		while(!temp.empty()){
			cout<<id_string_table[temp.top()->number]<<" ";
			temp.pop();
		}
		cout<<"栈底";
		cout<<"    ";
		//打印当前正在处理的token
		cout<<"输入: ";
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
				cout<<"!!!!发生了语法错误!!!!"<<endl;
				break;
			}
			cout<<endl; 
		}
		else{
			s.pop();
			unordered_set<int> productionIndexs = analyze_table[x][tokens[tokenPos]]; //查表
			//虽然是Indexs，但是讲道理，只有一个才是正常的,我这里就当一个算了
			if(productionIndexs.size() != 1){
				error = 1;
				cout<<endl;
				if(productionIndexs.size() ==0 )cout<<"!!!!发生了语法错误,分析表对应为空!!!!"<<endl;
				if(productionIndexs.size() >1 )cout<<"!!!!发生了语法错误,分析表对应选择大于1!!!!"<<endl;
				result.tokenWrongIndex = tokenPos;	
				break;
			}
			vector<int> right;
			int index = -1;
			for(int productionIndex:productionIndexs){
				index = productionIndex;
				right = productions[productionIndex].right;
			}
			cout<<"   应用文法：";
			printProduction(index);
			
			for(int i=right.size()-1;i>=0;i--)
			{
				TreeNode* pushNode = new TreeNode; pushNode->number =right[i]; pushNode->sons.clear();
				s.push(pushNode);
				curNode->sons.push_back(pushNode); //这里栈要求倒着push，没办法，只好也倒着存进sons了 
			}
				
		}
	}
	if(!error) cout<<"语法分析完成无误"<<endl;
	result.grammerTree = root;
	return result;
}


void printGrammerTree(TreeNode* root){
	//注意，这里node里面的sons是倒着存的哈
	//采用层序遍历打出来看看
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
			cout<<"处于第"<<curLevel<<"层的  "<<id_string_table[frontNode->number]<<"是终结符"; 
		}
		else{
			cout<<"处于第"<<curLevel<<"层的  "<<id_string_table[frontNode->number]<<"  "<<"-> ";
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
		cout<<"语法分析发生错误,错误位置在 index "<<result.tokenWrongIndex<<endl;
	}
	else{
		//把语法树打出来看看
		cout<<"下面是语法树输出结果: "<<endl;
		printGrammerTree(result.grammerTree);
	}
	 
	 
	return 0;
}
