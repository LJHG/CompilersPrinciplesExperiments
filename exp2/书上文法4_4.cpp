void initTable(){
	//现在根据一个特定的文法来初始化table 和 TNT试试
	table["E"] = 1; id_string_table[1] = "E";
	table["E'"]= 2; id_string_table[2] = "E'";
	table["T"] = 3; id_string_table[3] = "T";
	table["T'"] = 4; id_string_table[4] = "T'";
	table["F"] = 5; id_string_table[5] = "F";
	table["+"] = 6; id_string_table[6] = "+";
	table["*"] = 7; id_string_table[7] = "*";
	table["id"] = 8; id_string_table[8] = "id";
	table["("] = 9; id_string_table[9] = "(";
	table[")"] = 10; id_string_table[10] = ")";
	table["$"] = 11; id_string_table[11] = "$"; //终止符，我感觉还是要加进来，不然很奇怪 
	 
}

void initTNT(){
	//初始化nonterminal和terminal
	vector<int> nt = {1,2,3,4,5};
	vector<int> t = {6,7,8,9,10,11};
	for(int x:nt) nonTerminal.insert(x);
	for(int x:t) terminal.insert(x);
}

void initProductions(){
	int left;
	vector<int> right;
	//纯手打文法，没办法啊 
	///////////////////
	left = 1;
	right.clear(); right.push_back(3);right.push_back(2);
	addProduction(left,right);
	///////////////////
	left = 2;
	right.clear(); right.push_back(6);right.push_back(3);right.push_back(2);
	addProduction(left,right);
	///////////////////
	left = 2;
	right.clear();
	addProduction(left,right);
	///////////////////
	left = 3;
	right.clear(); right.push_back(5);right.push_back(4);
	addProduction(left,right);
	///////////////////
	left = 4;
	right.clear(); right.push_back(7);right.push_back(5);right.push_back(4);
	addProduction(left,right);
	///////////////////
	left = 4;
	right.clear();
	addProduction(left,right);
	///////////////////
	left = 5;
	right.clear(); right.push_back(9);right.push_back(1);right.push_back(10);
	addProduction(left,right);
	///////////////////
	left = 5;
	right.clear(); right.push_back(8);
	addProduction(left,right);
} 
