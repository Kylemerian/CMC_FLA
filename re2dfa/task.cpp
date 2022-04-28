#include "api.hpp"
#include <string>
#include <iostream>
#include <stack>
#include <algorithm>
#include <vector>
#include <map>
#include <set>

int cnt = 0;

std::map <int , std::string> poses;

int isAlpha(char c){
    return ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z'));
}

struct Tree {
	std::string sym;
	std::set <int> first;
	std::set <int> last;
	int nullable; 
	Tree * right;
	Tree * left;
	int id;
	Tree(std::string s){
		right = nullptr;
		left = nullptr;
		sym = s;
		id = -1;
		if(isAlpha(s[0]) || s == "#"){
			id = cnt;
			cnt++;
		}
	}
};

class Syntaxer {
	std::string s;
	Tree * tree;
	std::stack<std::string> RPN;
	std::stack<std::string> RPNrev;
	std::stack<Tree *> stack;
	int cntc;
	int id;
	char ch;
	void setChar() {
		cntc++;
		ch =  s[cntc - 1];
	}
	void S() {
		T();
		if(ch == '|') {
			setChar();
			S();
			RPN.push("|");
		}
	}

	void T() {
		V();
		if(ch == '&'){
			setChar();
			T();
			RPN.push("&");
		}
	}

	void V() {
		Q();
		if(ch == '*'){
			setChar();
			RPN.push("*");
		}
	}

	void Q() {
		if(isAlpha(ch) || ch == '^'){
			std::string tmp = "";
			tmp += ch;
			RPN.push(tmp);
			setChar();
		}
		else if(ch == '('){
			setChar();
			S();
			if(ch == ')')
				setChar();
		}
	}

public:
	void start(std::set<int> follow[]) {
		//std::cout << "string: " << s << "\n";
		setChar();
		S();
		while(!RPN.empty()) {
			std::string tmp = RPN.top();
			RPN.pop();
			RPNrev.push(tmp);
		}

		while(!RPNrev.empty()){
			std::string tmp = RPNrev.top();
			RPNrev.pop();

			Tree * node = new Tree(tmp);

			if(node -> sym == "*") {
				Tree * left = stack.top();
				stack.pop();
				node -> left = left;
				node -> nullable = 1;
				for(auto i : node -> left -> first)
					node -> first.insert(i);
				for(auto i : node -> left -> last)
					node -> last.insert(i);
				
				for(auto i : node -> left -> last)
					for(auto j : node -> left -> first)
						follow[i].insert(j);
			}
			else if(node -> sym == "|"){
				Tree * right = stack.top();
				stack.pop();
				node -> right = right;
				Tree * left = stack.top();
				stack.pop();
				node -> left = left;
				node -> nullable = node -> left -> nullable | right -> nullable;
				/**/
				for(auto i : node -> left -> first)
					node -> first.insert(i);
				for(auto i : node -> left -> last)
					node -> last.insert(i);
				for(auto i : node -> right -> first)
					node -> first.insert(i);
				for(auto i : node -> right -> last)
					node -> last.insert(i);
			}
			else if(node -> sym == "&"){
				Tree * right = stack.top();
				stack.pop();
				node -> right = right;
				Tree * left = stack.top();
				stack.pop();
				node -> left = left;
				node -> nullable = node -> right -> nullable * node -> left -> nullable;
				/**/
				for(auto i : node -> left -> first)
					node -> first.insert(i);
				if(node -> left -> nullable)
					for(auto i : node -> right -> first)
						node -> first.insert(i);
				
				for(auto i : node -> right -> last)
					node -> last.insert(i);
				if(node -> right -> nullable)
					for(auto i : node -> left -> last)
						node -> last.insert(i);

				for(auto i : node -> left -> last)
					for(auto j : node -> right -> first)
						follow[i].insert(j);
				
			}
			else if(node -> sym == "^"){
				node -> nullable = 1;
			}else if(isAlpha(node -> sym[0])){
				node -> nullable = 0;
				node -> first.insert(node -> id);
				node -> last.insert(node -> id);
			}
			
			stack.push(node);
		}

		tree = stack.top();
		
	}

	Syntaxer(std::string s1){
		s = s1;
		cntc = 0;
		id = 0;
	}

	Tree * getTree(std::set<int> follow[]){
		Tree * tr = new Tree("#");
		tr -> nullable = 0;
		tr -> last.insert(tr -> id);
		tr -> first.insert(tr -> id);
		Tree * node = new Tree("&");
		node -> nullable = 0;
		node -> left = tree;
		node -> right = tr;
		for(auto i : node -> left -> first)
			node -> first.insert(i);
		if(node -> left -> nullable)
			for(auto i : node -> right -> first)
				node -> first.insert(i);
				
		for(auto i : node -> right -> last)
			node -> last.insert(i);
		if(node -> right -> nullable)
			for(auto i : node -> left -> last)
				node -> last.insert(i);

		for(auto i : node -> left -> last)
			for(auto j : node -> right -> first)
				follow[i].insert(j);
		return node;
	}

