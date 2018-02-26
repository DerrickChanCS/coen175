
int foo(int x){
}

int* qoo(void){
}

void v(void){}
void **v2(void){}
void *v1(void){}


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
    x = 5 || v()||5; /* invalid operands */

    return 0;
}

