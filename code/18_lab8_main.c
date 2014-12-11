//-------------------------------------------------------------------------------
//	Name:		C2C Sabin Park
//	Term:		Fall 2014
//	MCU:		MSP430G2553
//	Date:		10 December 2014
//	Note:		Lab 8: main file the robot maze lab
//-------------------------------------------------------------------------------

#include "msp430g2553.h"
#include "robot_2.h"

unsigned int c, c1, c2, c3;
unsigned int r, r1, r2, r3;
unsigned int l, l1, l2, l3;

unsigned int centerCutoff = 572;//572;//562;//0x0200;//0x0232;
unsigned int rightCutoff = 750;
unsigned int leftCutoff = 700;

unsigned int rFar = 690;//740;// 700;
unsigned int rClose = 690;//740;// 700;
unsigned int cClose = 562;//562;// 572;

//----------------------------------------------------------------------
//----------------------------------------------------------------------
int main(void) {
	initMSP430();							// Setup MSP to process IR and buttons


	IFG1=0; 								// clear interrupt flag1
	WDTCTL = WDTPW + WDTHOLD;				// disable WDT

	BCSCTL1 = CALBC1_8MHZ;					// 8MHz clock
	DCOCTL = CALDCO_8MHZ;

	P1DIR = BIT0 | BIT6;					// Set the red and green LEDs as outputs

	ADC10AE0 = BIT3 | BIT4 | BIT5;	 		// make P1.3, P1.4, and P1.5 analog inputs

	 // LEFT MOTOR PWM is OUTPUT and set to the TIMER mode
	LEFT_PWN_ENABLE_SET_OUTPUT;				// P2.2 is associated with TA1CCR1
	LEFT_P2SEL_SET;							// P2.2 is associated with TA1CCTL1		// selects the primary peripheral module function
	// RIGHT MOTOR PWN is INPUT and set to the TIMER mode
	RIGHT_PWN_ENABLE_SET_OUTPUT;			// P2.4 is associated with TA1CCR2
	RIGHT_P2SEL_SET;						// P2.4 is associated with TA1CCTL2

	// set the initial direction of the motors (FORWARD)
	// LEFT
	LEFT_SET_OUTPUT;						// set P2DIR BIT1 as an output
	LEFT_SET_CCW;							// CCW
	// RIGHT
	RIGHT_SET_OUTPUT;						// set P2DIR BIT3 as an output
	RIGHT_SET_CW;							// CW

	// disable the enablers (NO MOVEMENT)
	// LEFT
	LEFT_ENABLE_SET_OUTPUT;					// set P2DIR BIT0 as an output
	LEFT_DISABLE;							// ensures that the enable is set to 0 (disable)
	// RIGHT
	RIGHT_ENABLE_SET_OUTPUT;				// same as above
	RIGHT_DISABLE;

	// SETTINGS
	TA1CTL = ID_3 | TASSEL_2 | MC_1;		// Use 1:8 presclar off MCLK
	TA1CCR0 = 100;							// set signal period
	// LEFT motor
	TA1CCR1 = 55;//60;
	TA1CCTL1 = OUTMOD_7;					// set TACCTL1 to Reset / Set mode
	// RIGHT motor
	TA1CCR2 = 40;//TA1CCR0-TA1CCR1;
	TA1CCTL2 = OUTMOD_7;					// set TACCTL1 to Reset / Set mode

	while(1) {

		// A FUNCTIONALITY //
		c = centerSensor();
		r = rightSensor();

		if(c > cClose) {					// center sense
			moveForward();
			_delay_cycles(100);
			rotateLeft();
			_delay_cycles(500);
		} else if(r > rFar && r < rClose) {	// boundary
			moveForward();
			_delay_cycles(1000);
		}
		else if(r > rClose) {				// right sense
			rotateLeft();
			_delay_cycles(500);
			moveForward();
			_delay_cycles(500);
			rotateLeft();
			_delay_cycles(100);
		} else if(r < rFar) {				// far from the right wall
			moveForward();
			_delay_cycles(800);
			rotateRight();
			_delay_cycles(700);
		} else if(r > rFar && r < rClose) {
			stop();
			moveForward();
			_delay_cycles(100);
		}
		moveForward();

	} // end infinite loop
} // end main
