/*
 * File:	generator.cpp
 *
 * Description:	This file contains the public and member function
 *		definitions for the code generator for Simple C.
 *
 *		Extra functionality:
 *		- putting all the global declarations at the end
 */

# include <sstream>
# include <iostream>
# include <vector>
# include "generator.h"
# include "machine.h"
# include "Label.h"

using namespace std;

static unsigned maxargs;
vector<Label *>* labels = new vector<Label *>();
vector<Label *>* funcRet = new vector<Label *>();

int temp_offset;

void assignTemp(Expression* expr){
    stringstream ss;
    temp_offset -= expr->type().size();
    ss << temp_offset << "(%EBP)";
    expr->_operand = ss.str();
}


/*
 * Function:	operator <<
 *
 * Description:	Convenience function for writing the operand of an
 *		expression.
 */

ostream &operator <<(ostream &ostr, Expression *expr)
{
    return ostr << expr->_operand;
}


/*
 * Function:	Identifier::generate
 *
 * Description:	Generate code for an identifier.  Since there is really no
 *		code to generate, we simply update our operand.
 */

void Identifier::generate()
{
    stringstream ss;


    if (_symbol->_offset != 0)
	ss << _symbol->_offset << "(%ebp)";
    else
	ss << global_prefix << _symbol->name();

    _operand = ss.str();
}


/*
 * Function:	Number::generate
 *
 * Description:	Generate code for a number.  Since there is really no code
 *		to generate, we simply update our operand.
 */

void Number::generate()
{
    stringstream ss;


    ss << "$" << _value;
    _operand = ss.str();
}


# if STACK_ALIGNMENT == 4

/*
 * Function:	Call::generate
 *
 * Description:	Generate code for a function call expression, in which each
 *		argument is simply a variable or an integer literal.
 */

void Call::generate()
{
    unsigned numBytes = 0;


    for (int i = _args.size() - 1; i >= 0; i --) {
	_args[i]->generate();
	cout << "\tpushl\t" << _args[i] << endl;
	numBytes += _args[i]->type().size();
    }

    cout << "\tcall\t" << global_prefix << _id->name() << endl;

    if (numBytes > 0)
	    cout << "\taddl\t$" << numBytes << ", %esp" << endl;
    assignTemp(this);
    cout<<"\tmovl\t%eax, "<<_operand<<endl;
}

# else

/*
 * If the stack has to be aligned to a certain size before a function call
 * then we cannot push the arguments in the order we see them.  If we had
 * nested function calls, we cannot guarantee that the stack would be
 * aligned.
 *
 * Instead, we must know the maximum number of arguments so we can compute
 * the size of the frame.  Again, we cannot just move the arguments onto
 * the stack as we see them because of nested function calls.  Rather, we
 * have to generate code for all arguments first and then move the results
 * onto the stack.  This will likely cause a lot of spills.
 *
 * For now, since each argument is going to be either a number of in
 * memory, we just load it into %eax and then move %eax onto the stack.
 */

void Call::generate()
{
    if (_args.size() > maxargs)
	maxargs = _args.size();

    for (int i = _args.size() - 1; i >= 0; i --) {
	_args[i]->generate();
	cout << "\tmovl\t" << _args[i] << ", %eax" << endl;
	cout << "\tmovl\t%eax, " << i * SIZEOF_ARG << "(%esp)" << endl;
    }

    cout << "\tcall\t" << global_prefix << _id->name() << endl;
    assignTemp(this);
    cout<<"\tmovl\t%eax, "<<_operand<<endl;
}

# endif


/*
 * Function:	Assignment::generate
 *
 * Description:	Generate code for this assignment statement, in which the
 *		right-hand side is an integer literal and the left-hand
 *		side is an integer scalar variable.  Actually, the way
 *		we've written things, the right-side can be a variable too.
 */

void Assignment::generate()
{
    bool indirect = 0;
    _right->generate();
    _left->generate(indirect);

    cout << "\tmovl\t" << _right << ", %eax" << endl;
    if(indirect){
        cout<<"\tmovl\t"<<_left<<", %ecx"<<endl;
        if(_left->type().size() == 1)
            cout<<"\tmovb\t%al, (%ecx)"<<endl;
        else
            cout<<"\tmovl\t%eax, (%ecx)"<<endl;
    } else{
        if(_left->type().size()==1)
            cout<<"\tmovb\t%al, "<<_left<<endl;
        else
            cout<<"\tmovl\t%eax, "<<_left<<endl;
    }
}


