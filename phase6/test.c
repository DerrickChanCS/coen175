int x, y,z;
int a,b,c,d;
char* hello;

int printf(char *s, ...);

int foo(void){
    return 1;
}

int test(int in){
    if(in == 0){
        printf("0\n");
    } else if(in == 1){
        printf("1\n");
    } else if(in == 2){
        printf("2\n"); 
    } else if(in >= 3){
        printf("hi\n");
    }
}

int main(void){
    int i;
    int *p;
    i = 5;
    if(i < 6)
        printf("i less than 6\n");
    else
        printf("i not less than 6\n");

    if(i > 6)
        printf("i greater than 6\n");
    else
        printf("i not greater than 6\n");
    
    if(i <= 5)
        printf("i less than equal 5\n");
    else 
        printf("i i not less than equal 5\n");
    
    if(i > 5)
        printf("i greater than 5\n");
    else
        printf("i not greater than 5\n");

    if(i > 3)
        printf("i greater than 3\n");
    else
        printf("i not greater than 3\n");
    if(i >= 5)
        printf("i greater than equl 5\n");
    else
        printf("i not greater than equal 5\n");
    
    if(i == 5)
        printf("i equal to 5\n");
    else
        printf("i not equal to 5\n");

    if(i != 5)
        printf("i not equal to 5\n");
    else
        printf("i equal to 5\n");

    if(!i)
        printf("not zero\n");
    if(i)
        printf("not zero\n");
    
    *p = 1; 
    printf("%d\n",*p);
    printf("%d\n",p);
    return 0;
}
