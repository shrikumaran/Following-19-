# Following-19-
Codes for my advanced line following robot for the Following'19 competition.

The competition involved detecting shapes printed out on papers where each shape had their corresponding coordinate, 
then this information is communicated to the robot via bluetooth.

The robot uses a Arduino UNO for processing and a HC05 bluetooth module. The IR sensor array was fabricated on a perf board with IR leds
and IR receivers. It runs a grid solving algorithm to find which coordinate it is currently at to better tailor its path to the destination node given during the competition. For motor drivers I used the L293d motor driver ICs. Used the H-bridge in it to control the direction.

The competition was divided into two phases, in phase 1 the target coordinates are manually entered from a script on the laptop which then send it to the robot via bluetooth. In phase 2 a few shapes are printed out and shown to the laptop camera and my python script identifies it and the assigned coordinates to the shapes. Then these coordinates are sent to the robot.
