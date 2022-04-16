#include "api.hpp"
#include <string>
#include <iostream>
#include <stack>
#include <algorithm>
#include <vector>

std::string poss[1000];

struct Tree {
	std::string sym;
	std::set <int> first;
	std::set <int> last;
	int nullable; 
	Tree * right;
	Tree * left;
	int id;
};

int isAlpha(char c){
    return (c >= 'a' && c <= 'z' || c >= '0' && c <= '9' || c >= 'A' && c <= 'Z');
}

std::string trns(std::string s) {
    std::string res = "";
    for(int i = 0; i < s.size()-1; i++) {
        res += s[i];
        if(isAlpha(s[i]) && isAlpha(s[i+1]) || isAlpha(s[i]) && s[i+1] == '('
            || isAlpha(s[i+1]) && s[i] == ')' || isAlpha(s[i+1]) && s[i] == '*'
			|| s[i] == ')' && s[i+1] == '(')
            res += "&";
    }
    res += s[s.size() - 1];
    return res;
}

class Syntaxer {
	std::string s;
	Tree * tree;
	std::stack<std::string> RPN;
	std::stack<std::string> RPNrev;
	std::stack<Tree *> stack;
	int cnt;
	int id;
	char ch;
	void setChar() {
		cnt++;
		ch =  s[cnt - 1];
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
		if(ch == '*') {
			setChar();
			RPN.push("*");
		}
	}

	void Q() {
		if(ch == '('){
			setChar();
			S();
			if(ch == ')')
				setChar();
		}
		else if(isAlpha(ch)) {
			std::string tmp = "";
			tmp += ch;
			//std::cout << ch << "\n";
			RPN.push(tmp);
			setChar();
		}
	}

public:
	void start() {
		std::cout << "string: " << s << "\n";
		setChar();
		S();
	}
	Tree * getTree() {return tree;}
	Syntaxer(std::string str) {
		s = trns(str);
		cnt = 0;
		id = 0;
	}
	void makeTree() {
		while(!RPN.empty()) {
			std::string tmp = RPN.top();
			//std::cout << tmp;
			RPN.pop();
			RPNrev.push(tmp);
		}
		/*while(!RPNrev.empty()){
			std::cout << RPNrev.top();
			RPNrev.pop();
		}*/

		while(!RPNrev.empty()) {
			std::string tmp = RPNrev.top();
			RPNrev.pop();
			Tree * node = new Tree;
			if(tmp != "&" && tmp != "*" && tmp != "|" && tmp != ""){
				node -> id = id;
				poss[id] = tmp;
				id++;
			}
			else
				node -> id = -id;
			
			node -> sym = tmp;
			if (tmp == "*") {
				Tree * l = stack.top();
				stack.pop();
				node -> left = l;
				node -> right = nullptr;
				stack.push(node);
			} else if (tmp == "|") {
				Tree * l = stack.top();
				stack.pop();
				Tree * r;
				if(!stack.empty()){
					r = stack.top();
					stack.pop();
				} else {
					r = new Tree;
					r -> sym = "";
					r -> id = -id;
				}
				node -> left = l; 
				node -> right = r;
				stack.push(node);
			} else if (tmp == "&") {
				Tree * r = stack.top();
				stack.pop();
				Tree * l = stack.top();
				stack.pop(); 
				node -> left = l;
				node -> right = r;
				stack.push(node);
			} else {
				node -> right = nullptr;
				node -> left = nullptr;
				stack.push(node);
			}
		}
		tree = stack.top();
		
		Tree * temp = new Tree;
		temp -> left = tree;
		temp -> right = new Tree;
		temp -> sym = "&";
		temp -> id = -100;
		temp -> right -> sym = "#";
		temp -> right -> id = id;
		temp -> right -> left = nullptr;
		temp -> right -> right = nullptr;
		tree = temp;
		poss[id] = "#";
	}
};

