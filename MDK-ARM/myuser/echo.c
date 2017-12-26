#include "main.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "echo.h"


//unsigned char SW1,SW2,SW3,SW4,SW5,SW6;
unsigned char SW1;
uint16_t ED1,ED2,ED3,ED4,ED5,ED6;
uint16_t EDjl1,EDjl2,EDjl3,EDjl4,EDjl5,EDjl6;
unsigned char temp1,temp2,temp3,temp4,temp5,temp6;	
uint16_t echo1[2] = {0}, echo2[2] = {0}, echo3[2] = {0}, echo4[2] = {0},echo5[2] = {0},echo6[2] = {0};

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

void UDelayUS (unsigned int ulCount)
{
	unsigned int i;
	for ( i = 0; i < ulCount; i ++ )
	{
		uint8_t uc = 12;     //设置值为12，大约延1微秒  	      
		while ( uc -- );     //延1微秒	
	}	
}
void Echo_Init(void)
{

	EDjl1 = 0;
	EDjl2 = 0;
	EDjl3 = 0;
	EDjl4 = 0;
	EDjl5 = 0;
	EDjl6 = 0;
	
	SW1 = 1;
//	SW2 = 1;
//	SW3 = 1;
//	SW4 = 1;
//	SW5 = 1;
//	SW6 = 1;
	ED1 = 0;
	ED2 = 0;
	ED3 = 0;
	ED4 = 0;
	ED5 = 0;
	ED6 = 0;
	temp1 = 1;
	temp2 = 1;
	temp3 = 1;
	temp4 = 1;
	temp5 = 1;
	temp6 = 1;
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{	
	  switch((uint32_t)htim->Instance)
	  {
		  case (uint32_t)TIM1:
		  {
			  switch(htim->Channel)
			  {
				  case HAL_TIM_ACTIVE_CHANNEL_1:
				  {
					  if(temp1==1)
					 {
						 echo1[0] =  HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
						 temp1 = 0;
					 }
					 else
					 {
						 echo1[1] =  HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
						 temp1 = 1;
					 }
				 }
				  break;
				  case HAL_TIM_ACTIVE_CHANNEL_2:
				  {
					  if(temp2==1)
					 {
						 echo2[0] =  HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_2);
						 temp2 = 0;
					 }
					 else
					 {
						 echo2[1] =  HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_2);
						 temp2 = 1;
					 }
				 }
				  break;
				  case HAL_TIM_ACTIVE_CHANNEL_3:
				  {
					  if(temp3==1)
					 {
						 echo3[0] =  HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_3);
						 temp3 = 0;
					 }
					 else
					 {
						 echo3[1] =  HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_3);
						 temp3 = 1;
					 }
				 }
				  break;	
				  case HAL_TIM_ACTIVE_CHANNEL_4:
				  {
					  if(temp4==1)
					 {
						 echo4[0] =  HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_4);
						 temp4 = 0;
					 }
					 else
					 {
						 echo4[1] =  HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_4);
						 temp4 = 1;
					 }
				 }
				  break;	
				  default:break;
			 }
		 }
		 break;
		 case (uint32_t)TIM2:
		 {
			 switch(htim->Channel)
			 {
				  case HAL_TIM_ACTIVE_CHANNEL_3:
				  {
					  if(temp5==1)
					 {
						 echo5[0] =  HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_3);
						 temp5 = 0;
					 }
					 else
					 {
						 echo5[1] =  HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_3);
						 temp5 = 1;
					 }
				 }
				  break;	
				  case HAL_TIM_ACTIVE_CHANNEL_4:
				  {
					  if(temp6==1)
					 {
						 echo6[0] =  HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_4);
						 temp6 = 0;
					 }
					 else
					 {
						 echo6[1] =  HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_4);
						 temp6 = 1;
					 }
				 }
				  break;	
				  default:break;
			 }
		 }
		 break;
		 default:break;
	 }
} 
uint16_t ED1t,ED2t,ED3t,ED4t,ED5t,ED6t;
extern osThreadId main_1Handle;
extern uint8_t moto_control1;

void echo_1(void)
{
	
}

