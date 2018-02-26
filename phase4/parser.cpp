/*
 * File:	parser.c
 *
 * Description:	This file contains the public and private function and
 *		variable definitions for the recursive-descent parser for
 *		Simple C.
 */

# include <cstdlib>
# include <iostream>
# include "checker.h"
# include "tokens.h"
# include "lexer.h"
# include <string>

using namespace std;

static int lookahead;
static Type expression(bool &lvalue);
static void statement();
static int loopCount;

string E1 = "break statement not within loop";
string E2 = "invalid return type";
string E3 = "invalid type for test expression";
string E4 = "lvalue required in expression";
string E5 = "invalid operands to binary %s";
string E6 = "invalid operand to unary %s";
string E7 = "called object is not a function";
string E8 = "invalid arguments to called function";

Type t_int = Type(INT, 0);
Type func_return = Type();


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
 * Function:	expect
 *
 * Description:	Match the next token against the specified token, and
 *		return its lexeme.  We must save the contents of the buffer
 *		from the lexical analyzer before matching, since matching
 *		will advance to the next token.
 */

static string expect(int t)
{
    string buf = yytext;
    match(t);
    return buf;
}


/*
 * Function:	number
 *
 * Description:	Match the next token as a number and return its value.
 */

static unsigned number()
{
    int value;


    value = strtoul(expect(NUM).c_str(), NULL, 0);
    return value;
}


/*
 * Function:	isSpecifier
 *
 * Description:	Return whether the given token is a type specifier.
 */

