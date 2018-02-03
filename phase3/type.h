#ifndef TYPE_H
#define TYPE_H
#include <ostream>
#include <vector>
#include "tokens.h"

typedef std::vector<class Type> Parameters;

class Type{
    int _specifier;
    unsigned _indirection;
    enum{ARRAY, SCALAR, FUNCTION, ERROR, DOTS} _kind;
    unsigned _length;
    Parameters *_parameters;
public:
    Type();
    Type(int specifier, unsigned indirection);
    Type(int specifier, unsigned indirection, unsigned length);
    Type(int specifier, unsigned indirection, Parameters *params);
    Type(int ellipsis);
    Type(const Type &t);
    //make copy constructor
    bool operator==(const Type &rhs) const;
    bool operator!=(const Type &rhs) const;
    int getSpecifier() const;
    bool isError() const;
    bool isScalar() const;
    bool isArray() const;
    bool isFunction() const;
    bool isEllipsis() const;
    unsigned indirection() const;
    int kind() const;
    unsigned length() const;
    Parameters *parameters() const;
};

std::ostream &operator<<(std::ostream &ostr, const Type &type);
#endif
