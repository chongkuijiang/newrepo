



CW401-1.2   
date: 2019.2.28
��ӿ���ʹ��ʧ�ܼ�鹦��checkOff,PB5
adr.bit.ID7 = 0;
	checkOff = (GPIOB->IDR & GPIO_Pin_5)? 0:1;  //ID7-PB5  
	if(checkOff==0){
	//����Ȧ>11.7R 	�ȵ���<15R}