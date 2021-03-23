#include "timer.h"


clock_t static timestamp;


void timer_start(){
    timestamp = clock();
    return;
}

int timer_notExpired(int duration){ 
    if (((clock()-timestamp)/CLOCKS_PER_SEC) >= duration){
        return 0;
    }
    return 1;
}
