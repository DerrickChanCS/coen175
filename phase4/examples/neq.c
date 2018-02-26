
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

    /* EQ test*/
    x = 1!=2;
    x = foo!=p;
    x = p!=foo;
    x = d!=*s;
    x = *p!=i;
    x = p!=i; /* invalid operands */
    x = g() != g();
    x = v() != v(); /* invalid operands */
    x = v() != g(); /* invalid operands */
    x = s != p; /* invalid operands */
    x = s != s;
    x = p != p;
    x = foo != foo;
    x = 'c' != 1;
    x = a != 'a'; /* invalid operands */
    x = 'a' != a; /* invalid operands */
    x = a != a; 
    x = qoo() != p;
    x = qoo() != s; /* invalid operands */
    x = qoo() != d;/* invalid operands */
    x = d != d != a; /* invalid operands */
    x = a != d != 5; /* invalid operands */
    x = 2*3 != p+3; /* invalid operands */
    x = p+3 != 2*3; /* invalid operands */

    return 0;

}

