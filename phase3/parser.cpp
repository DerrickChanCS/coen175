/*
 * File:	parser.c
 *
 * Description:	This file contains the public and private function and
 *		variable definitions for the recursive-descent parser for
 *		Simple C.
 */

# include <cstdlib>
# include <iostream>
# include "tokens.h"
# include "lexer.h"
# include <string>
# include "scope.h"
# include <vector>
# include "checker.cpp"

using namespace std;

static int lookahead;
static void expression();
static void statement();

//static Scope* currentScope;

typedef std::vector<Symbol *> Symbols;
typedef std::vector<Type> Types;


/*
 * Function:	error
 *
 * Description:	Report a syntax error to standard error.
 */

static void error()
{
    if (lookahead == DONE)
	    report("syntax error at end of file");
    else
	    report("syntax error at '%s'", yytext);

    exit(EXIT_FAILURE);
}


/*
 * Function:	match
 *
 * Description:	Match the next token against the specified token.  A
 *		failure indicates a syntax error and will terminate the
 *		program since our parser does not do error recovery.
 */

static void match(int t)
{
    if (lookahead != t)
	    error();

    lookahead = yylex();
}


/*
 * Function:	isSpecifier
 *
 * Description:	Return whether the given token is a type specifier.
 */

static bool isSpecifier(int token)
{
    //cout<<"specifier token "<< token<< endl;
    //cout<<"ENUM int is: "<<INT<<endl;
    return token == INT || token == CHAR || token == VOID;
}


/*
 * Function:	specifier
 *
 * Description:	Parse a type specifier.  Simple C has only ints, chars, and
 *		void types.
 *
 *		specifier:
 *		  int
 *		  char
 *		  void
 */

static int specifier()
{
    int type = 0;
    //cout<<"In specifier"<<endl;
    if (isSpecifier(lookahead)){
        //cout<<type<<endl;
        type = lookahead;
	    match(lookahead);
        return type;
    }
    else{
        cout<<lookahead<<endl;
	    error();
        return -1;
    }
}


/*
 * Function:	pointers
 *
 * Description:	Parse pointer declarators (i.e., zero or more asterisks).
 *
 *		pointers:
 *		  empty
 *		  * pointers
 */

static unsigned pointers()
{
    unsigned count = 0;
    while (lookahead == '*'){
	    match('*');
        count++;
    }
    return count;
}


/*
 * Function:	declarator
 *
 * Description:	Parse a declarator, which in Simple C is either a scalar
 *		variable or an array, with optional pointer declarators.
 *
 *		declarator:
 *		  pointers identifier
 *		  pointers identifier [ num ]
 */

static void declarator(int typespec)
{
    char *ptr;
    unsigned indirection = pointers();
    string id = yytext;
    match(ID);

    if (lookahead == '[') {
        match('[');
        unsigned len = strtoul(yytext,&ptr,0);
        match(NUM);
        match(']');
        Type *t = new Type(typespec, indirection, len);
        Symbol *s = new Symbol(*t, id);
        checkError(s);
        //cout<<"Typespec: "<<typespec<<" Indirection: "<<indirection<<" ID: "<<id<<" length: "<<len<<endl;
    } else{
        Type *t = new Type(typespec, indirection);
        Symbol *s = new Symbol(*t, id);
        //cout<<"current scope "<<currentScope<<endl;
        checkError(s);
        //(*currentScope).insert(s);
        //cout<<"Typespec: "<<typespec<<"Indirection: "<<indirection<<" ID: "<<id<<endl;
    }
}


/*
 * Function:	declaration
 *
 * Description:	Parse a local variable declaration.  Global declarations
 *		are handled separately since we need to detect a function
 *		as a special case.
 *
 *		declaration:
 *		  specifier declarator-list ';'
 *
 *		declarator-list:
 *		  declarator
 *		  declarator , declarator-list
 */

static void declaration()
{
    int type = specifier();
    declarator(type);

    while (lookahead == ',') {
        match(',');
        declarator(type);
    }

    match(';');
}


