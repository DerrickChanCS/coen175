#include "scope.h"
#include <iostream>

using namespace std;

int main(void){
    Scope *s = new Scope(0);
    Scope *s1 = new Scope(s);
    Type *t = new Type(INT, 0);
    Symbol *symb = new Symbol(*t, "test");
    Symbol *symb1 = new Symbol(*t, "newname");
    (*s).insert(symb);
    (*s1).insert(symb1);
    Symbol *temp = &(*s->find("test"));
    cout<<"trying to find test in *s"<<endl;
    if( temp != 0)
        cout<< *temp << endl;
    else    
        cout<< "not found" << endl;
    
    cout<<"trying to find t in *s"<<endl;
    temp = &(*s->find("t"));
    if( temp != 0)
        cout<< *temp << endl;
    else    
        cout<< "not found" << endl;

    temp = &(*s->lookup("test"));
    cout<<"trying to lookup test"<<endl;
    if(temp != 0)
        cout<< *temp <<endl;
    else
        cout<<" not found" << endl;
}
