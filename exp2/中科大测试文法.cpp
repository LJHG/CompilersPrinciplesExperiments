void initTable(){
	//现在根据一个特定的文法来初始化table 和 TNT试试
	table["X"] = 1; id_string_table[1] = "X";
	table["Y"] = 2; id_string_table[2] = "Y";
	table["Z"] = 3; id_string_table[3] = "Z";
	table["a"] = 4; id_string_table[4] = "a";
	table["c"] = 5; id_string_table[5] = "c";
	table["d"] = 6; id_string_table[6] = "d";
}

void initTNT(){
	//初始化nonterminal和terminal 
	nonTerminal.insert(1);nonTerminal.insert(2);nonTerminal.insert(3);
	terminal.insert(4);terminal.insert(5);terminal.insert(6);	
}

void initProductions(){
	int left;
	vector<int> right;
	//纯手打文法，没办法啊 
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
	right.clear(); //右部为空，什么都不push 
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
