int x;
int printf(char *s, ...);

int count(void){
    x = x - 1;
    return x;
}

int retTest(int in){
    if(in == 0)
        return 0;
    else if(in == 1)
        return 1;
    else if(in == 2)
        return 2;
    return -1;
}

int main(void){
    int i;
    int *p;
    x = 5;
    while(count()){
        printf("Hi\n");
    }
    for(i = 0; i<4; i=i+1){
        printf("%d\n", retTest(i));
    }
    return 0;
}
