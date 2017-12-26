#include "moto.h"
#include "main.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "echo.h"
#include "pidcontoller.h"
#include "math.h"
#include "uart.h"

#include<stdio.h>
#include<math.h>

//double angle;
extern uint16_t EDjl1,EDjl2,EDjl3,EDjl4,EDjl5,EDjl6;
extern int16_t YAW;
extern int16_t SPEEDA,SPEEDB;
int16_t SPDA,SPDB;
uint16_t SA=0,SB=0,Timec=0;
int32_t setSPA,setSPB;

//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//	if(GPIO_Pin == GPIO_PIN_10)
//	{
//		SA++;
//	}
//	if(GPIO_Pin == GPIO_PIN_11)
//	{
//		SB++;
//	}
//}

#define TurnSpeed 2000
#define MiniDis 160
#define MaxDis  180


int16_t angle_temp1 = 0,angle_temp2 = 0,angle_temp3 = 0,angle_temp4=0,angle_time = 0;
int16_t moto_angle(unsigned char SF,int16_t setangle,unsigned char a)
{  
	if(angle_temp1 == 0)
	{
		//angle_temp2 = YAW;
		angle_temp1 = 1;
		angle_time = 0;
		if(SF ==1)
		{
			angle_temp2 = YAW - setangle;
			moto_right(2);
			//moto_right(0);
		}
		else
		{
			angle_temp2 = YAW + setangle;
			
			moto_left(2);
		}
		if(angle_temp2>18000) angle_temp2 = -36000 + angle_temp2;
		if(angle_temp2<-18000) angle_temp2 = 36000 + angle_temp2;
		angle_temp3 = angle_temp2 + 100;
		if(angle_temp3>18000) angle_temp3 = -36000 + angle_temp3;
		angle_temp4 = angle_temp2 - 100;
		if(angle_temp4<-18000) angle_temp4 = angle_temp4 + 36000;
	}
	if(SF == 1)
	{
		if(angle_time>10) moto_right(a);
		else
		{
			angle_time++;
			//moto_right(3);
		}
		if(angle_temp3<angle_temp4)
		{
			if(YAW>=angle_temp4)
			{
//				moto_left(a);
//				osDelay(3);
				moto_stop();
				
				angle_temp1 = 0;
				return HAL_OK;
			}
		}
		else
		{			
			if(YAW<angle_temp3)
			{
				if(YAW>=angle_temp4)
				{
//					moto_left(a);
//					osDelay (3);
					moto_stop();
					angle_temp1 = 0;
					return HAL_OK;
				}
				//else moto_left(1);
			}	
	   }		

	}
	else
	{
		if(angle_time>10) moto_left(a);
		else
		{
			angle_time++;
			//moto_left(3);
		}
		if(YAW>angle_temp4)
		{
			if(YAW<=angle_temp3)
			{
//				moto_right(a);
//				osDelay (3);
				moto_stop();	
				angle_temp1 = 0;
				return HAL_OK;
			}
		  // else moto_right(1);
		}
		//else moto_left(1);
	}	
	return HAL_BUSY;
}

