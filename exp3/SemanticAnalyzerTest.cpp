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
	tokenNums.push_back(200); //push终止符
	tokenStrings.push_back("$");
	grammerAnalyzeResult grammerResult  = grammerAnalyze(tokenNums,tokenStrings);
	cout<<endl<<endl;
	if(grammerResult.tokenWrongIndex != -1){
		int index = grammerResult.tokenWrongIndex;
		cout<<"语法分析发生错误，错误的token的index为"<<index<<"，对应的字符串为 "<<id_string_table[tokenNums[index]]<<endl;
		cout<<"位置在第"<<""<<lexicalResult.tokens[index].rowNum<<"行"<<"，第"<<""<<lexicalResult.tokens[index].colNum<<"列"<<endl;
		markWrongPos("test.txt",lexicalResult.tokens[index].rowNum,lexicalResult.tokens[index].colNum);
		cout<<grammerResult.errorInfo<<endl;
	}
	else{
		//把语法树打出来看看
		cout<<"下面是语法树输出结果: "<<endl;
		printGrammerTree(grammerResult.grammerTree);
	}
	
	//之前的语法树里的sons是倒着存的，这里把它改正。。。。
	reverseParseTree(grammerResult.grammerTree);
	
	init_kinds();
	ASTnode* ASTroot = processProgram(grammerResult.grammerTree,nullptr);
	cout<<endl<<endl;
	cout<<"**************下面打印AST******************"<<endl; 
	printAST(ASTroot);
	
	cout<<endl<<"**************下面进行语义分析******************"<<endl; 
	errorInfo e = SemanticAnalyze(ASTroot);
	if(e.hasError){
		markWrongPos("test.txt",lexicalResult.tokens[e.errorIndex].rowNum,lexicalResult.tokens[e.errorIndex].colNum);
		cout<<e.errorDetail<<endl;
	}else{
		cout<<"语义分析结果无误"<<endl;
	}
	
	return 0;
}
