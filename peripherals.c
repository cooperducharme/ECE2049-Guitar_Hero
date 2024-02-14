//Peripherals.c
/*
* peripherals.c
*
* Created on: Jan 29, 2014
* Author: deemer
* Updated on Jan 3, 2016
* smj
* Updated on July 7, 2018
* deemer
* Updated on Jan 14, 2018
* smj
*
*/
#include "peripherals.h"
// Globals
tContext g_sContext; // user defined type used by graphics library
/*
* Enable a PWM-controlled buzzer on P3.5
* This function makes use of TimerB0.
*/
void BuzzerOn(void)
{
// Initialize PWM output on P3.5, which corresponds to TB0.5
P3SEL |= BIT5; // Select peripheral output mode for P3.5
P3DIR |= BIT5;
TB0CTL = (TBSSEL__ACLK|ID__1|MC__UP); // Configure Timer B0 to use ACLK, divide by
1, up mode
TB0CTL &= ~TBIE; // Explicitly Disable timer interrupts for safety
// Now configure the timer period, which controls the PWM period
// Doing this with a hard coded values is NOT the best method
// We do it here only as an example. You will fix this in Lab 2.
TB0CCR0 = 128; // Set the PWM period in ACLK ticks
TB0CCTL0 &= ~CCIE; // Disable timer interrupts
// Configure CC register 5, which is connected to our PWM pin TB0.5
TB0CCTL5 = OUTMOD_7; // Set/reset mode for PWM
TB0CCTL5 &= ~CCIE; // Disable capture/compare interrupts
TB0CCR5 = TB0CCR0/2; // Configure a 50% duty cycle
}
/*
* Disable the buzzer on P7.5
*/
void BuzzerOff(void)
{
// Disable both capture/compare periods
TB0CCTL0 = 0;
TB0CCTL5 = 0;
}
void initLaunchpadButtons(void)
{
P1SEL &= ~BIT1;
P1DIR &= ~BIT1;
P1REN |= BIT1;
P1OUT |= BIT1;
P2SEL &= ~BIT1;
P2DIR &= ~BIT1;
P2REN |= BIT1;
P2OUT |= BIT1;
}
char readLaunchpadButtons(void)
{
// Read input registers for both buttons
// into variable that is 0 or 1
char b0 = (~P1IN & BIT1) >> 1;
char b1 = (~P2IN & BIT1) >> 1;
// Finally, combine the variables to create the output
// in the format we specified
return (b1 << 1) | b0;
}
void initLeds(void)
{
// Configure LEDs as outputs, initialize to logic low (off)
// Note the assigned port pins are out of order test board
// Red P6.2
// Green P6.1
// Blue P6.3
// Yellow P6.4
// smj -- 27 Dec 2016
P6SEL &= ~(BIT4|BIT3|BIT2|BIT1);
P6DIR |= (BIT4|BIT3|BIT2|BIT1);
P6OUT &= ~(BIT4|BIT3|BIT2|BIT1);
}
void KeypadInit(void)
{
// Configure digital IO for keypad
// smj -- 27 Dec 2015
// Col1 = P1.5 =
// Col2 = P2.4 =
// Col3 = P2.5 =
// Row1 = P4.3 =
// Row2 = P1.2 =
// Row3 = P1.3 =
// Row4 = P1.4 =
P1SEL &= ~(BIT5|BIT4|BIT3|BIT2);
P2SEL &= ~(BIT5|BIT4);
P4SEL &= ~(BIT3);
// Columns are ??
P2DIR |= (BIT5|BIT4);
P1DIR |= BIT5;
P2OUT |= (BIT5|BIT4); //
P1OUT |= BIT5; //
// Rows are ??
P1DIR &= ~(BIT2|BIT3|BIT4);
P4DIR &= ~(BIT3);
P4REN |= (BIT3); //
P1REN |= (BIT2|BIT3|BIT4);
P4OUT |= (BIT3); //
P1OUT |= (BIT2|BIT3|BIT4);
}
unsigned char KeypadGetKey(void)
{
// Returns ASCII value of key pressed from keypad or 0.
// Does not decode or detect when multiple keys pressed.
// smj -- 27 Dec 2015
// Updated -- 14 Jan 2018
unsigned char ret_val = 0;
// Set Col1 = ?, Col2 = ? and Col3 = ?
P1OUT &= ~BIT5;
P2OUT |= (BIT5|BIT4);
// Now check value from each rows
if ((P4IN & BIT3)==0)
ret_val = '1';
if ((P1IN & BIT2)==0)
ret_val = '4';
if ((P1IN & BIT3)==0)
ret_val = '7';
if ((P1IN & BIT4)==0)
ret_val = '*';
P1OUT |= BIT5;
// Set Col1 = ?, Col2 = ? and Col3 = ?
P2OUT &= ~BIT4;
// Now check value from each rows
if ((P4IN & BIT3)==0)
ret_val = '2';
if ((P1IN & BIT2)==0)
ret_val = '5';
if ((P1IN & BIT3)==0)
ret_val = '8';
if ((P1IN & BIT4)==0)
ret_val = '0';
P2OUT |= BIT4;
// Set Col1 = ?, Col2 = ? and Col3 = ?
P2OUT &= ~BIT5;
// Now check value from each rows
if ((P4IN & BIT3)==0)
ret_val = '3';
if ((P1IN & BIT2)==0)
ret_val = '6';
if ((P1IN & BIT3)==0)
ret_val = '9';
if ((P1IN & BIT4)==0)
ret_val = '#';
P2OUT |= BIT5;
return(ret_val);
}
//take 2 on making a button press detectio
unsigned char ReadS1Button(void)
{
unsigned char ret_val = 0;
if ((P7IN & BIT0)==0)
ret_val = '4';
return(ret_val);
}
unsigned char ReadS2Button(void)
{
unsigned char ret_val = 0;
if ((P3IN & BIT6)==0)
ret_val = '3';
return(ret_val);
}
unsigned char ReadS3Button(void)
{
unsigned char ret_val = 0;
if ((P2IN & BIT2)==0)
ret_val = '2';
return(ret_val);
}
unsigned char ReadS4Button(void)
{
unsigned char ret_val = 0;
if ((P7IN & BIT4)==0)
ret_val = '1';
return(ret_val);
}
unsigned char ReadSButtonsOld(void)
{
unsigned char ret_val = 0;
if ((P7IN & BIT0)==0)
ret_val = BIT4;
if ((P3IN & BIT6)==0)
ret_val = BIT1;
if ((P2IN & BIT2)==0)
ret_val = BIT3;
if ((P7IN & BIT4)==0)
ret_val = BIT4;
return(ret_val);
}
//added not-quite peripheral, might want to move to main: read S1-S4
unsigned char readSButtons(void)
{
unsigned char ret_val = 0;
if ((P7IN & BIT0)==0)
ret_val = '8';
if ((P3IN & BIT6)==0)
ret_val = '4';
if ((P2IN & BIT2)==0)
ret_val = '2';
if ((P7IN & BIT4)==0)
ret_val = '1';
return(ret_val);
}
void configDisplay(void)
{
// Enable use of external clock crystals
P5SEL |= (BIT5|BIT4|BIT3|BIT2);
// Initialize the display peripheral
#if defined(DISPLAY_SHARP96x96)
Sharp96x96_Init();
#elif defined(DISPLAY_SHARP128x128)
Sharp128x128_initDisplay();
Sharp128x128_enable();
#endif
// Configure the graphics library to use this display.
// The global g_sContext is a data structure containing information the library uses
// to send commands for our particular display.
// You must pass this parameter to each graphics library function so it knows how to
// communicate with our display.
#if defined(DISPLAY_SHARP96x96)
Graphics_initContext(&g_sContext, &g_sharp96x96LCD);
#elif defined(DISPLAY_SHARP128x128)
Graphics_initContext(&g_sContext, &g_sharp128x128LCD);
#endif
Graphics_setForegroundColor(&g_sContext, ClrBlack);
Graphics_setBackgroundColor(&g_sContext, ClrWhite);
Graphics_setFont(&g_sContext, &g_sFontFixed6x8);
Graphics_clearDisplay(&g_sContext);
Graphics_flushBuffer(&g_sContext);
}
//------------------------------------------------------------------------------
// Timer1 A0 Interrupt Service Routine
//------------------------------------------------------------------------------
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR (void)
{
// Display is using Timer A1
// Not sure where Timer A1 is configured?
#if defined(DISPLAY_SHARP96x96)
Sharp96x96_SendToggleVCOMCommand(); // display needs this toggle < 1 per sec
#elif defined(DISPLAY_SHARP128x128)
Sharp128x128_SendToggleVCOMCommand();
#endif
}