void Echo_1task(void const * argument)
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 9;
	xLastWakeTime = xTaskGetTickCount();
  for(;;)
  {
	   if(SW1==1) 
		{
		   HAL_GPIO_WritePin(Trig_1_GPIO_Port,Trig_1_Pin|Trig_2_Pin|Trig_3_Pin|Trig_4_Pin|Trig_5_Pin|Trig_6_Pin,GPIO_PIN_SET);//Trig_1 = 1;			
			osDelay(1);
			temp1 = 1;
			temp2 = 1;
			temp3 = 1;
			temp4 = 1;
			temp5 = 1;
			temp6 = 1;
			HAL_GPIO_WritePin(Trig_1_GPIO_Port,Trig_1_Pin|Trig_2_Pin|Trig_3_Pin|Trig_4_Pin|Trig_5_Pin|Trig_6_Pin,GPIO_PIN_RESET);//Trig_1 = 0;
			SW1 = 0;
         
		}
		else
		{	
		   osDelayUntil(&xLastWakeTime, xFrequency);
		//osDelay(9);
			if(echo1[0]<echo1[1]) ED1 = echo1[1] - echo1[0];
			else ED1 = (50000-echo1[0]) + echo1[1];			   
			ED1*=0.17;
			if(((ED1 - ED1t) > 100) || ((ED1t - ED1) > 100)) EDjl1 = ED1t;
    		else	EDjl1 = ED1;
			ED1t = ED1;
			
			if(echo2[0]<echo2[1]) ED2 =  echo2[1] - echo2[0];
		   else ED2 = (50000-echo2[0]) + echo2[1];
		   ED2*=0.17;
			if(((ED2 - ED2t) > 100) || ((ED2t - ED2) > 100)) EDjl2 = ED2t;
    		else	EDjl2 = ED2;
			ED2t = ED2;
			
			if(echo3[0]<echo3[1]) ED3 = echo3[1] - echo3[0];
		   else ED3 = (50000-echo3[0]) + echo3[1];				 
		   ED3*=0.17;
			if(((ED3 - ED3t) > 100) || ((ED3t - ED3) > 100)) EDjl3 = ED3t;
    		else	EDjl3 = ED3;
			ED3t = ED3;	
			
			if(echo4[0]<echo4[1]) ED4 = echo4[1] - echo4[0];
			else ED4 = (50000-echo4[0]) + echo4[1];			
			ED4*=0.17;
			if(((ED4 - ED4t) > 100) || ((ED4t - ED4) > 100)) EDjl4 = ED4t;
    		else	EDjl4 = ED4;
			ED4t = ED4;
			
			if(echo5[0]<echo5[1]) ED5 = echo5[1] - echo5[0];
		   else ED5 = (50000-echo5[0]) + echo5[1];				 
		   ED5*=0.17;
			if(((ED5 - ED5t) > 100) || ((ED5t - ED5) > 100)) EDjl5 = ED5t;
    		else	EDjl5 = ED5;
			ED5t = ED5;
			
			if(echo6[0]<echo6[1]) ED6 = echo6[1] - echo6[0];
			else ED6 = (50000-echo6[0]) + echo6[1];			
			ED6*=0.17;
			if(((ED6 - ED6t) > 100) || ((ED6t - ED6) > 100)) EDjl6 = ED6t;
    		else	EDjl6 = ED6;
			ED6t = ED6;			
				SW1 = 1;			
		}
		osDelay(1);
  }
  /* USER CODE END 5 */ 
}
//void echo_2(void)
//{
//	if(SW2==1) 
//	{
//	  HAL_GPIO_WritePin(Trig_2_GPIO_Port,Trig_2_Pin,GPIO_PIN_SET);//Trig_1 = 1;			
//		temp2 = 1;
//		osDelay(1);
//		HAL_GPIO_WritePin(Trig_2_GPIO_Port,Trig_2_Pin,GPIO_PIN_RESET);//Trig_1 = 0;
//		SW2 = 0;
//		
//	}
//	else
//	{	
//			
//        osDelay(9);
//		  if(echo2[0]<echo2[1]) ED2 =  echo2[1] - echo2[0];
//		  else ED2 = (50000-echo2[0]) + echo2[1];
//		   ED2*=0.17;
//			if(((ED2 - ED2t) > 100) || ((ED2t - ED2) > 100)) EDjl2 = ED2t;
//    		else	EDjl2 = ED2;
//			ED2t = ED2;
////		  if(ED2<1600) EDjl2 = ED2*0.8+EDjl2*0.2;
////		  else EDjl2 = 1600;		  
//		  SW2 = 1;
//        //echo2[0]= 0 ;echo2[1]= 0 ;
//		  
//	}
//}
//void echo_3(void)
//{
//	if(SW3==1) 
//	{
//	  HAL_GPIO_WritePin(Trig_3_GPIO_Port,Trig_3_Pin,GPIO_PIN_SET);//Trig_1 = 1;			
//		temp3 = 1;
//		osDelay(1);
//		HAL_GPIO_WritePin(Trig_3_GPIO_Port,Trig_3_Pin,GPIO_PIN_RESET);//Trig_1 = 0;
//		SW3 = 0;
//		
//		//echo3[0]= 0 ;echo3[1]= 0 ;
//	}
//	else
//	{	
//		  osDelay(9);				
//		  if(echo3[0]<echo3[1]) ED3 = echo3[1] - echo3[0];
//		  else ED3 = (50000-echo3[0]) + echo3[1];				 
//		  ED3*=0.17;
//			if(((ED3 - ED3t) > 100) || ((ED3t - ED3) > 100)) EDjl3 = ED3t;
//    		else	EDjl3 = ED3;
//			ED3t = ED3;		
////		  if(ED3<1600) EDjl3 = ED3*0.8+EDjl3*0.2;
////		    else EDjl3 = 1600;
//		  SW3 = 1; 
//	}
//}
//void echo_4(void)
//{
//	if(SW4==1) 
//	{
//	   HAL_GPIO_WritePin(Trig_4_GPIO_Port,Trig_4_Pin,GPIO_PIN_SET);//Trig_1 = 1;			
//		temp4 = 1;
//		osDelay(1);
//		HAL_GPIO_WritePin(Trig_4_GPIO_Port,Trig_4_Pin,GPIO_PIN_RESET);//Trig_1 = 0;
//		SW4 = 0;
//		
//		//echo4[0]= 0 ;echo4[1]= 0 ;
//	}
//	else
//	{	
//			osDelay(9);			
//			if(echo4[0]<echo4[1]) ED4 = echo4[1] - echo4[0];
//			else ED4 = (50000-echo4[0]) + echo4[1];			
//			ED4*=0.17;
//			if(((ED4 - ED4t) > 100) || ((ED4t - ED4) > 100)) EDjl4 = ED4t;
//    		else	EDjl4 = ED4;
//			ED4t = ED4;		
////		   if(ED4<1600) EDjl4 = ED4*0.8+EDjl4*0.2;
////		   else EDjl4 = 1600;
//			SW4 = 1;
//	}
//}

