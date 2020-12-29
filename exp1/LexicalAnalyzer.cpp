#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<fstream>
#include<sstream>

using namespace std;

/*
***********************词法分析部分****************************** 
*/ 
struct tokenInfo{
	string s; //token串
	int num; //token编号
	int rowNum; //在第几行
	int colNum; //在第几列 
}; 

bool isLetter(char c){
	if(  (('a' <= c) && (c <= 'z')) ||  (('A' <= c) && (c<= 'Z')) ) return true;
	return false;
}

bool isDigit(char c){
	if( '0' <= c && c<='9') return true;
	return false; 
}

bool isOct(string s){
	if(s.length()<2){
		return false;
	}
	int pt=1;
	while(pt<=s.length()-1){
		if(s[pt]>='8') return false;
		pt++;
	}
	return true;
}
bool isHex(string s){
	if(s.length()<3){
		return false;
	}
	if(s[0] != '0' || s[1] != 'x') return false;
	int pt = 2;
	while(pt<=s.length()-1){
		if(!isDigit(s[pt]) && (s[pt] > 'F' || s[pt] <'A')) return false;
		pt++;
	}
	return true;
}

bool isNumber(string s){
	if(isHex(s)) return true;
	if(isOct(s)) return true;
	int len = s.size();
	for(int i=0;i<len;i++){
		if(!('0'<=s[i] && s[i]<='9')) return false;
	}
	return true;
}

bool isVariables(string s){
	int len = s.size();
	if(isDigit(s[0])) return false;
	return true; 
}

void showTokens(vector<tokenInfo>& v){
	for(auto x:v){
		cout<<"<"<<x.s<<" , "<<x.num<<">"<<" "<<"位置在第"<<x.rowNum<<"行"<<"第"<<x.colNum<<"列"<<endl;
	}
	cout<<endl;
}

bool isIllegal(char c){
	if(c == '@' || c=='~') return true;
	return false;
}

string readFileIntoString(char * filename)
{
	ifstream ifile(filename);
	//将文件读入到ostringstream对象buf中
	ostringstream buf;
	char ch;
	while(buf&&ifile.get(ch))
	buf.put(ch);
	//返回与流对象buf关联的字符串
	return buf.str();
}

struct lexicalAnalyzeResult{
	vector<tokenInfo> tokens;  //记号流
	int lineCnt; //总行数 
	int charCnt; //总字符数 
	map<string,int> wordsFreq;  //单词出现频次 
};


