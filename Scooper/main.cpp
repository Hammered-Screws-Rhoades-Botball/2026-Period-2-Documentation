#include <kipr/wombat.h>
#include <iostream>

enum ports{
    LMP = 3 /* Left Motor Port */,
    RMP = 0 /* Right Motor Port */,
    LAP = 1 /* Left Analog Port (No longer used) */,
    RAP = 0 /* Right Analog Port */
} ;

int subRight = 17 ; /* This is the value subtracted from the right motor's velocity to correct for a natural veer. (Note: replace with a multiplier so that motor velocity ratios remain constant.) */

void straight (int velocity, unsigned int distance, bool isOvershooting) ; /* Drive in a straight line for a given distance in motor ticks at a given velocity. */
void straddleLineFollow (int, int, unsigned int, int, bool) ; /* Line follow (correcting by veering) with two analog sensors that straddle the black tape. (Note: this is not used anymore; there is now only one analog sensor.) */
void pivotLineFollow (int, int, unsigned int, int, bool) ; /* Line follow (correcting by pivoting) with two analog sensors that straddle the black tape. (Note: this is not used anymore; there is now only one analog sensor.) */
void rightLineFollow (int, unsigned int, int, bool) ; /* Line follow (correcting by veering) with one analog sensor that is on the edge of the black tape. (Note: this does not work when the sensor is too far from the line.) */

int main()
{
    shut_down_in(20) ;

    /* Drive forward until the analog sensor sees the black tape. */
    while (analog(RAP) < 3200) {
		straight(100, 1, true) ;
    }
    
    /* Drive forward until the analog sensor is off the black tape */
    while (analog(RAP) > 3200) {
        straight(50, 1, true) ;
    }

    /* Line follow until the robot hits the back wall. */
    while (digital(0) == 1) {
        rightLineFollow(3200, 100, 80, true) ;
    }

    return 0;
}


void straight (int velocity, unsigned int distance, bool isOvershooting) { /* Change to a "do while" loop so that the distance argument can be 0. */
    cmpc(LMP) ;
    cmpc(RMP) ;

    if (isOvershooting == true) {
        while (abs(gmpc(LMP)) < distance && abs(gmpc(RMP)) < distance) {
            motor(LMP, velocity) ;
            motor(RMP, velocity - subRight) ;
        }
    }

    else if (isOvershooting == false) {
        while (abs(gmpc(LMP)) < distance || abs(gmpc(RMP)) < distance) {
            motor(LMP, velocity) ;
            motor(RMP, velocity - subRight) ;
        }
    }
}

void straddleLineFollow (int threshL, int threshR, unsigned int distance, int velocity, bool isOvershooting) {
    const int intensity = 20 ;
    cmpc(LMP) ;
    cmpc(RMP) ;

    if (isOvershooting == true) {
        while (abs(gmpc(LMP)) < distance || abs(gmpc(RMP)) < distance) {
            if (analog(LAP) > threshL && analog(RAP) < threshR) {
                motor(LMP, velocity - intensity) ;
                motor(RMP, (velocity + intensity) - subRight) ;
            }
            else if (analog(LAP) < threshL && analog(RAP) > threshR) {
                motor(LMP, velocity + intensity) ;
                motor(RMP, (velocity - intensity) - subRight) ;
            }
            else if (analog(LAP) < threshL && analog(RAP) < threshR) {
                motor(LMP, velocity) ;
                motor(RMP, velocity - subRight) ;
            }
            else if (analog(LAP) > threshL && analog(RAP) > threshR) {
                motor(LMP, velocity) ;
                motor(RMP, velocity - subRight) ;
            }


        }
    }

    else if (isOvershooting == false) {
        while (abs(gmpc(LMP)) < distance && abs(gmpc(RMP)) < distance) {
            if (analog(LAP) > threshL && analog(RAP) < threshR) {
                motor(LMP, velocity - intensity) ;
                motor(RMP, (velocity + intensity) - subRight) ;
            }
            else if (analog(LAP) < threshL && analog(RAP) > threshR) {
                motor(LMP, velocity + intensity) ;
                motor(RMP, (velocity - intensity) - subRight) ;
            }
            else if (analog(LAP) < threshL && analog(RAP) < threshR) {
                motor(LMP, velocity) ;
                motor(RMP, velocity - subRight) ;
            }
            else if (analog(LAP) > threshL && analog(RAP) > threshR) {
                motor(LMP, velocity) ;
                motor(RMP, velocity - subRight) ;
            }


        }
    }
}

void pivotLineFollow (int threshL, int threshR, unsigned int distance, int velocity, bool isOvershooting) {
    cmpc(LMP) ;
    cmpc(RMP) ;

    if (isOvershooting == true) {
        while (abs(gmpc(LMP)) < distance || abs(gmpc(RMP)) < distance) {
            if (analog(LAP) > threshL && analog(RAP) < threshR) {
                motor(LMP, 0) ;
                motor(RMP, velocity - subRight) ;
            }
            else if (analog(LAP) < threshL && analog(RAP) > threshR) {
                motor(LMP, velocity) ;
                motor(RMP, 0) ;
            }
            else if (analog(LAP) < threshL && analog(RAP) < threshR) {
                motor(LMP, velocity) ;
                motor(RMP, velocity - subRight) ;
            }
            else if (analog(LAP) > threshL && analog(RAP) > threshR) {
                motor(LMP, velocity) ;
                motor(RMP, velocity - subRight) ;
            }


        }
    }

    else if (isOvershooting == false) {
        while (abs(gmpc(LMP)) < distance && abs(gmpc(RMP)) < distance) {
            if (analog(LAP) > threshL && analog(RAP) < threshR) {
                motor(LMP, 0) ;
                motor(RMP, velocity - subRight) ;
            }
            else if (analog(LAP) < threshL && analog(RAP) > threshR) {
                motor(LMP, velocity) ;
                motor(RMP, 0) ;
            }
            else if (analog(LAP) < threshL && analog(RAP) < threshR) {
                motor(LMP, velocity) ;
                motor(RMP, velocity - subRight) ;
            }
            else if (analog(LAP) > threshL && analog(RAP) > threshR) {
                motor(LMP, velocity) ;
                motor(RMP, velocity - subRight) ;
            }


        }
    }
}

void rightLineFollow (int thresh, unsigned int distance, int velocity, bool isOvershooting) {
    const int intensity = 10 ;
    cmpc(LMP) ;
    cmpc(RMP) ;

    if (isOvershooting == true) {
        while (abs(gmpc(LMP)) < distance || abs(gmpc(RMP)) < distance) {
            if (analog(RAP) < thresh) {
                motor(LMP, velocity - intensity) ;
                motor(RMP, (velocity + intensity) - subRight) ;
            }
            else if (analog(RAP) > thresh) {
                motor(LMP, velocity + intensity) ;
                motor(RMP, (velocity - intensity) - subRight) ;
            }

        }
    }

    else if (isOvershooting == false) {
        while (abs(gmpc(LMP)) < distance && abs(gmpc(RMP)) < distance) {
            if (analog(RAP) < thresh) {
                motor(LMP, velocity - intensity) ;
                motor(RMP, (velocity + intensity) - subRight) ;
            }
            else if (analog(RAP) > thresh) {
                motor(LMP, velocity + intensity) ;
                motor(RMP, (velocity - intensity) - subRight) ;
            }


        }
    }
}
