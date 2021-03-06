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

void translationUnit(){
    functionGlobal();
}

void globalDeclaratorList(){
    pointers();
    match(IDENTIFIER);
    if( lookahead == LPAREN){
        match(LPAREN);
        parameters();
        match(RPAREN);
    } else if( lookahead == LBRACKET){
        match(LBRACKET);
        match(NUM);
        match(RBRACKET);
    }
    
    if( lookahead == COMMA){
        match(COMMA);
        globalDeclaratorList();
    }
}

/*
 * tu  -> epsilon tu
 *     |  specifier pointers id tu`
 *
 * tu` -> ; tu
 *     |  [num] tu``
 *     |  (params) tu```
 *
 * tu`` -> ; tu
 *      |  , global-declarator-list; tu
 *
 * tu```-> ; tu
 *     |  , global-declarator-list; tu
 *     |  {declarations statements} tu
 */
void functionGlobal(){
    while(1){
        if( lookahead != INT &&
            lookahead != CHAR &&
            lookahead != VOID)
            break;
        specifier();
        pointers();
        match(IDENTIFIER);
        //cout<<"Done with specifier and pointers"<<endl;
        if( lookahead == LPAREN){
            match(LPAREN);
            //cout<<"match lparen in fg"<<endl;
            parameters();
            match(RPAREN);
            //tu```
            if( lookahead == LBRACE){
                match(LBRACE);
                declarations();
                statements();
                match(RBRACE);
            } else if( lookahead == SEMICOLON){
                match(SEMICOLON);
            } else if( lookahead == COMMA){
                match(COMMA);
                globalDeclaratorList();
                match(SEMICOLON);
            }
        } else if( lookahead == LBRACKET){
            match(LBRACKET);
            match(NUM);
            match(RBRACKET);
            //tu``
            if( lookahead == SEMICOLON){
                match(SEMICOLON);
            } else if( lookahead == COMMA){
                match(COMMA);
                globalDeclaratorList();
                match(SEMICOLON);
            }
            
        } else if( lookahead == SEMICOLON){
            match(SEMICOLON);
        }
    }
}

/*
 * paramsPrime -> epsilon
 *             |  , parameters
 *             |  , ...
 */
void paramsPrime(){
    if(lookahead == RPAREN){
        //TODO Check if RPAREN is the proper terminator
        return;
    }else if( lookahead == COMMA){
        match(COMMA);
        if(lookahead == ELLIPSIS){
            match(ELLIPSIS);
            //TODO Remove the cout
            //cout<<"ellipsis test"<<endl;
            return;
        }
        //TODO check to see if you're supposed to call func
        parameters();
    }
}

/*
 * parameters -> void
 *            |  parameter_list()
 * parameters is left factored
 *      -> void
 *      |  int pointers parms`
 *      |  char pointers parms`
 *      |  void pointers parms`
 */
void parameters(){
    if( lookahead == VOID){
        match(VOID);
        if( lookahead == RPAREN){
            return;
        }
    } else if( lookahead == INT){
        match(INT);
    } else if( lookahead == CHAR){
        match(CHAR);
    }
    //cout<<"match specifiers"<<endl;
    pointers();
    match(IDENTIFIER);
    //cout<<"done matching identifiers in parameters"<<endl;
    paramsPrime();
}

void parameter_list(){}

/*
 * parameter -> specifier pointers id
 */
void parameter(){
    specifier();
    pointers();
    match(IDENTIFIER);
}

/*
 * specifier -> int
 *           |  char
 *           |  void
 */
void specifier(){
    if(lookahead == INT){
        match(INT);
        //cout<<"match int in specifier"<<endl;
    }
    else if( lookahead == CHAR){
        match(CHAR);
        //cout<<"match char in specifier"<<endl;
    }
    else if( lookahead == VOID){
        match(VOID);
        //cout<<"match void in specifier"<<endl;
    }
}

/*
 * declarations -> epsilon
 *              |  declaration declarations
 */
void declarations(){
    while(lookahead == INT  ||
          lookahead == CHAR ||
          lookahead == VOID)
        declaration();
}

/*
 * declaration -> specifier declaratior_list ;
 */
void declaration(){
    specifier();
    declarator_list();
    //cout<<"Declaration. Match semicolon"<<endl;
    match(SEMICOLON);
}

/* 
 * declarator_list -> declarator
 *                 |  declarator , declarator_list
 */
void declarator_list(){
    declarator();
    if( lookahead == COMMA){
        match(COMMA);
        declarator_list();
    }
}

/*
 * pointers -> epsilon
 *          |  * pointers
 */
void pointers(){
    if( lookahead == MULTIPLY){
        match(MULTIPLY);
        pointers();
    }
}

/*
 * declarator -> pointers id
 *            |  pointers id [ num ]
 */
void declarator(){
    pointers();
    //cout<<"Identifier in declarator "<< yytext<<endl;
    match(IDENTIFIER);
    
    if( lookahead == LBRACKET){
        match(LBRACKET);
        match(NUM);
        match(RBRACKET);
    }
}

/*
 * statements -> epsilon
 *            |  statement statements
 */
void statements(){
    while(lookahead != RBRACE)
        statement();
}

/*
 * statement -> {declarations statements}
 *           |  break;
 *           |  return expression;
 *           |  while ( expression) statement
 *           |  for(assignment;expression;assignment) statement
 *           |  if(expression) statement
 *           |  if(expression) statement else statement
 *           |  assignment ;
 */