/*
 * Function:	declarations
 *
 * Description:	Parse a possibly empty sequence of declarations.
 *
 *		declarations:
 *		  empty
 *		  declaration declarations
 */

static void declarations()
{
    while (isSpecifier(lookahead))
        declaration();
}


/*
 * Function:	primaryExpression
 *
 * Description:	Parse a primary expression.
 *
 *		primary-expression:
 *		  ( expression )
 *		  identifier ( expression-list )
 *		  identifier ( )
 *		  identifier
 *		  character
 *		  string
 *		  num
 *
 *		expression-list:
 *		  expression
 *		  expression , expression-list
 */

static void primaryExpression()
{
    if (lookahead == '(') {
        match('(');
        expression();
        match(')');

    } else if (lookahead == CHARACTER) {
        match(CHARACTER);

    } else if (lookahead == STRING) {
        match(STRING);

    } else if (lookahead == NUM) {
        match(NUM);

    } else if (lookahead == ID) {
        string name = yytext;
	    match(ID);
        checkDeclaration(name);

        if (lookahead == '(') {
            match('(');

            if (lookahead != ')') {
                expression();

                while (lookahead == ',') {
                    match(',');
                    expression();
                }
            }

            match(')');
	    }

    } else
        error();
}


/*
 * Function:	postfixExpression
 *
 * Description:	Parse a postfix expression.
 *
 *		postfix-expression:
 *		  primary-expression
 *		  postfix-expression [ expression ]
 */

static void postfixExpression()
{
    primaryExpression();

    while (lookahead == '[') {
	match('[');
	expression();
	match(']');
	cout << "index" << endl;
    }
}


/*
 * Function:	prefixExpression
 *
 * Description:	Parse a prefix expression.
 *
 *		prefix-expression:
 *		  postfix-expression
 *		  ! prefix-expression
 *		  - prefix-expression
 *		  * prefix-expression
 *		  & prefix-expression
 *		  sizeof prefix-expression
 */

static void prefixExpression()
{
    if (lookahead == '!') {
	match('!');
	prefixExpression();
	cout << "not" << endl;

    } else if (lookahead == '-') {
	match('-');
	prefixExpression();
	cout << "neg" << endl;

    } else if (lookahead == '*') {
	match('*');
	prefixExpression();
	cout << "deref" << endl;

    } else if (lookahead == '&') {
	match('&');
	prefixExpression();
	cout << "addr" << endl;

    } else if (lookahead == SIZEOF) {
	match(SIZEOF);
	prefixExpression();
	cout << "sizeof" << endl;

    } else
	postfixExpression();
}


/*
 * Function:	multiplicativeExpression
 *
 * Description:	Parse a multiplicative expression.  Simple C does not have
 *		cast expressions, so we go immediately to prefix
 *		expressions.
 *
 *		multiplicative-expression:
 *		  prefix-expression
 *		  multiplicative-expression * prefix-expression
 *		  multiplicative-expression / prefix-expression
 *		  multiplicative-expression % prefix-expression
 */

static void multiplicativeExpression()
{
    prefixExpression();

    while (1) {
	if (lookahead == '*') {
	    match('*');
	    prefixExpression();
	    cout << "mul" << endl;

	} else if (lookahead == '/') {
	    match('/');
	    prefixExpression();
	    cout << "div" << endl;

	} else if (lookahead == '%') {
	    match('%');
	    prefixExpression();
	    cout << "rem" << endl;

	} else
	    break;
    }
}


/*
 * Function:	additiveExpression
 *
 * Description:	Parse an additive expression.
 *
 *		additive-expression:
 *		  multiplicative-expression
 *		  additive-expression + multiplicative-expression
 *		  additive-expression - multiplicative-expression
 */

static void additiveExpression()
{
    multiplicativeExpression();

    while (1) {
	if (lookahead == '+') {
	    match('+');
	    multiplicativeExpression();
	    cout << "add" << endl;

	} else if (lookahead == '-') {
	    match('-');
	    multiplicativeExpression();
	    cout << "sub" << endl;

	} else
	    break;
    }
}