	void printTree(Tree * n) {
		std::cout << n -> id << " " << n -> sym << " " << n -> nullable;
		if(n -> left)
			std::cout << " left: " << n -> left -> id << " right: ";
		if(n -> right)
			std::cout << n -> right -> id << " ";
		std::cout << " \t\t\tfirst: ";
		for(auto i : n -> first)
			std::cout << i << " ";
		std::cout << "||| ";
		std::cout << "last: ";
		for(auto i : n -> last)
			std::cout << i << " ";
		std::cout << "\n";
		
		if(n -> left)
			printTree(n -> left);
		if(n -> right)
			printTree(n -> right);
	}
};

std::string trans(std::string s1){
	std::string res = "";
    for(int i = 0; i < s1.size()-1; i++) {
        res += s1[i];
        if(    (isAlpha(s1[i]) && isAlpha(s1[i+1]))
			|| ((isAlpha(s1[i]) || s1[i] == '^') && s1[i+1] == '(')
            || ((isAlpha(s1[i+1]) || s1[i+1] == '^') && s1[i] == ')')
			|| ((isAlpha(s1[i+1]) || s1[i+1] == '^') && s1[i] == '*')
			|| (s1[i] == ')' && s1[i+1] == '(')
			|| (s1[i] == '*' && s1[i+1] == '('))
            res += "&";
    }
    res += s1[s1.size() - 1];
    return res;
}

std::string addEps(std::string t){
	std::string res = "";
	for(int i = 0; i < t.size()-1; i++){
		res += t[i];
		if(t[i] == '(' && t[i+1] == ')'
			|| t[i] == '|' && t[i+1] == ')'
			|| t[i] == '(' && t[i+1] == '|'
			|| t[i] == '|' && t[i+1] == '|'
			|| t[i] == '*' && t[i+1] == '*'
			|| t[i] == '|' && t[i+1] == '*'
			|| t[i] == '(' && t[i+1] == '*')
			res += '^';
	}
    res += t[t.size() - 1];
    return res;
}

void setPoses(std::string s) {
	int id = 0;
	for(char c : s){
		if(isAlpha(c)){
			std::string tmp = "";
			tmp += c;
			poses[id] = tmp;
			id++;
		}
	}
	poses[id] = "#";

	/*
	for(auto i : poses){
		std::cout << i.first << " ";
		for(auto j : i.second)
			std::cout << j << " ";
		std::cout << "\n";
	}*/
}

int isExistState(std::set<std::pair<std::string, std::set<int>>> & states,
	std::pair<std::string, std::set<int>>& followp){

	int res = 0;
	for(auto i : states){
		if(followp.second == i.second){
			res = 1;
			break;
		}
	}
	return res;
}

int lenStr(std::string s){
	int res = 0;
	for(char c : s)
		if(isAlpha(c))
			res++;
	return res;
}

std::string findName(std::set<std::pair<std::string, std::set<int>>> & states,
	std::pair<std::string, std::set<int>>& followp){

	for(auto i : states){
		if(followp.second == i.second)
			return i.first;
	}
}

DFA re2dfa(const std::string &s) {
	int stname = 0;
	Syntaxer syntaxer(trans(addEps(s)));
	setPoses(s);
	std::set<int> follow[lenStr(s)]; //+1
	syntaxer.start(follow);
	Tree * tree = syntaxer.getTree(follow);
	//syntaxer.printTree(tree);
	DFA res = DFA(Alphabet(s));

	std::pair<std::string, std::set<int>> q;
	q.first = std::to_string(stname);
	for(auto i : tree -> first)
		q.second.insert(i);
	stname++;
	
	

	std::set<std::pair<std::string, std::set<int>>> states;
	std::set<std::pair<std::string, std::set<int>>> marked;
	states.insert(q);
	res.create_state(q.first);
	res.set_initial(q.first);
	for(int iii = 0; iii < 2; iii++)
		for(auto i : states){
			if(marked.find(i) != marked.end())
				continue;

			std::pair<std::string, std::set<int>> temp;
			for(auto m : i.second)
				temp.second.insert(m);
			temp.second = i.second;
			marked.insert(temp);

			for(auto c : Alphabet(s).to_string()){
				std::pair<std::string, std::set<int>> followp;
				for(auto j : i.second){
					std::string tstr = "";
					tstr += c; 
					if(poses[j] == tstr){
						for(auto u : follow[j])
							followp.second.insert(u);
					}
				}

				if(!followp.second.empty()){
					if(!isExistState(states, followp)){
						followp.first = std::to_string(stname);
						stname++;
						states.insert(followp);
						res.create_state(followp.first);
						std::cout << "create: " << followp.first << "\n";
					}
					std::cout << i.first << " " << c << " " << findName(states, followp) << "\n";
					res.set_trans(i.first, c, findName(states, followp));
				}
			}
			
		}

	for(auto i : follow){
		for(auto j : i)
			std::cout << j << " ";
		std::cout << "\n";
	}

	for(auto i : states){
		if(i.second.find(cnt - 1) != i.second.end())
			res.make_final(i.first);
	}

	return res;
}
