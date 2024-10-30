/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2021/06/06
 * Description        : Main program body.
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/

#include "globals.h"

/* Global define */
#define THREAD_COUNT    12

/* Global Variable */

struct pt threads[THREAD_COUNT];

static void threads_init(void) {
    for (int i = 0; i < THREAD_COUNT; i+= 1) {
        PT_INIT(&threads[i]);
    }
}

/*********************************************************************
 * @fn      GPIO_Toggle_init
 *
 * @brief   Initializes GPIO
 *
 * @return  none
 */
void GPIO_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);

    //GPIOA Init Alternate Functions
    //PA0 SERVO_PWM [TIM5_CH1]
    //PA1 FAN_TRIG_IN1 [TIM2_CH2]
    //PA2 FAN_TRIG_IN2 [TIM2_CH3]
    //PA8 VENT_OPEN_IN1 [TIM1_CH1]
    //PA9 DEBUG_UART_TX [UART1_TX]
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //GPIOA Init Analog
    //PA3 FAN_TRIG_CURRENT_SENSE
    //PA4 CLOSE_CURRENT_SENSE
    //PA5 OPEN_CURRENT_SENSE
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //GPIOA Init outputs
    //PA6 CAN_STB
    //PA15 CAN_WAKE
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //GPIOA Init inputs - pull up
    //PA10 DEBUG_UART_RX [UART1_RX]
    //PA11 USB- [PA11]
    //PA12 USB+ [PA12]
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //GPIOB Init outputs
    //PB2 BOOT1 (unused)
    //PB4 CAN_EN
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //GPIOB Init inputs - pull up
    //PB3 CAN_ERR
    //PB8 CAN RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //GPIOB Init inputs - pull down
    //PB11 EXT_START_IN
    //PB12 EXT_STOP_IN
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //GPIOB Init Analog
    //PB1 Analog trigger in
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);//map TIM3 to PB5
    GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);//map CAN1 to PB8/PB9
    //GPIOB Init Alternate Functions Push-Pull
    //PB0 Heartbeat [TIM3_CH3]
    //PB5 Buzzer Drive [TIM3_CH2]
    //PB9 CAN_TX
    //PB13 VENT_POPEN_IN2 [TIM1_CH1N]
    //PB14 VENT_CLOSE_IN1 [TIM1_CH2N]
    //PB15 VENT_CLOSE_IN2 [TIM1_CH3N]
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_5 | GPIO_Pin_9 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //GPIOB Init Alternate Functions Open-drain
    //PB6 I2C1_SCL
    //PB7 I2C1_SDA
    //PB10 USART3_TX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //GPIOC Init inputs - pull up
    //PC13 BUTTON_1
    //PC14 BUTTON_2
    //PC15 BUTTON_3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void pwm_init(void) {
    TIM_TimeBaseInitTypeDef time_base;
    TIM_OCInitTypeDef oc_init;
    TIM_BDTRInitTypeDef bdtr_init;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM5, ENABLE);

    TIM_TimeBaseStructInit(&time_base);
    //The divisor will be overwritten when the buzzer frequency changes, but this is a fine default
    time_base.TIM_ClockDivision = SystemCoreClock / (HB_PWM_FREQ * PWM_FULL_SCALE);
    time_base.TIM_Period = PWM_FULL_SCALE;
    TIM_TimeBaseInit(TIM3, &time_base);

    //Configure buzzer & heartbeat PWM
    TIM_OCStructInit(&oc_init);
    oc_init.TIM_OCMode = TIM_OCMode_PWM1;
    oc_init.TIM_OutputState = TIM_OutputState_Enable;
    oc_init.TIM_OCPolarity = TIM_OCPolarity_High;
    oc_init.TIM_Pulse = 0xFFFF;
    TIM_OC2Init(TIM3, &oc_init);//buzzer
    TIM_OC3Init(TIM3, &oc_init);//heartbeat

    //Configure servo output PWM
    time_base.TIM_ClockDivision = SystemCoreClock / (SERVO_PWM_FREQ * PWM_FULL_SCALE);
    time_base.TIM_Period = PWM_FULL_SCALE;
    TIM_TimeBaseInit(TIM5, &time_base);
    //    TIM_OCStructInit(&oc_init);
    //    oc_init.TIM_OCMode = TIM_OCMode_PWM1;
    //    oc_init.TIM_OutputState = TIM_OutputState_Enable;
        oc_init.TIM_Pulse = PWM_FULL_SCALE / 2;
    TIM_OC1Init(TIM5, &oc_init);

    //Configure open output PWM - CH1/CH1N enable
    time_base.TIM_ClockDivision = SystemCoreClock / (VALVE_FAN_PWM_FREQ * PWM_FULL_SCALE);
    time_base.TIM_Period = PWM_FULL_SCALE;
    time_base.TIM_CounterMode = TIM_CounterMode_Up;
    time_base.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &time_base);
    TIM_BDTRStructInit(&bdtr_init);
