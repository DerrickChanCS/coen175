int main(void){
    int x,y,z;
    char d;
    void * foo;
    if(1){
        return 1;
    }
    for(x = 0; x<1; x=x+1){
        break;
        while(1) break;
        break;
    }
    break; /* error */
    return d;
    return foo; /* invalid return */
    break; /* error */
}

int bar(void){
    return 1;
}

char foo(void){
    return 1;
}

void fb(void){
    return 0; /* invalid return */
}

void* v(void){
    void * x;
    int* blah;
    return x;
    return blah;
}

void** v1(void){
    void* x;
    return &x;
}

char* c(void){
    int* p;
    char* d;
    char a[10];
    return d;
    return p; /* invalid return */
    return a;
}

int* e(void){
    int* p;
    char* d;
    return d; /* invalid return */
    return p; 
}
