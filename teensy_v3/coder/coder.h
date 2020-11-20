#ifndef CODER_H
#define CODER_H
#include <arduino.h>
#include <stdbool.h>

// coders input
#define M1In1 5
#define M1In0 6
#define M2In1 8  // sens inverse de M1
#define M2In0 7

void setup_coder_pin() ;
void ISRM1() ;
void ISRM2() ;
void depl_value(int *d1, int *d2);
void delta_value(int *delta1, int *delta2);
#endif