//    bdtr_init.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
//    bdtr_init->TIM_Break = TIM_Break_Disable;
    TIM_BDTRConfig(TIM1, &bdtr_init);
//    TIM_OCStructInit(&oc_init);
//    oc_init.TIM_OCMode = TIM_OCMode_PWM1;
//    oc_init.TIM_OCPolarity = TIM_OCPolarity_High;
//    oc_init.TIM_OutputState = TIM_OutputState_Enable;
    oc_init.TIM_OutputNState = TIM_OutputNState_Enable;
    oc_init.TIM_OCNPolarity = TIM_OCNPolarity_High;
    oc_init.TIM_OCIdleState = TIM_OCIdleState_Reset;
    oc_init.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    oc_init.TIM_Pulse = 0x00;
    TIM_OC1Init(TIM1, &oc_init);

    //Configure Close output PWM - CH2N/CH3N enable
    oc_init.TIM_OutputState = TIM_OutputState_Disable;
    TIM_OC2Init(TIM1, &oc_init);
    TIM_OC3Init(TIM1, &oc_init);

    //Configure fan output PWM
    TIM_TimeBaseInit(TIM2, &time_base);
//    TIM_OCStructInit(&oc_init);
//    oc_init.TIM_OCMode = TIM_OCMode_PWM1;
    oc_init.TIM_OutputState = TIM_OutputState_Enable;
//    oc_init.TIM_Pulse = 0x00;
    TIM_OC2Init(TIM2, &oc_init);
    TIM_OC3Init(TIM2, &oc_init);

    TIM_Cmd(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);

    TIM_Cmd(TIM2, ENABLE);
    TIM_CtrlPWMOutputs(TIM2, ENABLE);

    TIM_Cmd(TIM3, ENABLE);
    TIM_CtrlPWMOutputs(TIM3, ENABLE);

    TIM_Cmd(TIM5, ENABLE);
    TIM_CtrlPWMOutputs(TIM5, ENABLE);
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */

int main(void)
{
    RCC_ClocksTypeDef RCC_ClocksStatus = {0};

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    Delay_Init();
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    GPIO_init();
    pwm_init();
    UART_Init();
    led_dma_init();
    can_init();
    IIC_Init(400000, 0);

    SystemCoreClockUpdate();
    printf("SystemClk:%d\r\n",SystemCoreClock);

    RCC_GetClocksFreq(&RCC_ClocksStatus);
    printf("SYSCLK_Frequency-%d\r\n", RCC_ClocksStatus.SYSCLK_Frequency);
    printf("HCLK_Frequency-%d\r\n", RCC_ClocksStatus.HCLK_Frequency);
    printf("PCLK1_Frequency-%d\r\n", RCC_ClocksStatus.PCLK1_Frequency);
    printf("PCLK2_Frequency-%d\r\n", RCC_ClocksStatus.PCLK2_Frequency);

    SysTick_Init(1000);

    threads_init();

    leds_init();

    oled_test();

#if 1
    while (1)
    {
//        (void)PT_SCHEDULE(heartbeat_task(&threads[0]));
        (void)PT_SCHEDULE(breathe_task(&threads[0]));
        (void)PT_SCHEDULE(status_led_task(&threads[1]));
        (void)PT_SCHEDULE(button_task(&threads[2]));
        (void)PT_SCHEDULE(buzzer_task(&threads[3]));
        (void)PT_SCHEDULE(can_tx_task(&threads[4]));
        (void)PT_SCHEDULE(can_rx_task(&threads[5]));
        (void)PT_SCHEDULE(vent_control_task(&threads[6]));
//        (void)PT_SCHEDULE(display_task(&threads[7]));
        (void)PT_SCHEDULE(i2c_task(&threads[8]));
    }
#endif

}
