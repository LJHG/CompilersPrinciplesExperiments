void initTable(){
	//���ڸ���һ���ض����ķ�����ʼ��table �� TNT����
	table["X"] = 1; id_string_table[1] = "X";
	table["Y"] = 2; id_string_table[2] = "Y";
	table["Z"] = 3; id_string_table[3] = "Z";
	table["a"] = 4; id_string_table[4] = "a";
	table["c"] = 5; id_string_table[5] = "c";
	table["d"] = 6; id_string_table[6] = "d";
}

void initTNT(){
	//��ʼ��nonterminal��terminal 
	nonTerminal.insert(1);nonTerminal.insert(2);nonTerminal.insert(3);
	terminal.insert(4);terminal.insert(5);terminal.insert(6);	
}

void initProductions(){
	int left;
	vector<int> right;
	//���ִ��ķ���û�취�� 
	///////////////////
	left = 3;
	right.clear(); right.push_back(6);
	addProduction(left,right);
	///////////////////
	left = 3;
	right.clear();right.push_back(1);right.push_back(2);right.push_back(3);
	addProduction(left,right);
	///////////////////
	left = 2;
	right.clear();right.push_back(5);
	addProduction(left,right);
	///////////////////
	left = 2;
	right.clear(); //�Ҳ�Ϊ�գ�ʲô����push 
	addProduction(left,right);
	///////////////////
	left = 1;
	right.clear();right.push_back(2);
	addProduction(left,right);
	//////////////////
	left = 1;
	right.clear();right.push_back(4);
	addProduction(left,right);
	///////////////////////// 
} 
