#include "LL1GrammerAnalyzer.cpp"
#include<iostream>

using namespace std;

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
