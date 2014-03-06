/*                          RS232 SUCHAI DRIVER
 *                              For PIC24F
 *
 *      Copyright 2012, Carlos Gonzalez Cortes, carlgonz@ug.uchile.cl
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SERIAL_COM_H
#define SERIAL_COM_H

/******************************************************************************
 *
 *                  RS232 SUCHAI PERIPHERAL INTERFACE HEADER FILE
 *
 ******************************************************************************
 * FileName:        rs232_suchai.h
 * Dependencies:    uart.h
 * Processor:       PIC24F
 * Compiler:        MPLAB C30
 * Company:         SUCHAI
 *****************************************************************************/
#include <uart.h>

/* Define diferencia entre mdulos UART  */
#define RS2_M_UART1 0	// Usar modulo UART1
#define	RS2_M_UART2 1	// Usar modulo UART2

/*------------------------------
 * 	    ABAUD VALUES TABLE
 *------------------------------
 * Baudrate	FCY	Abaud
 * -----------------------------
 *  500K	16MHz	1
 *  56K		16MHZ	17
 *  38.4K	16MHz	25
 *  19200	16MHz	51
 *  9600	16MHz	103
 *  2400	16MHz	416
 *------------------------------*/
 
void ConfigRS232(unsigned int baudrate, unsigned int modulo);
void SendRS232(unsigned char* datos, unsigned int largo, unsigned int modulo);
void SendRS232(unsigned char* datos, unsigned int largo, unsigned int modulo);
void SendStrRS232(char* str, unsigned int modulo);
unsigned int ReadRS232(unsigned int modulo);
int ReadStrRS232_ISR(int *buff_count, char *buffer, int len, unsigned int modulo);
int ReadFrameRS232_ISR(int *buff_count, char *buffer, int len, unsigned int modulo);
void EchoRS232(unsigned int modulo);
void RS232EnableRxInterrupt(unsigned int modulo);
 
 
#endif //RS232 SUCHAI_H
