#include "api.hpp"
#include <string>
#include <set>
#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>

std::vector<std::set<std::string>> P1;

void split(std::set<std::string>& R, std::pair <std::set<std::string>, char>& C,
    std::set<std::string>& R1, std::set<std::string>& R2, DFA &dfa) {   // R = R1 + R2

    for(auto ri : R) {
        int flag = 0;
        for(auto ci : C.first) {
            if(dfa.has_trans(ri, C.second))
                if(dfa.get_trans(ri, C.second) == ci){
                    flag = 1;
                    R1.insert(ri);
                }
        }
        if(!flag)
            R2.insert(ri);
    }
}

void replace(std::set<std::string>& R, std::set<std::string>& R1,
    std::set<std::string>& R2, std::vector<std::set<std::string>>& P) {     //R -> R1 R2
    
    
    P1.erase(find(P1.begin(), P1.end(), R));
    P1.push_back(R1);
    P1.push_back(R2);
}

DFA dfa_minim(DFA &d) {
    d.create_state("void");
    for(auto i : d.get_states()){
        for(char c :d.get_alphabet().to_string())
            if(!d.has_trans(i, c))
                d.set_trans(i, c, "void");
    }
    std::vector<std::set<std::string>> P;
    std::set<std::string> states = d.get_states();
    std::set<std::string> final = d.get_final_states();
    std::set<std::string> newstates;
    for(auto i : states) {
        int flag = 1;
        for(auto k : final)
            if(k == i)
                flag = 0;
        if(flag)
            newstates.insert(i);
    }

    if(!final.empty())
        P.push_back(final);
    if(!newstates.empty())
        P.push_back(newstates);

    std::vector<std::pair<std::set<std::string>, char>> S;
    std::string alpha = d.get_alphabet().to_string();

    for(char c : alpha)
        for(auto R : P)
            S.push_back(std::make_pair(R, c));
    P1 = P;
    while(!S.empty()) { //classification
        std::pair <std::set<std::string>, char> C;
        C = S.front();
        S.erase(S.begin());

        for(auto R : P) {
            std::set<std::string> R1;
            std::set<std::string> R2;
            split(R, C, R1, R2, d);
            if(!R1.empty() && !R2.empty()) {
                //std::cout << R1.size() << " " << R2.size() << "\n";
                replace(R, R1, R2, P);
                for(auto c : alpha) {
                    S.push_back(std::make_pair(R1, c));
                    S.push_back(std::make_pair(R2, c));
                }
            }
        }
        P = P1;
    }

    for(auto cls : P) {
        std::string classname = *(cls.begin());
        //std::cout << classname << "\n";
        for(auto state: cls){
            if(classname == state)
                continue;
            for(char c : alpha){ // old (c) -> 
                if(!d.has_trans(state, c))
                    continue;
                std::string tmp = d.get_trans(state, c);
                //std::cout << "DBG  " << tmp << " " << state << " " << c << "\n";
                d.delete_trans(state, c);
                d.set_trans(classname, c, tmp);
            }
            std::set<std::string> tmpstates = d.get_states();
            for(auto k : tmpstates){
                for(char c : alpha){
                    if(!d.has_trans(k, c))
                        continue;
                    if(d.get_trans(k, c) == state){
                        d.delete_trans(k, c);
                        d.set_trans(k, c, classname);
                    }
                }
            }
            if(d.is_initial(state))
                d.set_initial(classname);
            d.delete_state(state);
        }
    }
    
    int deleted = 1;
    while(deleted){
        deleted = 0;
        for(auto state : d.get_states()){
            if(d.is_initial(state))
                continue;
            //std::cout << "state:" << state <<"\n";
            int flag = 0; //isout
            int flag2 = 0; //isin
            for(char c : alpha){        //out node
                if(d.has_trans(state, c))
                    if(d.get_trans(state, c) != state)
                        flag = 1;
            }

            for(auto state2 : d.get_states()){   //in node
                if(state == state2)
                    continue;
                for(char c : alpha){
                    if(!d.has_trans(state2, c))
                        continue;
                    if(d.get_trans(state2, c) == state)
                        flag2 = 1;
                }
            }

            if(!((flag && flag2) || (flag2 && !flag && d.is_final(state)) || (flag && d.is_initial(state)))){
                d.delete_state(state);
                deleted = 1;
            }
        }
    }
/*

    std::string newalpha = "";  //make new alpha

    for(char ch : alpha){
        int fl = 0;
        for (auto state : d.get_states()){
            if(d.has_trans(state, ch)){
                fl = 1;
            }
        }
        if(fl)
            newalpha += ch;
    }

    DFA newd(newalpha);
    //std::cout << newalpha << "\n";
    
    for(auto state : d.get_states()){       //copy states
        newd.create_state(state);
        if(d.is_final(state))
            newd.make_final(state);
        if(d.is_initial(state))
            newd.set_initial(state);
    }

    for(auto state1 : d.get_states()){      //copy trans
        for(auto state2 : d.get_states()){
            for(char c : newalpha){
                if(d.has_trans(state1, c))
                    if(d.get_trans(state1, c) == state2)
                        newd.set_trans(state1, c ,state2);
            }
        }
    }*/
    
    return /*new*/d;
}
