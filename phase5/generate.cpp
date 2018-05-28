#include "Tree.h"
#include <iostream>
#include <vector>
#include "tokens.h"

using namespace std;

extern Scope *outermost, *toplevel;

bool isGlobal(string name){
    return outermost->find(name) != NULL;
}

int calcOffset(vector<Symbol *> s, int params){
    int total = 0;
    int paramOffset = 4;
    for(int i = 0; i<(int)s.size(); i++){
        if(i < params){
            paramOffset += s[i]->type().size();
            s[i]->offset = paramOffset;
        } else if(!isGlobal(s[i]->name())){
            Type t = s[i]->type();
            total += t.size();
            s[i]->offset = -total;
        }
        //cout<<s[i]->name()<<" "<<s[i]->offset<<endl;
    }
    return total;
}

void Call::generate(){
    int size = (int)_args.size();
    int paramOffset = 0;
    for(int i = 0; i<size;i++){
        cout<<"\tpushl\t";
        _args[size-1-i]->generate();
        paramOffset += _args[size-1-i]->type().size();
        cout<<endl;
    }
    cout<<"\tcall\t"<<_id->name()<<endl;
    cout<<"\taddl\t$"<<paramOffset<<", %esp"<<endl;
}

void Identifier::generate(){
    if(_symbol->type().isScalar()){
        if(isGlobal(_symbol->name())){
            cout<<_symbol->name();
        }else {
            cout<<_symbol->offset<<"(%ebp)";
        }
    }
}

void Number::generate(){
    cout<<"$"<<_value;
}

void Assignment::generate(){
    cout<<"\tmovl\t";
    _right->generate();
    cout<<", ";
    _left->generate();
    cout<<endl;
}

void Block::generate(){
    for(int i = 0; i<(int)_stmts.size();i++){
        _stmts[i]->generate(); 
    }
}

void Function::generate(){
    int offset = 0;
    vector<Symbol *> localVars = _body->declarations()->symbols();
    int numParams = (int)_id->type().parameters()->_types.size();
    int localSize = calcOffset(localVars, numParams);
    //cout<<"local var size "<<localSize<<endl;
    offset = localSize;
    cout<<".globl\t"<<_id->name()<<endl;
    cout<<_id->name()<<":"<<endl;;
    cout<<"\tpushl\t%ebp"<<endl;
    cout<<"\tmovl\t%esp, %ebp"<<endl;
    cout<<"\tsubl\t$"<<offset<<", %esp"<<endl;

    _body->generate();
    
    cout<<"\tmovl\t%ebp, %esp"<<endl;
    cout<<"\tpopl\t%ebp"<<endl;
    cout<<"\tret"<<endl;
}
