#include "scope.h"
#include <string>

static Scope* currentScope;

std::string conflicting   = "conflicting types for '%s'";
std::string redefinition  = "redefinition of '%s'";
std::string redeclaration = "redeclaration of '%s'";
std::string undeclared    = "'%s' undeclared";
std::string voidType      = "'%s' has type void";

static void openGlobalScope(){
    std::cout<<"OPEN GLOBAL"<<std::endl;
    Scope *global = new Scope(0);
    currentScope = global;
}

static void openScope(){
    std::cout<<"OPEN SCOPE"<<std::endl;
    Scope* functionScope = new Scope(currentScope);
    currentScope = functionScope;
}

static void closeScope(){
    std::cout<<"CLOSE SCOPE"<<std::endl;
    std::cout<<*currentScope<<std::endl;
    std::cout<<std::endl;
    currentScope = (*currentScope).enclosing();
}

static void setDefined(std::string name){
    Symbol *s = currentScope->find(name);
    if(s->isDefined()){
        //report(redefinition, name);
    } else{
        currentScope->find(name)->setDefine(true);
    }
}

static void checkDeclaration(std::string name){
    Symbol* temp = currentScope->lookup(name);
    
    std::cout<<"check decl "<<name<<std::endl;
    if( temp == NULL){
        Type* t = new Type();
        Symbol* s = new Symbol(*t, name);
        currentScope->insert(s);
        report(undeclared, name);
    }
}

static void checkError(Symbol* symb){
    Symbol* temp = (*currentScope).find((*symb).getName());
    if( temp == NULL){
        Type* t = symb->getType();
        //std::cout<<"VOID is "<<VOID<<std::endl;
        //std::cout<<"spec is "<<t->getSpecifier()<<std::endl;
        if( t->getSpecifier() == VOID && !t->indirection() && !t->isFunction()){
            //E5
            report(voidType, (*symb).getName());
            Type* t = new Type();
            Symbol* s = new Symbol(*t, symb->getName());
            currentScope->insert(s);
            return;
        }
        std::cout<<"inserting "<< symb->getName()<<std::endl;
        (*currentScope).insert(symb);
    } else if( (*currentScope).enclosing() != NULL){
        //currentScope is local
        // E3
        report(redeclaration, (*symb).getName());
    } else if( *(symb->getType()) != *(temp->getType())){
        //If the types do not match
        // E1
        report(conflicting, symb->getName());
    } else if( symb->getType()->isFunction() && symb->isDefined()){
        // E2
        report(redefinition, symb->getName());
    }
}
