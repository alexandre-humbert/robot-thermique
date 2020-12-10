#ifndef H_MOTEUR
#define H_MOTEUR
#define UART_PERIPH "/dev/ttyO1"
void commande_robot(const char*, unsigned char* ancienne_commande);
void commande_robot_arg(const char* nom_commande, const int i);
#endif