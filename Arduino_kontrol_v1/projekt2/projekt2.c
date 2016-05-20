/*
 * LAB12_1.c
 *
 * Created: 24-11-2015 07:36:18
 *  Author: Emil
 */ 


#include <avr/io.h>
#include "uart_int.h"
#define F_CPU 16000000
#include <util/delay.h>
#include <avr/interrupt.h>
#include "LED.h"


static int temp = 0;
static int correct = 0;

int main(void)
{
	InitUART(9600, 8, 'O', 1);
	init_LEDport();
	sei();
	
	
    while(1)
    {
        //unsigned int temp = LM75_temperature(0);
		//
		//SendString("Temperaturen er ");
		//SendInteger(temp/2);
		//if (temp & 0b00000001 == 1)
		//{
			//SendString(",5");
		//}
		//else
		//{
			//SendString(",0");
		//}
		//SendString(" grader.\n");
		//_delay_ms(1000);
		//SendString("Faggot\n");
		//_delay_ms(1000);
		
		
    }
	
}

int ValidateCode(char r1, char r2, char r3, char r4)
{
	char conf1 = '1';
	char conf2 = '2';
	char conf3 = '3';
	char conf4 = '4';
	
	
	if(r1 == conf1 && r2 == conf2 && r3 == conf3 && r4 == conf4)
	{
		toggleLED(1);
		return 1;
	}
	if(r1 != conf1 || r2 != conf2 || r3 != conf3 || r4 != conf4)
	{
		toggleLED(7);
		return 0;
	}
}

void InitTest(char test)
{
	toggleLED(5);
}

ISR (USART0_RX_vect)
{
		if (temp == 1)
		{
				char test;
				test = ReadChar();
				InitTest(test);
		}
	
		
		
		if (temp == 0)
		{
			char r1,r2,r3,r4;
			
			r1 = ReadChar();
			r2 = ReadChar();
			r3 = ReadChar();
			r4 = ReadChar();
			
			correct = ValidateCode(r1,r2,r3,r4);
			SendInteger(correct);
			
		}
		
		if (correct == 1)
		{
			temp = 1;
		}
		
		
	
}	