/*
 * Function:	relationalExpression
 *
 * Description:	Parse a relational expression.  Note that Simple C does not
 *		have shift operators, so we go immediately to additive
 *		expressions.
 *
 *		relational-expression:
 *		  additive-expression
 *		  relational-expression < additive-expression
 *		  relational-expression > additive-expression
 *		  relational-expression <= additive-expression
 *		  relational-expression >= additive-expression
 */

static void relationalExpression()
{
    additiveExpression();

    while (1) {
	if (lookahead == '<') {
	    match('<');
	    additiveExpression();
	    cout << "ltn" << endl;

	} else if (lookahead == '>') {
	    match('>');
	    additiveExpression();
	    cout << "gtn" << endl;

	} else if (lookahead == LEQ) {
	    match(LEQ);
	    additiveExpression();
	    cout << "leq" << endl;

	} else if (lookahead == GEQ) {
	    match(GEQ);
	    additiveExpression();
	    cout << "geq" << endl;

	} else
	    break;
    }
}


/*
 * Function:	equalityExpression
 *
 * Description:	Parse an equality expression.
 *
 *		equality-expression:
 *		  relational-expression
 *		  equality-expression == relational-expression
 *		  equality-expression != relational-expression
 */

static void equalityExpression()
{
    relationalExpression();

    while (1) {
	if (lookahead == EQL) {
	    match(EQL);
	    relationalExpression();
	    cout << "eql" << endl;

	} else if (lookahead == NEQ) {
	    match(NEQ);
	    relationalExpression();
	    cout << "neq" << endl;

	} else
	    break;
    }
}


/*
 * Function:	logicalAndExpression
 *
 * Description:	Parse a logical-and expression.  Note that Simple C does
 *		not have bitwise-and expressions.
 *
 *		logical-and-expression:
 *		  equality-expression
 *		  logical-and-expression && equality-expression
 */

static void logicalAndExpression()
{
    equalityExpression();

    while (lookahead == AND) {
	match(AND);
	equalityExpression();
	cout << "and" << endl;
    }
}


/*
 * Function:	expression
 *
 * Description:	Parse an expression, or more specifically, a logical-or
 *		expression, since Simple C does not allow comma or
 *		assignment as an expression operator.
 *
 *		expression:
 *		  logical-and-expression
 *		  expression || logical-and-expression
 */

static void expression()
{
    logicalAndExpression();

    while (lookahead == OR) {
        match(OR);
        logicalAndExpression();
        cout << "or" << endl;
    }
}


/*
 * Function:	statements
 *
 * Description:	Parse a possibly empty sequence of statements.  Rather than
 *		checking if the next token starts a statement, we check if
 *		the next token ends the sequence, since a sequence of
 *		statements is always terminated by a closing brace.
 *
 *		statements:
 *		  empty
 *		  statement statements
 */

static void statements()
{
    while (lookahead != '}')
        statement();
}


/*
 * Function:	Assignment
 *
 * Description:	Parse an assignment statement.
 *
 *		assignment:
 *		  expression = expression
 *		  expression
 */

static void assignment()
{
    expression();

    if (lookahead == '=') {
        match('=');
        expression();
    }
}


/*
 * Function:	statement
 *
 * Description:	Parse a statement.  Note that Simple C has so few
 *		statements that we handle them all in this one function.
 *
 *		statement:
 *		  { declarations statements }
 *		  break ;
 *		  return expression ;
 *		  while ( expression ) statement
 *		  for ( assignment ; expression ; assignment ) statement
 *		  if ( expression ) statement
 *		  if ( expression ) statement else statement
 *		  assignment ;
 */

