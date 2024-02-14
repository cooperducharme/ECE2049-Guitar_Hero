/************** ECE2049 DEMO CODE ******************/
/************** 14 May 2018   **********************/
/***************************************************/
// Guitar Hero Template
#include <grlib.h>
#include <LcdDriver/sharp96/Sharp96x96.h>
#include <msp430f5529.h>
#include <peripherals.h>

//#include "lecture.h"//maybe unnessesary
//#include "utils/ustdlib.h"
unsigned long int timer = 0;
// Function Prototypes
void swDelay(char numLoops);
void configTimer();
void drawSomeThings(void);
void setLeds(unsigned char state);
void pause(unsigned long int duration);
void buzzerTicks(int ticks);
void Timer_A2_ISR(void);


//void lightOn(int num);

// Declare globals here

unsigned long int globalTime = 0;//timer for entire main

// Main
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer. Always need to stop this!!
                                 // You can then configure it properly, if desired
    _BIS_SR(GIE); //global max interrupts
    // *** System initialization ***
    initLaunchpadButtons();
    initLeds();
    configDisplay();
    KeypadInit();

    Graphics_clearDisplay(&g_sContext); // Clear the display

    // Look at this function and the comments for some examples of using the LCD
    // You can also refer back to lab 0 and the state machine example
    // for how to use the keypad and LEDs
    drawSomeThings();


    //Main Additions

    //might go after the rest
    P6SEL &= (BIT3|BIT2|BIT1|BIT4); // Set LED pins for digital I/O           edit: used to have ~
    //P6DIR |= (BIT3|BIT2|BIT1|BIT0); // Set LED pins as outputs
    //P6OUT &= ~(BIT3|BIT2|BIT1|BIT0); // Turn LEDs off (set output register to 0)

    //LED Configuration


    P6DIR |= BIT2;//red
    P6DIR |= BIT1;//green
    P6DIR |= BIT3;//blue
    P6DIR |= BIT4;//yellow

    P6REN &= ~BIT2;//red
    P6REN &= ~BIT1;//green
    P6REN &= ~BIT3;//blue
    P6REN &= ~BIT4;//yellow

    P6OUT &= ~(BIT2&BIT1&BIT3&BIT4); // Turn LEDs off (set output register to 0)
    //P6OUT |= (BIT2|BIT1|BIT3|BIT4);
    //Button Configuration


    P7SEL &= ~(BIT0);// Set button pins for digital I/O
    P2SEL &= ~(BIT2); // Set button pins for digital I/O
    P3SEL &= ~(BIT6); // Set button pins for digital I/O
    P7SEL &= ~(BIT4); // Set button pins for digital I/O


    P7DIR &= ~(BIT0); // Set button pins for digital I/O
    P2DIR &= ~(BIT2); // Set button pins for digital I/O
    P3DIR &= ~(BIT6); // Set button pins for digital I/O
    P7DIR &= ~(BIT4); // Set button pins for digital I/O

    P7REN |= BIT0;//S1
    P3REN |= BIT6;//S2
    P2REN |= BIT2;//S3
    P7REN |= BIT4;//S4

    P7OUT |= BIT0;//S1
    P3OUT |= BIT6;//S2
    P2OUT |= BIT2;//S3
    P7OUT |= BIT4;//S4


    //Buzzer Configuration
        //at this stage, maybe treat like an LED
    P4REN = BIT1;

        //initLeds();// most recent edit: 4/16

    unsigned char ret_val=0;

    int loop = 0;
    int A = 440;
    int B = 494;
    int c = 523;
    int D = 587;
    int E = 659;
    int F = 698;
    int G = 784;

    int state = 0;

    char endTrigger = 1;
    while (endTrigger)    // Forever loop
    {
    //  char ret_val = readLaunchpadButtons();
  //    setLeds(~ret_val);
        char ret_val_begin = readSButtons();
        int songNotes[32] = {B, A, G, B, A, G, c, A, G, c, D, G, B, B, E, F, A, c, B, F, F, E, B, A, D, F, E, B, A, D, c, A};
        int songDelay[32] = {10, 10, 50, 10, 10, 50, 50, 50, 250, 50, 50, 30, 50, 50,50, 50, 25, 50, 50, 30, 10, 50, 50, 250, 50, 50, 25, 50, 50, 10, 10, 10};
        int wrongNotes = 0;

        configClock();

        //playing a song automaticaly

        int playSongIfEqual1 = 1;
        while (playSongIfEqual1){
            volatile unsigned int j;

            for (j=0; j<31; j++)
            {
                buzzerTicks(songNotes[j]);
                //swDelay(songDelay[j]);
                //pauseInternal(songDelay[j]*10000000);
                pause(songDelay[j]);
                //setLeds('1');
            }
        }
        //end of automatic songplaying

        //int keyboardModeIfEqual1 = 1;
        switch (state)
        {

            case 0: //Welcome

            BuzzerOff();
            char ret_val_begin = readSButtons();


            while(ret_val_begin)
            {
                //BuzzerOn();

                //pause(2);
                //BuzzerOff();
                //pause(10);


                char ret_val = readSButtons();
                            setLeds(~ret_val);
                if (~P7IN & BIT0)
                    buzzerTicks(A);
                    setLeds('7');//'8');
                    //pause(20);
                if (~P3IN & BIT6)
                    buzzerTicks(c);
                    setLeds('11');//'4');
                    //pause(20);
                if (~P2IN & BIT2)
                    buzzerTicks(E);
                    setLeds('13');//'2');
                    //pause(20);
                if (~P7IN & BIT4)
                    buzzerTicks(G);
                    setLeds('14');//'1');
                    //pause(20);
                    //ret_val_begin = 0;
                    //endTrigger = 0;
            }
        }
    }  // end while (1)
}




