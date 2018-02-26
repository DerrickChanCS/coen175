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

    char d, *s;

    /*+ test */
    x = 1 + 1;
    x = 'c' + 1;
    x = 'c' + 'd';
    x = 1 + 'd';
    x = "123" + 4; /* invalid operands to = */
    p = "123" + 4; /* invalid operands to = */
    s = "123" + 4;
    
    x = 4 + "123"; /* invalid operands to = */
    p = 4 + "123"; /* invalid operands to = */
    s = 4 + "123"; 

    x = foo + 4; /* invalid opearnds to + */
    x = 4 + foo; /* invalid opearnds to + */
    foo = v2() + 4;
    s = v2() + 4; /* invalid operands to = */
    

    return 0;

}

