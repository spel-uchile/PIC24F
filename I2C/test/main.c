/*                          I2C DRIVER TEST
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

#include <xc.h>
#include <PPS.h>
#include <timer.h>

#include "serial_com.h"
#include "console_tasks.h"
#include "i2c_comm.h"


/* PIC24F CONFIGURATION */
_CONFIG1(JTAGEN_OFF& GCP_OFF& GWRP_OFF& FWDTEN_OFF& FWPSA_PR128& WDTPS_PS32768 );
_CONFIG2(IESO_OFF& FNOSC_PRIPLL& FCKSM_CSECME& OSCIOFNC_OFF& IOL1WAY_ON& POSCMOD_XT);
_CONFIG3(WPCFG_WPCFGDIS& WPDIS_WPDIS);

/* TIMER 1 INTERRUPT FOR CONSOLE */
void __attribute__((__interrupt__, auto_psv)) _T1Interrupt( void )
{
    /* Clear the timer interrupt. */
    T1_Clear_Intr_Status_Bit;
    WriteTimer1(0);

    con_cmd_handler();
}

void configure_ports(void)
{
    /* CUBESAT KIT MB CONFIGURATION */
    //-OE_USB -INT //6	RPI38/CN45/RC1	=> pin es RC1
    _TRISC1=0; 			//pin 0-output 1-input.
    //-OE_MHX //98	CN60/PMD2/RE2	=> pin es RE2
    _TRISE2=0; 			//pin 0-output 1-input.
    //-ON_MHX //99	CN61/PMD3/RE3	=> pin es RE3
    _TRISE3=0; 			//pin 0-output 1-input.
    //-ON_SD //100	CN62/PMD4/RE4	=> pin es RE4
    _TRISE4=0; 			//pin 0-output 1-input.

    _LATE2 = 1; 		/* -OE_MHX OFF */
    _LATE3 = 0; 		/* -ON_MHX ON */
    _LATC1 = 0; 		/* -OE_USB ON */

//    /* UART 1 for LAGMUIR */
//    /* Maping UART1's RX pin */
//    // H1.19 - U1RX - RP30/CN70/RF2
//    iPPSInput(IN_FN_PPS_U1RX,IN_PIN_PPS_RP30);
//
//    /* Maping UART1's TX pin */
//    // H1.20 - U1TX - RP16/CN71/RF3
//    iPPSOutput(OUT_PIN_PPS_RP16,OUT_FN_PPS_U1TX);

    //Conifg para Consola:
    // H1.17 - U1RX - RP10 - IO.7 - UART 1 PARA CONSOLA SERIAL
    iPPSInput(IN_FN_PPS_U1RX,IN_PIN_PPS_RP10);
    // H1.18 - U1TX - RP17 - IO.6 - UART 1 PARA CONSOLA SERIAL
    iPPSOutput(OUT_PIN_PPS_RP17,OUT_FN_PPS_U1TX);
}

int main()
{
    /* Configure PPS and MB ports */
    configure_ports();

    /* Configure Timer 1 for console */
    ConfigIntTimer1(T1_INT_PRIOR_4 & T1_INT_ON);
    OpenTimer1(T1_ON & T1_IDLE_CON & T1_GATE_OFF & T1_PS_1_64 & T1_SYNC_EXT_OFF
             & T1_SOURCE_INT, 25000); /* t[s]=X*ps/fcy => X=100ms*16MIPS/64.0 = 25000 */
    WriteTimer1(0);

    /* UART1 - CONSOLE - 19200, 8, N, 1 */
    ConfigRS232(51, RS2_M_UART1);
    EnableIntU1RX;
    SetPriorityIntU1RX(5);

    i2c1_open(157, 0x0A);
    i2c2_open(157, 0x0B);
    i2c3_open(157, 0x0C);

    printf("\n\n===== I2C DRIVER TEST =====\r\n");

    while(1)
    {
        ClrWdt();
    }

    return 0;
}

/********* PRINTF HACK *********/

#define STDIN   0
#define STDOUT  1
#define STDERR  2
#define LF   '\n'
#define CR   '\r'
#define STDOUT_NO_CR

void    mon_putc(char ch);

int __attribute__((__weak__, __section__(".libc")))
write(int handle, void * buffer, unsigned int len)
{
    int i = 0;
    switch (handle)
    {
        case STDOUT:
        case STDERR:
            while (i < len)
                mon_putc(((char*)buffer)[i++]);
            break;
    }
    return (len);  // number of characters written
}

#define STDOUT_NO_CR_WITH_LF
void mon_putc(char ch)
{
    while(U1STAbits.UTXBF);  /* wait if the buffer is full */
#ifndef STDOUT_NO_CR_WITH_LF
    if (LF == ch)
        putcUART1(CR);
#endif
#ifdef STDOUT_NO_CR
    if (CR == ch)
        return;
#endif
    putcUART1(ch);
}