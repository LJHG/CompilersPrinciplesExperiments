#include "LexicalAnalyzer.cpp"
#include<iostream>
using namespace std;

int main()
{
	lexicalAnalyzeResult result = lexicalAnalyze("test.txt");
	cout<<"�ʷ����������"<<endl;
	showTokens(result.tokens);
	cout<<"*********************ͳ����Ϣ************************"<<endl; 
	cout<<"��������"<<result.lineCnt<<endl;
	cout<<"�ַ�������"<<result.charCnt<<endl;
	cout<<"�����ʳ���Ƶ�����£�"<<endl;
	cout<<"����"<<"       "<<"���ִ���"<<endl; 
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