void moto_front(void)
{
	HAL_GPIO_WritePin(N1_GPIO_Port,N1_Pin|N4_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(N2_GPIO_Port,N2_Pin|N3_Pin,GPIO_PIN_SET);
}
void moto_back(void)
{
	HAL_GPIO_WritePin(N1_GPIO_Port,N1_Pin|N4_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(N2_GPIO_Port,N2_Pin|N3_Pin,GPIO_PIN_RESET);
}



void moto_right(unsigned char a)
{	
	setSPA = TurnSpeed;
   setSPB = TurnSpeed;
   switch(a)
	{
		case 0:
		{
		   HAL_GPIO_WritePin(N1_GPIO_Port,N1_Pin,GPIO_PIN_RESET);
	      HAL_GPIO_WritePin(N4_GPIO_Port,N2_Pin|N3_Pin|N4_Pin,GPIO_PIN_SET);
		}
		break;
		case 1:
		{
		    HAL_GPIO_WritePin(N1_GPIO_Port,N1_Pin|N3_Pin,GPIO_PIN_RESET);
	       HAL_GPIO_WritePin(N4_GPIO_Port,N2_Pin|N4_Pin,GPIO_PIN_SET);
		}
      break;
		case 2:
		{
			HAL_GPIO_WritePin(N1_GPIO_Port,N1_Pin|N2_Pin|N4_Pin,GPIO_PIN_SET);
		   HAL_GPIO_WritePin(N3_GPIO_Port,N3_Pin,GPIO_PIN_RESET);
		}
		break;
		default:break;
	}
	
}

void moto_left(unsigned char a)
{	
   setSPA = TurnSpeed;
   setSPB = TurnSpeed;
	switch(a)
	{
		case 0:
		{
			HAL_GPIO_WritePin(N2_GPIO_Port,N2_Pin|N1_Pin|N3_Pin,GPIO_PIN_SET);
	      HAL_GPIO_WritePin(N3_GPIO_Port,N4_Pin,GPIO_PIN_RESET);
		}
		break;
		case 1:
		{
			HAL_GPIO_WritePin(N1_GPIO_Port,N1_Pin|N3_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(N4_GPIO_Port,N4_Pin|N2_Pin,GPIO_PIN_RESET);
		}
		break;
		case 2:
		{
			HAL_GPIO_WritePin(N1_GPIO_Port,N1_Pin|N4_Pin|N3_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(N2_GPIO_Port,N2_Pin,GPIO_PIN_RESET);
		}
		break;
		default:break;
	}
}

void moto_stop(void)
{
	HAL_GPIO_WritePin(N1_GPIO_Port,N1_Pin|N2_Pin|N3_Pin|N4_Pin,GPIO_PIN_RESET);
}

uint8_t moto_control1;
extern osThreadId main_1Handle;
extern osThreadId moto_jzHandle;
extern osThreadId mpuHandle;
extern osThreadId moto_controlHandle;
extern osThreadId bzHandle;

void moto_jztask(void const * argument)
{
//	int32_t tempA,tempB;
//   osThreadId hmc5983Handle;
  // moto_front();
	//osDelay(500);
    //int16_t YAW_temp;
	//osDelay(10);
	//int32_t angle_t;
	moto_control1 = 0;
   SPEEDA = 0;
	SPEEDB = 0;
	for(;;)
	{
		//if(EDjl2>EDjl3) 
		switch(moto_control1)
		{
			case 1:
			{
				if(EDjl2<MiniDis)
				{
						moto_front();
						setSPA = SPEEDA;
						setSPB = 1200;
				}
				else if(EDjl2>MaxDis)
				{	
					 moto_front();
					 setSPA = 1200;
					 setSPB = SPEEDB;			
				}
				else
				{
					if(EDjl3<MiniDis)
					{
						moto_left(0);
					}
					else if(EDjl3>MaxDis)
					{
						moto_right(0);
					}
					else
					{
						setSPA = SPEEDA;
						setSPB = SPEEDB;
						moto_front();
					}
				 }	
			 }
			break;
			case 2:
			{
				if(EDjl3<MiniDis)
				{
					//if(EDjl2<175)
					//{
						moto_back();
						setSPA = SPEEDA;
						setSPB = 1000;
					//}
				}
				else if(EDjl3>MaxDis)
				{	
					   moto_back();
						setSPA = 1000;
						setSPB = SPEEDB;			
				}
				else
				{
					if(EDjl2<MiniDis)
					{
						moto_left(2);
					}
					else if(EDjl2>MaxDis)
					{
						moto_right(2);
					}
					else
					{
						setSPA = SPEEDA;
						setSPB = SPEEDB;
						moto_back();
					}
				}	
			}
			break;
			case 3:
			{
				vTaskResume(mpuHandle);
			   vTaskSuspend(bzHandle);
			   //osDelay(4);
           // vTaskSuspend(moto_controlHandle);
			   //TIM9->CCR1 = 5000;
				//TIM9->CCR2 = 5000;
				if(moto_angle(1,9000,1) == HAL_OK)
				{
					
					moto_control1 = 0;
					//osDelay(1000);
					//vTaskResume(moto_controlHandle);
					vTaskResume(bzHandle);
					vTaskResume(main_1Handle);
					vTaskSuspend(mpuHandle);
				}
			}
			break;
			case 4:
			{
				vTaskResume(mpuHandle);
				vTaskSuspend(bzHandle);
				//osDelay(4);
				//vTaskSuspend(moto_controlHandle);
					//TIM9->CCR1 = 5000;
					//TIM9->CCR2 = 5000;			
				if(moto_angle(0,9000,1) == HAL_OK)
				{
					
					moto_control1 = 0;
					//osDelay(200);
					//vTaskResume(moto_controlHandle);
					vTaskResume(bzHandle);
					vTaskResume(main_1Handle);
					vTaskSuspend(mpuHandle);
				}
			}
			break;
			default:break;
		}
	osDelay(15);
	}
		
}
extern uint16_t setSPEED;
extern uint8_t setangle,angleJS;


void bztask(void const * argument)
{
	uint8_t swdj_1,swdj_2,bzjs,bzbjs_1,bzbjs_2,swjs_1,swjs_2,swjs_3,swjs_4;
	swjs_1 = 0;
	swjs_2 = 0;
	swjs_3 = 0;
	swjs_4 = 0;
	bzbjs_1 = 0;
	bzbjs_2 = 0;
	bzjs = 0;
	swdj_1 = 1;
	swdj_2 = 1;
	uint16_t pwm[] = {500,612,746,869,1001,1168,1335,1491,1658,1825,1992,2159};
	for(;;)
	{
		 if(HAL_GPIO_ReadPin(HW_1_GPIO_Port,HW_1_Pin) == 1)
		{
			osDelay(2);
			if(HAL_GPIO_ReadPin(HW_1_GPIO_Port,HW_1_Pin) == 1)
			{
				 if(swdj_1 == 0)
				 {
					swdj_1 = 1;
					 bzbjs_1++;
					 swjs_1 = 0;
					 swjs_3 = 0;
		//			 if(bzbjs_1 == 4) 
		//			 {
		//				 
		//				 bzjs++;
		//				 TIM3->CCR1 = pwm[bzjs];
		//				 vTaskSuspend(main_1Handle);
		//				 vTaskSuspend(moto_jzHandle);
		//				 moto_stop();
		//				 osDelay(1000);
		//				 vTaskResume(main_1Handle);
		//				 vTaskResume(moto_jzHandle);
		//			 }	 
						 
				 }
							
			}
			if(swjs_3>20) bzbjs_1 = 0;
			else swjs_3++;	
		}
		else
		{
			swdj_1 = 0;			
			if(swjs_1>60) bzbjs_1 = 0;
			else swjs_1++;				
		}
		if(HAL_GPIO_ReadPin(HW_2_GPIO_Port,HW_2_Pin) == 1)
		{
			osDelay(2);
			if(HAL_GPIO_ReadPin(HW_2_GPIO_Port,HW_2_Pin) == 1)
		  {
				if(swdj_2 == 0)
				{
					swdj_2 = 1;
					bzbjs_2 ++;
					swjs_2 = 0;
					swjs_4 = 0;
				}
				if(swjs_4>20) bzbjs_2 = 0;
			   else swjs_4++;	
			}

		}
		else
		{
			swdj_2 = 0;
			if(swjs_2>60) bzbjs_2 = 0;
			else swjs_2++;	
		}	
     if(angleJS >= 1)
	  {
		  while(angleJS >= 10)
		  {
			  osDelay(1);
		  }
		  if(setangle == 1)
		  {
			if(bzbjs_1 == 3)
			{
				bzjs++;
				bzbjs_2 = 0;
				bzbjs_1 = 0;
				vTaskSuspend(main_1Handle);
				vTaskSuspend(moto_jzHandle);
				vTaskSuspend(moto_controlHandle);
				setSPEED = 7000;
				TIM9->CCR1 = 8000;
				TIM9->CCR2 = 8000;

				moto_back();
				//osDelay(150);
				UDelayUS(50000);
				moto_stop();
				
				TIM3->CCR1 = pwm[bzjs];
				osDelay(1200);
				moto_front();

				vTaskResume(main_1Handle);
				vTaskResume(moto_jzHandle);
				vTaskResume(moto_controlHandle);
				osDelay(600);
			}
			switch(bzbjs_2)
			{
				case 0 :
				    setSPEED = 7000;
				break;
				case 1 :
			      setSPEED = 3000;
				break;
				case 2:
					setSPEED = 1500;
				break;
				case 3:
					setSPEED = 0;
				break;
			}
		 }
		  else
		  {
			if(bzbjs_2 == 3)
			{
				bzjs++;
				bzbjs_2 = 0;
				bzbjs_1 = 0;
				vTaskSuspend(main_1Handle);
				vTaskSuspend(moto_jzHandle);
				vTaskSuspend(moto_controlHandle);
				setSPEED = 7000;
				TIM9->CCR1 = 9000;
				TIM9->CCR2 = 9000;

				moto_front();
				//osDelay(150);
				UDelayUS(50000);
				moto_stop();
				
				TIM3->CCR1 = pwm[bzjs];
				osDelay(1200);
				moto_back();

				vTaskResume(main_1Handle);
				vTaskResume(moto_jzHandle);
				vTaskResume(moto_controlHandle);
				osDelay(600);
			}
			switch(bzbjs_1)
			{
				case 0 :
				    setSPEED = 7000;
				break;
				case 1 :
			      setSPEED = 3000;
				break;
				case 2:
					setSPEED = 1500;
				break;
				case 3:
					setSPEED = 0;
				break;
			}
		}
	}
//		if(bzbjs_2 ==1)
//		{

//		}
//		if(bzbjs_2 == 2)
//		{
//			setSPEEDA = 3000;
//			setSPEEDB = 3000;
//		}
//		if(bzbjs_2 == 3)
//		{
//			setSPEEDA = 1000;
//			setSPEEDB = 1000;
//		}
		
   	osDelay(5);
	}
}
extern UART_HandleTypeDef huart3;
void moto_controltask(void const * argument)
{
	//xQueueHandle MsgQueue;
	int32_t SPA_temp,SPB_temp;
   //857
	TIM9->CCR1 = 0;
	TIM9->CCR2 = 0;
	moto_front();
	setSPA = 0;
	setSPB = 0;
	SPDA = 0;
	SPDB = 0;
	
	//HAL_UART_Receive_DMA(&huart3,aa,2); 
	for(;;)
	{
		
		
		
		SPDA = fabs((float)SPDA);
		SPDB = fabs((float)SPDB);
		    // pid.ActualSpeed = 0;
		SPA_temp = User_PidSpeedControlA(setSPA,SPDA*49.8);
		SPB_temp = User_PidSpeedControlB(setSPB,SPDB*49.8);
		print_usart2("A%d\r\n",SPDB);
		print_usart2("B%d\r\n",SPB_temp);
//		print_usart2("key1count=%d\n",SPA_temp);
//		print_usart2("Kcount=%d\n",SPDB);
//		print_usart2("key2count=%d\n",SPB_temp);
		//SPB_temp = User_PidSpeedControlB(setSPB,SPDB);
      if(SPA_temp > 10000)  SPA_temp =   10000-1 ;//上限 CCR的值必须小于或等于ARR的值
	   if(SPA_temp <-10000)  SPA_temp = -(10000-1);//下限 

	   if(SPA_temp<0) TIM9->CCR1 = 0;
	   else TIM9->CCR1 = SPA_temp;
      if(SPB_temp > 10000)  SPB_temp =   10000-1 ;//上限 CCR的值必须小于或等于ARR的值
	   if(SPB_temp <-10000)  SPB_temp = -(10000-1);//下限 
	   if(SPB_temp<0) 		
		 TIM9->CCR2 = 0;
	
	   else TIM9->CCR2 = SPB_temp;	   
		osDelay(15);
		
	}
}
