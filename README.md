ECE382_Lab8
===========

## *Functionality Update*

|Functionality|Status|Date Completed|Checked by|
|:-:|:-:|:-:|:-:|
| Required | Complete | 11 Dec 14 @ 0700 | Dr. Coulston, Dr. York |
| B | Complete | 11 Dec 14 @ 0700 | Dr. Coulston, Dr. York |
| A | Complete | 11 Dec 14 @ 0700 | Dr. Coulston, Dr. York |

A functionality may be viewd [here](http://youtu.be/e6VjWnEAKvg).

## Overview
Use the previous laboratory assignments (both the robot and the code) to program my robot to autonomously navigate through a maze.

## Requirements
1. Your robot must always start at the home position.
2. Your robot is considered successful only if it finds one of the three exits and moves partially out of the maze.
3. A large portion of your grade depends on which door you exit.
    * Door 1: Required Functionality
    * Door 2: B Functionality
    * Door 3: A Functionality (cannot hit a wall)
    * Bonus: Navigate from the A functionality door back to the entrance using the same algorithm (cannot hit a wall)
4. Your robot must solve the maze in less than three minutes.
5. Your robot will be stopped if it touches the wall more than twice.
6. Your robot must use the IR sensors to find its path through the maze.

*NOTE*: Each functionality attempted will have that corresponding door open. We do not have to have the robot sense for gaps or openings in the walls.

## Prelab
#### Pseudo-code
For the prelab, I will simply use Lab 7 code which lit up the LEDs using the sensors. This time, instead of just toggling the LEDs, I will also control the motors based on the IR sensor inputs.

```
// Door 1:

// initially drive forward (slow to moderate) and detect walls that are apx. 5 inches away
// if right sensor is ON
    // turn 45 deg. left
    // if center sensor voltage is greater than right sensor voltage, turn again
// else if left sensor is ON
    // turn 45 deg. right
    // if center sensor voltage is greater than left sensor voltage, turn again
// else (if neither the left or right sensors are ON)
    // turn 45 deg. right (default)
// drive forward and repeat


// Door 2 and Door 3 [Following a wall method]

// Use right and center sensors (use methods to store the MEM values for each sensor into an int]
// if center sensor reads value higher than cutoff_center...
    // turn left
// else if right sensor reads value higher than cutoff_right...
    // turn left
// else if right sensor reads too low of a value (meaning the robot is drifting off to the left)...
    // right right
// else move forward
```

## Lab

### Required Functionality
I began by copying over the code from lab 7 and retesting the LEDs. As expected, the LEDs lit up when I placed an object in front of the transmitter/receiver pair. Next, I created new main.c and implementation.c  files. The implementation.c file was updated with the movement functions used in lab 6.

Although the code worked properly when I tested the LEDs only, when I added the movements from the motors, I was getting very strange results. Instead of turning on and off with the same responsiveness of the LEDs, the motors were erratic in their responses. After trying various things, I eventually was able to get decent responses from the motors (please see the debugging section for details).

I now had to code my maze-solving algorithm. For required functionality, all I had to do was make my robot drive forward until it arrived at the first perpendicular wall and then turn 90 degrees left to exit the maze through door 1.

I started by creating three functions, one for each sensor: right, center, and left. These functions are used to compare the converted packet data from the threshold voltages characterized in lab 7. Below is the method for the center sensor.

```
unsigned int centerSensor() {
	ADC10CTL0 = 0;
	ADC10CTL1 = INCH_4 | ADC10DIV_3 ;						// Make P1.3 analog input
	ADC10CTL0 = SREF_0 | ADC10SHT_3 | ADC10ON | ENC;		// Vcc & Vss as reference

	ADC10CTL0 |= ADC10SC;									// Start a conversion
	while(ADC10CTL1 & ADC10BUSY);							// Wait for conversion to complete

	return ADC10MEM;
}
```

Since I had to check for the first corner (NE from the robot's perspective), I had the robot turn left when the center or right sensors were reading in high voltages. To be sure the robot would not hit the left side of the wall, I made another check to turn the robot to the right slightly if the left sensor was activated. In between the left and right walls, I made the robot go forward. 

```
	while(1) {

		c = centerSensor();
		r = rightSensor();
		l = leftSensor();

		int counter = 0;

		if((c > centerCutoff || r > rightCutoff) && l < leftCutoff) {
			rotateLeft();
			_delay_cycles(1000);
		} else if(l > leftCutoff) {
			rotateRight();
			_delay_cycles(1000);
		}
		else {
			stop();
			moveForward();
			_delay_cycles(1000);
		}

		moveForward();
	}
```

Using these simple checks, my robot passed through the first portion of the maze and cleared door #1.

*Required Functionality Success!*

#### Required Functionality [Debugging]
I had some problems with the motors. The IR sensors were properly detecting voltage changes when used with the LEDs only, but when I started to run the motors and try to toggle the motors' changes using the same threshold voltages, I got very inconsistent results. Even when I figured out the approximate threshold voltage for each of the motors, I still could not properly toggle any changes. It seemed that the extra voltage used to rotate the motors interfered with the IR readings.

The main problem I had with this was that at random times, the motor would infinitely repeat a given movement even if it no longer sensed a high enough voltage which would toggle the appropriate response.  

I spent quite a bit of time trying various things to solve this issue, but in the end, I could not figure it out. It is possible that this may be a software issue, but after looking through my code and comparing with other classmates, I concluded that it may actually be a hardware issue. 

Having spent too much time on this issue, I decided to call it good and move on to actually coding my robot to move through the maze.

### B Functionality
*NOTE*: I moved straight into A Functionality. Please see below.
#### B Functionality [Debugging]
*NOTE*: I moved straight into A Functionality. Please see below.

### A Functionality
For A functionality, I had to code the robot to go through door #3. This meant the robot would essentially have to account for both the NE and the NW corner. Thus, the robot would somehow have state conditions that would allow it to know when to take a right or left turn at each corner. 

But perhaps I could try something else. Another method would be to have the robot hug either the right or left wall. In this way, the robot would simply use either the left or right sensor and ensure that particular sensor will always have something detecting. 

I decided to use the wall-hugging method using the right side of the maze (and correspondingly, the right sensor).

Similar to the code from the required functionality, I created various if-statements to check for the few cases my robot would have to navigate through.

Cases:
* Center sensor only
   * This situation accounts for when the robot is faced with a wall straight-on and has either side open
   * The robot will be told to turn to the left in these cases
* Between the min and max range from the right wall
   * I did not want my robot to stray too far off to the left side or too close to the right side
   * Between these two extremes, I will tell my robot to go forth and conquer (though somewhat in an S-curve path)
* Close to the right wall
   * The robot will sense that it is too close to the right wall and will veer to the left
* Too far from the right wall
   * Once the robot has veered too far from the right wall, it will turn right to make sure its right sensor is still tracking a wall

```
	while(1) {

			c = centerSensor();
			r = rightSensor();

			if(c > cClose) {					// center sense
				moveForward();
				_delay_cycles(100);
				rotateLeft();
				_delay_cycles(500);
			} else if(r > rFar && r < rClose) {	// boundary
				//stop();
				moveForward();
				_delay_cycles(1000);
			}
			else if(r > rClose) {				// right sense
				moveForward();
				_delay_cycles(500);
				rotateLeft();
				_delay_cycles(500);
			} else if(r < rFar) {				// far from the right wall
				moveForward();
				_delay_cycles(800);
				rotateRight();
				_delay_cycles(700);
			} 
		   moveForward();
		}
```

#### A Functionality [Debugging]
After getting required functionality, getting A funcitonality to work was not that difficult. There are, however, a few things I had to work around (literally).

For example, I had to figure out a way for the robot to get around the hair-pin turn located in the middle of the maze. At first, I noticed that the robot would be fine tracking the wall, but when it approaches the tip of the hair-pin turn, it would immediately begin to turn to the right and crash into the edge of the turn. To counter this problem, I recoded the robot to delay a little bit longer while going forward before turning at the hair-pin. This seemed to work for some of the trials, but it was not very reliable. 

I then tried a different approach (literally). Instead of having the robot start off perfectly straight-facing the maze, I angled the starting position of the robot 45 degrees to the right. By changing the launch angle, the robot traversed the maze using a different path. The different path allowed the robot to bounce off the wall right before the hair-pin turn and allowed it to make a smooth, rounded turn, completely avoiding the hair-pin. Unfortunately, this was not 100% reliable, but it was better than before.

Another thing I currently have problems with is the robot's consistency. As of now, the robot successfully makes it through the maze about 1 out of ten times. Again, I believe the root of the problem lies with the hardware issues I was having from the required functionality section. However, after all the debugging hours I spent on it, I decided to move on to bigger and better things...

### Bonus Functionality
Incomplete. Needs some tweaking.
#### Bonus Functionality [Debugging]
N/A

## Competition
Jeff 2.0 had his debut today in the ECE Robot Maze Competition. The competition consisted of going through the maze and exiting through Door #3. Though his practice run was a 10.48 seconds the other day, he still managed to pull off a 14.34 seconds for today's competition. I think Jeff 2.0 was a bit nervous and his hardware started to get a bit hot...but regardless, good job, Jeff 2.0!

*UPDATE*: Jeff 2.0 tried the maze again with a slightly tweaked set of new instructions. His new time as of now is 12.24 seconds without touching any walls.

## Documentation
* C2C Sean Bapty gave me advice to use my `stop()` methods in between other movement calls
