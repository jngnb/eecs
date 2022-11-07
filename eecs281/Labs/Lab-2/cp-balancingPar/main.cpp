// Project Identifier: 1CAEF3A0FEDD0DEC26BA9808C69D4D22A9962768

#include <iostream>
#include <algorithm>
#include <stack>
#include <iterator>
#include "xcode_redirect.hpp"
using namespace std;

char correctRhsOf(const char &lhs);
//bool balance(stack<char> &deq);

int main(int argc, char* argv[]) {
    
    ios_base::sync_with_stdio(false);
    xcode_redirect(argc, argv);
    
    stack<char> opens;
    stack<char> closes;
    
    istream_iterator<char> eos;
    istream_iterator<char> iit (cin);
    
    while (iit!=eos){
        
        if (*iit == '(' || *iit == '[' || *iit == '{'){
            opens.push(*iit);
        }
        else if (*iit == ')' || *iit == ']' || *iit == '}'){
            closes.push(*iit);
        }
        
        if (!closes.empty() && opens.empty()){
            cout << "Not balanced";
            return 0;
        }
        else if (!closes.empty() && !opens.empty()){
            if (correctRhsOf(opens.top()) == closes.top()){
                opens.pop();
                closes.pop();
            }
            else if (correctRhsOf(opens.top()) != closes.top()){
                cout << "Not balanced";
                return 0;
            }
        }
        
        ++iit;
    }
    
    if (closes.empty() && opens.empty()){
        cout << "Balanced";
        return 0;
    }
    cout << "Not balanced";
    return 0;

}

char correctRhsOf(const char &lhs){
    if (lhs == '('){
        return ')';
    }
    else if (lhs == '['){
        return ']';
    }
    else{
        return '}';
    }
}