static void statement()
{
    if (lookahead == '{') {
        openScope();
        match('{');
        declarations();
        statements();
        match('}');
        closeScope();

    } else if (lookahead == BREAK) {
        match(BREAK);
        match(';');

    } else if (lookahead == RETURN) {
        match(RETURN);
        expression();
        match(';');

    } else if (lookahead == WHILE) {
        match(WHILE);
        match('(');
        expression();
        match(')');
        statement();

    } else if (lookahead == FOR) {
        match(FOR);
        match('(');
        assignment();
        match(';');
        expression();
        match(';');
        assignment();
        match(')');
        statement();

    } else if (lookahead == IF) {
        match(IF);
        match('(');
        expression();
        match(')');
        statement();

        if (lookahead == ELSE) {
            match(ELSE);
            statement();
        }

    } else {
        assignment();
        match(';');
    }
}


/*
 * Function:	parameter
 *
 * Description:	Parse a parameter, which in Simple C is always a scalar
 *		variable with optional pointer declarators.
 *
 *		parameter:
 *		  specifier pointers identifier
 */

static Symbol* parameter()
{
    int s = specifier();
    int p = pointers();
    std::string name = yytext;
    match(ID);
    Type *t = new Type(s,p);
    //(*currentScope)->insert(new Symbol(*t,name));
    //return *t;
    return new Symbol(*t, name);
}


/*
 * Function:	parameters
 *
 * Description:	Parse the parameters of a function, but not the opening or
 *		closing parentheses.
 *
 *		parameters:
 *		  void
 *		  void pointers identifier remaining-parameters
 *		  char pointers identifier remaining-parameters
 *		  int pointers identifier remaining-parameters
 *
 *		remaining-parameters:
 *		  empty
 *		  , ...
 *		  , parameter remaining-parameters
 */

static Symbols* parameters()
{
    int s;
    //Parameters* params = new Parameters();
    Symbols* functionSymbols = new Symbols();
    if (lookahead == VOID) {
	    match(VOID);

	if (lookahead == ')')
        //if the function has void params return empty symbol vector
	    return functionSymbols;
    } else
	    s = specifier();

    int p = pointers();
    std::string name = yytext;
    match(ID);
    
    Type *t = new Type(s,p);
    Symbol* temp = new Symbol(*t, name);
    
    //params->push_back(t);
    functionSymbols->push_back(temp);

    while (lookahead == ',') {
        match(',');

        if (lookahead == ELLIPSIS) {
            //create a new type and name to match the ellipsis
            match(ELLIPSIS);
            t = new Type(ELLIPSIS);
            name = "";
            temp = new Symbol(*t, name);
            functionSymbols->push_back(temp);
            //params->push_back(parameter());
            break;
        }

        functionSymbols->push_back(parameter());
       // params->push_back(parameter());
    }
    return functionSymbols;
}


/*
 * Function:	globalDeclarator
 *
 * Description:	Parse a declarator, which in Simple C is either a scalar
 *		variable, an array, or a function, with optional pointer
 *		declarators.
 *
 *		global-declarator:
 *		  pointers identifier
 *		  pointers identifier [ num ]
 *		  pointers identifier ( parameters )
 */

static void globalDeclarator(int spec)
{
    int p = pointers();
    string name = yytext;
    match(ID);

    Symbol *symb;
    Type* t;

    if (lookahead == '[') {
        match('[');
        int length = atoi(yytext);
        match(NUM);
        match(']');

        t = new Type(spec, p, length);
        symb = new Symbol(*t, name);

    } else if (lookahead == '(') {
        match('(');
        Symbols* params = parameters();
        Types* types = new Types();
        if(params == NULL){
            //params is null
        }else{
            // extracts the types from the symbols
            for(Symbols::size_type i = 0; i < params->size(); i++){
                Type *t = (*params)[i]->getType();
                types->push_back(*t);
            }
        }
        t = new Type(spec, p, types);
        symb = new Symbol(*t, name);
        match(')');

    } else{
        t = new Type(spec, p);
        symb = new Symbol(*t, name);
    }
    
    checkError(symb);
}


/*
 * Function:	remainingDeclarators
 *
 * Description:	Parse any remaining global declarators after the first.
 *
 * 		remaining-declarators
 * 		  ;
 * 		  , global-declarator remaining-declarators
 */

