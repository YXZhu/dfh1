#include "mpu.h"
#include "main.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "dmpctl.h"
#include "uart.h"
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;
uint8_t mpurebuf[8] = {0};
int16_t YAW,PITCH,ROLL;


int i2c_write(unsigned char slave_addr,
                     unsigned char reg_addr,
                     unsigned char length,
                     unsigned char const *data)
		{
			osDelay(1);
					if(HAL_I2C_Mem_Write(&hi2c1,slave_addr,reg_addr,I2C_MEMADD_SIZE_16BIT,(uint8_t*)data,length,10)!=HAL_OK)
					{
						print_usart2("\r\nHAL_I2C_Mem_Write Error\r\n");
						return -1;
					}
					return 0;
		}
		
int i2c_read(unsigned char slave_addr,
                    unsigned char reg_addr,
                    unsigned char length,
                    unsigned char *data)
		{ 
			osDelay(1);
					if(HAL_I2C_Mem_Read(&hi2c1,slave_addr,reg_addr,I2C_MEMADD_SIZE_16BIT,(uint8_t*)data,length,10)!=HAL_OK)
						{
							print_usart2("\r\nHAL_I2C_Mem_Read Error\r\n");
							return -1;
						}
						return 0;
		}
void mputask(void const * argument)
{
//	uint8_t mpuinit[] = {0xA5,0x55};
//	HAL_UART_Transmit_DMA(&huart1,mpuinit,2);
//	osDelay(10);
//	mpuinit[1] = 0x52;
//	HAL_UART_Transmit_DMA(&huart1,mpuinit,2);
//	osDelay(10);
	YAW = 0;
	PITCH = 0;
	ROLL = 0;
	 HAL_UART_Receive_DMA(&huart2, mpurebuf,8); 
	
	for(;;)
	{
		 if(!(mpurebuf[0]==0xaa))//如果帧头错误，清缓存
		 {	
			HAL_UART_DMAStop(&huart2);
			HAL_UART_Receive_DMA(&huart2, mpurebuf,8); 
			mpurebuf[0]=0;
		 }
		 else
		 {
			 if(mpurebuf[7]==0x55)
		   {
			 YAW=(mpurebuf[1]<<8|mpurebuf[2]);//YAW，PITCH,ROLL为真实值的100倍
			 //PITCH=(mpurebuf[3]<<8|mpurebuf[4]);
			// ROLL=(mpurebuf[5]<<8|mpurebuf[6]);
		
		   }
		}
		 osDelay(5);
	 }
 }
