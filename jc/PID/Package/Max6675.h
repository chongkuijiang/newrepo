/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MAX6675_H
#define _MAX6675_H

#define	  sck_1  GPIO_SetBits(GPIOB,GPIO_Pin_13)      
#define	  sck_0  GPIO_ResetBits(GPIOB,GPIO_Pin_13)        
#define	  cs1_1  GPIO_SetBits(GPIOB,GPIO_Pin_12)        
#define	  cs1_0  GPIO_ResetBits(GPIOB,GPIO_Pin_12)          
#define	  so_status  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)  // 只能往外输出数据


void max6675_delay(u16 n);
float read_max6675_temper(void);
#endif /* _MAX6675_H */