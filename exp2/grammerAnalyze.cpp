#include<iostream>
#include "../exp1/LexicalAnalyzer.cpp"
#include "LL1GrammerAnalyzer.cpp" 
using namespace std;

int main(){
	lexicalAnalyzeResult lexicalResult = lexicalAnalyze("test.txt");
	cout<<"�ʷ����������"<<endl;
	showTokens(lexicalResult.tokens);
	cout<<"*********************ͳ����Ϣ************************"<<endl; 
	cout<<"��������"<<lexicalResult.lineCnt<<endl;
	cout<<"�ַ�������"<<lexicalResult.charCnt<<endl;
	cout<<"�����ʳ���Ƶ�����£�"<<endl;
	cout<<"����"<<"       "<<"���ִ���"<<endl; 
	for(auto x:lexicalResult.wordsFreq){
		cout<<x.first;
		int l = 14-x.first.size();
		while(l--){
			cout<<" ";
		}
		cout<<x.second<<endl;
	}
	
	//֮ǰ�ʷ�������tokens������Ϣ̫�࣬�﷨������ֻҪnum���м��ɣ��������¸�һ��
	vector<int> tokenNums;
	for(auto x:lexicalResult.tokens){
		tokenNums.push_back(x.num);
	}
	tokenNums.push_back(200); //push��ֹ�� 
	grammerAnalyzeResult grammerResult  = grammerAnalyze(tokenNums);
	cout<<endl<<endl;
	
	if(grammerResult.tokenWrongIndex != -1){
		int index = grammerResult.tokenWrongIndex;
		cout<<"�﷨�����������󣬴����token��indexΪ "<<index<<"��Ӧ���ַ���Ϊ  "<<id_string_table[tokenNums[index]]<<endl;
		cout<<"λ���ڵ�"<<" "<<lexicalResult.tokens[index].rowNum<<"��"<<"����"<<" "<<lexicalResult.tokens[index].colNum<<"��"<<endl;
	}
	else{
		//���﷨�����������
		cout<<"�������﷨��������: "<<endl;
		printGrammerTree(grammerResult.grammerTree);
	}
	 
	 
	return 0;
}