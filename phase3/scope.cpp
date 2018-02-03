#include "scope.h"
#include "symbol.h"
#include <string>
#include <iostream>

Scope::Scope(Scope *enclosing){
    _enclosing = enclosing;
    _symbols = new Symbols();
}
void Scope::insert(Symbol *s){
    if(find(s->getName()) == 0){
        std::cout<<"trying to pushback"<<std::endl;
        _symbols->push_back(s);
    }
}

Symbol *Scope::find(const std::string &name) const{
    //std::cout<<"size: "<<_symbols->size()<<std::endl;
    if(_symbols->size() == 0)
        return NULL;
    for(std::vector<Symbol *>::iterator it = _symbols->begin(); it != _symbols->end(); ++it){
        if((*it)->getName() == name){
            //std::cout<<"found name"<<std::endl;
            return *it;
        } 
    }
    //std::cout<<"not found"<<std::endl;
    return 0;
}

Symbol *Scope::lookup(const string &name) const{
    Symbol *s = find(name);
    if( s == 0)
        if( _enclosing != 0)
            return (_enclosing)->lookup(name);
        else
            return NULL;
    else
        return s;
}

Scope *Scope::enclosing() const{
    return _enclosing;
}

const Symbols &Scope::symbols() const{
    return *_symbols;
}
