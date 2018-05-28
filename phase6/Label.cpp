#include "Label.h"
#include <iostream>

using namespace std;

unsigned Label::_counter = 0;
Label::Label(){
    _number = _counter++;
}

unsigned Label::number() const{ return _number;}

ostream& operator<<(ostream &os, const Label &lbl){
    return os<<".L"<<lbl.number();
}