//void echo_5(void)
//{
//	if(SW5==1) 
//	{
//	  HAL_GPIO_WritePin(Trig_5_GPIO_Port,Trig_5_Pin,GPIO_PIN_SET);//Trig_1 = 1;			
//		temp5 = 1;
//		osDelay(1);
//		HAL_GPIO_WritePin(Trig_5_GPIO_Port,Trig_5_Pin,GPIO_PIN_RESET);//Trig_1 = 0;
//		SW5 = 0;
//		
//		//echo3[0]= 0 ;echo3[1]= 0 ;
//	}
//	else
//	{	
//		  osDelay(9);				
//		  if(echo5[0]<echo5[1]) ED5 = echo5[1] - echo5[0];
//		  else ED5 = (50000-echo5[0]) + echo5[1];				 
//		  ED5*=0.17;
//			if(((ED5 - ED5t) > 100) || ((ED5t - ED5) > 100)) EDjl5 = ED5t;
//    		else	EDjl5 = ED5;
//			ED5t = ED5;		
////		  if(ED5<1600) EDjl5 = ED5*0.8+EDjl5*0.2;
////		   else EDjl5 = 1600;
//		  SW5 = 1; 
//	}
//}
//void echo_6(void)
//{
//	if(SW6==1) 
//	{
//	   HAL_GPIO_WritePin(Trig_6_GPIO_Port,Trig_6_Pin,GPIO_PIN_SET);//Trig_1 = 1;			
//		temp6 = 1;
//		osDelay(1);
//		HAL_GPIO_WritePin(Trig_6_GPIO_Port,Trig_6_Pin,GPIO_PIN_RESET);//Trig_1 = 0;
//		SW6 = 0;
//		
//		//echo4[0]= 0 ;echo4[1]= 0 ;
//	}
//	else
//	{	
//			osDelay(9);	
//			
//			  if(echo6[0]<echo6[1]) ED6 = echo6[1] - echo6[0];
//			  else ED6 = (50000-echo6[0]) + echo6[1];			
//			  ED6*=0.17;
//			if(((ED6 - ED6t) > 100) || ((ED6t - ED6) > 100)) EDjl6 = ED6t;
//    		else	EDjl6 = ED6;
//			ED6t = ED6;		
////		     if(ED6<1600) EDjl6 = ED6*0.8+EDjl6*0.2;
////		     else EDjl6 = 1600;
//			  SW6 = 1;
//	}
//}
