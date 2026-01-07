#include <stdio.h>
#include <unistd.h>
int num = 22;
int mul;


const char *msg = "El resultado es:\n";


int main(void) {
    static int factor = 2;


    mul = num * factor;
    printf("%s%i\n", msg, mul);


    sleep(600);


    return 0;
}
