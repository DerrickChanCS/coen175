#include "symbol.h"
#include <string>

Symbol::Symbol(const Type &t, std::string name){
    _type = new Type(t);
    _name = name;
}

Symbol::Symbol(const Type &t, const char* name){
    _type = new Type(t);
    _name = name;
}

Symbol::Symbol(const Symbol &t){
    _type = new Type(*(t.getType()));
    _name = t.getName();
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
    return ostr;
}

bool Symbol::operator==(const Symbol &rhs) const{
    return _type == rhs._type && _name == rhs._name;
}
bool Symbol::operator!=(const Symbol &rhs) const{
    return !operator==(rhs);
}
