#include<iostream> 
#include<fstream>
using namespace std;

void markWrongPos(char* fileName,int row,int col){
	ifstream in(fileName);
	int curRow = 1;
	string s;
	while(curRow != row){
		getline(in,s);
		curRow++;
	}
	getline(in,s);
	cout<<s<<endl;
	int curCol = 0;
	while(curCol != col-1){
		cout<<" ";
		curCol++;
	}
	cout<<"^"<<endl;
} 

