/*
 * software_timer.c
 *
 *  Created on: Sep 24, 2023
 *      Author: HaHuyen
 */

#include "software_timer.h"

#define TIMER_CYCLE_2 1


uint16_t flag_timer2 = 0;
uint16_t timer2_counter = 0;
uint16_t timer2_MUL = 0;

uint16_t flag_timerSendTemp = 0;
uint16_t timerSendTemp_counter = 0;
uint16_t timerSendTemp_MUL = 0;

void timer_init(){
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Start(&htim1);
}

void timer_EnableDelayUs(){
	HAL_TIM_Base_Start(&htim1);
}

void setTimer2(uint16_t duration){
	timer2_MUL = duration/TIMER_CYCLE_2;
	timer2_counter = timer2_MUL;
	flag_timer2 = 0;
}

// LAB 8
void setTimerSendTemp(uint16_t duration){
	timerSendTemp_MUL = duration/TIMER_CYCLE_2;
	timerSendTemp_counter = timer2_MUL;
	flag_timerSendTemp = 0;
}

uint16_t isFlagSendTemp()
{
	if(flag_timerSendTemp == 1)
	{
		flag_timerSendTemp = 0;
		return 1;
	}
	return 0;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM2){
		if(timer2_counter > 0){
			timer2_counter--;
			if(timer2_counter == 0) {
				flag_timer2 = 1;
				timer2_counter = timer2_MUL;
			}
		}
		if(timerSendTemp_counter > 0){
			timerSendTemp_counter--;
			if(timerSendTemp_counter == 0) {
				flag_timerSendTemp = 1;
				timerSendTemp_counter = timerSendTemp_MUL;
			}
		}
		led7_Scan();
	}
}

void delay_us (uint16_t us)
{
	__HAL_TIM_SET_COUNTER(&htim1,0);  // set the counter value a 0
	while (__HAL_TIM_GET_COUNTER(&htim1) < us);  // wait for the counter to reach the us input in the parameter
}


