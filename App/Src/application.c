/*
 * application.c
 *
 *  Created on: May 12, 2025
 *      Author: tykim
 */

#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// UART 핸들러 선언
extern UART_HandleTypeDef huart3;

// LED 상태 관리를 위한 변수들
static uint32_t last_toggle_time = 0;  // 마지막으로 LED 상태를 바꾼 시간
static uint8_t current_led = 0;        // 현재 켜져있는 LED 번호
static uint8_t led_state = 0;          // LED 상태 (0: 꺼짐, 1: 켜짐)

// UART 시간 전송을 위한 변수
static uint32_t last_uart_time = 0;    // 마지막으로 UART로 시간을 전송한 시간

// UART로 시간 전송하는 함수
void send_time_uart(void)
{
    char time_str[50];
    uint32_t current_time = HAL_GetTick();
    uint32_t milliseconds = current_time % 1000;
    uint32_t seconds = current_time / 1000;
    uint32_t minutes = seconds / 60;
    uint32_t hours = minutes / 60;
    
    // HH:MM:SS.mmm 형식으로 문자열 생성
    snprintf(time_str, sizeof(time_str), "Time: %02lu:%02lu:%02lu.%03lu\r\n", 
             hours % 24, minutes % 60, seconds % 60, milliseconds);
    
    // UART3로 전송
    HAL_UART_Transmit(&huart3, (uint8_t*)time_str, strlen(time_str), 100);
}

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
    
    // UART 시간 전송 (1초마다)
    if(current_time - last_uart_time >= 1000)
    {
        send_time_uart();
        last_uart_time = current_time;
    }
}
