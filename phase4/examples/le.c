int foo(int x){
    char d;
    x = 0;
    return  d;
    return x;
}

int* qoo(void){
    int* x;
    return x;
}

void v(void){}
void * v1(void){}
void ** v2(void){}

char g(void){}
char* hu(void){
    char a[10];
    return a;
}

int fun(int x, int *y);

int main(void){
    void* foo;
    void* yu[10];

    int *p, i, x;
    int a[10];

    char d, *s;

    /*> test */
    x = 1<2;
    x = 'c'<3;
    x = 'c'<'d';
    x = foo < 1; /* invalid operands */
    x = foo < foo;
    x = g() < 1; 
    x = 1 < g();
    x = 'd' < g();
    x = v() < v(); /* invalid operands */
    x = v() < 1; /* invalid operands */
    x = 1 < v(); /* invalid operands */
    x = (1*p)<4; /* invalid operands * */
    i = (p < v);
    x = v1() < v2(); /* invalid operands */

    return 0;

}

