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
    /*OR test*/
    x = foo || p;
    x = g || hu; /* invalid operands */
    x = g || 1; /* invalid operands */
    x = 1 || 2;
    x = 'c' || 0;
    x = 1 || 'c';
    x = g() || *hu();
    x = 'c' || *p;
    x = *&*&i || 1;
    x = yu[0] || a[0];
    x = v() || 5; /* invalid operands */
    x = 5 || v(); /* invalid operands */
    x = 'c' || 'u';
    x = a || 5;
    x = a[0] || a[1];

    /*AND test*/
    x = foo && p;
    x = g && hu; /* invalid operands */
    x = g && 1; /* invalid operands */
    x = 1 && 2;
    x = 'c' && 0;
    x = 1 && 'c';
    x = g() && *hu();
    x = 'c' && *p;
    x = *&*&i && 1;
    x = yu[0] && a[0];
    x = v() && 5; /* invalid operands */
    x = 5 && v(); /* invalid operands */
    x = 'c' && 'u';
    x = a && 5;
    x = a[0] && a[1];

    /* EQ test*/
    x = 1==2;
    x = foo==p;
    x = p==foo;
    x = d==*s;
    x = *p==i;
    x = p==i; /* invalid operands */
    x = g() == g();
    x = v() == v(); /* invalid operands */
    x = v() == g(); /* invalid operands */
    x = s == p; /* invalid operands */
    x = s == s;
    x = p == p;
    x = foo == foo;
    x = 'c' == 1;
    x = a == 'a'; /* invalid operands */
    x = 'a' == a; /* invalid operands */
    x = a == a; 
    x = qoo() == p;
    x = qoo() == s; /* invalid operands */
    x = qoo() == d;/* invalid operands */

    /* NEQ test */
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
    x = qoo() != d; /* invalid operands */

    /*> test */
    x = 1>2;
    x = 'c'>3;
    x = 'c'>'d';
    x = foo > 1;
    x = foo > foo;
    x = g() > 1;
    x = 1 > g();
    x = 'd' > g();
    x = v() > v();
    x = v() > 1;
    x = 1 > v();

    i = (p < v);
    &x = p;
    /*
    p = &d;
    p = &a;
    p = &a[0];
    p = &i;
    */

    return 0;

}

