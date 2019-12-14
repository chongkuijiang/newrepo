
401开机检测顺序：

1.保险丝断：F1_OP
2.可控硅短路：TR_ST
3.感温线接反：TC_RE(有余温)
4.感温线断：TC_OP
5.加热圈与感温线接反：HC_RE
6.电热线电流大：HT_ST
//7.电热线断：HT_OP
//8.加热------20s到计时显示
//9.感温线接反：TC_RE
//10.感温线短路：TC_ST



CW401-1.2   
date: 2019.2.28
添加开机使能失能检查功能checkOff,PB5
adr.bit.ID7 = 0;
	checkOff = (GPIOB->IDR & GPIO_Pin_5)? 0:1;  //ID7-PB5  
	if(checkOff==0){
	//加热圈>11.7R 	热电耦<15R}
	
	

1.4版本修改：
在升温同步状态下设置表芯温度会被上位机修改，增加svDelay;

	