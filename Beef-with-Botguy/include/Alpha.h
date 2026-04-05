#pragma once
/*
#include <kipr/wombat.h>
#include <thread>
#include <vector>
#include <string>
#include <iostream>

int thresh0=0;
int thresh1=0;
int thresh2=0;

using namespace std::literals::chrono_literals;
using string = std::string;


enum dir{r=0,l,fr,fl,bl,br,f,b};
// declares vars for funcs

bool c_done = false;

constexpr unsigned int bias0  __attribute__((aligned(2)))=505;
constexpr unsigned int bias1  __attribute__((aligned(2)))=505;
constexpr unsigned int bias2  __attribute__((aligned(2)))=505;
constexpr unsigned int bias3  __attribute__((aligned(2)))=505;
constexpr double value __attribute__((aligned(8)))=46.66666666667;

constexpr double bias12  __attribute__((aligned(2)))=0;
constexpr double bias03  __attribute__((aligned(2)))=0;


int gmpc0=gmpc(0)-bias03;
int gmpc1=gmpc(1)-bias12;
int gmpc2=gmpc(2)-bias12;
int gmpc3=gmpc(3)-bias03;

constexpr double ticksPerMsleep __attribute__((aligned(4)))=0.01418;

struct error{
    error(const std::string&& message, const int&& _code) : msg(message), code(_code){}
    const std::string msg;
    const int code;
    [[noreturn]] void activate() const noexcept{
        std::cout<<"\nERROR: "<<this->msg<<"\n"<<std::endl;
        std::exit(code);
    }
};


class mec// all funcs to do with mecanum wheels/basic movement
{
    public:
    ~mec(){std::cout<<"destructor called for: mec"<<std::endl;}

    void ac()const noexcept{
        cmpc(0);
        cmpc(1);
        cmpc(2);
        cmpc(3);
    }
    void fwd(const int speed, const int time)//moves fwd
        const noexcept{
        double ticks=time * ticksPerMsleep * speed;


        gmpc0=gmpc(0);
        gmpc1=gmpc(1);
        gmpc2=gmpc(2);
        gmpc3=gmpc(3);

        while(gmpc(0)<ticks + gmpc0 || gmpc(3)<ticks + gmpc3 || gmpc(1)<ticks + gmpc1 || gmpc(2)<ticks + gmpc2)
        {
            motor(0,speed);
            motor(3,speed);
            motor(1,speed);
            motor(2,speed);
        }ao();

    }
    void fwdt(const int speed, const int ticks)//moves fwd
        const noexcept{

        gmpc0=gmpc(0);
        gmpc1=gmpc(1);
        gmpc2=gmpc(2);
        gmpc3=gmpc(3);

        while(gmpc(0)<ticks + gmpc0 || gmpc(3)<ticks + gmpc3 || gmpc(1)<ticks + gmpc1 || gmpc(2)<ticks + gmpc2)
        {
            motor(0,speed);
            motor(3,speed);
            motor(1,speed);
            motor(2,speed);
        }ao();

    }



    void bwd(const int speed, const int time)// moves backward
        const noexcept{
        double ticks=time * ticksPerMsleep * speed;


        gmpc0=gmpc(0);
        gmpc1=gmpc(1);
        gmpc2=gmpc(2);
        gmpc3=gmpc(3);

        while(gmpc(0)>gmpc0-ticks || gmpc(3)>gmpc3-ticks || gmpc(1)>gmpc1-ticks || gmpc(2)>gmpc2-ticks)
        {
            motor(0,-speed);
            motor(3,-speed);
            motor(1,-speed);
            motor(2,-speed);
        }ao();

    }
    void bwdt(const int speed, const int ticks)// moves backward
        const noexcept{


        gmpc0=gmpc(0);
        gmpc1=gmpc(1);
        gmpc2=gmpc(2);
        gmpc3=gmpc(3);

        while(gmpc(0)>gmpc0-ticks || gmpc(3)>gmpc3-ticks || gmpc(1)>gmpc1-ticks || gmpc(2)>gmpc2-ticks)
        {
            motor(0,-speed);
            motor(3,-speed);
            motor(1,-speed);
            motor(2,-speed);
        }ao();

    }


    inline void control(const int speed0,const int speed1,const int speed2,const int speed3,const int time)// a function to control speeds of all four motors and msleep
        const noexcept{
        motor(0,speed0);
        motor(1,speed1);
        motor(2,speed2);
        motor(3,speed3);
        msleep(time);  
    }
    inline void control(const int speed0,const int speed1,const int speed2,const int speed3)// override of control w/o time 
        const noexcept{
        motor(0,speed0);
        motor(1,speed1);
        motor(2,speed2);
        motor(3,speed3);

    }
    inline void ctrl(const int speed0,const int speed1,const int speed2,const int speed3)//override of control for faster typing
        const noexcept{
        motor(0,speed0);
        motor(1,speed1);
        motor(2,speed2);
        motor(3,speed3);

    }
    inline void ctrl(const int speed0,const int speed1,const int speed2,const int speed3,const int time)//override of control for faster typing
        const noexcept{
        motor(0,speed0);
        motor(1,speed1);
        motor(2,speed2);
        motor(3,speed3);
        msleep(time);
    }

    inline void mecwsleep(const int speed0,const int speed1,const int speed2,const int speed3)// ctrl without time(was used for transfer from c to c++
        const noexcept{
        motor(0,speed0);
        motor(1,speed1);
        motor(2,speed2);
        motor(3,speed3);
    }

    inline void wait(const int time)// all motors sleep for an indicated time
        const noexcept{
        ctrl(0,0,0,0,time);
    }


    void diag(const int direction, const int speed, const int sleep)// moves diagonally(using mecanum wheels) with specifications for speed, time, and direction
        const noexcept{
        if(direction == dir::fl)
        {
            ctrl(speed,0,speed,0,sleep);
        }
        else if(direction == dir::fr)
        {
            ctrl(0,speed,0,speed,sleep); 
        }

        else if(direction == dir::br)
        {
            ctrl(-speed,0,-speed,0,sleep); 
        }

        else if(direction == dir::bl)
        {
            ctrl(0,-speed,0,-speed,sleep); 
        }
    }
    void strafe(const int direction, const int speed, const int time)//strafe function 
        const noexcept{
        double ticks=time * ticksPerMsleep * speed;
        gmpc0=gmpc(0);
        gmpc1=gmpc(1);

        if (direction==dir::r)
        {
            while(gmpc(1)<gmpc1 + ticks){
                ctrl(-speed,speed,-speed,speed);
            }
        }
        if (direction==dir::l)
        {
            while(gmpc(0)<gmpc0 + ticks){
                ctrl(speed,-speed,speed,-speed);
            }
        }

    }
    void strafe(const int direction, const int speed)//strafe function 
        const noexcept{


        if (direction==dir::r)
        {

            ctrl(-speed,speed,-speed,speed);

        }
        if (direction==dir::l)
        {

            ctrl(speed,-speed,speed,-speed);

        }
    }
};
class mpc//class with all funcs  related to motor position counter
{

    public:
    ~mpc(){std::cout<<"destructor called for: mpc"<<std::endl;}

    void STA(const int direction, const int theta)//perfect degree turn (needs fix)
        const noexcept{

        if(direction==dir::r)
        {

            while(gmpc(3)<((value*theta)-bias3)/2 + gmpc3 && gmpc(1)<((value*theta)-bias1)/2 + gmpc1 && gmpc(0)<((value*theta)-bias0)/2 + gmpc0 && gmpc(2)<((value*theta)-bias2)/2 + gmpc2)
            {
                motor(0,-50);
                motor(3,-50);
                motor(2,50);
                motor(1,50);
            }
        }
        else if(direction==dir::l)
        {

            while(gmpc(3)<((value*theta)-bias3)/2 + gmpc3 && gmpc(1)<((value*theta)-bias1)/2 + gmpc1 && gmpc(0)<((value*theta)-bias0)/2 + gmpc0 && gmpc(2)<((value*theta)-bias2)/2 + gmpc2)
            {
                motor(0,50);
                motor(3,50);
                motor(2,-50);
                motor(1,-50);
            }
        }
    }

};
class turn : public mec// all funcs to do with turning
{
    public:
    ~turn(){std::cout<<"destructor called for: turn"<<std::endl;}

    void pvt(const int wheel, const int speed, const int sleep)
        const noexcept{
        double ticks=sleep * ticksPerMsleep * abs(speed);
        gmpc0=gmpc(0);
        gmpc1=gmpc(1);
        if (wheel==dir::r)
        {
            if(speed>=0){
                while(gmpc(0)<gmpc0+ticks){
                    ctrl(speed,0,0,speed);
                }
            }
            if(speed<0){
                while(gmpc(0)>gmpc0-ticks){
                    ctrl(speed,0,0,speed);
                }
            }
        }
        else if (wheel==dir::l)
        {
            if(speed>=0){
                while(gmpc(1)<gmpc1+ticks){
                    ctrl(0,speed,speed,0);
                }
            }
            if(speed<0){
                while(gmpc(1)>gmpc1-ticks){
                    ctrl(0,speed,speed,0);
                }
            }
        }
    }

    void spin(const int direction, const int speed, const int sleep)
        const noexcept{
        double ticks=sleep * ticksPerMsleep * abs(speed);
        gmpc0=gmpc(0);
        gmpc1=gmpc(1);

        if (direction==dir::l)
        {

            while(gmpc(0)<gmpc0+ticks){
                ctrl(speed,-speed,-speed,speed);
            }


        }
        else if (direction==dir::r)
        {

            while(gmpc(1)<gmpc1+ticks){
                ctrl(-speed,speed,speed,-speed);
            }


        }
    }

    void veer(const int direction, const int speed, const int veer, const int sleep)
        const noexcept{
        if (direction==dir::l)
        {
            ctrl(speed,speed-veer,speed-veer,speed,sleep); 
        }
        else if (direction==dir::r)
        {
            ctrl(speed-veer,speed,speed,speed-veer,sleep); 
        }
    }



};
class analogsensors : public mec// all analog sensor funcs
{
    public:
    ~analogsensors(){std::cout<<"destructor called for: analogsensors"<<std::endl;}

    void lightarray()
        const noexcept{
        printf("                 |       |\n");
        printf("                 |       |\n");
        printf("                /        \\\n");
        printf("               /          \\\n");
        printf("              /            \\\n");
        printf("             (     _____    )\n");
        printf("              \\  |     |   /\n");
        printf("               \\ |     |  /\n");
        printf("                __________\n");


    }

    [[deprecated("CALIB_ANALOG IS NOT COMMENTED OUT YET!!!!")]] void calib_analog()
        noexcept{
        error calib_err("BAD CALIBRATION FOR ANALOG! RETRY!!!",0x79);
        while(true){
            lightarray();
            std::cout<<"BLACK VALUE 0: "<<analog(0)<<std::endl;
            std::cout<<"\nBLACK VALUE 1: "<<analog(1)<<std::endl;
            std::this_thread::sleep_for(50ms);
            console_clear();
            if(a_button_clicked() || b_button_clicked() || c_button_clicked()){analog0b=analog(0); analog1b=analog(1); break;}
        }
        while(true){
            lightarray();
            std::cout<<"BLACK VALUE 0: "<<analog0b<<std::endl;
            std::cout<<"BLACK VALUE 1: "<<analog1b<<std::endl;
            std::cout<<"\nWHITE VALUE 0: "<<analog(0)<<std::endl;
            std::cout<<"WHITE VALUE 1: "<<analog(1)<<std::endl;
            std::this_thread::sleep_for(50ms);
            console_clear();
            if(a_button_clicked() || b_button_clicked() || c_button_clicked()){analog0w=analog(0); analog1w=analog(1); break;}
        }

        std::cout<<"thresh0: "<<((analog0b + analog0w)/2)+100<<std::endl;
        std::cout<<"thresh1: "<<((analog1b + analog1w)/2)+100<<std::endl;
        std::cout<<std::endl; 

        if(analog0w>analog0b-200 || analog1w>analog1b-200){
            calib_err.activate();
            if(analog0w>analog0b-200){std::cout<<"error is in analog 0"<<std::endl;}
            if(analog1w>analog1b-200){std::cout<<"error is in analog 1"<<std::endl;}
        }
        std::cout<<"analog calibraton complete!! comment out/delete NOW\n"<<std::endl;
        std::exit(0x43);
    }

    void loki()
        const noexcept{
        while(analog(1)<thresh1 || analog(0)<thresh0)
        {
            mecwsleep(50,50,50,50);
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
    void loki_calib()
        noexcept{
        while(analog(1)<thresh1 || analog(0)<thresh0)
        {
            analog0w=analog(0);
            analog1w=analog(1);
            mecwsleep(50,50,50,50);
            while(analog(1)<thresh1 && analog(0)>thresh0)
            {
                analog0b=analog(0);
                ctrl(0,50,50,0);
            }
            while(analog(1)>thresh1 && analog(0)<thresh0)
            {
                analog1b=analog(1);
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
        thresh0=((analog0w + analog0b)/2)+100;
        thresh1=((analog1w + analog1b)/2)+100;
    }
    void loki(const int speed)
        const noexcept{
        while(analog(1)<thresh1 || analog(0)<thresh0)
        {
            mecwsleep(speed,speed,speed,speed);
            while(analog(1)<thresh1 && analog(0)>thresh0)
            {
                ctrl(0,speed,speed,0);
            }
            while(analog(1)>thresh1 && analog(0)<thresh0)
            {
                ctrl(speed,0,0,speed);
            }
        }
        while(analog(1)>thresh1 || analog(0)>thresh0)
        {
            ctrl(speed,speed,speed,speed);
            while(analog(1)>thresh1 && analog(0)<thresh0)
            {
                ctrl(0,speed,speed,0);
            }
            while(analog(1)<thresh1 && analog(0)>thresh0)
            {
                ctrl(speed,0,0,speed);
            }
        }
    }
    //double f_loki(const int speed, const double f_time)
    //const noexcept{
    void sideloki(const int speed)
        const noexcept{
        while(analog(3)<thresh1 || analog(0)<thresh0)
        {
            mecwsleep(speed,-speed,speed,-speed);
            while(analog(3)<thresh1 && analog(0)>thresh0)
            {
                ctrl(0,speed,speed,0);
            }
            while(analog(3)>thresh1 && analog(0)<thresh0)
            {
                ctrl(speed,0,0,speed);
            }
        }
    }

    //}
    void bloki()
        const noexcept{
        while(analog(1)<thresh1 || analog(0)<thresh0)
        {
            mecwsleep(50,50,50,50);
            while(analog(1)<thresh1 && analog(0)>thresh0)
            {
                ctrl(-50,0,0,-50);
            }
            while(analog(1)>thresh1 && analog(0)<thresh0)
            {
                ctrl(0,-50,-50,0);
            }
        }
        while(analog(1)>thresh1 || analog(0)>thresh0)
        {
            mecwsleep(50,50,50,50);
            while(analog(1)>thresh1 && analog(0)<thresh0)
            {
                ctrl(-50,0,0,-50);
            }
            while(analog(1)<thresh1 && analog(0)>thresh0)
            {
                ctrl(0,-50,-50,0);
            }
        }
    }
    void sloki(const int speed)
        const noexcept{
        while(analog(1)<thresh1 || analog(0)<thresh0)
        {
            mecwsleep(speed,speed,speed,speed);
            while(analog(1)<thresh1 && analog(0)>thresh0)
            {
                ctrl(-speed,speed,speed,-speed);
            }
            while(analog(1)>thresh1 && analog(0)<thresh0)
            {
                ctrl(speed,-speed,-speed,speed);
            }
        }
        while(analog(1)>thresh1 || analog(0)>thresh0)
        {
            ctrl(speed,speed,speed,speed);
            while(analog(1)>thresh1 && analog(0)<thresh0)
            {
                ctrl(-speed,speed,speed,-speed);
            }
            while(analog(1)<thresh1 && analog(0)>thresh0)
            {
                ctrl(speed,-speed,-speed,speed);
            }
        }
    }
    private:
    volatile short analog0w  __attribute__((aligned(2)));
    volatile short analog1w  __attribute__((aligned(2)));
    volatile short analog0b  __attribute__((aligned(2)));
    volatile short analog1b  __attribute__((aligned(2)));
};
class servo : public mec//servo functions
{
    public:

    void servox(const int port,const int final,const int time)  
        const noexcept{
        ao();
        enable_servo(port);
        int pos = get_servo_position(port);
        double mssleep;
        int distance;
        if(pos>final){
            distance = pos - final;
            mssleep = time/distance;
        }
        else if(pos<final){
            distance = final - pos;
            mssleep = time/distance;
        }
        while(pos<final)
        {
            pos+=1;
            set_servo_position(port, pos);
            msleep(mssleep);
        }
        while(pos>final)
        {
            pos-=1;
            set_servo_position(port, pos);
            msleep(mssleep);
        }
        //disable_servo(port);
    }
    void servoy(const int port, const int pos, const int timePerTick)const noexcept{
        error err("too much msleep per servo tick", 0x15);
        if(get_servo_position(port)==pos){return;}
        if(timePerTick>11){err.activate();}
        while(get_servo_position(port)<pos || get_servo_position(port)>pos){
            servox(port,get_servo_position(port),timePerTick);
        }
    }
};

[[nodiscard]] bool operator==(const std::string val, const std::vector<std::string> list){
    for(size_t i=0; i<list.size();i++){
        if(val == list[i]){return true;}
        continue;
    }
    return false;
}
class cam// all functions to do with color camera
{
    private:
    char ret;

    public:
    ~cam(){std::cout<<"destructor called for: cam"<<std::endl;}

    [[nodiscard]] char snap7(const std::string&& color)
        noexcept{

        camera_load_config("failure");
        camera_open();
        camera_update();
        const int p = get_object_center_x(0, 0);
        const int b = get_object_center_x(1, 0);
        const int g = get_object_center_x(2, 0);
        camera_close();
        c_done = true;

        if(p == -1 || b == -1 || g == -1){ 
            std::cout << "camera got a bad read: " << std::endl; 
            if(p == -1) std::cout << "p ";
            if(b == -1) std::cout << "b ";
            if(g == -1) std::cout << "g ";
            std::cout << std::endl;
            return 'l';
        }

        auto checkpos2=[=]()->void{
            if(color==mistakeP){
                if(p<b && p<g){
                    //pinkpos=l2;
                    std::cout<<"pink is on the left"<<std::endl; 
                    ret = 'l';
                }
                if(p>b && p>g){
                    //pinkpos=r2;
                    std::cout<<"pink is on the right"<<std::endl;
                    ret = 'r';
                }
                if((b<p && p<g) || (g<p && p<b)){
                    //pinkpos=m;
                    std::cout<<"pink is in the middle"<<std::endl;
                    ret = 'm';
                }
                //x=pinkpos;
            }
            else if(color==mistakeG){
                if(g<b && g<p){
                    //greenpos=l2;
                    std::cout<<"green is on the left"<<std::endl;
                    ret = 'l';
                }
                if(g>b && g>p){
                    //greenpos=r2;
                    std::cout<<"green is on the right"<<std::endl;
                    ret = 'r';
                }
                if((b<g && g<p) || (p<g && g<b)){
                    //greenpos=m;
                    std::cout<<"green is in the middle"<<std::endl;
                    ret = 'm';
                }
            }
            else{
                ret = 'l';

            }


        };
        checkpos2();

        //returners
        //if(L){return 'l';}
        //else if(M){return 'm';}
        //else if(R){return 'r';}
        return ret;


    }
    private:
    std::vector<std::string> mistakeP = {"pink","pnk","pik","ink","nk","pin","pi","p","i","n","k","pkpink","okpink","ponk","pk","plink","piik","pnik","pkpnik"};
    std::vector<std::string> mistakeG = {"green","grn","gren","reen","gree","gre","geen","gen","ren","pkgreen","okgreen","gn","gteen","geeen","gern","pkgern"};

};
inline unsigned int thresh(const int&& num){
    error invalid_num("invalid thresh index", 0x54);
    if (num == 0) return thresh0;
    if(num == 1) return thresh1;
    if(num == 2) return thresh2;
    else invalid_num.activate();
}
class digitalsensors : private mec{
    public:
    void loki(const int speed)
        const noexcept{
        while(!digital(1) || !digital(0))
        {
            mecwsleep(speed,speed,speed,speed);
            while(digital(1) && !digital(0))
            {
                ctrl(0,speed,speed,0);
            }
            while(!digital(1) && digital(0))
            {
                ctrl(speed,0,0,speed);
            }
        }
    }

};

*/