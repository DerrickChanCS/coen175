# include <cctype>
# include <iostream>
# include "lexer.h"
# include "lexfunctions.h"

using namespace std;

int lookahead;
void expr_or();

void error(){
	cout << "There was an error" << endl;
}

void globalDeclarator(){}

void match(int t){
    if( t == lookahead){
        lookahead = yylex();
        //cout<<yytext<<endl;
    }
    else
        error();
}

void fgprime(){
    if( lookahead == SEMICOLON){
        match(SEMICOLON);
        globalDeclarator();
        fgprime();
    }
}

void functionGlobal(){
    specifier();
    pointers();
    match(IDENTIFIER);
    if( lookahead == LPAREN){
        match(LPAREN);
        parameters();
        match(RPAREN);
        if( lookahead == LBRACE){
            match(LBRACE);
            declarations();
            statements();
            match(RBRACE);
        } else {
            fgprime();
        }
    } else if( lookahead == LBRACKET){
        match(LBRACKET);
        match(NUM);
        match(RBRACKET);
        fgprime();
    } else{
        fgprime();
    }
}

void translationUnit(){
    //if( lookahead ==
}

void paramsPrime(){
    if(lookahead == RPAREN){
        //TODO Check if RPAREN is the proper terminator
        return;
    }else if( lookahead == COMMA){
        match(COMMA);
        if(lookahead == ELLIPSIS){
            match(ELLIPSIS);
            //TODO Remove the cout
            cout<<"ellipsis test"<<endl;
            return;
        }
        //TODO check to see if you're supposed to call func
        parameters();
    }
}

void parameters(){
    if(lookahead == VOID){
        match(VOID);
        if(lookahead == RPAREN)
            //TODO: check if RPAREN is the proper terminator
            return;
    }
    else{
        if(lookahead == CHAR){
            match(CHAR);
        } else if( lookahead == INT){
            match(INT);
        }
        pointers();
        paramsPrime();
    }
}

void parameterList(){
    parameter();
    if(lookahead == COMMA){
        match(COMMA);
        parameterList();
    }
}

void parameter(){
    specifier();
    pointers();
    match(IDENTIFIER);
}

void specifier(){
    if(lookahead == INT)
        match(INT);
    else if( lookahead == CHAR)
        match(CHAR);
    else if( lookahead == VOID)
        match(VOID);
    else
        cout<<"err in specifier"<<endl;
}

void declarations(){
    while(lookahead == INT  ||
          lookahead == CHAR ||
          lookahead == VOID)
        declaration();
}

void declaration(){
    specifier();
    declaratorList();
    match(SEMICOLON);
}

void declaratorList(){
    declarator();
    if( lookahead == COMMA){
        match(COMMA);
        declaratorList();
    }
}

void pointers(){
    if( lookahead == MULTIPLY){
        match(MULTIPLY);
        pointers();
    }
}

void declarator(){
    pointers();
    match(IDENTIFIER);
    if( lookahead == LBRACKET){
        match(LBRACKET);
        match(NUM);
        match(RBRACKET);
    }
}

void statements(){
    while(lookahead != RBRACE)
        statement();
}

void statement(){
    if(lookahead == LBRACE){
        declarations();
        statements();
    } else if( lookahead == BREAK){
        match(BREAK);
        match(SEMICOLON);
    } else if( lookahead == RETURN){
        match(RETURN);
        expr_or();
    } else if( lookahead == WHILE){
        match(WHILE);
        match(LPAREN);
        expr_or();
        match(RPAREN);
        statement();
    } else if( lookahead == FOR){
        match(FOR);
        match(LPAREN);
        assignment();
        match(SEMICOLON);
        expr_or();
        match(SEMICOLON);
        assignment();
        match(RPAREN);
        statement();
    } else if( lookahead == IF){
        match(IF);
        match(LPAREN);
        expr_or();
        match(RPAREN);
        statement();
        if( lookahead == ELSE){
            match(ELSE);
            statement();
        }
    } else {
        assignment();
        match(SEMICOLON);
    }

}

void assignment(){
    expr_or();
    if(lookahead == ASSIGN){
        expr_or();
    }
}

void expressionList(){
    expr_or();
    if(lookahead == COMMA){
         match(COMMA);
         expressionList();
    }
}

void expr_identifier(){
    if( lookahead == IDENTIFIER){
        match(IDENTIFIER);
        if( lookahead == LPAREN){
            match(LPAREN);
            if( lookahead == RPAREN){
                match(RPAREN);
            }else{
                expressionList();
                match(RPAREN);
            }
        }
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
        expr_or();
        match(RBRACKET);
        cout<<"index"<<endl;
    }
}

void expr_f(){
    while(1){
        if( lookahead == ADDR){
            match(ADDR);
            expr_f();
            cout<<"addr"<<endl;
        }
        else if( lookahead == MULTIPLY){
            match(MULTIPLY);
            expr_f();
            cout<<"deref"<<endl;
        }
        else if( lookahead == NOT){
            match(NOT);
            expr_f();
            cout<<"not"<<endl;
        }
        else if( lookahead == SUBTRACT){
            match(SUBTRACT);
            expr_f();
            cout<<"neg"<<endl;
        }
        else if( lookahead == SIZEOF){
            match(SIZEOF);
            expr_f();
            cout<<"sizeof"<<endl;
        }
        else{
            break;
        }
    }
    expr_k();
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
