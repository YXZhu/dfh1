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
extern int32_t setSPEED,speed;
//extern int16_t SPEEDA,SPEEDB;
int16_t SPDA,SPDB; //25ms 测的速度计数；
uint16_t SA=0,SB=0,Timec=0;
int32_t setSPA,setSPB; //设置目标电机速度值

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

#define TurnSpeed 3500
<<<<<<< HEAD
#define MiniDis 140
=======
#define MiniDis 135
>>>>>>> parent of 415e836... 淇敼3
#define MaxDis  160

int16_t angle_temp1 = 0,angle_temp2 = 0,angle_temp3 = 0,angle_temp4=0,angle_time = 0;
int16_t moto_angle(unsigned char SF,int16_t setangle,unsigned char a)
{  
	if(angle_temp1 == 0)
	{
		//angle_temp2 = YAW;
		setSPA = TurnSpeed;
      setSPB = TurnSpeed;
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
//	setSPA = TurnSpeed;
//   setSPB = TurnSpeed;
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
//   setSPA = TurnSpeed;
//   setSPB = TurnSpeed;
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

//设置机器行走固定距离，A走多少，B走多少，速度
//读取值 设定值
// direction 机器方向
int16_t ReadDisA = 0,ReadDisB = 0;
void moto_frontDis(int16_t setDisA,int16_t setDisB,int32_t speed1)
{

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 5;
	xLastWakeTime = xTaskGetTickCount();
	ReadDisA = 0;
	ReadDisB = 0;
	setSPB = setSPA = speed1;
	while(1)
	{
		if(ReadDisA>-setDisA)
		{
			HAL_GPIO_WritePin(N1_GPIO_Port,N1_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(N2_GPIO_Port,N2_Pin,GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(N1_GPIO_Port,N1_Pin|N2_Pin,GPIO_PIN_RESET);
			//if(ReadDisB<=-setDisB) break ;
		}
		if(ReadDisB>-setDisB)
		{
			HAL_GPIO_WritePin(N4_GPIO_Port,N4_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(N3_GPIO_Port,N3_Pin,GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(N1_GPIO_Port,N3_Pin|N4_Pin,GPIO_PIN_RESET);
			//if(ReadDisA<=-setDisA) break ;
		}
		if(ReadDisB<=-setDisB&ReadDisA<=-setDisA) break ;
		osDelayUntil(&xLastWakeTime, xFrequency);
	}
}
void moto_backDis(int16_t setDisA,int16_t setDisB,int32_t speed1)
{
	
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 10;
	xLastWakeTime = xTaskGetTickCount();
	ReadDisA = 0;
	ReadDisB = 0;
	setSPB = setSPA = speed1;
	while(1)
	{
		if(ReadDisA<setDisA)
		{
			HAL_GPIO_WritePin(N2_GPIO_Port,N2_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(N1_GPIO_Port,N1_Pin,GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(N1_GPIO_Port,N1_Pin|N2_Pin,GPIO_PIN_RESET);
			//if(ReadDisB<=-setDisB) break ;
		}
		if(ReadDisB<setDisB)
		{
			HAL_GPIO_WritePin(N3_GPIO_Port,N3_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(N4_GPIO_Port,N4_Pin,GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(N1_GPIO_Port,N3_Pin|N4_Pin,GPIO_PIN_RESET);
			//if(ReadDisA<=-setDisA) break ;
		}
		if(ReadDisB>=setDisB&ReadDisA>=setDisA) break ;
		osDelayUntil(&xLastWakeTime, xFrequency);
	}
}
extern osThreadId main_1Handle;
extern osThreadId moto_jzHandle;
extern osThreadId mpuHandle;
extern osThreadId moto_controlHandle;
extern osThreadId bzHandle;
extern osThreadId Echo_1Handle;

void moto_jztask(void const * argument)
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 5;
	const TickType_t xFrequency1 = 5;
	const TickType_t xFrequency2 = 120;
	xLastWakeTime = xTaskGetTickCount();
	moto_control1 = 0;
	//speed = 9990;
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
						setSPA = speed;
						setSPB = (speed/2);
				}
				else if(EDjl2>MaxDis)
				{	
					 moto_front();
					 setSPA = (speed/2);
					 setSPB = speed;			
				}
				else
				{
					if(EDjl3<MiniDis)
					{
						setSPB = setSPA = speed;
						moto_left(0);
					}
					else if(EDjl3>MaxDis)
					{
						setSPB = setSPA = speed;
						moto_right(0);
					}
					else
					{
						setSPA = speed;
						setSPB = speed;
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
						setSPA = speed;
						setSPB = (speed/2);
					//}
				}
				else if(EDjl3>MaxDis)
				{	
					   moto_back();
						setSPA = (speed/2);
						setSPB = speed;			
				}
				else
				{
					if(EDjl2<MiniDis)
					{
						setSPB = setSPA = speed;
						moto_right(2);
					}
					else if(EDjl2>MaxDis)
					{
						setSPB = setSPA = speed;
						moto_left(2);
					}
					else
					{
						setSPA = speed;
						setSPB = speed;
						moto_back();
					}
				}	
			}
			break;
			case 4:  
			{//右
				vTaskSuspend(Echo_1Handle);
				vTaskResume(mpuHandle);
			   vTaskSuspend(bzHandle);
				moto_stop();
			   osDelayUntil(&xLastWakeTime, xFrequency2);
           // vTaskSuspend(moto_controlHandle);
			   //TIM9->CCR1 = 5000;
				//TIM9->CCR2 = 5000;
            
					
					while(moto_angle(1,9000,1) != HAL_OK)
					{
						osDelayUntil(&xLastWakeTime,xFrequency1);
						//break;
					}
						moto_control1 = 0;
					   HAL_GPIO_WritePin(GPIOG, GPIO_PIN_15,GPIO_PIN_SET);
						//osDelay(1000);
						//vTaskResume(moto_controlHandle);
						vTaskSuspend(mpuHandle);
						vTaskResume(Echo_1Handle);
						osDelayUntil(&xLastWakeTime, xFrequency2);
						vTaskResume(bzHandle);
						vTaskResume(main_1Handle);
				
			}
			break;
			case 3:  
			{//左  
				vTaskSuspend(Echo_1Handle);
				vTaskResume(mpuHandle);
				vTaskSuspend(bzHandle);
				moto_stop();
			   osDelayUntil(&xLastWakeTime, xFrequency2);     			
					while(moto_angle(0,9000,1) != HAL_OK)
					{
						osDelayUntil(&xLastWakeTime,xFrequency1);
	
					   //break;	
					}
					   moto_control1 = 0;
					   HAL_GPIO_WritePin(GPIOG, GPIO_PIN_15,GPIO_PIN_SET);
						//osDelay(200);
						//vTaskResume(moto_controlHandle);
						vTaskSuspend(mpuHandle);
						vTaskResume(Echo_1Handle);
						osDelayUntil(&xLastWakeTime, xFrequency2);
						vTaskResume(bzHandle);
						vTaskResume(main_1Handle);
				
			}
			break;
			case 5: //前进
			{
				moto_front();
				setSPB = setSPA = speed;
			}
			break;
			case 6: //后退
			{
				moto_back();
				setSPB = setSPA = speed;
			}
			default:break;
		}
	osDelayUntil(&xLastWakeTime, xFrequency);
		//osDelay(10);
	}
		
}

extern uint8_t angleJS;
void bztask(void const * argument)
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 1;
	const TickType_t xFrequency1 = 5;
	const TickType_t xFrequency2 = 10;
	const TickType_t xFrequency3 = 1000;
	const TickType_t xFrequency4 = 2;
	xLastWakeTime = xTaskGetTickCount();
	uint8_t swdj_1,bzbjs_1,swjs_1,swjs_3;
	swjs_1 = 0;
	swjs_3 = 0;
	bzbjs_1 = 0;
	swdj_1 = 1;
//	setSPEED = 8000;
//	speed = setSPEED;
//	angleJS = 6;
	HAL_GPIO_WritePin(ENA2_GPIO_Port,ENA2_Pin|ENB2_Pin,GPIO_PIN_RESET);
	for(;;)
	{
//		
//		if(HAL_GPIO_ReadPin(HW_1_GPIO_Port,HW_1_Pin)==GPIO_PIN_RESET)
//		{
//		HAL_GPIO_WritePin(ENA2_GPIO_Port,ENA2_Pin|ENB2_Pin,GPIO_PIN_SET);
//		for (uint16_t j=0; j<2; j++) 
//	  {
//		  HAL_GPIO_WritePin(ENA2_GPIO_Port,A_Pin|D_Pin,GPIO_PIN_SET);
//		  HAL_GPIO_WritePin(ENA2_GPIO_Port,C_Pin|B_Pin,GPIO_PIN_RESET);
//		  osDelayUntil(&xLastWakeTime, xFrequency1);		  
//		  HAL_GPIO_WritePin(ENA2_GPIO_Port,D_Pin,GPIO_PIN_SET);
//		  HAL_GPIO_WritePin(ENA2_GPIO_Port,A_Pin|B_Pin|C_Pin,GPIO_PIN_RESET);
//		  osDelayUntil(&xLastWakeTime, xFrequency1);			  
//		  HAL_GPIO_WritePin(ENA2_GPIO_Port,D_Pin|C_Pin,GPIO_PIN_SET);
//		  HAL_GPIO_WritePin(ENA2_GPIO_Port,A_Pin|B_Pin,GPIO_PIN_RESET);
//		  osDelayUntil(&xLastWakeTime, xFrequency1);		  
//		  HAL_GPIO_WritePin(ENA2_GPIO_Port,C_Pin,GPIO_PIN_SET);
//		  HAL_GPIO_WritePin(ENA2_GPIO_Port,A_Pin|B_Pin|D_Pin,GPIO_PIN_RESET);
//		  osDelayUntil(&xLastWakeTime, xFrequency1);			  
//		  HAL_GPIO_WritePin(ENA2_GPIO_Port,B_Pin|C_Pin,GPIO_PIN_SET);
//		  HAL_GPIO_WritePin(ENA2_GPIO_Port,A_Pin|D_Pin,GPIO_PIN_RESET);
//		  osDelayUntil(&xLastWakeTime, xFrequency1);		  
//		  HAL_GPIO_WritePin(ENA2_GPIO_Port,B_Pin,GPIO_PIN_SET);
//		  HAL_GPIO_WritePin(ENA2_GPIO_Port,A_Pin|C_Pin|D_Pin,GPIO_PIN_RESET);
//		  osDelayUntil(&xLastWakeTime, xFrequency1);	
//		  HAL_GPIO_WritePin(ENA2_GPIO_Port,B_Pin|A_Pin,GPIO_PIN_SET);
//		  HAL_GPIO_WritePin(ENA2_GPIO_Port,C_Pin|D_Pin,GPIO_PIN_RESET);
//		  osDelayUntil(&xLastWakeTime, xFrequency1);	
//		  HAL_GPIO_WritePin(ENA2_GPIO_Port,A_Pin,GPIO_PIN_SET);
//		  HAL_GPIO_WritePin(ENA2_GPIO_Port,B_Pin|C_Pin|D_Pin,GPIO_PIN_RESET);
//		  osDelayUntil(&xLastWakeTime, xFrequency1);	
//	  }
//	  for (uint16_t j=0; j<43-2; j++) //0.703125 * 42
//	  {
//		  HAL_GPIO_WritePin(ENA2_GPIO_Port,A_Pin|D_Pin,GPIO_PIN_SET);
//		  HAL_GPIO_WritePin(ENA2_GPIO_Port,C_Pin|B_Pin,GPIO_PIN_RESET);
//		  osDelayUntil(&xLastWakeTime, xFrequency);			  
//		  HAL_GPIO_WritePin(ENA2_GPIO_Port,D_Pin,GPIO_PIN_SET);
//		  HAL_GPIO_WritePin(ENA2_GPIO_Port,A_Pin|B_Pin|C_Pin,GPIO_PIN_RESET);
//		  osDelayUntil(&xLastWakeTime, xFrequency);			  
//		  HAL_GPIO_WritePin(ENA2_GPIO_Port,D_Pin|C_Pin,GPIO_PIN_SET);
//		  HAL_GPIO_WritePin(ENA2_GPIO_Port,A_Pin|B_Pin,GPIO_PIN_RESET);
//		  osDelayUntil(&xLastWakeTime, xFrequency);			  
//		  HAL_GPIO_WritePin(ENA2_GPIO_Port,C_Pin,GPIO_PIN_SET);
//		  HAL_GPIO_WritePin(ENA2_GPIO_Port,A_Pin|B_Pin|D_Pin,GPIO_PIN_RESET);
//		  osDelayUntil(&xLastWakeTime, xFrequency);			  
//		  HAL_GPIO_WritePin(ENA2_GPIO_Port,B_Pin|C_Pin,GPIO_PIN_SET);
//		  HAL_GPIO_WritePin(ENA2_GPIO_Port,A_Pin|D_Pin,GPIO_PIN_RESET);
//		  osDelayUntil(&xLastWakeTime, xFrequency);			  
//		  HAL_GPIO_WritePin(ENA2_GPIO_Port,B_Pin,GPIO_PIN_SET);
//		  HAL_GPIO_WritePin(ENA2_GPIO_Port,A_Pin|C_Pin|D_Pin,GPIO_PIN_RESET);
//		  osDelayUntil(&xLastWakeTime, xFrequency);
//		  HAL_GPIO_WritePin(ENA2_GPIO_Port,B_Pin|A_Pin,GPIO_PIN_SET);
//		  HAL_GPIO_WritePin(ENA2_GPIO_Port,C_Pin|D_Pin,GPIO_PIN_RESET);
//		  osDelayUntil(&xLastWakeTime, xFrequency);			  
//		  HAL_GPIO_WritePin(ENA2_GPIO_Port,A_Pin,GPIO_PIN_SET);
//		  HAL_GPIO_WritePin(ENA2_GPIO_Port,B_Pin|C_Pin|D_Pin,GPIO_PIN_RESET);
//		  osDelayUntil(&xLastWakeTime, xFrequency);
//	  }
//	  HAL_GPIO_WritePin(ENA2_GPIO_Port,ENA2_Pin|ENB2_Pin,GPIO_PIN_RESET);
//     }
		 if(HAL_GPIO_ReadPin(HW_1_GPIO_Port,HW_1_Pin) == 1)
		{
			osDelayUntil(&xLastWakeTime, xFrequency4);
			if(HAL_GPIO_ReadPin(HW_1_GPIO_Port,HW_1_Pin) == 1)
			{
				 if(swdj_1 == 0)
				 {
					swdj_1 = 1;
					 bzbjs_1++;
					 swjs_1 = 0;
					 swjs_3 = 0; 	 
				 }
							
			}
			if(swjs_3>20) bzbjs_1 = 0;
			else swjs_3++;	
		}
		else
		{
			swdj_1 = 0;			
			if(swjs_1>80) bzbjs_1 = 0;
			else swjs_1++;				
		}
     if(angleJS >= 1)
	  {
		  while(angleJS >= 10)
		  {
			  osDelayUntil(&xLastWakeTime, xFrequency3);
		  }
			if(bzbjs_1 == 3)
			{
				bzbjs_1 = 0;
				vTaskSuspend(main_1Handle);
				vTaskSuspend(moto_jzHandle);
				vTaskSuspend(moto_controlHandle);
				moto_stop();			
				HAL_GPIO_WritePin(ENA2_GPIO_Port,ENA2_Pin|ENB2_Pin,GPIO_PIN_SET);
				for (uint16_t j=0; j<2; j++) 
			  {
				  HAL_GPIO_WritePin(ENA2_GPIO_Port,A_Pin|D_Pin,GPIO_PIN_SET);
				  HAL_GPIO_WritePin(ENA2_GPIO_Port,C_Pin|B_Pin,GPIO_PIN_RESET);
				  osDelayUntil(&xLastWakeTime, xFrequency1);		  
				  HAL_GPIO_WritePin(ENA2_GPIO_Port,D_Pin,GPIO_PIN_SET);
				  HAL_GPIO_WritePin(ENA2_GPIO_Port,A_Pin|B_Pin|C_Pin,GPIO_PIN_RESET);
				  osDelayUntil(&xLastWakeTime, xFrequency1);			  
				  HAL_GPIO_WritePin(ENA2_GPIO_Port,D_Pin|C_Pin,GPIO_PIN_SET);
				  HAL_GPIO_WritePin(ENA2_GPIO_Port,A_Pin|B_Pin,GPIO_PIN_RESET);
				  osDelayUntil(&xLastWakeTime, xFrequency1);		  
				  HAL_GPIO_WritePin(ENA2_GPIO_Port,C_Pin,GPIO_PIN_SET);
				  HAL_GPIO_WritePin(ENA2_GPIO_Port,A_Pin|B_Pin|D_Pin,GPIO_PIN_RESET);
				  osDelayUntil(&xLastWakeTime, xFrequency1);			  
				  HAL_GPIO_WritePin(ENA2_GPIO_Port,B_Pin|C_Pin,GPIO_PIN_SET);
				  HAL_GPIO_WritePin(ENA2_GPIO_Port,A_Pin|D_Pin,GPIO_PIN_RESET);
				  osDelayUntil(&xLastWakeTime, xFrequency1);		  
				  HAL_GPIO_WritePin(ENA2_GPIO_Port,B_Pin,GPIO_PIN_SET);
				  HAL_GPIO_WritePin(ENA2_GPIO_Port,A_Pin|C_Pin|D_Pin,GPIO_PIN_RESET);
				  osDelayUntil(&xLastWakeTime, xFrequency1);	
				  HAL_GPIO_WritePin(ENA2_GPIO_Port,B_Pin|A_Pin,GPIO_PIN_SET);
				  HAL_GPIO_WritePin(ENA2_GPIO_Port,C_Pin|D_Pin,GPIO_PIN_RESET);
				  osDelayUntil(&xLastWakeTime, xFrequency1);	
				  HAL_GPIO_WritePin(ENA2_GPIO_Port,A_Pin,GPIO_PIN_SET);
				  HAL_GPIO_WritePin(ENA2_GPIO_Port,B_Pin|C_Pin|D_Pin,GPIO_PIN_RESET);
				  osDelayUntil(&xLastWakeTime, xFrequency1);	
			  }
			  for (uint16_t j=0; j<43-2; j++) //0.703125 * 42
			  {
				  HAL_GPIO_WritePin(ENA2_GPIO_Port,A_Pin|D_Pin,GPIO_PIN_SET);
				  HAL_GPIO_WritePin(ENA2_GPIO_Port,C_Pin|B_Pin,GPIO_PIN_RESET);
				  osDelayUntil(&xLastWakeTime, xFrequency);			  
				  HAL_GPIO_WritePin(ENA2_GPIO_Port,D_Pin,GPIO_PIN_SET);
				  HAL_GPIO_WritePin(ENA2_GPIO_Port,A_Pin|B_Pin|C_Pin,GPIO_PIN_RESET);
				  osDelayUntil(&xLastWakeTime, xFrequency);			  
				  HAL_GPIO_WritePin(ENA2_GPIO_Port,D_Pin|C_Pin,GPIO_PIN_SET);
				  HAL_GPIO_WritePin(ENA2_GPIO_Port,A_Pin|B_Pin,GPIO_PIN_RESET);
				  osDelayUntil(&xLastWakeTime, xFrequency);			  
				  HAL_GPIO_WritePin(ENA2_GPIO_Port,C_Pin,GPIO_PIN_SET);
				  HAL_GPIO_WritePin(ENA2_GPIO_Port,A_Pin|B_Pin|D_Pin,GPIO_PIN_RESET);
				  osDelayUntil(&xLastWakeTime, xFrequency);			  
				  HAL_GPIO_WritePin(ENA2_GPIO_Port,B_Pin|C_Pin,GPIO_PIN_SET);
				  HAL_GPIO_WritePin(ENA2_GPIO_Port,A_Pin|D_Pin,GPIO_PIN_RESET);
				  osDelayUntil(&xLastWakeTime, xFrequency);			  
				  HAL_GPIO_WritePin(ENA2_GPIO_Port,B_Pin,GPIO_PIN_SET);
				  HAL_GPIO_WritePin(ENA2_GPIO_Port,A_Pin|C_Pin|D_Pin,GPIO_PIN_RESET);
				  osDelayUntil(&xLastWakeTime, xFrequency);
				  HAL_GPIO_WritePin(ENA2_GPIO_Port,B_Pin|A_Pin,GPIO_PIN_SET);
				  HAL_GPIO_WritePin(ENA2_GPIO_Port,C_Pin|D_Pin,GPIO_PIN_RESET);
				  osDelayUntil(&xLastWakeTime, xFrequency);			  
				  HAL_GPIO_WritePin(ENA2_GPIO_Port,A_Pin,GPIO_PIN_SET);
				  HAL_GPIO_WritePin(ENA2_GPIO_Port,B_Pin|C_Pin|D_Pin,GPIO_PIN_RESET);
				  osDelayUntil(&xLastWakeTime, xFrequency);
			  }
			  HAL_GPIO_WritePin(ENA2_GPIO_Port,ENA2_Pin|ENB2_Pin,GPIO_PIN_RESET);
			  
            setSPEED = 7000;
			   speed = setSPEED;
				vTaskResume(main_1Handle);
			   vTaskResume(moto_jzHandle);				
				vTaskResume(moto_controlHandle);
				osDelayUntil(&xLastWakeTime, xFrequency3);
			}
			switch(bzbjs_1)
			{
				case 0 :
				    setSPEED = 7000;
				    speed = setSPEED;
				break;
				case 1 :
			      setSPEED = 2500;
				   speed = setSPEED;
				break;
				case 2:
					setSPEED = 3000;
				   speed = setSPEED;
				break;
				case 3:
					setSPEED = 0;
				   speed = setSPEED;
				break;
				default:break;
			}
		
		
	}
   	osDelayUntil(&xLastWakeTime, xFrequency2);
	}
}
extern UART_HandleTypeDef huart3;
void moto_controltask(void const * argument)
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 15;
	xLastWakeTime = xTaskGetTickCount();
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

//		SPDA = fabs((float)SPDA);
//		SPDB = fabs((float)SPDB);
		if(SPDA<0) SPDA = - SPDA;
		if(SPDB<0) SPDB = - SPDB;
		    // pid.ActualSpeed = 0;
		SPA_temp = User_PidSpeedControlA(setSPA,SPDA*49.8);
		SPB_temp = User_PidSpeedControlB(setSPB,SPDB*49.8);
//		print_usart2("A%d\r\n",SPDB);
//		print_usart2("B%d\r\n",SPB_temp);
//		print_usart2("key1count=%d\n",SPA_temp);
//		print_usart2("Kcount=%d\n",SPDB);
//		print_usart2("key2count=%d\n",SPB_temp);
		//SPB_temp = User_PidSpeedControlB(setSPB,SPDB);
      if(SPA_temp > 10000)  SPA_temp =   10000-1 ;//上限 CCR的值必须小于或等于ARR的值
	   if(SPA_temp <-10000)  SPA_temp = -(10000-1);//下限 
      if(SPB_temp > 10000)  SPB_temp =   10000-1 ;//上限 CCR的值必须小于或等于ARR的值
	   if(SPB_temp <-10000)  SPB_temp = -(10000-1);//下限 
		
	   if(SPA_temp<0) TIM9->CCR1 = 0;
	   else TIM9->CCR1 = SPA_temp;
      
	   if(SPB_temp<0) TIM9->CCR2 = 0;		
	   else TIM9->CCR2 = SPB_temp;	   
		//osDelay(15);
		osDelayUntil(&xLastWakeTime, xFrequency);
		
	}
}
