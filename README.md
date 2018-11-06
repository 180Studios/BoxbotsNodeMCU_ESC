# BoxbotsNodeMCU_ESC
Electronic Speed Controller (ESC) for Boxbots NodeMCU with EXP12E Motor Shield
BoxBots robot motor control for NodeMCU & ESP12E Motor Shield 
Converts 2 RC Servo 20ms Pulse width signals to high speed PWM signal for 2 channel bidirectional motor speed control
Adapted from AMT Boxbots control for arduino

## Hardware: 
* NodeMCU ESP12E mounted on...
* ESP12E Motor Shield. 
    * *Note: The L293D driver IC is wired on the PCB in a way that does not support braking*

### Radio channel connection:
* CH1 - Steering, connect to D5 on shield board
* CH2 - Throttle, connect to D6 on shield board
* CH3 - Weapon, not used. Suggest connect to D7

### Motor Wiring: 
*When looking from front of robot:*
* Left Motor wired to motor terminals A, Red to MINUS terminal (A-) 
* Right Motor wired to motor terminals B, Red to PLUS terminal (B+)

*When looking at front of the axel (motor shaft):*
* Solder Red wire to motor terminal that makes the motor turn clockwise when a positive voltage is applied

### Power Connections:
#### Standard Power Connection
* Battery connected to **VM** terminals
* VIN terminals not used
* Install header shunt VIN to VM pins
#### With Motor Voltage booster
*  Battery and Booster Input wires connected to **VIN** terminals
   * _that's two wires per terminal_
*  connect Booster output to **VM** terminals
*  Do **NOT** install header shunt VIN to VM
