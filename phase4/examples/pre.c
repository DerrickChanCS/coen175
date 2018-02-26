int foo(int x){
}

int* qoo(void){
}

void v(void){}
void * v1(void){}
void ** v2(void){}

char g(void){}
char* hu(void){
}

int fun(int x, int *y);

int main(void){
    void* foo;
    void* yu[10];

    int **w, *p, i, x;
    int a[10];
    char **arr;

    char d, *s;

    /*+ test */
    x = !fun(1, p);
    x = !v(); /*invalid*/
    x = !qoo();
    x = !a;
    x = !w;
    x = !d;
    x = !v1();
    x = !v2();
    x = !s;
    x = !arr;
    x = !1;

    x = -foo; /* invalid */
    x = - -1;
    x = -a; /* invalid */
    x = -*p;
    x = -d;
    x = -v2(); /* invalid */
    x = -g();
    x = -"yo"; /* invalid */
    x = -a[0];
    x = -(x<x);

    x = sizeof x;
    x = sizeof v(); /* invalid */
    x = sizeof v; /*invalid */
    x = sizeof v1();
    x = sizeof foo; 
    x = sizeof yu;
    x = sizeof w;
    x = sizeof **w;
    x = sizeof 'c';
    x = sizeof "hi";
    x = sizeof hu();

    p = &d; /* invalid */
    s = &d;
    p = &1; /* lvalue required */
    p = &'d'; /* lvalue required */
    s = &"hi"; /* lvalue required */
    x = &a; /* lvalue required */
    p = &a[0];
    p = w[0];
    p = *&*&p;
    
    foo = v1();
    foo = *v2();
    p = foo;
    w = &p;
    p = *w;
    s = &x; /* invalid binary */
    p = *v();
    

    return 0;

}

