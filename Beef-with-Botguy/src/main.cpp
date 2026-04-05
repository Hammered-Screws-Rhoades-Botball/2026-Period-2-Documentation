#include <kipr/wombat.h>
#include <universalArm.h>
#include <iostream>
#include <mav.h>
#include <welp.h>

int bClawClose = 1200;
int bClawReady = 700;
int bClawDown = 400;

int main()
{

    std::thread toShared1(goAngleAnalog, 25, 100, 0, 2500, false, true);
    std::thread toShared2(stepper, 1200, 300, 1);
    std::thread toShared3(stepper, bClawReady, 300, 0);
    toShared1.join();
    toShared2.join();
    toShared3.join();

    goAngleAnalog(0, 100, 1, 2500, true, false);

    goAngleAnalog(44, 100, 1, 2500, false, true);

    //goAngleAnalog(180, 100, 0, 2500, false, true);
    goAngleAnalog(180, 100, 1, 2500, true, false);
    goAngleAnalog(180, 100, 0, 2500, true, false);

    goAngle(120, 100);
    msleep(1200);
    usleep(1);
    std::thread toBotguy1 (goAngleAnalog, 93, 100, 1, 2500, false, true);
    std::thread toBotguy2 (stepper, 1600, 100, 1);
    std::thread tobotguy4 (stepper, bClawDown, 100, 0);

    toBotguy1.join();
    toBotguy2.join();
    msleep(500);
    tobotguy4.join();

    fwm(1000);
    stepper(bClawClose, 300, 1);
    msleep(100);
    stepper(bClawReady, 300, 0);

    goAngle(270, 100);
    msleep(1000);

    return 0;
}
