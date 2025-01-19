# Line Tracer 
[HL Mando & Klemove IVS 3rd] Line tracer를 통한 차량용 임베디드 시스템의 이해



### RC Car & Map
***
![rccar](https://github.com/user-attachments/assets/9a0e733e-e32b-40ab-823b-4c08f7659155)
<img width="966" alt="track" src="https://github.com/user-attachments/assets/0c9ee86f-8855-434f-8bdc-1af393c9d385" />



### Competiton Rule
***
1. Measure the lap time after completing two consecutive laps of line tracing on the designated track, and determine the ranking based on the results.
2. If line tracing is performed using a single sensor, an advantage of -10 seconds will be granted.
- Stopping midway or deviating from the line will result in disqualification.
- Remote control adjustments are not allowed during driving.




### Hardware
***
- Arduino Uno
- BreadBoard
- DC Motor (2EA)
- Motor Driver Module(L9110S) 
- IR Sensor(TCRT5000) (2EA)


### Software


### Entire Logic of the Program
***
This program is designed to enable an RC car (Line Tracer) to follow a track using infrared sensors and implement precise control through a PID algorithm. 

During initialization, the `setup()` function configures the hardware, including IR remote reception and motor control pins, and ensures the motors start in a stopped state.

In the main loop (`loop()`), the program processes inputs from the IR remote to activate or deactivate the line-tracing functionality. When activated, the program reads data from the infrared sensors to detect deviations from the track. It calculates the error between sensor readings and applies a PID control algorithm to correct this error. The PID algorithm combines proportional (P), integral (I), and derivative (D) components to adjust the car's speed and direction, ensuring stable and accurate tracking of the line.

The calculated motor speeds and directions are then applied through the `processMotor()` function, which uses PWM signals to control each motor's speed and direction based on the PID output. This process runs continuously, allowing the RC car to dynamically adjust its movement in real-time as it follows the track.

