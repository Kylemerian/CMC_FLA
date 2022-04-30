#include "api.hpp"
#include <string>
#include <set>
#include <algorithm>
#include <iostream>

std::set<std::string> setQ(std::string s, std::set<std::pair<std::pair<std::string, std::string>, std::string>> d) {
    std::set<std::string> res;
    for(auto i : d)
        if(i.first.second == s && i.first.first != s)
            res.insert(i.first.first);
    return res;
}

std::set<std::string> setP(std::string s, std::set<std::pair<std::pair<std::string, std::string>, std::string>> &d) {
    std::set<std::string> res;
    for(auto i : d)
        if(i.first.first == s && i.first.second != s)
            res.insert(i.first.second);
    return res;
}

std::string findQP(std::string Q, std::string P, std::set<std::pair<std::pair<std::string, std::string>, std::string>> d){
    for(auto i : d)
        if(i.first.first == Q && i.first.second == P)
            return i.second;
}

int isS(std::string S, std::set<std::pair<std::pair<std::string, std::string>, std::string>> d){
    int res = 0;
    for(auto i : d)
        if(i.first.first == S && i.first.second == S)
            res = 1;
    return res;
}

int isQP(std::string Q, std::string P, std::set<std::pair<std::pair<std::string, std::string>, std::string>> d){
    int res = 0;
    for(auto i : d)
        if(i.first.first == Q && i.first.second == P)
            res = 1;
    return res;
    
}

void deleteQP(std::string Q, std::string P, std::set<std::pair<std::pair<std::string, std::string>, std::string>> d){
    for(auto i : d){
        if(i.first.first == Q && i.first.second == P){
            d.erase(i);
            return;
        }
    }
}

std::string dfa2re(DFA &d) {
    
    std::set<std::pair<std::pair<std::string, std::string>, std::string>> trans;

    for(auto s1 : d.get_states()){
        for(auto s2 : d.get_states()){
            std::string tr = "";
            for(char c : d.get_alphabet().to_string())
                if(d.has_trans(s1, c))
                    if(d.get_trans(s1, c) == s2)
                        tr = tr + "|" + c;
            
            if (tr != ""){
                tr.erase(0, 1);
                trans.insert( std::make_pair( std::make_pair(s1, s2), tr));
            }
        }
    }
    std::set<std::string> states = d.get_states();
    std::set<std::string> notfin;
    for(auto i : states){
        if(d.is_final(i))
            continue;
        notfin.insert(i);
    }

    while(!notfin.empty()){
        std::set<std::string> q = setQ(*notfin.begin(), trans);
        std::set<std::string> p = setP(*notfin.begin(), trans);

        int s = isS(*notfin.begin(), trans);
        for(auto i : q){
            for(auto j : p){
                int qp = isQP(i, j, trans);
                if(s && qp){

                }
                if(s && !qp){

                }
                if(!s && qp){

                }
                if(!s && !qp){

                }
            }
        }

        notfin.erase(*notfin.begin());
    } 
    

    return d.to_string();
}
