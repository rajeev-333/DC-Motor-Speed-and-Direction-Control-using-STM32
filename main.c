#include "main.h"

/* Peripheral handles created by STM32 HAL */
ADC_HandleTypeDef hadc1;
TIM_HandleTypeDef htim3;

/* ---------------- Motor Driver Pins ---------------- */

/* L298N IN1 pin */
#define MOTOR_IN1_PORT GPIOB
#define MOTOR_IN1_PIN  GPIO_PIN_0

/* L298N IN2 pin */
#define MOTOR_IN2_PORT GPIOB
#define MOTOR_IN2_PIN  GPIO_PIN_1

/* --------------------------------------------------- */

/* Variables to store ADC and PWM values */
uint32_t adc_value = 0;
uint32_t pwm_value = 0;

/* Function Prototypes */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM3_Init(void);

void Motor_Forward(void);
void Motor_Reverse(void);
void Motor_SetSpeed(uint32_t speed);

/* ================= MAIN FUNCTION ================= */

int main(void)
{

  /* Reset peripherals and initialize Flash + SysTick */
  HAL_Init();

  /* Configure system clock */
  SystemClock_Config();

  /* Initialize GPIO pins */
  MX_GPIO_Init();

  /* Initialize ADC peripheral */
  MX_ADC1_Init();

  /* Initialize Timer for PWM generation */
  MX_TIM3_Init();

  /* Start PWM generation on Timer3 Channel1 (PA6) */
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);

  /* Set default motor direction */
  Motor_Forward();

  while (1)
  {

    /* Start ADC conversion */
    HAL_ADC_Start(&hadc1);

    /* Wait until conversion is complete */
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);

    /* Read ADC value (0 - 4095) */
    adc_value = HAL_ADC_GetValue(&hadc1);

    /*
       Convert ADC value to PWM duty cycle.

       Timer period = 999
       ADC range = 0–4095

       So we scale ADC value to PWM range.
    */
    pwm_value = (adc_value * 999) / 4095;

    /* Apply speed to motor */
    Motor_SetSpeed(pwm_value);

    /* Small delay for stable operation */
    HAL_Delay(10);
  }
}

/* ================= MOTOR FUNCTIONS ================= */

/*
Function: Motor_Forward
Purpose : Rotate motor in forward direction
*/
void Motor_Forward(void)
{
  HAL_GPIO_WritePin(MOTOR_IN1_PORT, MOTOR_IN1_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(MOTOR_IN2_PORT, MOTOR_IN2_PIN, GPIO_PIN_RESET);
}

/*
Function: Motor_Reverse
Purpose : Rotate motor in reverse direction
*/
void Motor_Reverse(void)
{
  HAL_GPIO_WritePin(MOTOR_IN1_PORT, MOTOR_IN1_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(MOTOR_IN2_PORT, MOTOR_IN2_PIN, GPIO_PIN_SET);
}

/*
Function: Motor_SetSpeed
Purpose : Control motor speed using PWM
*/
void Motor_SetSpeed(uint32_t speed)
{
  /* Limit speed to timer range */
  if(speed > 999)
      speed = 999;

  /*
     Update PWM duty cycle.

     This changes the ON time of PWM signal
     which controls motor speed.
  */
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, speed);
}

/* ================= GPIO INITIALIZATION ================= */

static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Enable GPIOB clock */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* Configure PB0 and PB1 as output pins */
  GPIO_InitStruct.Pin = MOTOR_IN1_PIN | MOTOR_IN2_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/* ================= ADC INITIALIZATION ================= */

static void MX_ADC1_Init(void)
{

  ADC_ChannelConfTypeDef sConfig = {0};

  /* Configure ADC instance */
  hadc1.Instance = ADC1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = DISABLE;

  HAL_ADC_Init(&hadc1);

  /* Configure ADC channel (PA0) */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;

  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}

/* ================= TIMER PWM INITIALIZATION ================= */

static void MX_TIM3_Init(void)
{

  TIM_OC_InitTypeDef sConfigOC = {0};

  /* Configure Timer3 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 84-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

  HAL_TIM_PWM_Init(&htim3);

  /* Configure PWM channel */
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

  HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1);
}

/* ================= SYSTEM CLOCK CONFIG ================= */

void SystemClock_Config(void)
{
  /* Default CubeMX generated clock configuration will go here */
}