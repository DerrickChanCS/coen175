int z;
int z;
int a[10];
int main(int x);
/* Error type void */
void blah;

int fib(int x, int y);
int fib(int z, int x);
int fb(int x, ...);

/* conflicting type for main */
int main(void){
    int x;
    char y;
    /* redecl of x */
    void x;
    /* redecl of x */
    char x;
    
    if(1){
        int x;
    }

    if(1){
        x = x+y;
    }
    
    /* undecl of i 
     * Note, should only throw the error once */
    for(i = 0; i < 10; i=i+1){
        int blah;
        x = 1+1;
    }
    if(1){
        int inner;
        if(1){
            int nextInner;
        } else{
            int mostInner;
        }
    }
    /* undecl noEx */
    noEx(x);
}

/* conflicting type for main */
int main(void){
    int testVar;
    /* undecl nonExistant */
    /* undecl i */
    testVar = nonexistant[i];
    return testVar;
}

int fb(int nodecl, ...){
    return 0;
}

void fubar(int asdfl, ...);
/* should conflicting types */
void fubar(char x, ...){
    return 0;
}   

/*check that x is parsed*/
void blah(void){
    /* x has type void */
    void x;
    int x;
    /* redecl */
    int x;
    /* redecl */
    void x;
    int *a;
    /* redecl */
    int a[10];
    int b[10];
    int*********** c[50];
    void* y;
    void* d[10];
}
