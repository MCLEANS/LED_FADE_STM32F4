#include "stm32f4xx.h"
#include "clockconfig.h"
#include "Timerconfiguration.h"

#define DELAY_PRESCALER  65
#define DELAY_ARR_VALUE 640

volatile uint32_t delay_counter = 0;

custom_libraries::clock_config system_clock;
custom_libraries::Timer_configuration delay_timer(TIM3,DELAY_PRESCALER,DELAY_ARR_VALUE);

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

  //Set delay timer Interrupt priority and Enable
  NVIC_SetPriority(TIM3_IRQn,0x02);
  NVIC_EnableIRQ(TIM3_IRQn);

  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  
  GPIOA->MODER |= GPIO_MODER_MODER7_0;
  GPIOA->MODER |= GPIO_MODER_MODER6_0;

  GPIOA->ODR |= GPIO_ODR_ODR_6;
  GPIOA->ODR &= ~GPIO_ODR_ODR_7;  

  while(1){
    delay_ms(500);
    GPIOA->ODR ^= (1<<6);
    GPIOA->ODR ^= (1<<7);

  }
}
