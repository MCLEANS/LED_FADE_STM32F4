#include "stm32f4xx.h"
#include "clockconfig.h"
#include "Timerconfiguration.h"
#include "PWM.h"

#define DELAY_PRESCALER  65
#define DELAY_ARR_VALUE 640

#define LED_PRESCALER 1000;
#define LED_ARR_VALUE 1000;

volatile uint32_t delay_counter = 0;

uint8_t duty_cycle = 0;

bool count_up = true;

custom_libraries::clock_config system_clock;
custom_libraries::Timer_configuration delay_timer(TIM3,DELAY_PRESCALER,DELAY_ARR_VALUE);
custom_libraries::PWM red_led(TIM8,
                            custom_libraries::channel1,
                            GPIOC,
                            6,
                            custom_libraries::AF3,
                            1000,
                            100);

custom_libraries::PWM blue_led(TIM9,
                            custom_libraries::channel1,
                            GPIOE,
                            5,
                            custom_libraries::AF3,
                            1000,
                            100);

custom_libraries::PWM green_led(TIM9,
                            custom_libraries::channel2,
                            GPIOE,
                            6,
                            custom_libraries::AF3,
                            1000,
                            100);

//delay timer Interrupt service routine
extern "C" void TIM3_IRQHandler(void){
  //check that we are here because of TIM3
  if(TIM3->SR & TIM_SR_UIF){
    TIM3->SR &= ~TIM_SR_UIF;
    delay_counter++;
  }
}

//Delay function
void delay_ms(uint32_t duration){
  delay_counter = 0;
  while(delay_counter < duration){}
}

int main(void) {
  
  system_clock.initialize();
  delay_timer.initialize();

  red_led.begin();
  red_led.set_duty_cycle(duty_cycle);

  blue_led.begin();
  blue_led.set_duty_cycle(100-duty_cycle);

  green_led.begin();
  green_led.set_duty_cycle(duty_cycle);

  //Set delay timer Interrupt priority and Enable
  NVIC_SetPriority(TIM3_IRQn,0x02);
  NVIC_EnableIRQ(TIM3_IRQn); 

  while(1){    

    for(volatile int i = 0; i < 100; i++){
      red_led.set_duty_cycle(i);
      blue_led.set_duty_cycle(100-i);
      green_led.set_duty_cycle(i);
       delay_ms(20);
    }

    for(volatile int i = 0; i < 100; i++){
      red_led.set_duty_cycle(100-i);
      blue_led.set_duty_cycle(i);
      green_led.set_duty_cycle(100-i);
       delay_ms(20);
    }
    
  }
}
