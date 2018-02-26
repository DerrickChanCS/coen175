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

    x = 1 * 2;
    x = 'd' * 1;
    x = 'd' * 'd';
    x = 1 * 'd';
    x = p * 1; /* invalid operand to * */
    x = 1 * p; /* invalid operand to * */
    x = a * s; /* invalid operands to * */
    x = foo * s; /* invalid operands to * */
    x = s * foo; /* invalid operands to * */
    x = v * v; /* invalid operands */
    x = foo * foo * foo;
    

    return 0;

}

