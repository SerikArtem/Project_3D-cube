#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "cube.h"

// Опредедение настроек USART:
// Тактовая частота кварца 
#define FOSC 14745600
// Скорость обмена данными (в бодах)
//#define BAUD 38400
#define BAUD 9600
// Скорость обмена данными определяется содержимым регистра UBRR (в этом случае MYUBRR). 
// Скорость обмена выбирается из ряда стандартных значений, в нашем примере она равна 9600 бод. 
// Значение UBRR для обычного асинхронного режима (есть еще асинхронный режим с удвоенной скоростью обмена) 
//вычисляется по формуле: UBRR = (Fck/(16*BAUD)) – 1,
//где Fck – тактовая частота микроконтроллера, BAUD скорость обмена в бодах.
// В нашем случае MYUBRR = 23,5 (~24). 
//#define MYUBRR (((((FOSC * 10) / (16L * BAUD)) + 5) / 10) - 1)
//Если записать точно по формуле, то тоже в итоге получается 24. Тогда зачем платить больше?! :)
#define MYUBRR (FOSC/(16L * BAUD)-1)

#define DATA_BUS PORTA
#define LAYER_SELECT PORTC
#define LATCH_ADDR PORTB
#define LATCH_MASK 0x07
#define LATCH_MASK_INV 0xf8
#define OE_PORT PORTB
#define OE_MASK 0x08

// Красный светодиод D2
#define LED_RED 0x04
// Зелёный светодиод D3
#define LED_GREEN 0x08
// Программный светодиод D4
#define LED_PGM 0x10;
// Светодиоды подключены к порту D
#define LED_PORT PORTD
// Rs232 кнопка D5
#define RS232_BTN 0x20
// Главная кнопка B4
#define MAIN_BTN 0x10

void ioinit (void);
void bootmsg (void);

volatile unsigned char current_layer;
volatile unsigned char pgm_mode;
void rs232(void);
unsigned int bootwait (void);
#endif