void setTree(Tree * tr, std::set<int> follow[]) {
	if(tr -> left)
		setTree(tr -> left, follow);
	if(tr -> right)
		setTree(tr -> right, follow);
	if(tr -> sym == "*") {
		tr -> nullable = 1;
		for(auto i : tr -> left -> first)
			tr -> first.insert(i);
		for(auto i : tr -> left -> last)
			tr -> last.insert(i);

		for(auto i : tr -> left -> last)
			for(auto j : tr -> left -> first){
				follow[i].insert(j);
			}
	} else if(tr -> sym == "|") {
		tr -> nullable = tr -> left -> nullable | tr -> right -> nullable;
		for(auto i : tr -> left -> first)
			tr -> first.insert(i);
		for(auto i : tr -> left -> last)
			tr -> last.insert(i);
		for(auto i : tr -> right -> first)
			tr -> first.insert(i);
		for(auto i : tr -> right -> last)
			tr -> last.insert(i);
	} else if(tr -> sym == "&") {
		tr -> nullable = tr -> left -> nullable * tr -> right -> nullable;
		for(auto i : tr -> left -> first)
			tr -> first.insert(i);
		if(tr -> left -> nullable){
			for(auto i : tr -> right -> first)
				tr -> first.insert(i);
		}
		for(auto i : tr -> right -> last)
			tr -> last.insert(i);
		if(tr -> right -> nullable){
			for(auto i : tr -> left -> last)
				tr -> last.insert(i);
		}

		for(auto i : tr -> left -> last)
			for(auto j : tr -> right -> first){
				follow[i].insert(j);
			}
	} else {
		if(tr -> sym != "") {
			tr -> nullable = 0;
			tr -> last.insert(tr -> id);
			tr -> first.insert(tr -> id);
		}
		else {
			tr -> nullable = 1;
		}
	}
}

void printTree(Tree * tree) {
	if(tree -> left)
		printTree(tree -> left);
	if(tree -> right)
		printTree(tree -> right);
	std :: cout << tree -> sym << " id:" << tree -> id << " " << tree -> nullable << "\n";
	if(tree -> left)
		std::cout << "left: " << tree -> left -> id << "\n";
	if(tree -> right)
		std::cout << "right: " << tree -> right -> id << "\n\n";
	for(auto i : tree -> first)
		std::cout << "first: " << i << "\n";
	for(auto i : tree -> last)
		std::cout << "last: " << i << "\n";
	std::cout << "\n";
}

int lenStr(std::string s) {
	int res = 0;
	for(int i = 0; i < s.length(); i++) {
		if(isAlpha(s[i]))
			res++;
	}
	return res;
}

std::string findIndex(std::vector<std::set<int>> & a, std::set<int> & b) {
	for(int i = 0; i < a.size(); i++)
		if(a[i] == b)
			return std::to_string(i);
	return "-1";
}

DFA re2dfa(const std::string &s) {
	int cnt = 0;
	Tree * tree;
	Syntaxer syntaxer(s);
	std::set<int> follow[lenStr(s) + 1];
	syntaxer.start();
	syntaxer.makeTree();
	tree = syntaxer.getTree();
	setTree(tree, follow);
	//printTree(tree);
	/*for(int i = 0; i < lenStr(s) + 1; i++){
		std:: cout << i << ": ";
		for(auto j : follow[i])
			std::cout << j << " ";
		std::cout << "\n";
	}*/
	std::vector<std::set<int>> stnames;

	DFA res = DFA(Alphabet(s));
	std::set<std::set<int>> states;
	std::set<std::set<int>> marked;
	std::set<int> q;
	for(auto i : tree -> first)
		q.insert(i);
	states.insert(q);
	stnames.push_back(q);
	res.create_state("0");
	res.set_initial("0");
	for(auto i : states){
		if(marked.find(i) != marked.end())
			continue;

		std::set<int> temp;
		for(auto m : i)
			temp.insert(m);
		marked.insert(temp);

		for(auto c : Alphabet(s).to_string()){
			std::set<int> followp;
			for(auto j : i){
				std::string temp = "";
				temp += c;
				if(poss[j] == temp){
					for(auto u : follow[j])
						followp.insert(u);
				}
			}
			if(!followp.empty()){
				if(states.find(followp) == states.end()){
					states.insert(followp);
					stnames.push_back(followp);
					res.create_state(std::to_string(stnames.size() - 1));
				}
				res.set_trans( findIndex(stnames, i), c, findIndex(stnames, followp) );
			}
		}
	}

	/*for(auto i : states){
		for(auto j : i)
			std::cout << j << " ";
		std::cout << "\n";
	}
	for(auto i : stnames){
		for(auto j : i)
			std::cout << j << " ";
		std::cout << "\n";
	}*/

	int kkk = 0;
	while(poss[kkk] != "#")
		kkk++;

	for(auto m : states){
		if(m.find(kkk) != m.end())
			res.make_final(findIndex(stnames, m));
	}
	return res;
}
