#include<iostream>
#include<unordered_map>
#include<unordered_set>
#include<vector>

using namespace std;

int main(){
	unordered_map<int,unordered_set<int>> m; 
	m[0].insert(1);
	m[0].insert(2);
	m[1].insert(2);
	for(auto x:m){
		for(auto xx:x.second){
			cout<<xx<<" ";
		}
		cout<<endl;
	}
}
