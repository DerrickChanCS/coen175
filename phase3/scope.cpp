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
        //std::cout<<"trying to pushback"<<std::endl;
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
    return NULL;
}

Symbol *Scope::lookup(const string &name) const{
    Symbol *s = find(name);
    //if symbol is not found
    if( s == NULL)
        if( _enclosing != 0)
            //if there is a parent
            return (_enclosing)->lookup(name);
        else
            return NULL;
    else
        return s;
}

Scope *Scope::enclosing() const{
    return _enclosing;
}

Symbols* Scope::symbols() const{
    return _symbols;
}

std::ostream &operator<<(std::ostream &ostr, const Scope& scope){
    Symbols* symb = (scope).symbols();
    ostr<<"Number of symbols: "<< symb->size() << std::endl;
    for(Symbols::size_type i = 0; i < symb->size(); i++){
        ostr<<"\t"<<*(*symb)[i];
        //ostr<<"\t"<< *((*symb)[i]->getType())<<" "<<((*symb)[i])->getName();
        if( i != symb->size() -1)
            ostr<<std::endl;
    }
    return ostr;
}
