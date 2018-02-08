#include "symbol.h"
#include <string>

Symbol::Symbol(const Type &t, std::string name){
    _type = new Type(t);
    _name = name;
    defined = false;
}

Symbol::Symbol(const Type &t, const char* name){
    _type = new Type(t);
    _name = name;
    defined = false;
}

Symbol::Symbol(const Type &t, std::string name, bool def){
    _type = new Type(t);
    _name = name;
    defined = def;
}

Symbol::Symbol(const Type &t, const char* name, bool def){
    _type = new Type(t);
    _name = name;
    defined = def;
}

Symbol::Symbol(const Symbol &t){
    _type = new Type(*(t.getType()));
    _name = t.getName();
    defined = t.defined;
}

Type* Symbol::getType() const{
    return _type;
}

std::string Symbol::getName() const{
    return _name;
}

std::ostream &operator<<(std::ostream &ostr, const Symbol &s){
    ostr<<*(s.getType());
    ostr<<" "<< s.getName();
    if(s.getType()->isFunction()){
        if(s.isDefined()){
            ostr<<" defined";
        } else{
            ostr<<" not defined";
        }
    }
    return ostr;
}

bool Symbol::operator==(const Symbol &rhs) const{
    return _type == rhs._type && _name == rhs._name;
}
bool Symbol::operator!=(const Symbol &rhs) const{
    return !operator==(rhs);
}

bool Symbol::isDefined() const{
    return defined;
}

void Symbol::setDefine(bool in){
    defined = in;
}
