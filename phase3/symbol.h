#ifndef SYMBOL_H
#define SYMBOL_H
#include "type.h"
#include <string>
#include <ostream>

class Symbol{
    Type *_type;
    std::string _name;
    bool isDefined;
public:
    Symbol(const Type &t, std::string name);
    Symbol(const Type &t, const char* name);
    Symbol(const Symbol &t);
    bool operator==(const Symbol &rhs) const;
    bool operator!=(const Symbol &rhs) const;
    Type* getType() const;
    std::string getName() const;
};

std::ostream &operator<<(std::ostream &ostr, const Symbol &symbol);

#endif
