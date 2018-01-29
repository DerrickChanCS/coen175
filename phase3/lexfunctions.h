# ifndef LEXFUNC_H
# define LEXFUNC_H
void expr_or();
void expr_and();
void expr_eq();
void expr_cmp();
void expr_add();
void expr_mul();
void expr_pre();
void expr_post();
void expr_identifier();

void expression_list();
void assignment();
void statement();
void statements();
void declarator();
void declarator_list();
void declaration();
void declarations();
void parameter();
void parameter_list();
void parameters();
void functionDefinition();
void specifier();
void pointers();
void globalDeclarator();
void globalDeclaratorList();
void globalDeclaration();
void translationUnit();

void functionGlobal();
void fgprime();

# endif
