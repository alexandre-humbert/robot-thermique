#ifndef MOTOR_H
#define MOTOR_H
#include <arduino.h>
#include <stdbool.h>

// sorties  de controle des moteurs
#define M2Enable 23
#define M1Enable 22
#define M2Out1 16
#define M2Out0 17
#define M1Out1 14
#define M1Out0 15

void setup_motor_pin() ;
// fonction bas niveau de commande des moteurs
void MotorCC1_TOR( bool bEnable, bool bSens);
void MotorCC2_TOR( bool bEnable, bool bSens);
void MotorCC1__( int iPuissance, bool bSens);
void MotorCC2__( int iPuissance, bool bSens);
void MotorCC1( int iPuissance);
void MotorCC2( int iPuissance);
// divers
void Stop();

#endif