lexicalAnalyzeResult lexicalAnalyze(char* fileName){
	lexicalAnalyzeResult result;
	
	string s = readFileIntoString(fileName);
	s += '\n'; //一个trick 
	int state = 0;
	int pos = 0; 
	string curString = "";
	map<string,int> keywords;
	keywords["int"]=1;keywords["if"]=2;keywords["else"]=3;keywords["get"]=4;keywords["put"]=5;keywords["("]=6;keywords[")"]=7;
	keywords[">"]=8;keywords["<"]=9;keywords["="]=10;keywords[">="]=11;keywords["<="]=12;keywords["=="]=13;keywords["!"]=14;keywords["!="]=15;
	keywords["&&"]=16;keywords["||"]=17;keywords["while"]=18;keywords[";"]=19;keywords[","]=20;keywords["+"]=21;keywords["-"]=22;
	keywords["{"]=23;keywords["}"]=24;keywords["|"]=25;keywords["&"] = 26;keywords["bool"]=27;
	vector<tokenInfo> ans;
	int lineCnt = 1;
	int charCnt = 0;
	int curColPos = 0;
	string curLine ="";
	int prePos = -1;
	map<string,int> cnt; 
	while(pos<s.size())
	{
		if(prePos < pos){
			curLine += s[pos];
			prePos = pos;
			charCnt++;
			curColPos++;
		}
			
		switch(state){
			case 0:{
				if(s[pos] != ' ' && s[pos]!='\n') curString += s[pos];
				if(isLetter(s[pos])) state = 1;
				if(isDigit(s[pos])) state = 2;
				if(s[pos] == ' ') state = 98;
				if(s[pos] == '+' || s[pos] == '-') state =3;
				if(s[pos] == '=') state = 4;
				if(s[pos] == '>') state=6;
				if(s[pos] == '!') state = 8;
				if(s[pos] == '&') state = 10;
				if(s[pos] == '|') state = 12;
				if(s[pos] == '(') state = 14;
				if(s[pos] == ')') state = 15;
				if(s[pos] == '{') state = 16;
				if(s[pos] == '}') state = 17;
				if(s[pos] == ';') state = 18;
				if(s[pos] == '<') state = 19;
				if(s[pos] == '\n') state = 97;
				if(s[pos] == ',') state  = 22;
				pos++;
				break;
			}
			case 1:{
				if(isLetter(s[pos]) || isDigit(s[pos]) || s[pos] == '_'){
					state = 1;
					curString += s[pos];
					pos++;
				}else if(isIllegal(s[pos])){
					curString += s[pos];
					state = 100;
				}
				else{
					state = 99;
				}
				
				break;
			}
			case 2:{
				if(isDigit(s[pos]) || isLetter(s[pos])){ //isDigit(s[pos]) || s[pos] == 'x'
					state =2;
					curString += s[pos];
					pos++;
				}else if(isIllegal(s[pos])){ //isLetter(s[pos]) || 
					state = 100;
				}
				else{
					state = 99;
				}
				break;
			}
			case 3:{
				state = 99;
				break;
			}
			case 4:{
				if(s[pos] == '='){
					curString += "=";
					state =5;
					pos++;
				}else{
					state = 99;
				}
				break;
			}
			case 5:{
				state = 99;
				break;
			}
			case 6:{
				if(s[pos] == '='){
					curString += "=";
					pos++;
					state = 7;
				}else{
					state = 99;
				}
				break;
			}
			case 7:{
				state = 99;
				break;
			}
			case 8:{
				if(s[pos] == '='){
					curString += "=";
					pos++;
					state = 9;
				}else{
					state = 99;
				}
				break;
			}
			case 9:{
				state = 99;
				break;
			}
			case 10:{
				if(s[pos] == '&'){
					curString += "&";
					pos++;
					state = 11;
				}else{
					state = 99;
				}
				break;
			}
			case 11:{
				state = 99;
				break;
			}
			case 12:{
				if(s[pos] == '|'){
					curString += "|";
					pos++;
					state = 13;
				}else{
					state = 99;
				}
				break;
			}
			case 13:{
				state = 99;
				break;
			}
			case 14:{
				state = 99;
				break;
			}
			case 15:{
				state = 99;
				break;
			}
			case 16:{
				state = 99;
				break;
			}
			case 17:{
				state=99;
				break;
			}
			case 18:{
				state = 99;
				break;
			}
			case 19:{
				if(s[pos] == '='){
					curString += "=";
					pos++;
					state = 20;
				}else if(s[pos] == '>'){
					curString += ">";
					pos++;
					state = 21;
				}else{
					state = 99;
				}
				break;
			}
			case 20:{
				state = 99;
				break;
			}
			case 21:{
				state = 99;
				break;
			}
			case 22:{
				state = 99;
				break;
			}
			case 97:{
				//跟空格一样无视换行符 
				lineCnt++;
				curLine = "";
				prePos--; //不-- curline就没有换行后第一个字符
				state = 0;
				curColPos = 0;
				break;
			}
			case 98:{
				//读空格
				state = 0;
				break;
			}
			case 99:{
				//输出态
				//cout<<curString<<endl;
				//cout<<curString<<endl;
				tokenInfo ti;
				ti.rowNum = lineCnt;
				ti.colNum = curColPos - curString.size(); //y1s1，我也搞不清楚这个位置了，但是凑出来是对的 
				if(keywords[curString] == 0 ){
					if(isNumber(curString)){
						ti.s = curString;
						ti.num = 66;
						ans.push_back(ti); //如果是一个数字，就存为<数字,66> 
					}else if(isVariables(curString)){
						ti.s = curString;
						ti.num = 77;
						ans.push_back(ti); //如果是一个变量，就存为<变量,77> 
					}
					else{
						state = 100;
						break;
					}
				}else{
					ti.s = curString;
					ti.num = keywords[curString];
					ans.push_back(ti);
				}
				cnt[curString]++;
				curString = "";
				state = 0;
				break;
			}
			case 100:{
				//错误态 
				cout<<"Wrong at line:"<<lineCnt<<endl;
				int wrongPos = curLine.length()-2;
				pos++;
				while(1){
					if(s[pos]!='\n') {
						curLine += s[pos];
						pos++;
					}
					else 
						break;
				}
				cout<<curLine<<endl;
				while(wrongPos--){
					cout<<" ";
				}
				cout<<"^";
				return result;
			}
			default:{
				cout<<"走错地方了";
				break;
			}
		}
	}
	
	result.tokens = ans;
	result.lineCnt = lineCnt;
	result.charCnt = charCnt;
	result.wordsFreq = cnt;
	
	return result;
}


