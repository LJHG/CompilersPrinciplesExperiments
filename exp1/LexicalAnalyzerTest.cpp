#include "LexicalAnalyzer.cpp"
#include<iostream>
using namespace std;

int main()
{
	lexicalAnalyzeResult result = lexicalAnalyze("test.txt");
	cout<<"词法分析结果："<<endl;
	showTokens(result.tokens);
	cout<<"*********************统计信息************************"<<endl; 
	cout<<"总行数："<<result.lineCnt<<endl;
	cout<<"字符总数："<<result.charCnt<<endl;
	cout<<"各单词出现频次如下："<<endl;
	cout<<"单词"<<"       "<<"出现次数"<<endl; 
	for(auto x:result.wordsFreq){
		cout<<x.first;
		int l = 14-x.first.size();
		while(l--){
			cout<<" ";
		}
		cout<<x.second<<endl;
	}
	
	return 0;	
}  
