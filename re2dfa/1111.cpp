#include <iostream>

using namespace std;

int main(){
    string s = "c|*()";
    string t = "c|*()";
    for(char c : s)
        for(char m : t)
            cout << c << " & " << m << endl;
}



c & c +
c & ( + 
* & c +
* & ( +
) & c +
) & ( +

