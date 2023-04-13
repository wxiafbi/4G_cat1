/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include "bmp.h"
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint8_t Senbuff[] = "\r\n**** Serial Output Message by DMA ***\r\n   UART DMA Test \r\n   Zxiaoxuan"; // 定义数据发�?�数�?

#define RXBUFFERSIZE 256           // �������ֽ���
char RxBuffer[RXBUFFERSIZE] = {0}; // ��������
uint8_t aRxBuffer;                 // �����жϻ���
uint8_t Uart1_Rx_Cnt = 0;          // ���ջ������
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void oled_show(void);
void Data_parsing(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_TIM1_Init();
    MX_TIM2_Init();
    MX_TIM3_Init();
    MX_TIM4_Init();
    MX_USART1_UART_Init();
    MX_USART2_UART_Init();
    MX_USART3_UART_Init();
    /* USER CODE BEGIN 2 */
    HAL_TIM_Base_Start_IT(&htim1);
    HAL_TIM_Base_Start_IT(&htim2);
    HAL_TIM_Base_Start_IT(&htim3);
    HAL_TIM_Base_Start_IT(&htim4);

    OLED_Init();
    OLED_ColorTurn(0);   // 0������ʾ��1��ɫ��ʾ
    OLED_DisplayTurn(0); // 0������ʾ��1��Ļ��ת��ʾ
    HAL_UART_Receive_DMA(&huart1, (uint8_t *)&aRxBuffer, 1);
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        oled_show();
    }
    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState       = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL     = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */
void oled_show(void)
{
    uint8_t t = ' ';
    OLED_ShowPicture(0, 0, 128, 64, BMP1, 1);
    OLED_Refresh();
    HAL_Delay(1500);
    OLED_Clear();
    OLED_ShowChinese(0, 0, 0, 16, 1);   // ��
    OLED_ShowChinese(18, 0, 1, 16, 1);  // ��
    OLED_ShowChinese(36, 0, 2, 16, 1);  // ԰
    OLED_ShowChinese(54, 0, 3, 16, 1);  // ��
    OLED_ShowChinese(72, 0, 4, 16, 1);  // ��
    OLED_ShowChinese(90, 0, 5, 16, 1);  // ��
    OLED_ShowChinese(108, 0, 6, 16, 1); // ��
    OLED_ShowString(8, 16, "ZHONGJINGYUAN", 16, 1);
    OLED_ShowString(20, 32, "2014/05/01", 16, 1);
    OLED_ShowString(0, 48, "ASCII:", 16, 1);
    OLED_ShowString(63, 48, "CODE:", 16, 1);
    OLED_ShowChar(48, 48, t, 16, 1); // ��ʾASCII�ַ�
    t++;
    if (t > '~') t = ' ';
    OLED_ShowNum(103, 48, t, 3, 16, 1);
    OLED_Refresh();
    HAL_Delay(1500);
    OLED_Clear();
    OLED_ShowChinese(0, 0, 0, 16, 1);   // 16*16 ��
    OLED_ShowChinese(16, 0, 0, 24, 1);  // 24*24 ��
    OLED_ShowChinese(24, 20, 0, 32, 1); // 32*32 ��
    OLED_ShowChinese(64, 0, 0, 64, 1);  // 64*64 ��
    OLED_Refresh();
    HAL_Delay(1500);
    OLED_Clear();
    OLED_ShowString(0, 0, "ABC", 8, 1);   // 6*8 ABC
    OLED_ShowString(0, 8, "ABC", 12, 1);  // 6*12 ABC
    OLED_ShowString(0, 20, "ABC", 16, 1); // 8*16 ABC
    OLED_ShowString(0, 36, "ABC", 24, 1); // 12*24 ABC
    OLED_Refresh();
    HAL_Delay(1500);
    // OLED_ScrollDisplay(11, 4, 1);  //���Ǹ�����ѭ��
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == (&htim1)) {
        /* code */
        HAL_GPIO_TogglePin(GPIOA, LED1_Pin);
        // Data_parsing();
    }
    if (htim == (&htim2)) {
        /* code */
        HAL_GPIO_TogglePin(GPIOA, LED2_Pin);
    }
    if (htim == (&htim3)) {
        /* code */
        HAL_GPIO_TogglePin(GPIOA, LED3_Pin);
    }
    if (htim == (&htim4)) {
        /* code */
        HAL_GPIO_TogglePin(GPIOB, LED4_Pin);
    }
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == (&huart1)) {
        /* code */
        RxBuffer[Uart1_Rx_Cnt++] = aRxBuffer;
        if (Uart1_Rx_Cnt >= 19) {
            /* code */
            HAL_UART_Transmit_DMA(&huart1, (uint8_t *)&RxBuffer, Uart1_Rx_Cnt);
            memset(RxBuffer, 0, Uart1_Rx_Cnt);
            Uart1_Rx_Cnt = 0;
        }
    }
    HAL_UART_Receive_DMA(&huart1, (uint8_t *)&aRxBuffer, 1);
}
void Data_parsing(void)
{
    if (Uart1_Rx_Cnt >= 10) {
        /* code */

        //     HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET);
        //     HAL_Delay(200);
        //     HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);
    }
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
