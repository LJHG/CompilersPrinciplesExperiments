#include "LL1GrammerAnalyzer.cpp"
#include<iostream>

using namespace std;

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
