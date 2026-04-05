#include <Alpha.h>

float L ;
float R ;

void goAngle(double heading, unsigned short speed) {
	
    while (heading < 0) {
        heading += 360 ;
    }
    
	while (heading >= 360) {
        heading -= 360 ;
    }
    
    if(0 <= heading && heading < 90) {
        R = 1 - (heading / 45);
        L = 1;
    }
    
    else if(90 <= heading && heading < 180) {
        L = 1 - ((heading - 90) / 45);
        R = -1;
    }
    
    else if(180 <= heading && heading < 270) {
        R = -(1 - ((heading - 180) / 45));
        L = -1;
    }
    
    else if(270 <= heading && heading < 360) {
        L = -(1 - ((heading - 270) / 45));
        R = 1;
    }

    motor(0, R * speed);
    motor(2, R * speed);
    motor(1, L * speed);
    motor(3, L * speed);
}

void goAngleAnalog(double heading, unsigned short speed, unsigned short analogPort, int thresh, bool stopBelowThresh, bool stopAboveThresh) {
    if (stopBelowThresh == true) {
        while (analog(analogPort) >= thresh) {
            goAngle(heading, speed) ;
        }
    }
    
    if (stopAboveThresh == true) {
        while (analog(analogPort) <= thresh) {
            goAngle(heading, speed) ;
        }
    }
}
