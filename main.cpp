#include "stm32f4xx.h"
#include "clockconfig.h"

custom_libraries::clock_config system_clock;

int main(void) {
  
  system_clock.initialize();
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  
  GPIOA->MODER |= GPIO_MODER_MODER7_0;
  GPIOA->MODER |= GPIO_MODER_MODER6_0;

  GPIOA->ODR |= GPIO_ODR_ODR_6;
  GPIOA->ODR &= ~GPIO_ODR_ODR_7;  

  while(1){
    for(volatile int i = 0; i < 2000000; i++){}
    GPIOA->ODR ^= (1<<6);
    GPIOA->ODR ^= (1<<7);

  }
}
