ECE382_Lab8
===========

## Prelab
#### Pseudo-code
```
// Door 1:

// initially drive forward and detect walls that are apx. 5 inches away
// if right sensor is ON
    // turn 45 deg. left
    // if center sensor voltage is greater than right sensor voltage, turn again
// else if left sensor is ON
    // turn 45 deg. right
    // if center sensor voltage is greater than left sensor voltage, turn again
// drive forward and repeat