static bool isSpecifier(int token)
{
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
    int typespec = ERROR;


    if (isSpecifier(lookahead)) {
	typespec = lookahead;
	match(lookahead);
    } else
	error();

    return typespec;
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


    while (lookahead == '*') {
	match('*');
	count ++;
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
    unsigned indirection;
    string name;


    indirection = pointers();
    name = expect(ID);

    if (lookahead == '[') {
	match('[');
	declareVariable(name, Type(typespec, indirection, number()));
	match(']');
    } else
	declareVariable(name, Type(typespec, indirection));
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
    int typespec;


    typespec = specifier();
    declarator(typespec);

    while (lookahead == ',') {
	match(',');
	declarator(typespec);
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

static Type primaryExpression(bool &lvalue)
{
    /*
    cout<<"type test"<<endl;
    Type t1 = Type(INT, 0);
    Type t2 = Type(INT, 0);
    cout<< (t1.promote() == t2.promote()) <<endl;
    cout<< (t1.promote() == Type(INT,0)) <<endl;
    cout<<"end type test"<<endl;
    */
    Type t;
    if (lookahead == '(') {
        match('(');
        t = expression(lvalue);
        match(')');

    } else if (lookahead == CHARACTER) {
        match(CHARACTER);
        t = Type(CHAR, 0);
        lvalue = false;

    } else if (lookahead == STRING) {
        string str = yytext;
        match(STRING);
        t = Type(CHAR, 0, str.length());
        lvalue = false;

    } else if (lookahead == NUM) {
        match(NUM);
        t = Type(INT, 0);
        lvalue = false;

    } else if (lookahead == ID) {
        Symbol* s = checkIdentifier(expect(ID));
        t = s->type();
        cout<<"checked name "<<s->name()<<"; Type: "<<t<<endl;
        if(t.isScalar()){
            lvalue = true;
        }else{
            lvalue = false;
        }

        if (lookahead == '(') {
            bool isFunc = t.isFunction();
            if(!isFunc){
                report(E7);
                t = Type();
            }
            match('(');
            vector<class Type> params = vector<class Type>();
            //cerr<<"func: "<<t<<endl;
            if (lookahead != ')') {
                t = expression(lvalue);
                params.push_back(t.promote());
                //cerr<<"read in: "<<t.promote()<<endl;

                while (lookahead == ',') {
                    match(',');
                    t = expression(lvalue);
                    params.push_back(t.promote());
                }
            }
            if(isFunc){
                vector<class Type> temp = vector<class Type>();
                temp = s->type().parameters()->_types;
                //temp has the types defined in the symbol table
                if(s->type().parameters()->_varargs){
                    params.resize(temp.size());
                }
                bool isOkay = true;
                for(int i=0; i<(int)params.size(); i++){
                    if(!temp[i].isCompatible(params[i])){
                    //if(!temp[i].isCompatible(params[i]) || !params[i].isCompatible(temp[i]) ){
                        report(E8);
                        t = Type();
                        isOkay = false;
                        break;
                    }
                    isOkay = true;
                }
                if(isOkay){
                    lvalue = false;
                    t = Type(s->type().specifier(), s->type().indirection());
                }
            }
            match(')');
            cout<<"end of function call "<<t<<endl;
        }

    } else
        error();
    return t;
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

static Type postfixExpression(bool &lvalue)
{
    Type left = primaryExpression(lvalue);
    Type ret = left;
    while (lookahead == '[') {
        match('[');
        Type right = expression(lvalue);
        match(']');
        if(left.isError() || right.isError()){
            return ret;
        }
        Type temp = left.promote();
        cout<<"index :"<<temp<<endl;
        if(temp.isScalar() && temp.indirection() > 0){
            if(temp.indirection() == 1 && temp.specifier() == VOID){
                return Type();
            }
            temp = right.promote();
            if(right == Type(INT,0)){
                //cerr<<"array is "<<left<<endl;
                ret = Type(left.specifier(), left.promote().indirection()-1);
                lvalue = true;
            }else{
                report(E5,"[]");
                return Type();
            }
        }else{
            report(E5,"[]");
            return Type();
        }
    }
    return ret;
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

static Type prefixExpression(bool &lvalue)
{
    Type right;
    Type ret = right;
    if (lookahead == '!') {
        match('!');
        right = prefixExpression(lvalue);
        if(right.isError())
            return ret;
        if(!right.isValue())
            report(E6, "!");
        else {
            ret = Type(INT, 0); cout<<"!: ";
            lvalue = false;
        }
        cout<<"! :";
        cout<<right<<"->"<<ret<<endl;
    } else if (lookahead == '-') {
        match('-');
        right = prefixExpression(lvalue);
        Type r1 = right.promote();
        if(right.isError())
            return ret;
        if(r1.specifier() != INT || r1.indirection()!=0){
            report(E6, "-");
        } else{
            ret = Type(INT,0);
            lvalue = false;
        }
        cout<<"- : ";
        cout<<right<<" -> "<<ret<<endl;
    } else if (lookahead == '*') {
        match('*');
        right = prefixExpression(lvalue);
        if(right.isError())
            return ret;
        cout<<"before promote: "<<right<<endl;
        Type r1 = right.promote();
        cout<<"deref: "<<r1<<endl;
        if(r1.indirection() < 1){
            report(E6, "*");
            ret = Type();
        }else{
            lvalue = true;
            ret = Type(r1.specifier(), r1.indirection()-1);
            cout<<"returning: "<<ret<<endl;
        }
        cout<<"*: ";
        cout<<right<<" -> "<<ret<<endl;
    } else if (lookahead == '&') {
        match('&');
        right = prefixExpression(lvalue);
        if(right.isError())
            return ret;
        if(!lvalue){
            report(E4);
            return ret;
        } else{
            lvalue = false;
            ret = Type(right.specifier(), right.indirection()+1);
        }
        cout<<"&: ";
        cout<<right<<" -> "<<ret<<endl;
    } else if (lookahead == SIZEOF) {
        match(SIZEOF);
        right = prefixExpression(lvalue);
        if(right.isError())
            return ret;
        if(!right.promote().isValue())
            report(E6, "sizeof");
        else{
            lvalue = false;
            ret = Type(INT,0);
        }
    } else{
        right = postfixExpression(lvalue);
        if(right.isError()){
            return ret;
        }else{
            ret = right;
        }
    }
    return ret;
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

static Type multiplicativeExpression(bool &lvalue)
{
    Type left = prefixExpression(lvalue);
    Type ret = left;
    //cout<<left<<endl;

    while (1) {
	if (lookahead == '*') {
	    match('*');
	    Type right = prefixExpression(lvalue);
        Type l1 = left.promote();
        Type r1 = right.promote();
        if(l1.isError() || r1.isError()){
            ret = Type();
            continue;
        }
        if(l1 == r1 && r1 == t_int && l1.indirection() == 0 ){
            ret = Type(INT,0);
            cout<<"*: ";
            cout<<"int x int -> int"<<endl;
            lvalue = false;
            left = ret;
        }else{
            report(E5,"*");
            left = Type();
            ret = Type();
        }

	} else if (lookahead == '/') {
	    match('/');
	    Type right = prefixExpression(lvalue);
        Type l1 = left.promote();
        Type r1 = right.promote();
        if(l1.isError() || r1.isError()){
            ret = Type();
            continue;
        }
        if(l1 == r1 && r1 == t_int && l1.indirection() == 0 ){
            ret = Type(INT,0);
            cout<<"/: ";
            cout<<"int x int -> int"<<endl;
            lvalue = false;
            left = ret;
        }else{
            report(E5,"/");
            left = Type();
            ret = Type();
        }

	} else if (lookahead == '%') {
	    match('%');
	    Type right = prefixExpression(lvalue);
        Type l1 = left.promote();
        Type r1 = right.promote();
        if(l1.isError() || r1.isError()){
            ret = Type();
            continue;
        }
        if(l1 == r1 && r1 == t_int && l1.indirection() == 0 ){
            ret = Type(INT,0);
            cout<<"%: ";
            cout<<"int x int -> int"<<endl;
            lvalue = false;
            left = ret;
        }else{
            report(E5,"%");
            left = Type();
            ret = Type();
        }

	} else
	    break;
    }
    return ret;
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

static Type additiveExpression(bool &lvalue)
{
    Type left = multiplicativeExpression(lvalue);
    Type ret = left;
    while (1) {
        if (lookahead == '+') {
            match('+');
            lvalue = false;
            Type right = multiplicativeExpression(lvalue);
            Type l1 = left.promote();
            Type r1 = right.promote();
            cout<<"addition checking: "<<left<<" and "<<right<<endl;
            if(left.isError() || right.isError()){
                ret = Type();
                continue;
            }
            if(l1 == Type(INT,0) && r1== Type(INT,0)){
                ret = Type(INT,0);
                left = ret;
                cout<<"+: ";
                cout<<"int x int -> int";
                cout<<" Returned: "<<ret<<endl;
            } else if( l1.isScalar() && l1.indirection()>0){
                if(l1.indirection() == 1 && l1.specifier() == VOID){
                    cout<<"l1 is void pointer"<<endl;
                    report(E5,"+");
                    left = Type();
                    ret = Type();
                    //return ret;
                } else if( r1 == Type(INT,0)){
                    cout<<"+: ";
                    cout<<left<<" + "<<right<<" -> ";
                    ret = Type(left.specifier(), l1.indirection());
                    left = ret;
                    cout<<ret<<endl;
                } else{
                    report(E5, "+");
                    left = Type();
                    ret = Type();
                    //return ret;
                }
            } else if(l1 == Type(INT,0)){
                if(r1.isScalar() && r1.indirection()>0){
                    if(r1.indirection() == 1 && r1.specifier() == VOID){
                        report(E5,"+");
                        ret = Type();
                        left = ret;
                        //return ret;
                    } else{
                        cout<<"+: ";
                        cout<<left<<" + "<<right<<" -> ";
                        ret = Type(right.specifier(), r1.indirection());
                        left = ret;
                        cout<<ret<<endl;
                    }
                } else{
                    report(E5,"+");
                    left = Type();
                    ret = Type();
                    //return ret;
                }
            }else{
                report(E5, "+");
                left = Type();
                ret = Type();
                //return ret;
            }

        } else if (lookahead == '-') {
            match('-');
            lvalue = false;
            Type right = multiplicativeExpression(lvalue);
            Type l1 = left.promote();
            Type r1 = right.promote();
            if(left.isError() || right.isError()){
                ret = Type();
                continue;
            }
            if(l1 == Type(INT,0) && r1 == Type(INT,0)){
                ret = Type(INT,0);
                cout<<"-: ";
                cout<<left<<" - "<<right<<" -> "<<ret<<endl;
            } else if(l1.isScalar() && l1.indirection()>0){ //if left is a pointer
                cout<<"-: ";
                cout<<left<<" - "<<right<<" -> ";
                if(l1.indirection() == 1 && l1.specifier() == VOID){//left is a pointer to void
                    report(E5,"-");
                    left = Type();
                    ret = Type();
                } else if(r1.indirection() == 1 && r1.specifier() == VOID){ //right is a pointer to void
                    report(E5,"-");
                    left = Type();
                    ret = Type();
                } else if(r1 == Type(INT,0)){
                    ret = Type(left.specifier(), l1.indirection());
                } else if(r1.indirection() == l1.indirection() && r1.specifier() == l1.specifier()){
                    ret = Type(INT, 0);
                    
                }else{
                    report(E5,"-");
                    left = Type();
                    ret = Type();
                }
                cout<<ret<<endl;
            }else {


/*
            else if( l1.isScalar() && l1.indirection()>0){
                if(l1.indirection() == 1 && l1.specifier() == VOID){
                    report(E5,"-");
                    left = Type();
                    ret = Type();
                }else if( r1 == Type(INT,0)){
                    ret = Type(left.specifier(), left.indirection());
                    cout<<"-: ";
                    cout<<left<<" - "<<right<<" -> "<<ret<<endl;
                } else{
                    report(E5, "-");
                    left = Type();
                    ret = Type();
                }
            } else if(l1.isScalar() && r1.isScalar()){
                if(l1.indirection() == 1 && l1.specifier() != VOID && r1.indirection() == 1 && l1.specifier() != VOID){
                    ret = Type(INT,0);
                    cout<<"-: ";
                    cout<<left<<" - "<<r1<<"-> "<<ret;
                }else if(l1.indirection() > 1 && r1.indirection() > 1 && l1.specifier() == r1.specifier()){
                    ret = Type(INT,0);
                    cout<<"-: ";
                    cout<<left<<" - "<<r1<<"-> "<<ret;
                }else{
                    report(E5, "-");
                    left = Type();
                    ret = Type();
                }
            } else{
*/
                report(E5, "-");
                left = Type();
                ret = Type();
            }

        } else{
            break;
        }
    }
    return ret;
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

static Type relationalExpression(bool &lvalue)
{
    Type left = additiveExpression(lvalue);
    Type ret = left;
    while (1) {
        if (lookahead == '<') {
            match('<');
            Type right = additiveExpression(lvalue);
            if(left.isError() || right.isError()){
                ret = Type();
                continue;
            }
            cout<<"< :";
            cout<<left<<" x "<<right<<" -> ";
            Type l1 = left.promote();
            Type r1 = right.promote();
            if(l1.isValue()&&r1.isValue() && l1.indirection()==r1.indirection() &&l1.specifier() == r1.specifier()){
                lvalue = false;
                ret = Type(INT,0);
                left = ret;
            } else{
                report(E5,"<");
                ret = Type();
                left = Type();
            }
            cout<<ret<<endl;

        } else if (lookahead == '>') {
            match('>');
            Type right = additiveExpression(lvalue);
            if(left.isError() || right.isError()){
                ret = Type();
                continue;
            }
            cout<<"> :";
            cout<<left<<" x "<<right<<" -> ";
            Type l1 = left.promote();
            Type r1 = right.promote();
            if(l1.isValue() && r1.isValue() &&l1.indirection()==r1.indirection()&& l1.specifier() == r1.specifier()){
                lvalue = false;
                ret = Type(INT,0);
                left = ret;
            } else{
                report(E5,">");
                ret = Type();
                left = Type();
            }
            cout<<ret<<endl;

        } else if (lookahead == LEQ) {
            match(LEQ);
            Type right = additiveExpression(lvalue);
            if(left.isError() || right.isError()){
                ret = Type();
                continue;
            }
            cout<<"<= :";
            cout<<left<<" x "<<right<<" -> int"<<endl;
            Type l1 = left.promote();
            Type r1 = right.promote();
            if(l1.isValue() && r1.isValue() &&l1.indirection()==r1.indirection()&& l1.specifier() == r1.specifier()){
                lvalue = false;
                ret = Type(INT,0);
                left = ret;
            } else{
                report(E5,"<=");
                ret = Type();
                left = Type();
            }

        } else if (lookahead == GEQ) {
            match(GEQ);
            Type right = additiveExpression(lvalue);
            if(left.isError() || right.isError()){
                ret = Type();
                continue;
            }
            cout<<">= :";
            cout<<left<<" x "<<right<<" -> int"<<endl;
            Type l1 = left.promote();
            Type r1 = right.promote();
            if(l1.isValue() && r1.isValue() &&l1.indirection()==r1.indirection()&& l1.specifier() == r1.specifier()){
                lvalue = false;
                ret = Type(INT,0);
                left = ret;
            } else{
                report(E5,">=");
                ret = Type();
                left = Type();
            }

        } else
            break;
    }
    return ret;
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

static Type equalityExpression(bool &lvalue)
{
    Type left = relationalExpression(lvalue);
    Type ret = left;
    while (1) {
        if (lookahead == EQL) {
            match(EQL);
            Type right = relationalExpression(lvalue);
            if(left.isError() || right.isError())
                return ret;

            cout<<"==: ";
            cout<<left<<" == "<<right<<" -> ";
            if(!left.isCompatible(right)){
            //if(!left.isCompatible(right) || !right.isCompatible(left)){
                report(E5, "==");
                ret = Type();
                left = ret;
            } else{
                lvalue = false;
                ret = Type(INT, 0);
                left = ret;
            }
            cout<<ret<<endl;

        } else if (lookahead == NEQ) {
            match(NEQ);
            Type right = relationalExpression(lvalue);
            cout<<"!=: ";
            cout<<left<<" != "<<right<<" -> ";
            if(left.isError() || right.isError())
                return ret;
            if(!left.isCompatible(right)){
            //if(!left.isCompatible(right) || !right.isCompatible(left)){
                report(E5, "!=");
                ret = Type();
                left = ret;
            } else{
                lvalue = false;
                ret = Type(INT, 0);
                left = ret;
            }
            cout<<ret<<endl;

        } else
            break;
    }
    return ret;
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

static Type logicalAndExpression(bool &lvalue)
{
    Type left = equalityExpression(lvalue);
    Type ret = left;

    while (lookahead == AND) {
        match(AND);
        Type right = equalityExpression(lvalue);
        Type og = left;
        Type l1 = left.promote();
        Type r1 = right.promote();
        if(left.isError() || right.isError()){
            ret = Type();
            return ret;
        }
        if(!l1.isValue() || !r1.isValue()){
            report(E5, "&&");
            ret = Type();
            left = Type();
        }else{
            ret = Type(INT, 0);
            left = ret;
            lvalue = false;
        }
        cout<<"AND: ";
        cout<<og;
        cout<<" && "<<right;
        cout<<" -> int"<<endl;
    }
    return ret;
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

static Type expression(bool &lvalue)
{
    Type left = logicalAndExpression(lvalue);
    Type ret = left;
    while (lookahead == OR) {
        match(OR);
        Type right = logicalAndExpression(lvalue);
        Type og = left;
        Type l1 = left.promote();
        Type r1 = right.promote();
        if(left.isError() || right.isError()){
            return Type();
        }
        if(!l1.isValue() || !r1.isValue()){
            report(E5, "||");
            ret = Type();
            left = Type();
        }else{
            ret = Type(INT, 0);
            left = ret;
            lvalue = false;
        }
        cout<<"OR: ";
        cout<<og;
        cout<<" || "<<right;
        cout<<" -> "<<ret<<endl;
    }
    return ret;
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
    bool lvalue = false;
    Type left = expression(lvalue);
    
    Type right;

    if (lookahead == '=') {
        match('=');
        if(!lvalue)
            report(E4);
        right = expression(lvalue);
    }
    Type l1 = left.promote();
    Type r1 = right.promote();
    /*
    cout<<"Assignment: ";
    cout<<"left: "<<l1;
    cout<<" right: "<<r1<<endl;
    cout<<"left compat with right? : "<<!l1.isCompatible(r1)<<endl;
    cout<<"right compat with left? : "<<!r1.isCompatible(l1)<<endl;
    */
    //report("");
    if(left.isError() || right.isError())
        return;
    if(!l1.isCompatible(r1)){
    //if(!l1.isCompatible(r1) && !r1.isCompatible(l1)) {
        report(E5,"=");
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
 *		  for ( ssignment ; expression ; assignment ) statement
 *		  if ( expression ) statement
 *		  if ( expression ) statement else statement
 *		  assignment ;
 */

static void statement()
{
    //cout<<"loopcount is: "<<loopCount<<endl;
    //cout<<"lookahead is: "<<yytext<<endl;
    if (lookahead == '{') {
        match('{');
        openScope();
        declarations();
        statements();
        closeScope();
        match('}');

    } else if (lookahead == BREAK) {
        match(BREAK);
        if(loopCount == 0){
            cout<<loopCount<<endl;
            report(E1);
        }
        match(';');

    } else if (lookahead == RETURN) {
        bool lvalue = false;
        match(RETURN);
        Type t = expression(lvalue);

        //cout<<"promote check"<<endl;
        //cout<<"in return "<<t<<endl;
        //bool compat = !t.isCompatible(func_return) || !func_return.isCompatible(t);
        bool compat = !t.isCompatible(func_return);
        //bool indire = t.indirection() != func_return.indirection();
        if(compat)
            report(E2);
        match(';');

    } else if (lookahead == WHILE) {
        bool lvalue = false;
        match(WHILE);
        match('(');
        Type t = expression(lvalue);
        match(')');
        if(!t.isError() && !t.isValue()){
            report(E3);
        }
        loopCount++;
        statement();
        loopCount--;

    } else if (lookahead == FOR) {
        bool lvalue = false;
        match(FOR);
        match('(');
        assignment();
        match(';');
        Type t =expression(lvalue);
        if(!t.isError() && !t.isValue()){
            report(E3);
        }
        match(';');
        assignment();
        match(')');
        loopCount++;
        statement();
        loopCount--;

    } else if (lookahead == IF) {
        bool lvalue = false;
        match(IF);
        match('(');
        Type t =expression(lvalue);
        if(!t.isError() && !t.isValue()){
            report(E3);
        }
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

static Type parameter()
{
    int typespec;
    unsigned indirection;
    string name;
    Type type;


    typespec = specifier();
    indirection = pointers();
    name = expect(ID);

    type = Type(typespec, indirection);
    declareVariable(name, type);
    return type;
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

static Parameters *parameters()
{
    int typespec;
    unsigned indirection;
    Parameters *params;
    string name;
    Type type;


    openScope();
    params = new Parameters();
    params->_varargs = false;

    if (lookahead == VOID) {
        typespec = VOID;
        match(VOID);

        if (lookahead == ')')
            return params;

    } else
	typespec = specifier();

    indirection = pointers();
    name = expect(ID);

    type = Type(typespec, indirection);
    declareVariable(name, type);
    params->_types.push_back(type);

    while (lookahead == ',') {
        match(',');

        if (lookahead == ELLIPSIS) {
            params->_varargs = true;
            match(ELLIPSIS);
            break;
        }

        params->_types.push_back(parameter());
    }

    return params;
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

static void globalDeclarator(int typespec)
{
    unsigned indirection;
    string name;


    indirection = pointers();
    name = expect(ID);

    if (lookahead == '[') {
	match('[');
	declareVariable(name, Type(typespec, indirection, number()));
	match(']');

    } else if (lookahead == '(') {
	match('(');
	declareFunction(name, Type(typespec, indirection, parameters()));
	closeScope();
	match(')');

    } else
	declareVariable(name, Type(typespec, indirection));
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

static void remainingDeclarators(int typespec)
{
    while (lookahead == ',') {
	match(',');
	globalDeclarator(typespec);
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
    int typespec;
    unsigned indirection;
    Parameters *params;
    string name;


    typespec = specifier();
    indirection = pointers();
    name = expect(ID);

    if (lookahead == '[') {
	match('[');
	declareVariable(name, Type(typespec, indirection, number()));
	match(']');
	remainingDeclarators(typespec);

    } else if (lookahead == '(') {
	match('(');
	params = parameters();
	match(')');

	if (lookahead == '{') {
	    defineFunction(name, Type(typespec, indirection, params));
        func_return = Type(typespec, indirection);
	    match('{');
	    declarations();
	    statements();
	    closeScope();
	    match('}');

	} else {
	    closeScope();
	    declareFunction(name, Type(typespec, indirection, params));
	    remainingDeclarators(typespec);
	}

    } else {
	declareVariable(name, Type(typespec, indirection));
	remainingDeclarators(typespec);
    }
}


/*
 * Function:	main
 *
 * Description:	Analyze the standard input stream.
 */

int main()
{
    openScope();
    lookahead = yylex();
    loopCount = 0;

    while (lookahead != DONE)
	topLevelDeclaration();

    closeScope();
    exit(EXIT_SUCCESS);
}
