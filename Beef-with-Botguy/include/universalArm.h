#include <kipr/wombat.h>
#include <thread>
#include <unistd.h>

void ctrl(int spee, int speed, int speedd, int speeddd)//override of control for faster typing
{
    motor(0,spee);
    motor(1,speed);
    motor(2,speedd);
    motor(3,speeddd);
}
/*void loki(int thresh1, int thresh0)
{

    while(analog(1)<thresh1 || analog(0)<thresh0)
    {
        ctrl(50,50,50,50);
        while(analog(1)<thresh1 && analog(0)>thresh0)
        {
            ctrl(0,50,50,0);
        }
        while(analog(1)>thresh1 && analog(0)<thresh0)
        {
            ctrl(50,0,0,50);
        }
    }
    while(analog(1)>thresh1 || analog(0)>thresh0)
    {
        ctrl(50,50,50,50);
        while(analog(1)>thresh1 && analog(0)<thresh0)
        {
            ctrl(0,50,50,0);
        }
        while(analog(1)<thresh1 && analog(0)>thresh0)
        {
            ctrl(50,0,0,50);
        }
    }
}
*/
void stepper(int _final, int time, int port)  
{
    ao();
    enable_servo(port);
    int pos = get_servo_position(port);
    double mssleep;
    int distance;
    if(pos>_final){
        distance = pos - _final;
        mssleep = time/distance;
    }
    else if(pos<_final){
        distance = _final - pos;
        mssleep = time/distance;
    }
    while(pos<_final)
    {
        pos+=1;
        set_servo_position(port, pos);
        msleep(mssleep);
    }
    while(pos>_final)
    {
        pos-=1;
        set_servo_position(port, pos);
        msleep(mssleep);
    }
    printf("I");
    disable_servo(port);
}
void fwd(int tikz, int velocity)
{
    cmpc(0);
    cmpc(1);
    cmpc(2);
    cmpc(3);

    while(gmpc(1)<tikz||gmpc(0)<tikz||gmpc(2)<tikz||gmpc(3)<tikz)
    {
        motor(0,velocity);
        motor(1,velocity);
        motor(2,velocity);
        motor(3,velocity);
    }
}
void coneClawReady()
{
    stepper(1900,300,2);
    stepper(1400,300,3);
}
void coneClawGrab()
{
    stepper(800,300,3);
}
void coneClawUpC()
{
    stepper(50,1000,2);
}
void fwm(int sleep)
{
    motor(3,100);
    motor(2,-100);
    motor(0,100);
    motor(1,-100);
    msleep(sleep);
}