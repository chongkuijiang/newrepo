



CW401-1.2   
date: 2019.2.28
添加开机使能失能检查功能checkOff,PB5
adr.bit.ID7 = 0;
	checkOff = (GPIOB->IDR & GPIO_Pin_5)? 0:1;  //ID7-PB5  
	if(checkOff==0){
	//加热圈>11.7R 	热电耦<15R}