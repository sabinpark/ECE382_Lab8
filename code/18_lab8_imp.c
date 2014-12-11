//-------------------------------------------------------------------------------
//	Name:		C2C Sabin Park
//	Term:		Fall 2014
//	MCU:		MSP430G2553
//	Date:		10 December 2014
//	Note:		Lab 8: implementation file for the robot maze lab
//-------------------------------------------------------------------------------


#include "msp430g2553.h"
#include "robot_2.h"

void initMSP430() {

	IFG1=0; 							// clear interrupt flag1
	WDTCTL=WDTPW+WDTHOLD; 				// stop WD

	BCSCTL1 = CALBC1_8MHZ;
	DCOCTL = CALDCO_8MHZ;

	P2SEL  &= ~BIT6;					// Setup P2.6 as GPIO not XIN
	P2SEL2 &= ~BIT6;
	P2DIR &= ~BIT6;
	P2IFG &= ~BIT6;						// Clear any interrupt flag
	P2IE  |= BIT6;						// Enable PORT 2 interrupt on pin change

	HIGH_2_LOW;
	P1DIR |= BIT0 | BIT6;				// Enable updates to the LED
	LEDS_OFF;							// And turn the LED off

	TA0CCR0 = 0x8000;					// create a 16mS roll-over period
	TACTL &= ~TAIFG;					// clear flag before enabling interrupts = good practice
	TACTL = ID_3 | TASSEL_2 | MC_1;		// Use 1:1 presclar off MCLK and enable interrupts

	_enable_interrupt();
}

unsigned int centerSensor() {
	ADC10CTL0 = 0;
	ADC10CTL1 = INCH_4 | ADC10DIV_3 ;						// Make P1.3 analog input
	ADC10CTL0 = SREF_0 | ADC10SHT_3 | ADC10ON | ENC;		// Vcc & Vss as reference

	ADC10CTL0 |= ADC10SC;									// Start a conversion
	while(ADC10CTL1 & ADC10BUSY);							// Wait for conversion to complete

	return ADC10MEM;
}

unsigned int rightSensor() {
	ADC10CTL0 = 0;
	ADC10CTL1 = INCH_5 | ADC10DIV_3 ;						// Make P1.3 analog input
	ADC10CTL0 = SREF_0 | ADC10SHT_3 | ADC10ON | ENC;		// Vcc & Vss as reference

	ADC10CTL0 |= ADC10SC;									// Start a conversion
	while(ADC10CTL1 & ADC10BUSY);							// Wait for conversion to complete

	return ADC10MEM;
}

unsigned int leftSensor() {
	ADC10CTL0 = 0;
	ADC10CTL1 = INCH_3 | ADC10DIV_3 ;						// Make P1.3 analog input
	ADC10CTL0 = SREF_0 | ADC10SHT_3 | ADC10ON | ENC;		// Vcc & Vss as reference

	ADC10CTL0 |= ADC10SC;									// Start a conversion
	while(ADC10CTL1 & ADC10BUSY);							// Wait for conversion to complete

	return ADC10MEM;
}

/*
 * stops the motors
 */
void stop() {

	LEFT_DISABLE;
	RIGHT_DISABLE;

	LEDS_OFF;
}

/*
 * enables the left and right PWMs
 */
void go() {
	LEFT_ENABLE;
	RIGHT_ENABLE;
}


void moveForward() {
	go();
	LEFT_OM_HI;
	RIGHT_OM_HI;

	LEFT_SET_CCW;
	RIGHT_SET_CW;

	RED_OFF;
	GREEN_ON;
}

/*
 * moves the robot backward
 */
void moveBackward() {
	go();

	LEFT_OM_LO;
	RIGHT_OM_LO;

	// LEFT MOTOR
	LEFT_SET_CW;

	// RIGHT MOTOR
	RIGHT_SET_CCW;

	RED_ON;							// red LED ON
	GREEN_OFF;						// green LED OFF
}

/*
 * turns the robot CCW
 */
void rotateLeft() {
	go();

	LEFT_OM_LO;
	RIGHT_OM_HI;

	// LEFT MOTOR
	LEFT_SET_CW;
	// RIGHT MOTOR
	RIGHT_SET_CW;

	RED_ON;							// red LED ON
	GREEN_OFF; 						// green LED OFF
}

void rotateLeft2(int deg_delay) {
	go();

	LEFT_OM_LO;
	RIGHT_OM_HI;

	// LEFT MOTOR
	LEFT_SET_CW;
	// RIGHT MOTOR
	RIGHT_SET_CW;

	RED_ON;							// red LED ON
	GREEN_OFF; 						// green LED OFF

	int i;

	switch(deg_delay) {
	case 15:
		_delay_cycles(DELAY_15);
		break;
	case 45:
		_delay_cycles(DELAY_45);
		break;
	case 90:
		_delay_cycles(DELAY_90);
		break;
	case 180:
		_delay_cycles(DELAY_180);
		break;
	case 360:
		_delay_cycles(DELAY_360);
		break;
	default:
		for(i=0; i<deg_delay; i++) {
			_delay_cycles(DELAY_360/DEG_360);
		}
	}
}

/*
 * turns the robot CW
 */
void rotateRight() {
	go();

	LEFT_OM_LO;
	RIGHT_OM_HI;//RIGHT_OM_LO;

	// LEFT MOTOR
	LEFT_SET_CCW;
	// RIGHT MOTOR
	RIGHT_SET_CCW;

	GREEN_ON;						// green LED ON
	RED_OFF;						// red LED OFF
}

void driveNE() {
	go();

	// LEFT MOTOR
	LEFT_SET_CCW;
	LEFT_OM_HI;

	// RIGHT MOTOR
	RIGHT_SET_CW;
	RIGHT_OM_LO;

	GREEN_ON;						// green LED ON
	RED_OFF;						// red LED OFF

	//_delay_cycles(LONG_DELAY/2);
}

/*
 * updates the timer signal lengths
 */
void updateSignal() {
	TA1CCR2 = TA1CCR0-TA1CCR1;
}

/*
 * increase the duty cycle
 */
void increaseDC() {
	if(TA1CCR1 < 90) {
		TA1CCR1 += 5;
	}
	updateSignal();

	GREEN_ON;						// green LED ON
	RED_OFF;						// red LED OFF
}

/*
 * decrease the duty cycle
 */
void decreaseDC() {
	if(TA1CCR1 > 40) {
		TA1CCR1 -= 5;
	}
	updateSignal();

	GREEN_OFF;						// green LED OFF
	RED_ON	;						// red LED ON
}

/*
 * reset the duty cycle back to the original
 */
void resetSignal() {
	TA1CCR1 = 60;
	TA1CCR2 = TA1CCR0-TA1CCR1;

	GREEN_ON;						// green LED ON
	RED_ON;							// red LED ON
}
