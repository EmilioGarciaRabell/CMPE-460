#include "lab6/timers.h"

void part1A(void);
void part1B(void);

void part1A(){
	TIMA0_PWM_init(25, 3199, 0, 20.0);
}




int main(void){
	part1A();
}
