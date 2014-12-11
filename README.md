ECE382_Lab8
===========

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
