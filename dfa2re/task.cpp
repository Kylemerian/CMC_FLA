#include "api.hpp"
#include <string>
#include <set>
#include <algorithm>
#include <iostream>
/*
std::set<std::string> setQ(std::string s, std::set<std::pair<std::pair<std::string, std::string>, std::string>> d) {
    std::set<std::string> res;
    for(auto i : d)
        if(i.first.second == s && i.first.first != s)
            res.insert(i.first.first);
    return res;
}

std::set<std::string> setP(std::string s, std::set<std::pair<std::pair<std::string, std::string>, std::string>> d) {
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

void deleteQP(std::string Q, std::string P, std::set<std::pair<std::pair<std::string, std::string>, std::string>>& d){
    for(auto i : d){
        if(i.first.first == Q && i.first.second == P){
            int a = d.erase(i);
            return;
        }
    }
}
*/
std::string dfa2re(DFA &d) {
    std::cout << "was hwereewrrere\n";

    /*
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
    std::set<std::string> fin;
    for(auto i : states){
        if(d.is_final(i))
            fin.insert(i);
        else
            notfin.insert(i);
    }
    std::cout << "was hrere\n";
-*/
/*
    while(notfin.size() != 1){
        std::set<std::string> q = setQ(*notfin.begin(), trans);
        std::set<std::string> p = setP(*notfin.begin(), trans);

        int s = isS(*notfin.begin(), trans);
        for(auto i : q){
            for(auto j : p){
                int qp = isQP(i, j, trans);
                if(s && qp){
                    std::string fir = findQP(i ,j, trans);
                    std::string sec = findQP(i, *notfin.begin(), trans) + "(" + findQP(*notfin.begin(), *notfin.begin(), trans) + ")*" + findQP(*notfin.begin(), j, trans);
                    deleteQP(i, j, trans);
                    trans.insert(std::make_pair(std::make_pair(i, j), fir + "|" + sec));
                    deleteQP(i, *notfin.begin(), trans);
                    deleteQP(*notfin.begin(), j, trans);
                }
                if(s && !qp){
                    trans.insert(std::make_pair(std::make_pair(i, j), findQP(i, *notfin.begin(), trans) + "(" + findQP(*notfin.begin(), *notfin.begin(), trans ) + ")*" + findQP(*notfin.begin(), j, trans)));
                    deleteQP(i, *notfin.begin(), trans);
                    deleteQP(*notfin.begin(), j, trans);
                }
                if(!s && qp){
                    std::string fir = findQP(i ,j, trans);
                    std::string sec = findQP(i, *notfin.begin(), trans) + findQP(*notfin.begin(), j, trans);
                    deleteQP(i, j, trans);
                    trans.insert(std::make_pair(std::make_pair(i, j), fir + "|" + sec));
                    deleteQP(i, *notfin.begin(), trans);
                    deleteQP(*notfin.begin(), j, trans);
                }
                if(!s && !qp){
                    trans.insert(std::make_pair(std::make_pair(i, j), findQP(i, *notfin.begin(), trans) + findQP(*notfin.begin(), j, trans)));
                    deleteQP(i, *notfin.begin(), trans);
                    deleteQP(*notfin.begin(), j, trans);
                }
            }
        }
        std::cout << "was hrere\n";

        notfin.erase(*notfin.begin());
    } 
    std::cout << "was hrere\n";

    fin.insert(*notfin.begin());*/
    /*
    while(fin.size() != 1){
        std::set<std::string> q = setQ(*fin.begin(), trans);
        std::set<std::string> p = setP(*fin.begin(), trans);

        int s = isS(*fin.begin(), trans);
        for(auto i : q){
            for(auto j : p){
                int qp = isQP(i, j, trans);
                if(s && qp){
                    std::string fir = findQP(i ,j, trans);
                    std::string sec = findQP(i, *fin.begin(), trans) + "(" + findQP(*fin.begin(), *fin.begin(), trans) + ")*" + findQP(*fin.begin(), j, trans);
                    deleteQP(i, j, trans);
                    trans.insert(std::make_pair(std::make_pair(i, j), fir + "|" + sec));
                    deleteQP(i, *fin.begin(), trans);
                    deleteQP(*fin.begin(), j, trans);
                }
                if(s && !qp){
                    trans.insert(std::make_pair(std::make_pair(i, j), findQP(i, *fin.begin(), trans) + "(" + findQP(*fin.begin(), *fin.begin(), trans ) + ")*" + findQP(*fin.begin(), j, trans)));
                    deleteQP(i, *fin.begin(), trans);
                    deleteQP(*fin.begin(), j, trans);
                }
                if(!s && qp){
                    std::string fir = findQP(i ,j, trans);
                    std::string sec = findQP(i, *fin.begin(), trans) + findQP(*fin.begin(), j, trans);
                    deleteQP(i, j, trans);
                    trans.insert(std::make_pair(std::make_pair(i, j), fir + "|" + sec));
                    deleteQP(i, *fin.begin(), trans);
                    deleteQP(*fin.begin(), j, trans);
                }
                if(!s && !qp){
                    trans.insert(std::make_pair(std::make_pair(i, j), findQP(i, *fin.begin(), trans) + findQP(*fin.begin(), j, trans)));
                    deleteQP(i, *fin.begin(), trans);
                    deleteQP(*fin.begin(), j, trans);
                }
            }
        }

        fin.erase(*fin.begin());
    }
*//*
    std::cout << "was hrere\n";
    for(auto i : trans){
        //if(i.first.first == *fin.begin() || i.first.second == *fin.begin() || i.first.first == *notfin.begin() || i.first.second == *notfin.begin())
            std::cout << i.first.first << " " << i.first.second << " " << i.second << "\n";
          //  return i.second;
    }

    for(auto i : fin){
        std::cout << "fin" << i << " ";
    }
    for(auto i : notfin){
        std::cout << "notfin" << i << " ";
    }
*/
    return "a";
}
