#include "../include/LIB/std_types.h"
#include "../include/LIB/bit_math.h"

#include "../include/MCAL/RCC/RCC_interface.h"
#include "../include/MCAL/NVIC/NVIC_interface.h"
#include "../include/MCAL/SYSTICK/SYSTICK_interface.h"
#include "../include/MCAL/SCB/SCB_interface.h"
#include "../include/MCAl/GPIO/GPIO_interface.h"
#include "../include/MCAL/EXTI/EXTI_interface.h"

volatile u8 u8_StartFlag=0;
volatile u32 u32_DataFrame[50]={0};
volatile u8 u8_Counter=0;
volatile u8 u8_PureData=0;


/*play_func responsible for the operation should be done after receiving the order*/
void play_func(void)
{
	/*button 1, blue light on the RGB*/
	if(u8_PureData==12)
	{
		MGPIO_voidSetPinValue(GPIO_PORTA,GPIO_PIN5,GPIO_LOW);
		MGPIO_voidSetPinValue(GPIO_PORTA,GPIO_PIN6,GPIO_HIGH);
		MGPIO_voidSetPinValue(GPIO_PORTA,GPIO_PIN7,GPIO_HIGH);
	}
	/*button 2, green light on the RGB*/
	else if(u8_PureData==24)
	{
		MGPIO_voidSetPinValue(GPIO_PORTA,GPIO_PIN5,GPIO_HIGH);
		MGPIO_voidSetPinValue(GPIO_PORTA,GPIO_PIN6,GPIO_LOW);
		MGPIO_voidSetPinValue(GPIO_PORTA,GPIO_PIN7,GPIO_HIGH);
	}
	/*button 3, red light on the RGB*/
	else if(u8_PureData==94)
	{
		MGPIO_voidSetPinValue(GPIO_PORTA,GPIO_PIN5,GPIO_HIGH);
		MGPIO_voidSetPinValue(GPIO_PORTA,GPIO_PIN6,GPIO_HIGH);
		MGPIO_voidSetPinValue(GPIO_PORTA,GPIO_PIN7,GPIO_LOW);
	}
	else
	{
		MGPIO_voidSetPinValue(GPIO_PORTA,GPIO_PIN5,GPIO_LOW);
		MGPIO_voidSetPinValue(GPIO_PORTA,GPIO_PIN6,GPIO_LOW);
		MGPIO_voidSetPinValue(GPIO_PORTA,GPIO_PIN7,GPIO_LOW);
	}
	/*reset the variable storing the data-byte*/
	u8_PureData=0;

}

/*this function is called after receiving the whole frame*/
void void_TakeAction(void)
{
	/*time of 8-start bits, average 13500 micro second*/
	if(u32_DataFrame[0]>=10000 && u32_DataFrame[0]<=14000)
	{
		for(u8 i=0;i<8;i++)
		{
			/*check for the time if in this range the bit sent is 1, else the bit sent is 0*/
			/*constant(17) is added as the first element in the array is storing the time for the start byte*/
			/* from element 1 to element 16 are storing the address*/
			if(u32_DataFrame[i+17]>2000 && u32_DataFrame[i+17]<2300)
			{
				/*set the corresponding bit*/
				u8_PureData |=(1<<i);
			}
			else
			{
				/*clear the corresponding bit*/
				u8_PureData &=~(1<<i);
			}
		}
		/*call the play_func responsible for the operation should be done after receiving the order*/
		play_func();
	}
	/*initial state*/
	u8_StartFlag=0;
	u32_DataFrame[0]=0;
	u8_Counter=0;
}

/*call back function for EXT1 channel*/
void void_GetFrame(void)
{
	/*check if that is the first interrupt happens, which means */
	if(u8_StartFlag==0)
	{
		/*let the systick counts the time taken by the start byte*/
		MSYSTICK_voidIntervalSingle(1000000,void_TakeAction);
		u8_StartFlag=1;
	}
	else
	{
		/*with every falling edge the time elapsed will be saved in the u32_DataFrame array*/
		u32_DataFrame[u8_Counter]=MSYSTICK_u32GetElapsedTime();
		/*this void_TakeAction function will be called only after 1sec which ensuring that the whole frame was received*/
		MSYSTICK_voidIntervalSingle(1000000,void_TakeAction);
		u8_Counter++;
	}
}

void main(void)
{
	/*HSI_CLK=8MHZ , all busses prescaller=1*/
	MRCC_Init();
	/*enable clock for GPIOA*/
	MRCC_SetPeripheralConfiguration(RCC_PERIPHERAL_IOPA,RCC_PERIPHERAL_APB2_BUS,RCC_PERIPHERAL_ENABLE);
	/*enable clock for AFIO*/
	MRCC_SetPeripheralConfiguration(RCC_PERIPHERAL_AFIO,RCC_PERIPHERAL_APB2_BUS,RCC_PERIPHERAL_ENABLE);

	/*PA5 output*/
	MGPIO_voidSetDirection(GPIO_PORTA,GPIO_PIN5,GPIO_OUTPUT_2M_SPEED_MODE,GPIO_OUTPUT_PUSH_PULL);
	/*PA6 output*/
	MGPIO_voidSetDirection(GPIO_PORTA,GPIO_PIN6,GPIO_OUTPUT_2M_SPEED_MODE,GPIO_OUTPUT_PUSH_PULL);
	/*PA7 output*/
	MGPIO_voidSetDirection(GPIO_PORTA,GPIO_PIN7,GPIO_OUTPUT_2M_SPEED_MODE,GPIO_OUTPUT_PUSH_PULL);
	/*PA1 input1*/
	MGPIO_voidSetDirection(GPIO_PORTA,GPIO_PIN1,GPIO_INPUT_MODE,GPIO_FLOATING_INPUT );
	/*adjust AFIO in GPIO */
	MGPIO_voidSetPinAlternativeFunction(AF_EXTI_PORTA,AF_EXTI_CHANNEL1,AFIO_EXTI);

	/*adjust system group priority with the pre-configured value: 16-GP , 0-SUB*/
	MSCB_voidSetSystemGroupPriority();
	/*set priority for systick*/
	MNVIC_voidSetIntPriority(SCB_VECT_SYSTICK_ID,1,1);
	/*set priority for EXTI1*/
	MNVIC_voidSetIntPriority(7,2,1);
	/*enbale EXTI1 in NVIC*/
	MNVIC_voidEnablePeripheral(7);

	/*Init systick*/
	MSYSTICK_voidInit();

	/*init EXTI*/
	MEXTI_voidInit(AF_EXTI_CHANNEL1,EXTI_SENSE_FALLING_EDGE);
	/*set call back*/
	MEXTI_voidSetCallBack(void_GetFrame);
	/*enable EXTI*/
	MEXTI_voidEnableEXTI(AF_EXTI_CHANNEL1);


	while (1)
  	{

	}
}