/*
 * Function:	Block::generate
 *
 * Description:	Generate code for this block, which simply means we
 *		generate code for each statement within the block.
 */

void Block::generate()
{
    for (unsigned i = 0; i < _stmts.size(); i ++)
	_stmts[i]->generate();
}


/*
 * Function:	Function::generate
 *
 * Description:	Generate code for this function, which entails allocating
 *		space for local variables, then emitting our prologue, the
 *		body of the function, and the epilogue.
 */

void Function::generate()
{
    int offset = 0;
    Label * end = new Label();
    


    /* Generate our prologue. */

    allocate(offset);
    cout << global_prefix << _id->name() << ":" << endl;
    cout << "\tpushl\t%ebp" << endl;
    cout << "\tmovl\t%esp, %ebp" << endl;
    cout << "\tsubl\t$" << _id->name() << ".size, %esp" << endl;


    /* Generate the body of this function. */
    temp_offset = offset;

    maxargs = 0;
    funcRet->push_back(end);
    _body->generate();
    funcRet->pop_back();

    offset -= maxargs * SIZEOF_ARG;

    while ((offset - PARAM_OFFSET) % STACK_ALIGNMENT)
        offset --;

    offset = temp_offset;


    /* Generate our epilogue. */

    cout << *end <<":"<<endl;
    cout << "\tmovl\t%ebp, %esp" << endl;
    cout << "\tpopl\t%ebp" << endl;
    cout << "\tret" << endl << endl;

    cout << "\t.globl\t" << global_prefix << _id->name() << endl;
    cout << "\t.set\t" << _id->name() << ".size, " << -offset << endl;

    cout << endl;

    
}


/*
 * Function:	generateGlobals
 *
 * Description:	Generate code for any global variable declarations.
 */

void generateGlobals(const Symbols &globals)
{
    if (globals.size() > 0)
	cout << "\t.data" << endl;

    for (unsigned i = 0; i < globals.size(); i ++) {
	cout << "\t.comm\t" << global_prefix << globals[i]->name();
	cout << ", " << globals[i]->type().size();
	cout << ", " << globals[i]->type().alignment() << endl;
    }
}

void While::generate(){
    Label *startWhile = new Label();
    Label *endWhile = new Label();
        
    cout<<*startWhile<<":\t#While loop start"<<endl;
    labels->push_back(endWhile);

    _expr->generate();
    cout<<"\tmovl\t"<<_expr->_operand<<", %eax"<<endl;
    cout<<"\tcmpl\t$0, %eax"<<endl;
    cout<<"\tje\t"<<*endWhile<<endl;
    _stmt->generate();
    cout<<"\tjmp\t"<<*startWhile<<endl;

    cout<<*endWhile<<":"<<endl;
    labels->pop_back();
}

void Break::generate(){
    cout<< "\tjmp\t"<<*(labels->back())<<endl;
}

void Expression::generate(){cerr<<"oops"<<endl;}

void Expression::generate(bool &indirection){
    indirection = false;
    generate();
}

void Not::generate(){
    //cout<<"\tmovl\t"<<*_expr
    _expr->generate();
    cout<<"\tmovl\t"<<_expr->_operand<<", %eax"<<endl;
    cout<<"\tcmpl\t$0, %eax"<<endl;
    cout<<"\tsete\t%al"<<endl;
    cout<<"\tmovzbl\t%al, %eax"<<endl;
    assignTemp(this);
    cout<<"\tmovl\t%eax, "<<_operand<<endl;
}

void Negate::generate(){
    _expr->generate();
    cout<<"\tmovl\t"<<_expr->_operand<<", %eax"<<endl;
    cout<<"\tnegl\t%eax"<<endl;
    assignTemp(this);
    cout<<"\tmovl\t%eax, "<<_operand<<endl;
}

void Add::generate(){
    _left->generate();
    _right->generate();
    cout<<"\tmovl\t"<<_left->_operand<<", %eax"<<endl;
    cout<<"\taddl\t"<<_right->_operand<<", %eax"<<endl;
    assignTemp(this);
    cout<<"\tmovl\t%eax, "<<_operand<<endl;
}

void Subtract::generate(){
    _left->generate();
    _right->generate();
    cout<<"\tmovl\t"<<_left->_operand<<", %eax"<<endl;
    cout<<"\tsubl\t"<<_right->_operand<<", %eax"<<endl;
    assignTemp(this);
    cout<<"\tmovl\t%eax, "<<_operand<<endl;
}

