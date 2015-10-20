#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Author: Alex Aldana

const int NSIGN = 1;
const int NEXPONENT = 8;
const int NFRACTION = 23;

struct FloatPoint addFract(struct FloatPoint float1, struct FloatPoint float2);
struct FloatPoint readInput();
int expToDec(int *num);
int power(int base, int exp);
float negPower(int base, int exp);
void Alg(struct FloatPoint float1, struct FloatPoint float2);
struct FloatPoint shiftFraction(struct FloatPoint float1);
void printFract(struct FloatPoint float1);

struct FloatPoint {
    int sign;
    int exponent;
    int fraction[28];
    int fracEnd;
    int decimalpos;
};

int main(int argc, char* argv[]){
    // Define struct each each float
    struct FloatPoint float1;
    struct FloatPoint float2;
    // Read in the 32 bits for each float in the System input
    printf("This is a Float Point Addition program!\n\n");
    float1 = readInput();
    float2 = readInput();
    // call the add algorithm to add each of the float points
    Alg(float1, float2);
    return 0;
}


struct FloatPoint addFract(struct FloatPoint float1, struct FloatPoint float2){
    // Define variables
    int i;
    struct FloatPoint result;
    // overflow bit variable
    char c = 0;
    // Go through each index in each float add go through the adding algorithm
    for(i = NFRACTION-1; i >= 0; i-- ){
        // If both are ones then output 0 and change overflow bit to 1
        // If overflow bit is 1 then output 1
        if(float1.fraction[i] == 1 && float2.fraction[i] == 1){
            if(c == 1) {
                result.fraction[i] = 1;
            }
            else {
                result.fraction[i] = 0;
                c = 1;
            }
        }
        // If both are zeros, output 1
        // If overflow bit is 1, output 1 and change overflow bit to 0
        else if(float1.fraction[i] == 0 && float2.fraction[i] == 0) {
            if(c == 1) {
                    result.fraction[i] = 1;
                    c = 0;
            }
            else result.fraction[i] = 0;
        }
        // If only one has a 1, then output 1
        // If overflow bit is 1, then output 0
        else {
            if(c == 1) {
                result.fraction[i] = 0;
            }
            else result.fraction[i] = 1;
        }
    }
    // If adding each number in float, if overflow bit is still 1,
    // shift all result bits and add the overflow bit to the front
    if(c == 1){
        int temp;
        for(i = NFRACTION-1; i >= 0; i--){
            if(i == 0) {
                    result.fraction[i] = c;
                    result.fraction[i+1] = 0;
            }
            else {
                temp = result.fraction[i];
                result.fraction[i] = result.fraction[i+1];
                result.fraction[i+1] = temp;
            }
        }
        result.decimalpos +=1;
    }
    // return result
    return result;
}

struct FloatPoint readInput(){
    // Define variables
    struct FloatPoint float1;
    int exponent[9], i;

    printf("Enter a Floating Point to add with a space between each binary number:\n->");
    // Set initial variables
    float1.fracEnd = 0;
    float1.decimalpos = 0;
    // read in the sign bit for the float
    scanf("%d", &float1.sign);
    // read in the exponent bits for the float
    for(i = 0; i < NEXPONENT; i++){
        scanf("%d", &exponent[i]);
    }
    // change exponent to decimal
    float1.exponent = expToDec(exponent);
    // don't forget to add the implied "1. to the fraction"
    float1.fraction[0] = 1;
    for(i = 1; i < NFRACTION+1; i++){
        scanf("%d", &float1.fraction[i]);
    }
    // find the length of the fraction for printing cleanliness
    for(i = NFRACTION; i >= 0; i--){
        if(float1.fraction[i-1] == 1) {
            break;
        }
        else if(float1.fraction[i] == 1) {
            break;
        }
        float1.fracEnd +=1;
    }
    // return float
    return float1;
};

int expToDec(int *num){
    // define variables
    int length = NEXPONENT;
    int i, exp = 0, sum = 0;
    // add the each value when at 1
    for(i = length - 1; i >= 0; i--){
        sum += power(2, exp)*num[i];
        exp++;
    }
    sum = sum - 127;
    //return sum
    return sum;
}

int power(int base, int exp) {
     // Traditional power function ==
    if (exp == 0)
        return 1;
    else if (exp % 2)
        return base * power(base, exp - 1);
    else {
        int temp = power(base, exp / 2);
        return temp * temp;
    }
}

struct FloatPoint shiftFraction(struct FloatPoint float1){
    // define variables
    int i;
    // shift all bits to the right
    for(i = NFRACTION-float1.fracEnd; i >= 0; i--){
            float1.fraction[i] = float1.fraction[i-1];
    }
    //set first bit to 0
    float1.fraction[0] = 0;
    //set end of fraction again, and since we shift we add 1 to the exponent
    if(float1.fracEnd != 0) float1.fracEnd -= 1;
    float1.exponent++;
    //return float
    return float1;
}
float negPower(int base, int exp){
    exp = -1 * exp;
    return ( (float)1 / (float)power(base,exp) );
}

void Alg(struct FloatPoint float1, struct FloatPoint float2){
    // define variables
    int range, i;
    struct FloatPoint result;
    // If one float has a higher exponent, we must denormalize the smaller
    // one to begin adding
    if(float1.exponent > float2.exponent){
        range = float1.exponent - float2.exponent;
        for(i = 0; i < range; i++){
            float2 = shiftFraction(float2);
        }
    }
    else if(float1.exponent < float2.exponent){
        range = float2.exponent - float1.exponent;
        for(i = 0; i < range; i++){
            float1 = shiftFraction(float1);
        }
    }
    // Once exponents are equal, we can evaluate the floats
    if(float1.exponent == float2.exponent) {
        printf("\n\n");
        // call the adding algorithm
        result = addFract(float1, float2);
        // and set the result's exponent
        result.exponent = float1.exponent;
        // print first float
        printf("\First Float is = ");
        printFract(float1);
        // print second float
        printf("\Second Float is = ");
        printFract(float2);
        // decide if the result is negative or positive
        if(float1.sign == 0 && float2.sign == 0) result.sign = 0;
        else if(float1.sign == 1 && float2.sign == 1) result.sign = 0;
        else result.sign = 1;
        // make sure the result is normalized
        if(result.decimalpos != 0){
            result.decimalpos = 0;
            result.exponent++;
        }
        // set the length of the fraction for printing cleanliness
        int count =0;
        for(i = NFRACTION-1; i >= 0; i--){
            if(result.fraction[i-1] == 1) break;
            else if(result.fraction[i] == 1) break;
            count +=1;
        }
        result.fracEnd = count;
        // print out the result
        printf("\nResult is = ");
        printFract(result);
        // answer might be incorrect so we must check for overflows
        printf("\n\n If answer is incorrect check for overflows.\n");
    }
}

void printFract(struct FloatPoint float1){
    int d;
    if(float1.sign == 1) printf("-");
    for(d = 0; d < NFRACTION-float1.fracEnd; d++) {
        printf("%d ", float1.fraction[d]);
        if(float1.decimalpos == d) printf(". ");
    }
    printf(" x 2^(%d)\n", float1.exponent);
}





