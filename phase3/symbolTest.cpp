#include "symbol.h"
#include "type.h"
#include <iostream>

using namespace std;

int main(void){
    Type *t = new Type(INT, 0);
    Type *t1 = new Type(CHAR, 0);
    Symbol *s = new Symbol(*t, "test");
    Symbol *s1 = new Symbol(*t1, "test1");
    cout<<*s<<endl;
    Symbol *s2 = s1;
    cout<< (*s==*s1) <<endl;
    cout<< (*s1==*s1) <<endl;
    cout<< *s2 << endl;
    
}