void Multiply::generate(){
    _left->generate();
    _right->generate();
    cout<<"\tmovl\t"<<_left->_operand<<", %eax"<<endl;
    cout<<"\timull\t"<<_right->_operand<<", %eax"<<endl;
    assignTemp(this);
    cout<<"\tmovl\t%eax, "<<_operand<<endl;
}

void Divide::generate(){
    _left->generate();
    _right->generate();
    cout<<"\tmovl\t"<<_left->_operand<<", %eax"<<endl;
    cout<<"\tmovl\t"<<_right->_operand<<", %ecx"<<endl;
    cout<<"\tcltd\t"<<endl;
    cout<<"\tidivl\t%ecx"<<endl;
    assignTemp(this);
    cout<<"\tmovl\t%eax, "<<_operand<<endl;
}

void Remainder::generate(){
    _left->generate();
    _right->generate();
    cout<<"\tmovl\t"<<_left->_operand<<", %eax"<<endl;
    cout<<"\tmovl\t"<<_right->_operand<<", %ecx"<<endl;
    cout<<"\tcltd\t"<<endl;
    cout<<"\tidivl\t%ecx"<<endl;
    assignTemp(this);
    cout<<"\tmovl\t%edx, "<<_operand<<endl;
}

void LessThan::generate(){
    _left->generate();
    _right->generate();
    cout<<"\tmovl\t"<<_left->_operand<<", %eax"<<endl;
    cout<<"\tcmpl\t"<<_right->_operand<<", %eax"<<endl;
    cout<<"\tsetl\t"<<"%al"<<endl;
    cout<<"\tmovzbl\t"<<"%al, %eax"<<endl;
    assignTemp(this);
    cout<<"\tmovl\t"<<"%eax, "<<_operand<<endl;
}

void GreaterThan::generate(){
    _left->generate();
    _right->generate();
    cout<<"\tmovl\t"<<_left->_operand<<", %eax"<<endl;
    cout<<"\tcmpl\t"<<_right->_operand<<", %eax"<<endl;
    cout<<"\tsetg\t"<<"%al"<<endl;
    cout<<"\tmovzbl\t"<<"%al, %eax"<<endl;
    assignTemp(this);
    cout<<"\tmovl\t"<<"%eax, "<<_operand<<endl;
}

void LessOrEqual::generate(){
    _left->generate();
    _right->generate();
    cout<<"\tmovl\t"<<_left->_operand<<", %eax"<<endl;
    cout<<"\tcmpl\t"<<_right->_operand<<", %eax"<<endl;
    cout<<"\tsetle\t"<<"%al"<<endl;
    cout<<"\tmovzbl\t"<<"%al, %eax"<<endl;
    assignTemp(this);
    cout<<"\tmovl\t"<<"%eax, "<<_operand<<endl;
}

void GreaterOrEqual::generate(){
    _left->generate();
    _right->generate();
    cout<<"\tmovl\t"<<_left->_operand<<", %eax"<<endl;
    cout<<"\tcmpl\t"<<_right->_operand<<", %eax"<<endl;
    cout<<"\tsetge\t"<<"%al"<<endl;
    cout<<"\tmovzbl\t"<<"%al, %eax"<<endl;
    assignTemp(this);
    cout<<"\tmovl\t"<<"%eax, "<<_operand<<endl;
}

void Equal::generate(){
    _left->generate();
    _right->generate();
    cout<<"\tmovl\t"<<_left->_operand<<", %eax"<<endl;
    cout<<"\tcmpl\t"<<_right->_operand<<", %eax"<<endl;
    cout<<"\tsete\t"<<"%al"<<endl;
    cout<<"\tmovzbl\t"<<"%al, %eax"<<endl;
    assignTemp(this);
    cout<<"\tmovl\t"<<"%eax, "<<_operand<<endl;
}

void NotEqual::generate(){
    _left->generate();
    _right->generate();
    cout<<"\tmovl\t"<<_left->_operand<<", %eax"<<endl;
    cout<<"\tcmpl\t"<<_right->_operand<<", %eax"<<endl;
    cout<<"\tsetne\t"<<"%al"<<endl;
    cout<<"\tmovzbl\t"<<"%al, %eax"<<endl;
    assignTemp(this);
    cout<<"\tmovl\t"<<"%eax, "<<_operand<<endl;
}

