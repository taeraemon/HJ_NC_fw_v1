/*
 * application.c
 *
 *  Created on: May 12, 2025
 *      Author: tykim
 */

#include "main.h"
#include <stdint.h>

// LED 상태 관리를 위한 변수들
static uint32_t last_toggle_time = 0;  // 마지막으로 LED 상태를 바꾼 시간
static uint8_t current_led = 0;        // 현재 켜져있는 LED 번호
static uint8_t led_state = 0;          // LED 상태 (0: 꺼짐, 1: 켜짐)

void setup(void)
{
    // 모든 LED를 초기화 (꺼짐 상태)
    HAL_GPIO_WritePin(sv_0_GPIO_Port, sv_0_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(sv_1_GPIO_Port, sv_1_Pin, GPIO_PIN_RESET);
}

void loop(void)
{
    uint32_t current_time = HAL_GetTick();  // 현재 시간 가져오기
    
    // 1초(1000ms)가 지났는지 확인
    if(current_time - last_toggle_time >= 1000)
    {
        // 이전 LED 끄기
        switch(current_led)
        {
            case 0:
                HAL_GPIO_WritePin(sv_0_GPIO_Port, sv_0_Pin, GPIO_PIN_RESET);
                break;
            case 1:
                HAL_GPIO_WritePin(sv_1_GPIO_Port, sv_1_Pin, GPIO_PIN_RESET);
                break;
        }
        
        // 다음 LED로 이동
        current_led = (current_led + 1) % 2;  // 0과 1만 반복
        
        // 새로운 LED 켜기
        switch(current_led)
        {
            case 0:
                HAL_GPIO_WritePin(sv_0_GPIO_Port, sv_0_Pin, GPIO_PIN_SET);
                break;
            case 1:
                HAL_GPIO_WritePin(sv_1_GPIO_Port, sv_1_Pin, GPIO_PIN_SET);
                break;
        }
        
        // 마지막 토글 시간 업데이트
        last_toggle_time = current_time;
    }
    
    // 여기에 다른 작업을 추가할 수 있어
}
