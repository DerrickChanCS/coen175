# include <cctype>
# include <iostream>
# include "lexer.h"

using namespace std;

int lookahead;
void expr_or();

void error(){
	cout << "There was an error" << endl;
}

void match(int t){
    if( t == lookahead)
        lookahead = yylex();
    else
        error();
}

void expr_identifier(){
    if( lookahead == IDENTIFIER){
        match(IDENTIFIER);
    }
    else if( lookahead == NUM){
        match(NUM);
    }
    else if( lookahead == STRING){
        match(STRING);
    }
    else if( lookahead == CHARACTER){
        match(CHARACTER);
    }
    else if( lookahead == LPAREN){
        match(LPAREN);
        expr_or();
        match(RPAREN);
    }
    else{
        return;
    }
    //need ID(OR) and ID()
}

void expr_k(){
    expr_identifier();
    while(lookahead == LBRACKET){
        match(LBRACKET);
        expr();
        match(RBRACKET);
    }
}

void expr_f(){
    expr_k();
    /*
    while(1){
        if( lookahead == ADDR){
            match(ADDR);
            cout<<"addr"<<endl;
        }
        else if( lookahead == MULTIPLY){
            match(MULTIPLY);
            cout<<"deref"<<endl;
        }
        else if( lookahead == NOT){
            match(NOT);
            cout<<"not"<<endl;
        }
        else if( lookahead == SUBTRACT){
            match(SUBTRACT);
            cout<<"neg"<<endl;
        }
        else if( lookahead == SIZEOF){
            match(SIZEOF);
            cout<<"sizeof"<<endl;
        }
        else{
            break;
        }
    }
    expr_k();
    */
}

void expr_t(){
    expr_f();
    while(1){
        if( lookahead == MULTIPLY){
            match(MULTIPLY);
            expr_f();
            cout<<"mul"<<endl;
        }
        else if( lookahead == DIVIDE){
            match(DIVIDE);
            expr_f();
            cout<<"div"<<endl;
        }
        else if( lookahead == MODULO){
            match(MODULO);
            expr_f();
            cout<<"rem"<<endl;
        }
        else{
            break;
        }
    }
}

void expr_e(){
    expr_t();
    while(1){
        if( lookahead == ADD){
            match(ADD);
            expr_t();
            cout<<"add"<<endl;
        }
        else if( lookahead == SUBTRACT){
            match(SUBTRACT);
            expr_t();
            cout<<"sub"<<endl;
        }
        else{
            break;
        }
    }
}

void expr_cmp(){
    expr_e();
    while(1){
        if( lookahead == LESS){
            match(LESS);
            expr_e();
            cout<<"ltn"<<endl;
        }
        else if( lookahead == GREATER){
            match(GREATER);
            expr_e();
            cout<<"gtn"<<endl;
        }
        else if( lookahead == LESSEQUAL){
            match(LESSEQUAL);
            expr_e();
            cout<<"leq"<<endl;
        }
        else if( lookahead == GREATEREQUAL){
            match(GREATEREQUAL);
            expr_e();
            cout<<"geq"<<endl;
        }
        else{
            break;
        }
    }
}

void expr_eq(){
    expr_cmp();
    while(1){
        if( lookahead == EQUAL){
            match(EQUAL);
            expr_cmp();
            cout<<"eql"<<endl;
        }
        else if( lookahead == NOTEQUAL){
            match(NOTEQUAL);
            expr_cmp();
            cout<<"neq"<<endl;
        }
        else{
            break;
        }
    }
}

void expr_and(){
    expr_eq();
    while(1){
        if( lookahead == AND){
            match(AND);
            expr_eq();
            cout<<"and"<<endl;
        }
        else{
            break;
        }
    }
}

void expr_or(){
    expr_and();
    while(1){
        if( lookahead == OR){
            match(OR);
            expr_and();
            cout<<"or"<<endl;
        }
        else{
            break;
        }
    }
}

int main()
{
    lookahead = yylex();
    /*
    cout<<lookahead<<endl;
    cout<<IDENTIFIER<<endl;
    int a = 1;
    int b = 2;
    int c = 2;
    int d = 4;
    cout<<a+b-d*c<<endl;
    */
    while(lookahead != 0){
        expr_or();
    }

}
