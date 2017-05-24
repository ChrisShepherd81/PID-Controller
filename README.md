# PID-Controller
4th project of the 2nd term in Udacities Self Driving Car Course

#Problem Description

The task is to keep a car on a given track controlling the steering angle within the range [-25°, 25°] and a throttle within [0, 1]. The measured values are the cross-track-error (CTE) for the current position of the car as well as the speed of the car. 
The task is to maximize the speed while keeping the car on the street (minimize the CTE). 

#Limitations

It's not possible to set the start position of the car within the test track nor can you determine the current position or the driven distance.

#Considerations

High steering angles reduces also the speed of the car.
CTE measurement errors.