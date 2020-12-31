#include "SemanticAnalyzer.cpp"

int main()
{
	lexicalAnalyzeResult lexicalResult = lexicalAnalyze("test.txt");
	vector<int> tokenNums;
	vector<string> tokenStrings;
	for(auto x:lexicalResult.tokens){
		tokenNums.push_back(x.num);
		tokenStrings.push_back(x.s);
	}
	tokenNums.push_back(200); //push��ֹ��
	tokenStrings.push_back("$");
	grammerAnalyzeResult grammerResult  = grammerAnalyze(tokenNums,tokenStrings);
	cout<<endl<<endl;
	if(grammerResult.tokenWrongIndex != -1){
		int index = grammerResult.tokenWrongIndex;
		cout<<"�﷨�����������󣬴����token��indexΪ"<<index<<"����Ӧ���ַ���Ϊ "<<id_string_table[tokenNums[index]]<<endl;
		cout<<"λ���ڵ�"<<""<<lexicalResult.tokens[index].rowNum<<"��"<<"����"<<""<<lexicalResult.tokens[index].colNum<<"��"<<endl;
		markWrongPos("test.txt",lexicalResult.tokens[index].rowNum,lexicalResult.tokens[index].colNum);
		cout<<grammerResult.errorInfo<<endl;
	}
	else{
		//���﷨�����������
		cout<<"�������﷨��������: "<<endl;
		printGrammerTree(grammerResult.grammerTree);
	}
	
	//֮ǰ���﷨�����sons�ǵ��Ŵ�ģ��������������������
	reverseParseTree(grammerResult.grammerTree);
	
	init_kinds();
	ASTnode* ASTroot = processProgram(grammerResult.grammerTree,nullptr);
	cout<<endl<<endl;
	cout<<"**************�����ӡAST******************"<<endl; 
	printAST(ASTroot);
	
	cout<<endl<<"**************��������������******************"<<endl; 
	errorInfo e = SemanticAnalyze(ASTroot);
	if(e.hasError){
		markWrongPos("test.txt",lexicalResult.tokens[e.errorIndex].rowNum,lexicalResult.tokens[e.errorIndex].colNum);
		cout<<e.errorDetail<<endl;
	}else{
		cout<<"��������������"<<endl;
	}
	
	return 0;
}
