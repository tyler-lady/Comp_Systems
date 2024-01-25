//
// Created by dontm on 12/10/2023.
//
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

bool validateOperation(char* op){
    if(strcmp(op, "b") == 0 || strcmp(op, "f") == 0)
        return true;
    else
        return false;
}

int get_exp(float x)
{
    if(x == 0.0f)
        return -126;
    uint32_t *i = (uint32_t *)&x; // point to address of float so same bytes are seen
    int exp = ( ((*i >> 23) & 255) -127 );
    if(exp == -127)
        return -126;
    return ( ((*i >> 23) & 255) -127 ); //shift by 23 where exponent begins
}

float binToFloat(uint32_t x){
    float f = 0.0f;
    memcpy(&f, &x, sizeof(f) < sizeof(x) ? sizeof(f) : sizeof(x));
    return f;
}

unsigned int binary_to_uint(const char *b)
{
    unsigned int val = 0;
    int i = 0;

    if (b == NULL)
        return 0;

    while (b[i] == '0' || b[i] == '1')
    {  // Found another digit.
        val <<= 1;
        val += b[i]-'0';
        i++;
    }
    return val;
}

void get_ieee754 (float f, char* target[])
{
    union {
        float f;
        uint32_t u;
    } fu = { .f = f };
    int i = sizeof f * 8;

    int index = 0;

    while (i--) {
        char str[5];
        //printf("%d ", (fu.u >> i) & 0x1);
        sprintf(str, "%d", (fu.u >> i) & 0x1);

        //printf("%s", str);
        if(index == 0){
            strcat(target[0], str);
            //printf(" %s\n", target[0]);
        }else if((index > 0) & (index < 9)){
            strcat(target[1], str);
        } else {
            strcat(target[2], str);
        }

        index++;
    }
}

void floatOperations(char* arg){
    float number = strtof(arg, NULL); //convert the number

    char* buf[3];
    char signS[1000] = "";
    buf[0] = signS;
    char expS[1000] = "";
    buf[1] = expS;
    char manS[1000] = "";
    buf[2] = manS;
    // get ieee fills buf with the pieces of the float bits
    get_ieee754(number, buf);

    int exponent;
    float fraction; // [0.5f to 1.0)
    fraction = frexpf(number, &exponent);

    if(fraction < 0)
        fraction *= -1.0f;

    //char *sign[] = {signbit(number) ? "-" : "+"}; // get the sign of the float
    //printBinaryOfFloat(number);
    printf("Binary: %s%s%s\n", buf[0], buf[1], buf[2]);
    printf("Sign: %s\n", buf[0]);
    printf("Exponent: %d\n", get_exp(number));
    if(isinf(number))
        printf("Mantissa: %.7g\n", 1.0f);
    else if(isnan(number))
        printf("Mantissa: %g\n", 1.9999998807907104f);
    else
        printf("Mantissa: %.7g\n", fraction*2.0);
    printf("Value: %.7g \n", number);
}

void binaryOperations(char* arg){
    float number = binToFloat(binary_to_uint(arg));

    char* buf[3];
    char signS[1000] = "";
    buf[0] = signS;
    char expS[1000] = "";
    buf[1] = expS;
    char manS[1000] = "";
    buf[2] = manS;
    // get ieee fills buf with the pieces of the float bits
    get_ieee754(number, buf);
    //float man = binToFloat(binary_to_uint(buf[2]));

    int exponent;
    float fraction; // [0.5f to 1.0)
    fraction = frexpf(number, &exponent);

    if(fraction < 0)
        fraction *= -1.0f;

    printf("Binary: %s\n", arg);
    printf("Sign: %c\n", arg[0]);
    printf("Exponent: %d\n", get_exp(number));
    if(isinf(number))
        printf("Mantissa: %.7g\n", 1.0f);
    else if(isnan(number))
        printf("Mantissa: %g\n", 1.9999998807907104f);
    else
        printf("Mantissa: %.7g\n", fraction*2.0);
    printf("Value: %.7g \n", number);
}

int main(int argc, char *argv[]) {
    if( argc == 3 ) {
        //This is our arg location
        if(validateOperation(argv[1])){
            //move forward
            if(strcmp(argv[1], "f") == 0){ // f mode
                floatOperations(argv[2]);
            }else{ //b mode
                binaryOperations(argv[2]);
            }
        }else{
            printf("%s supplied argument is not valid\n", argv[1]);
        }
    }
    else if( argc > 3 ) {
        printf("Too many arguments supplied.\n");
    }
    else {
        printf("At least Two(2) arguments expected.\n");
    }

    return 0;
}