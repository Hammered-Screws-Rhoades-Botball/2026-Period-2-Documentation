#pragma once
#include <iostream>
#include <kipr/wombat.h>
#include <Alpha.h>
analogsensors an;
using namespace std::literals::chrono_literals;

int __wait_light_on(const int _port)noexcept{
    static int onval  __attribute__((aligned(2)));
    static int offval  __attribute__((aligned(2)));
    int thresh  __attribute__((aligned(2)));
    error calib_err("BAD CALIBRATION! RETRY!",0x24);
    
    std::cout << "calib start..." << std::endl;
    std::this_thread::sleep_for(1s);
    console_clear();

    while(true){
        an.lightarray();
        std::cout << "LIGHT ON: " << analog12(_port) << std::endl;
        std::this_thread::sleep_for(50ms);
        console_clear();
        if(a_button_clicked() || b_button_clicked() || c_button_clicked()){onval=analog12(_port); break;}
    }
    while(true){
        an.lightarray();
        std::cout << "LIGHT ON: " << onval << std::endl;
        std::cout << "LIGHT OFF: " << analog12(_port) << std::endl;
        std::this_thread::sleep_for(50ms);
        console_clear();
        if(a_button_clicked() || b_button_clicked() || c_button_clicked()){offval=analog12(_port); break;}
    }
    if(onval+20>=offval){calib_err.activate(); return 1;}
    thresh = (onval + offval/2)+ 7;
    std::cout<<"thresh: "<<thresh<<std::endl;
    std::this_thread::sleep_for(500ms);
    while(true){
        std::cout<<"waiting..."<<std::endl;
        std::this_thread::sleep_for(50ms);
        console_clear();
        if(analog12(_port)<thresh){return 0;}
    }
}