void swDelay(char numLoops)
{
    // This function is a software delay. It performs
    // useless loops to waste a bit of time
    //
    // Input: numLoops = number of delay loops to execute
    // Output: none
    //
    // smj, ECE2049, 25 Aug 2013

    volatile unsigned int i,j;  // volatile to prevent removal in optimization
                                // by compiler. Functionally this is useless code

    for (j=0; j<numLoops; j++)
    {
        i = 50000 ;                 // SW Delay
        while (i > 0)               // could also have used while (i)
           i--;
    }
}

void drawSomeThings(void)
{
    // Write some text to the display
    // Note:  the constants LCD_HORIZONTAL_MAX and LCD_VERTICAL_MAX
    //        hold the max coordinates of the screen

    int h_center = LCD_HORIZONTAL_MAX / 2; // Variable to keep track of center position
    Graphics_drawStringCentered(&g_sContext, "Welcome",  AUTO_STRING_LENGTH, h_center, 15, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "to",       AUTO_STRING_LENGTH, h_center, 25, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "ECE2049!", AUTO_STRING_LENGTH, h_center, 35, TRANSPARENT_TEXT);

    // Draw some circles
    Graphics_drawCircle(&g_sContext, 25, 60, 10);
    Graphics_drawCircle(&g_sContext, 25, 60, 7);

    // Horizontal and vertical lines
    Graphics_drawLineH(&g_sContext, 20, 80, 42);
    Graphics_drawLineV(&g_sContext, 10, 20, 80);

    // Arbitrary lines
    Graphics_drawLine(&g_sContext, 60, 60, 80, 80);
    Graphics_drawLine(&g_sContext, 60, 80, 80, 60);

    // To draw a rectangle, we need four points
    // To do this, the graphics library uses a struct to specify each point,
    // which we specify here:
    tRectangle rect; // Declare a struct
    rect.sXMin = 5;  // Fill in each of the four parameters of the struct
    rect.sXMax = 91;
    rect.sYMin = 5;
    rect.sYMax = 91;

    // Draw a rectangle by passing the struct to GrRectDraw
    // (Note that we need to pass the address of the struct!)
    Graphics_drawRectangle(&g_sContext, &rect);

    // This is another way to declare and initialize a struct (if you know all of the values)
    // Here, we also use our constants LCD_HORIZONTAL_MAX and LCD_VERTICAL_MAX
    Graphics_Rectangle box = {.xMin = 5, .xMax = LCD_HORIZONTAL_MAX - 5,
                              .yMin = 5, .yMax = LCD_VERTICAL_MAX - 5 };
    Graphics_drawRectangle(&g_sContext, &box);


    // We are now done writing to the display.  However, if we stopped here, we would not
    // see any changes on the actual LCD.  This is because we need to send our changes
    // to the LCD, which then refreshes the display.
    // Since this is a slow operation, it is best to refresh (or "flush") only after
    // we are done drawing everything we need.
    Graphics_flushBuffer(&g_sContext);
}

void setLeds(unsigned char state)
{
    // Turn on 4 colored LEDs on P6.1-6.4 to match the hex value
    // passed in on low nibble state. Unfortunately the LEDs are
    // out of order with 6.2 is the left most (i.e. what we think
    // of as MSB), then 6.1 followed by 6.3 and finally 6.4 is
    // the right most (i.e.  what we think of as LSB) so we have
    // to be a bit clever in implementing our LEDs
    //
    // Input: state = hex values to display (in low nibble)
    // Output: none
    //
    // smj, ECE2049, 27 Dec 2015

    unsigned char mask = 0;

    // Turn all LEDs off to start
    P6OUT &= ~(BIT4|BIT3|BIT2|BIT1);

    if (state & BIT0)
        mask |= BIT4;   // Right most LED P6.4
    if (state & BIT1)
        mask |= BIT3;   // next most right LED P.3
    if (state & BIT2)
        mask |= BIT1;   // third most left LED P6.1
    if (state & BIT3)
        mask |= BIT2;   // Left most LED on P6.2
    P6OUT |= mask;
}

void buzzerTicks(int ticks)
{
    // Initialize PWM output on P3.5, which corresponds to TB0.5
    P3SEL |= BIT5; // Select peripheral output mode for P3.5
    P3DIR |= BIT5;

    TB0CTL  = (TBSSEL__ACLK|ID__1|MC__UP);  // Configure Timer B0 to use ACLK, divide by 1, up mode
    TB0CTL  &= ~TBIE;                       // Explicitly Disable timer interrupts for safety

    // Now configure the timer period, which controls the PWM period
    // Doing this with a hard coded values is NOT the best method
    // We do it here only as an example. You will fix this in Lab 2.
    TB0CCR0 = 32768/ticks; // Set the PWM period in ACLK ticks
    TB0CCTL0 &= ~CCIE;                  // Disable timer interrupts

    // Configure CC register 5, which is connected to our PWM pin TB0.5
    TB0CCTL5  = OUTMOD_7;                   // Set/reset mode for PWM
    TB0CCTL5 &= ~CCIE;                      // Disable capture/compare interrupts
    TB0CCR5   = TB0CCR0/2;                  // Configure a 50% duty cycle
}

void pause(unsigned long int duration)
{
    volatile long int timeFinish = globalTime+duration;
    while(globalTime <=timeFinish){

    }
}


void configClock() //enables interupts with ACLK + divider + mode
{

    TA2CTL = TASSEL_1 + MC_1 + ID_0;
    TA2CCR0= 327/2;//origionaly 327
    TA2CCTL0 = CCIE;

}

#pragma vector=TIMER2_A0_VECTOR
__interrupt void Timer_A2_ISR(void)
{
    globalTime++;
}
//Final Version
