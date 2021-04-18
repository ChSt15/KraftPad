# Interval-Control
A simple to use class that helps to control loop rates for multiple things.
## How it works:
You simply create an object with this class and tell it the rate it should run at. Then simply use .isTimeToRun() to check if enough time has passed to run a section of code.
## The two modes:
### Time skipping:
This is the default mode. The timer will only limit the loop rate. Meaning if the timer is behind then it will still run at given rate.
### Time Locked:
The timer will try to run at a given amount of times in a certain time. Meaning if the timer is behind it will run as fast as possible to catch up. This is rarely needed.
## What functions it offers:
#### .isTimeToRun(false) 
will cause the timer to be locked. Meaning if the rate is at 2Hz and 1 second has passed, then the interval will run as fast as possible to catch up.
#### .isTimeToRun(deltaTime) 
will write the amount of time has passed into the deltaTime varible.
#### .block(true)
will stop the code from running. .block(false) reenables it.
#### .waitTillNextRun() 
is basically a delay that will wait until enough time has passed for the next run.
#### .waitTillNextRun(myFunction) 
will wait untill the next run but during that time call myFunction repeatedly. myFunction must be: void myFunction(void)
#### .syncInterval() 
will sync the timer when using time locked mode. Will stop the timer from trying to catch up.
#### .setLimit(true) 
switches to time skipping mode.
#### .setLimit(false) 
switches to time locked mode.
#### .setRate(1000) 
sets the rate at which the timer should run at.
