
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */
#include "STM_MY_LCD16X2.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
int liczba_sygnalow_zegara = 0;
int odebrane_bity[8];
int odebrany_znak = 0;
const char * klawiatura = "XXXXXXXXXXXXXT~XXASXXq1XXXzsaw2XXcxde43XX vftr5XXnbhgy6XXXmju78XX,kio09XX./l;p-XXX'X[=XXPSM]X\\XXXXXXXXBXXXXLXXXXXXDXRUEXX+X-*XXXX\0";
char symbol = '0';
int shift=0, caps=0;
int row=1,col=1;
char data2[2] = {};
_Bool cursor=true, blink=true;
int repeater=0, guard = 0, guard2 = 0,  guard4 = 0, guard5 = 0, guard6 = 0, enter_guard = 0,l_r_guard=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if(liczba_sygnalow_zegara != 0 && liczba_sygnalow_zegara < 9) {
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_SET) {
			odebrane_bity[liczba_sygnalow_zegara] = 1;
		} else
			odebrane_bity[liczba_sygnalow_zegara] = 0;
	} else if(liczba_sygnalow_zegara == 9) {
		odebrany_znak = BityNaZnak(odebrane_bity);
		symbol = klawiatura[odebrany_znak];
		char data[2] = {symbol, 0};


		if(shift==1 && symbol!='S'){
			switch(symbol){
			case 'P':
				break;
			case '`':
				data[0] = '~';
				break;
			case '1':
				data[0] = '!';
				break;
			case '2':
				data[0] = '@';
				break;
			case '3':
				data[0] = '#';
				break;
			case '4':
				data[0] = '$';
				break;
			case '5':
				data[0] = '%';
				break;
			case '6':
				data[0] = '^';
				break;
			case '7':
				data[0] = '&';
				break;
			case '8':
				data[0] = '*';
				break;
			case '9':
				data[0] = '(';
				break;
			case '0':
				data[0] = ')';
				break;
			case '-':
				data[0] = '_';
				break;
			case '=':
				data[0] = '+';
				break;
			case '[':
				data[0] = '{';
				break;
			case ']':
				data[0] = '}';
				break;
			case '\\':
				data[0] = '|';
				break;
			case ';':
				data[0] = ':';
				break;
			case '\'':
				data[0] = '\"';
				break;
			case ',':
				data[0] = '<';
				break;
			case '.':
				data[0] = '>';
				break;
			case '/':
				data[0] = '?';
				break;
			default: if(caps!=1  && (data[0] < 123 && data[0] > 96)) data[0]-=32;
		}} else {
			if(caps == 1 && symbol!='P' && symbol != 'S' && (data[0] < 123 && data[0] > 96) && shift != 1) {
				data[0]-=32;
			} else if(caps == 1 && symbol!='P' && symbol != 'S'  && (data[0] < 123 && data[0] > 96) && shift == 1) {
				data[0]+=32;
			} else
			if(shift==1 && symbol=='S') {
				shift = 0;
			}
			else if (symbol=='S' && shift==0 ) {
					shift=1;
				}
		}
		switch (symbol){
			case 'E' : LCD1602_clear2();
			row = 1;
			col = 1;
				break;
			case 'M' :
				if (enter_guard == 0) {
				if(row == 1) {
				row = 2; col = 1;
				LCD1602_setCursor(row, col);}
				else if (row == 2) {
					row = 1; col = 1;
					LCD1602_setCursor(row, col);
				}
				enter_guard = 1;
				} else enter_guard = 0;
				break;
			case 'U' :
					   LCD1602_setCursor(1, col);
					   row = 1;
				break;
			case 'P':
				if(guard6 == 0) {
				if(caps == 1) caps = 0;
				else caps = 1;
				guard6 = 1;
				} else guard6 = 0;
			break;
			case 'D' :
			   LCD1602_setCursor(2, col);
			   row = 2;
				break;
			case 'L' :	if(l_r_guard==0){
						naLewo();
						LCD1602_setCursor(row,col);
						l_r_guard=1;
			}
			else l_r_guard=0;
				break;
			case 'R' : 	if(l_r_guard==0){
						naPrawo();
						LCD1602_setCursor(row,col);
						l_r_guard=1;}
						else l_r_guard=0;
				break;
			case 'B' :	if (guard == 0) {
						naLewo();
						LCD1602_setCursor(row, col);
						LCD1602_print(" ");
						LCD1602_setCursor(row, col);
						guard = guard + 1;
			}
			else guard = 0;
				break;
			case 'T' : if(guard4 == 0) {
				if(cursor==true){

							LCD1602_noCursor();
							cursor=false;
						}
						else{
							LCD1602_cursor();
							cursor=true;
						}
				guard4 = 1;
			} else guard4 = 0;
				break;
			case 'A' : if(guard5 == 0) {
				if (blink == true) {

				LCD1602_noBlink();
				blink = false;
			} else {
				LCD1602_blink();
				blink = true;
			}
				guard5=1;
			} else
			guard5=0;
				break;
			case 'X' :
				break;
			case 'S' :
				break;
			default : if (repeater == 0) {
						LCD1602_print(data);
						repeater = repeater + 1;
						naPrawo();

			} else repeater = 0;

		}

		liczba_sygnalow_zegara = -2;
	}
	liczba_sygnalow_zegara += 1;
}
void naPrawo(){
	col=col+1;
	if(col==17&&row==1)	{
		col=1;
		row=2;
		LCD1602_setCursor(row, col);
	}
	else	if(col==17&&row==2){
		col=1;
		row=1;
		LCD1602_setCursor(row, col);
	}
}

void naLewo(){
	col=col-1;
	if(col==0&&row==1)	{
		col=16;
		row=2;
		LCD1602_setCursor(row, col);
	}
	else if(col==0&&row==2){
		col=16;
		row=1;
		LCD1602_setCursor(row, col);
	}
}

int BityNaZnak(int bity[]) {
	int podstawa = 1, wynik = 0;
	for(int i = 0; i < 8; i++) {
		if(bity[i] == 1)
			wynik += podstawa;
		podstawa = podstawa * 2;
	}
	return wynik/2;
}

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  /* USER CODE BEGIN 2 */
  //sekwencja testowa przy odpalaniu
  LCD1602_Begin4BIT(RS_GPIO_Port,RS_Pin,E_Pin,D4_GPIO_Port,D4_Pin,D5_Pin,D6_Pin,D7_Pin);
  for(int i=0;i<10;i++) {LCD1602_PrintInt(i);HAL_Delay(300);}
  for(int i=0;i<3;i++) {LCD1602_print(".");HAL_Delay(300);}
  LCD1602_2ndLine();
  LCD1602_print("PS2 Sniffer");
  HAL_Delay(1000);
  LCD1602_clear();
  LCD1602_1stLine();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

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

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, D0_Pin|D1_Pin|D2_Pin|D3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, D4_Pin|D5_Pin|D6_Pin|D7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, RS_Pin|E_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : Clock_Pin */
  GPIO_InitStruct.Pin = Clock_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Clock_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : Dane_Pin */
  GPIO_InitStruct.Pin = Dane_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Dane_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : D0_Pin D1_Pin D2_Pin D3_Pin */
  GPIO_InitStruct.Pin = D0_Pin|D1_Pin|D2_Pin|D3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : D4_Pin D5_Pin D6_Pin D7_Pin */
  GPIO_InitStruct.Pin = D4_Pin|D5_Pin|D6_Pin|D7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : RS_Pin E_Pin */
  GPIO_InitStruct.Pin = RS_Pin|E_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
