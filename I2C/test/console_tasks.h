/*                            SERIAL CONSOLE
 *                              For PIC24F
 *
 *      Copyright 2012, Francisco Reyes Aspe, komodotas@gmail.com
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

#ifndef CON_TASKS_H
#define CON_TASKS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "serial_com.h"
#include "i2c_comm.h"

#define CON_BUF_WIDTH 32
#define CON_ARG_WIDTH 8

#define CON_ARG_QTY 8
#define CON_HIS_QTY 20

#define con_putc(c) SendRS232((unsigned char *)(c),1,RS2_M_UART1)
#define con_putcl(c) while(U1STAbits.UTXBF);WriteUART1(c);
#define con_printf(c) SendStrRS232(c,RS2_M_UART1)
#define con_getc() ((char)ReadRS232(RS2_M_UART1))
#define con_strncpy(a,b,n) strncpy(a,b,n); *((a)+(n))='\0';

void con_init(void);
void con_char_handler(char newchar);
void con_cmd_from_entry(char *entry);
void con_cmd_handler();
void con_error_count_arg(void);
void con_error_invalid_arg(void);
void con_error_unknown_cmd(void);

BOOL StrIsInt(char *str);

#endif //CON_TASK_H