static void remainingDeclarators(int spec)
{
    while (lookahead == ',') {
        match(',');
        globalDeclarator(spec);
    }

    match(';');
}


/*
 * Function:	topLevelDeclaration
 *
 * Description:	Parse a global declaration or function definition.
 *
 * 		global-or-function:
 * 		  specifier pointers identifier remaining-decls
 * 		  specifier pointers identifier [ num ] remaining-decls
 * 		  specifier pointers identifier ( parameters ) remaining-decls 
 * 		  specifier pointers identifier ( parameters ) { ... }
 */

static void topLevelDeclaration()
{
    int s = specifier();
    int p = pointers();
    string name = yytext;
    match(ID);
    //cout<<"my name is "<<name<<endl;
    //Type *t;
    Symbol *symb;

    if (lookahead == '[') {
        match('[');
        int length = atoi(yytext);
        match(NUM);
        match(']');
    
        //make a global array
        Type* t = new Type(s, p, length);
        Symbol* symb = new Symbol(*t, name);
        
        //insert global array into the current scope
        checkError(symb);
        //(*currentScope).insert(symb);

        remainingDeclarators(s);

    } else if (lookahead == '(') {
        //cout<<"in function"<<endl; 
        match('(');
        //get the parameters from the function 
        Symbols* params = parameters();
        Types* types = new Types();
        if(params == NULL){
            //cout<<"params is null"<<endl;
        }else{
            //cout<<"params size "<<params->size()<<endl;
            // push the types into vector
            // vector of types used for function definition
            for(Symbols::size_type i = 0; i < params->size(); i++){
                Type *t = (*params)[i]->getType();
                types->push_back(*t);
            }
        }
        
        //declare the function type
        Type* func = new Type(s, p, types);
        symb = new Symbol(*func, name);
        //insert into the current scope
        checkError(symb);
        //if(!E1(symb))
        //    (*currentScope).insert(symb);
        
        match(')');
        if (lookahead == '{') {
            /*
            cout<<"\nfunction definition; creating new scope"<<endl;
            Scope *functionScope = new Scope(currentScope);
            cout<<"current scope "<<currentScope<<endl;
            cout<<"enclsoing scope "<<(*functionScope).enclosing();
            currentScope = functionScope;
            */
            //open the scope and set defined = true
            openScope();
            setDefined(name);
            if(params == NULL){
                //cout<<"params is null"<<endl;
            }else{
                //cout<<"params size "<<params->size()<<endl;
                //cout<<"putting function parameters into current scope"<<endl;
                for(Symbols::size_type i = 0; i < params->size(); i++){
                    //iterate through parameters
                    Symbol *s = (*params)[i];
                    checkError(s);
                    //(*currentScope).insert(s);
                }
            }
             //create new scope
            match('{');
            
            declarations();
            statements();
            match('}');
            //close scope
            closeScope();
            /*
            cout<<*currentScope<<endl;
            currentScope = (*currentScope).enclosing();
            */

        } else
            remainingDeclarators(s);

    } else{
        Type* t = new Type(s, p);
        Symbol* symb = new Symbol(*t, name);
        checkError(symb);
        remainingDeclarators(s);
    }
}


/*
 * Function:	main
 *
 * Description:	Analyze the standard input stream.
 */

int main()
{
    lookahead = yylex();
    openGlobalScope();
    //cout<<"Open global scope"<<endl;
    //Scope *global = new Scope(0);
    //currentScope = global;
   // cout<<yytext<<endl;
  //  cout<<"lookahead at beginnign: "<<lookahead<<endl;
 //   cout<<"OR: "<<OR<<endl;
//    cout<<"DOT: "<<DOT<<endl;

    while (lookahead != DONE)
	    topLevelDeclaration();
    cout<<"done parsing"<<endl;
    cout<<*currentScope<<endl;

    cout<<"Close global scope"<<endl;

    exit(EXIT_SUCCESS);
}
