#include <string>
#include <iostream>

using namespace std;

int isAlpha(char c){
    return (c >= 'a' && c <= 'z' || c >= '0' && c <= '9' || c >= 'A' && c <= 'Z');
}

string trns(string s) {
    string res = "";
    for(int i = 0; i < s.size()-1; i++) {
        res += s[i];
        if(isAlpha(s[i]) && isAlpha(s[i+1]) || isAlpha(s[i]) && s[i+1] == '('
            || isAlpha(s[i+1]) && s[i] == ')' || isAlpha(s[i+1]) && s[i] == '*')
            res += "&";
    }
    res += s[s.size() - 1];
    return res;
}

int main() {
    string s;
    cin >> s;
    s = trns(s);
    cout << s << endl;
    return 0;
}