void Address::generate(){
    bool indirect;
    _expr->generate(indirect);
    assignTemp(this);
    if(indirect)
        _operand = _expr->_operand;
    else{
        cout<<"\tleal\t"<<_expr->_operand<<", %eax"<<endl;
        cout<<"\tmovl\t%eax, "<<_operand<<endl;
    }
}

void Dereference::generate(bool &indirect){
    indirect = true;
    _expr->generate();
    _operand = _expr->_operand;
}

void Dereference::generate(){
    _expr->generate();
    assignTemp(this);
    cout<<"\tmovl\t"<<_expr->_operand<<", %eax"<<endl;
    cout<<(_type.size()==1?"\tmovsbl":"\tmovl")<<"\t(%eax), %eax"<<endl;
    cout<<"\tmovl\t%eax, "<<_operand<<endl;
}

void LogicalAnd::generate(){
    _left->generate();
    Label * end = new Label();
    assignTemp(this);
    cout<<"\tmovl\t"<<_left->_operand<<", %eax"<<endl;
    cout<<"\tcmpl\t$0, %eax"<<endl;
    cout<<"\tje\t"<<*end<<endl;
    
    _right->generate();
    cout<<"\tmovl\t"<<_right->_operand<<", %eax"<<endl;
    cout<<"\tcmpl\t$0, %eax"<<endl;
    cout<<*end<<":"<<endl;
    cout<<"\tsetne\t%al"<<endl;
    cout<<"\tmovzbl\t%al, %eax"<<endl;
    cout<<"\tmovl\t%eax, "<<_operand<<endl;
    
}

void LogicalOr::generate(){
    _left->generate();
    Label * end = new Label();
    assignTemp(this);
    cout<<"\tmovl\t"<<_left->_operand<<", %eax"<<endl;
    cout<<"\tcmpl\t$0, %eax"<<endl;
    cout<<"\tjne\t"<<*end<<endl;
    
    _right->generate();
    cout<<"\tmovl\t"<<_right->_operand<<", %eax"<<endl;
    cout<<"\tcmpl\t$0, %eax"<<endl;
    cout<<*end<<":"<<endl;
    cout<<"\tsetne\t%al"<<endl;
    cout<<"\tmovzbl\t%al, %eax"<<endl;
    cout<<"\tmovl\t%eax, "<<_operand<<endl;
    
}

void String::generate(){
    Label * l = new Label();
    cout<<".data"<<endl;
    cout<<*l<<": ";
    cout<<".asciz "<<_value<<endl;
    cout<<".text"<<endl;
    stringstream s;
    s << ".L" << l->number();
    _operand = s.str();
}

void Return::generate(){
    _expr->generate();
    cout<<"\tmovl\t"<<_expr->_operand<<", %eax"<<endl;
    cout<<"\tjmp\t"<<*(funcRet->back())<<endl;
}

void Promote::generate(){
    _expr->generate();
    cout<<"\tmovb\t"<<_expr->_operand<<endl;
    cout<<"\tmovsbl\t%al, %eax"<<endl;
    assignTemp(this);
    cout<<"\tmovl\t%eax, "<<_operand<<endl;
}

void If::generate(){
    Label * elseLabel = new Label();
    Label * endLabel = new Label();
    _expr->generate();
    cout<<"\tmovl\t"<<_expr->_operand<<", %eax"<<endl;
    cout<<"\tcmpl\t$0, %eax"<<endl;
    cout<<"\tje\t"<<*elseLabel<<"\t#jump to else"<<endl;
    _thenStmt->generate();
    cout<<"\tjmp\t"<<*endLabel<<"\t#jump to end of if"<<endl;
    cout<<*elseLabel<<":"<<"\t#else label"<<endl;
    if(_elseStmt != NULL){
        _elseStmt->generate();
    }
    cout<<*endLabel<<":"<<"\t# end of if"<<endl;
}

void For::generate(){
    Label * start = new Label();
    Label * end = new Label();

    _init->generate();

    cout<<*start<<":"<<endl;
    labels->push_back(end);

    _expr->generate();
    cout<<"\tmovl\t"<<_expr->_operand<<", %eax"<<endl;
    cout<<"\tcmpl\t$0, %eax"<<endl;
    cout<<"\tje\t"<<*end<<endl;

    _stmt->generate();
    
    _incr->generate();
    cout<<"\tjmp\t"<<*start<<endl;
    cout<<*end<<":"<<endl;
    labels->pop_back();
    
}