void statement(){
    if(lookahead == LBRACE){
        match(LBRACE);
        declarations();
        //cout<<"done declarations in statement"<<endl;
        statements();
        match(RBRACE);
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
        //cout<<"Done assignment"<<endl;
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
        //cout<<"matching semicolon in statement"<<endl;
        match(SEMICOLON);
    }

}

/*
 *  assignment -> expression=expression
 *             |  expression
 */
void assignment(){
    //cout<<"in assignment"<<endl;
    expr_or();
    if(lookahead == ASSIGN){
        match(ASSIGN);
        expr_or();
    }
}

/*
 * expression_list -> expression
 *                 |  expression , expression_list
 */
void expression_list(){
    expr_or();
    if(lookahead == COMMA){
         match(COMMA);
         expression_list();
    }
}

/*
 * expr_identifier -> id ( expression_list)
 *                 |  id ()
 *                 |  id
 *                 |  num
 *                 |  string
 *                 |  character
 *                 |  ( expr_or )
 */
void expr_identifier(){
    if( lookahead == IDENTIFIER){
        match(IDENTIFIER);
        if( lookahead == LPAREN){
            match(LPAREN);
            if( lookahead == RPAREN){
                match(RPAREN);
            }else{
                expression_list();
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
    }else if( lookahead == LPAREN){
        match(LPAREN);
        expr_or();
        match(RPAREN);
    }
}

/*
 *  Left associative; Left recursive
 *  expr_post -> expr_post[expr_or]
 *            |  expr_identifier
 */
void expr_post(){
    expr_identifier();
    while(lookahead == LBRACKET){
        match(LBRACKET);
        expr_or();
        match(RBRACKET);
        cout<<"index"<<endl;
    }
}

/*
 *  Right associative; Right Associative
 *  expr_pre -> &expr_post
 *           |  *expr_post
 *           |  !expr_post
 *           |  -expr_post
 *           |  sizeof expr_post
 *           |  expr_post
 */
void expr_pre(){
    if( lookahead == ADDR){
        match(ADDR);
        expr_pre();
        cout<<"addr"<<endl;
    }
    //TODO fix precedence issue.
    // Multiply sometime being treated as deref
    else if( lookahead == MULTIPLY){
        match(MULTIPLY);
        expr_pre();
        cout<<"deref"<<endl;
    }
    else if( lookahead == NOT){
        match(NOT);
        expr_pre();
        cout<<"not"<<endl;
    }
    else if( lookahead == SUBTRACT){
        match(SUBTRACT);
        expr_pre();
        cout<<"neg"<<endl;
    }
    else if( lookahead == SIZEOF){
        match(SIZEOF);
        expr_pre();
        cout<<"sizeof"<<endl;
    }
    else{
        expr_post();
    }
}

/*
 *  Left associative; Left recursive
 *  expr_mul -> expr_mul * expr_pre
 *           |  expr_mul / expr_pre
 *           |  expr_mul % expr_pre
 *           |  expr_pre
 */
void expr_mul(){
    expr_pre();
    while(1){
        if( lookahead == MULTIPLY){
            match(MULTIPLY);
            expr_pre();
            cout<<"mul"<<endl;
        }
        else if( lookahead == DIVIDE){
            match(DIVIDE);
            expr_pre();
            cout<<"div"<<endl;
        }
        else if( lookahead == MODULO){
            match(MODULO);
            expr_pre();
            cout<<"rem"<<endl;
        }
        else{
            break;
        }
    }
}

/*
 *  Left associative; Left recursive
 *  expr_add -> expr_add + expr_mul
 *           |  expr_add - expr_mul
 *           |  expr_mul
 */
void expr_add(){
    expr_mul();
    while(1){
        if( lookahead == ADD){
            match(ADD);
            expr_mul();
            cout<<"add"<<endl;
        }
        else if( lookahead == SUBTRACT){
            match(SUBTRACT);
            expr_mul();
            cout<<"sub"<<endl;
        }
        else{
            break;
        }
    }
}

/*
 *  Left associative; Left recursive
 *  expr_cmp -> expr_cmp <  expr_add
 *           |  expr_cmp >  expr_add
 *           |  expr_cmp <= expr_add
 *           |  expr_cmp >= expr_add
 *           |  expr_add
 */
void expr_cmp(){
    expr_add();
    while(1){
        if( lookahead == LESS){
            match(LESS);
            expr_add();
            cout<<"ltn"<<endl;
        }
        else if( lookahead == GREATER){
            match(GREATER);
            expr_add();
            cout<<"gtn"<<endl;
        }
        else if( lookahead == LESSEQUAL){
            match(LESSEQUAL);
            expr_add();
            cout<<"leq"<<endl;
        }
        else if( lookahead == GREATEREQUAL){
            match(GREATEREQUAL);
            expr_add();
            cout<<"geq"<<endl;
        }
        else{
            break;
        }
    }
}

/*
 *  Left associative; Left recursive
 *  expr_eq -> expr_eq == expr_cmp
 *          |  expr_eq != expr_cmp
 *          |  expr_cmp
 */
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

/*
 *  Left associative; Left recursive
 *  expr_and -> expr_and && expr_eq
 *           |  expr_eq
 */
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

/*
 *  Left associative; Left recursive
 *  expr_or -> expr_or || expr_and
 *          |  expr_and
 */
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
        translationUnit();
    }

}
