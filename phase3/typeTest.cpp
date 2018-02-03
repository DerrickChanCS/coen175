#include "type.h"
#include "tokens.h"
#include <iostream>

using namespace std;

int main(void){
    Type* t = new Type(INT, 0);
    Type* ta= new Type(CHAR, 1);
    Type* tb= new Type(VOID, 1);
    Type* t1 = new Type(CHAR, 5, 10);
    Type* tarray = new Type(INT, 0, 10);
    Type* ell = new Type(ELLIPSIS);
    cout<<*ell<<endl;
    //Type* cp = new Type(*t);

    Parameters *p = new Parameters();
    cout<<*t<<endl;
    p->push_back(*ta);
    //cout<<"pushed ta"<<endl;
    //cout<<"p size "<<p->size()<<endl;
    //cout<<"p[0] "<< (*p)[0] <<endl;
    cout<<"done pushing ta\n\n\n"<<endl;
    p->push_back(*t);
    cout<<"done pushing t"<<endl;
    //cout<<"pushed t"<<endl;
    //cout<<"p size "<<p->size()<<endl;
    //cout<<"p[1] "<< (*p)[1] <<endl;
    Type* func = new Type(VOID, 0, p);
    t = new Type(INT, 1);
    cout<<"test addr "<<*t<<endl;
    cout<<"test addr "<<*func<<endl;
    cout<<*func<<endl;
    cout<<*tarray<<endl;
}
