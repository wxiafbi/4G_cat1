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
uint8_t Senbuff[] = "\r\n**** Serial Output Message by DMA ***\r\n   UART DMA Test \r\n   Zxiaoxuan"; // 瀹氫箟鏁版嵁鍙戦?佹暟缁?

#define RXBUFFERSIZE 512               // 最大接收字节数
uint8_t RxBuffer[RXBUFFERSIZE]  = {0}; // 接收数据
uint8_t DATA[RXBUFFERSIZE]      = {0}; // 接收数据
uint8_t show_DATA[RXBUFFERSIZE] = {0}; // 显示数据

uint8_t aRxBuffer;     // 接收中断缓冲
char Uart1_Rx_Cnt = 0; // 接收缓冲计数
char flag         = 0; // 接收标志位
char show_flag    = 0; // 显示更新标志位
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
void show_parsing(void);
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
    OLED_ColorTurn(0);   // 0正常显示，1反色显示
    OLED_DisplayTurn(0); // 0正常显示，1屏幕翻转显示
    HAL_UART_Receive_DMA(&huart1, RxBuffer, 512);

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        oled_show();
        if (flag != 0) {
            /* code */
            HAL_GPIO_TogglePin(GPIOA, LED3_Pin);
            Data_parsing();
            flag = 0;
        }
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
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
    // uint8_t t = ' ';
    // OLED_ShowPicture(0, 0, 128, 64, BMP1, 1);
    OLED_Refresh();
    // HAL_Delay(1500);
    // OLED_Clear();
    // OLED_ShowChinese(0, 0, 0, 16, 1);   // 中
    // OLED_ShowChinese(18, 0, 1, 16, 1);  // 景
    // OLED_ShowChinese(36, 0, 2, 16, 1);  // 园
    // OLED_ShowChinese(54, 0, 3, 16, 1);  // 电
    // OLED_ShowChinese(72, 0, 4, 16, 1);  // 子
    // OLED_ShowChinese(90, 0, 5, 16, 1);  // 技
    // OLED_ShowChinese(108, 0, 6, 16, 1); // 术
    // OLED_ShowString(8, 16, "ZHONGJINGYUAN", 16, 1);
    // OLED_ShowString(20, 32, "2014/05/01", 16, 1);
    OLED_ShowString(0, 48, "ASCII:", 16, 1);
    OLED_ShowString(0, 0, show_DATA, 16, 1);
    // OLED_ShowChar(54, 0, Uart1_Rx_Cnt, 24, 1); // 显示ASCII字符
    // t++;
    // if (t > '~') t = ' ';
    OLED_ShowNum(103, 48, Uart1_Rx_Cnt, 3, 16, 1);
    // OLED_Refresh();
    // HAL_Delay(1500);
    // OLED_Clear();
    // OLED_ShowChinese(0, 0, 0, 16, 1);   // 16*16 中
    // OLED_ShowChinese(16, 0, 0, 24, 1);  // 24*24 中
    // OLED_ShowChinese(24, 20, 0, 32, 1); // 32*32 中
    // OLED_ShowChinese(64, 0, 0, 64, 1);  // 64*64 中
    // OLED_Refresh();
    // HAL_Delay(1500);
    // OLED_Clear();
    // OLED_ShowString(0, 0, "ABC", 8, 1);   // 6*8 ABC
    // OLED_ShowString(0, 8, "ABC", 12, 1);  // 6*12 ABC
    // OLED_ShowString(0, 20, "ABC", 16, 1); // 8*16 ABC
    // OLED_ShowString(0, 36, "ABC", 24, 1); // 12*24 ABC
    // OLED_Refresh();
    // HAL_Delay(1500);
    // OLED_ScrollDisplay(11, 4, 1);  //这是个无限循环
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == (&htim1)) {
        /* code */
    }
    if (htim == (&htim2)) {
        /* code */
        // HAL_GPIO_TogglePin(GPIOA, LED1_Pin);
    }
    if (htim == (&htim3)) {
        /* code */
        // HAL_GPIO_TogglePin(GPIOA, LED1_Pin);
    }
    if (htim == (&htim4)) {
        /* code */
        // HAL_GPIO_TogglePin(GPIOA, LED1_Pin);
    }
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == (&huart1)) {
        /* code */
        HAL_GPIO_TogglePin(GPIOA, LED1_Pin);
    }
    if (huart == (&huart2)) {
        /* code */
        // HAL_GPIO_TogglePin(GPIOA, LED1_Pin);
    }
}
void Data_parsing(void)
{
    for (size_t i = 0; i < Uart1_Rx_Cnt; i++) {
        /* code */
        DATA[i] = RxBuffer[i];
    }
    HAL_UART_Transmit_DMA(&huart1, (uint8_t *)&DATA, Uart1_Rx_Cnt);
    show_flag++;
    show_parsing();
    memset(RxBuffer, 0, Uart1_Rx_Cnt);
}

  
void show_parsing(void)
{
    if (show_flag != 0) {
        /* code */
        memset(show_DATA, 0, Uart1_Rx_Cnt);
        HAL_DMA_Start(&hdma_memtomem_dma1_channel1, (uint32_t)DATA, (uint32_t)show_DATA, 12);
        show_flag = 0;
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
