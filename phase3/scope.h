#ifndef SCOPE_H
#define SCOPE_H
#include "symbol.h"
#include <ostream>

typedef std::vector<Symbol *> Symbols;
class Scope{
    typedef std::string string;
    Scope *_enclosing;
    Symbols *_symbols;
public:
    Scope(Scope *enclosing = 0);
    void insert(Symbol *s);
    Symbol *find(const string &name) const;
    Symbol *lookup(const string &name) const;
    Scope *enclosing() const;
    Symbols* symbols() const;
};

std::ostream &operator<<(std::ostream &ostr, const Scope &scope);

#endif
