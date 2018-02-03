#include "type.h"
#include "tokens.h"
#include <iostream>

Type::Type(){
    _specifier = -1;
    _indirection = -1;
    _kind = ERROR;
    _length = 0;
    _parameters = new Parameters();
}

Type::Type(int specifier, unsigned indirection){
    _specifier = specifier;
    _indirection = indirection;
    _kind = SCALAR;
    _length = 0;
    _parameters = new Parameters();
}

Type::Type(int specifier, unsigned indirection, unsigned length){
    _specifier = specifier;
    _indirection = indirection;
    _kind = ARRAY;
    _length = length;
    _parameters = new Parameters();
}

Type::Type(int specifier, unsigned indirection, Parameters *params){
    _specifier = specifier;
    _indirection = indirection;
    _kind = FUNCTION;
    _length = 0;
    _parameters = params;
}

Type::Type(const Type &t){
    //std::cout<<"in copy constructor"<<std::endl;
    //std::cout<<"trying to copy "<<t<<std::endl;
    _specifier = t._specifier;
    _indirection = t._indirection;
    _kind = t._kind;
    _length = t._length;
    //std::cout<<"copy from size " << (t._parameters)->size()<<std::endl;
    _parameters = t._parameters;
        //std::cout<<"in the for"<<std::endl;
        //Type p = (*t._parameters)[i];
        //std::cout<<p<<std::endl;
        //std::cout<<t._parameters[i]<<std::endl;
}

Type::Type(int ellipsis){
    _specifier = -1;
    _indirection = 0;
    _kind = DOTS;
    _length = 0;
    _parameters = new Parameters();
}

int Type::getSpecifier() const{
    return _specifier;
}

unsigned Type::length() const{
    return _length;
}

bool Type::isError() const{
    return _kind==ERROR;
}

bool Type::isScalar() const{
    return _kind==SCALAR;
}

bool Type::isArray() const{
    return _kind==ARRAY;
}

bool Type::isFunction() const{
    return _kind==FUNCTION;
}

bool Type::isEllipsis() const{
    return _kind==DOTS;
}

unsigned Type::indirection() const{
    return _indirection;
}

Parameters* Type::parameters() const{
    return _parameters;
}

bool Type::operator==(const Type &rhs) const{
    if(_kind != rhs._kind)
        return false;
    if(_kind == ERROR)
        return true;
    if(_specifier != rhs._specifier)
        return false;
    if(_indirection != rhs._indirection)
        return false;
    if(_kind == ARRAY)
        if(_length != rhs._length)
            return false;
    if(_kind == FUNCTION)
        return *_parameters == *rhs._parameters;
    return true;
}

bool Type::operator!=(const Type &rhs) const{
    return !operator==(rhs);
}

std::ostream &operator<<(std::ostream &ostr, const Type &t){
    unsigned i;
    if(t.isError()){
        ostr<<"error";
    } else{
        if(t.getSpecifier() == INT)
            ostr<<"int";
        else if(t.getSpecifier() == VOID)
            ostr<<"void";
        else if(t.getSpecifier() == CHAR)
            ostr<<"char";
        if(t.isArray()){
            ostr<<"["<<t.length()<<"]";
        }
        if(t.indirection() > 0){
            ostr<<" ";
            for(i=0; i< t.indirection(); i++){ 
                ostr<<"*";
            }
        }
        if(t.isFunction()){
            ostr<<"(";
            if(t.parameters()->size() == 0)
                ostr<<"void";
            else
                for(i = 0; i < t.parameters()->size(); i++){
                    if(i > 0)
                        ostr<<",";
                    ostr<<(*t.parameters())[i];
                }
            ostr<<")";
        }
        if(t.isEllipsis()){
            ostr<<"...";
        }
    }
    return ostr;
}
