#include<iostream>
#include "../exp1/LexicalAnalyzer.cpp"
#include "LL1GrammerAnalyzer.cpp" 
#include "../commonFunctions/common.cpp"
using namespace std;

int main(){
	lexicalAnalyzeResult lexicalResult = lexicalAnalyze("test.txt");
//	cout<<"词法分析结果："<<endl;
//	showTokens(lexicalResult.tokens);
//	cout<<"*********************统计信息************************"<<endl; 
//	cout<<"总行数："<<lexicalResult.lineCnt<<endl;
//	cout<<"字符总数："<<lexicalResult.charCnt<<endl;
//	cout<<"各单词出现频次如下："<<endl;
//	cout<<"单词"<<"       "<<"出现次数"<<endl; 
//	for(auto x:lexicalResult.wordsFreq){
//		cout<<x.first;
//		int l = 14-x.first.size();
//		while(l--){
//			cout<<" ";
//		}
//		cout<<x.second<<endl;
//	}
	
	//之前词法分析的tokens包含信息太多，语法分析器只要num序列即可(并不是，不然信息越传越少了)，这里重新搞一搞
	vector<int> tokenNums;
	vector<string> tokenStrings;
	for(auto x:lexicalResult.tokens){
		tokenNums.push_back(x.num);
		tokenStrings.push_back(x.s);
	}
	tokenNums.push_back(200); //push终止符
	tokenStrings.push_back("$");
	grammerAnalyzeResult grammerResult  = grammerAnalyze(tokenNums,tokenStrings);
	cout<<endl<<endl;
	
	if(grammerResult.tokenWrongIndex != -1){
		int index = grammerResult.tokenWrongIndex;
		cout<<"语法分析发生错误，错误的token的index为"<<index<<"，对应的字符串为 "<<id_string_table[tokenNums[index]]<<endl;
		cout<<"位置在第"<<""<<lexicalResult.tokens[index].rowNum<<"行"<<"，第"<<""<<lexicalResult.tokens[index].colNum<<"列"<<endl;
		markWrongPos("test.txt",lexicalResult.tokens[index].rowNum,lexicalResult.tokens[index].colNum);
	}
	else{
		//把语法树打出来看看
		cout<<"下面是语法树输出结果: "<<endl;
		printGrammerTree(grammerResult.grammerTree);
	}
	 
	 
	return 0;
}
