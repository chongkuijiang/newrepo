陈国营修改的版本，从毅比道出差开始
1、2018-1-23日修改配置

CHdata4.TemVoltage = 0
增加随机设置，modfile功能，波特率115200
零点是520自带的。
18-7-13
波特率修改成38400
18-7-16 
SYS.ID=16 ;

18-11-27号      
最大采集测温点168点。指令30变成0x30；31变成0x31
函数USART1_IRQHandler 和函数TaskStartSaveData做了比较大的修改。

18-7-18号
CW520（115200-修改通信）  96点tmc温控板最新程序  
18-12-6号
CW520（168点 V1.0.0）  168点tmc温控板程序V1.0.0版本

18-12-19号
if(!(dataBuff[0]&0x0400)) SYS.ID+=32;	
最大id可以达到63    168点tmc温控板程序V1.0.1版本

19-1-7号
添加版本号程序   V1.0.2版本
else if(dataBuff[1]==0xaa)     // 读取版本号
{
	  SYS1.Cata7[0]=SYS.ID;
		SYS1.Cata7[1]=0xaa;
	SYS1.Cata7[2]=0xaa;
		SYS1.Cata7[3]=0x01;
		SYS1.Cata7[4]=0x00;
		SYS1.Cata7[5]=0x02;
		CCdataValue(&SYS1.Cata7[0],6);
		SYS1.Cata7[6]=SYS1.Cata1;
		SYS1.Cata7[7]=SYS1.Cata2;
		Tim_data(8);
}

19-3-15号
增加大电流程序，30A大电流，互感器硬件变动，电流扩大7倍 
V1.0.3版本

增加全局设置， 全局设置标识  由30修改成0x30  
case 6: dataBuff&=0x01;
		if((dataBuff==1)&&(data[0]==0x30))   
		
修改成   		if((dataBuff==1)&&(data[0]==0x30)) 
V1.0.4版本
电流扩大7倍  取消   
电流扩大2.25倍   
增加0x30 全局配置
CHdata1.DataGet[Current] *= 2.25 ;   //夏工测试的数据
V1.0.4版本

19-3-16号   注释掉
 // #define  _BIG_CURRENT_
增加0x30 全局配置 
 普通功率   V1.0.5版本 
19-6-17号  
添加4个   case HT_OP:
case TC_RE : case HT_ST:
case TR_ST : case HT_OP:
海外的客户反馈当加热丝不接时，HT_OP报警，但是输出百分比和继电器还是工作状态。 
修改以后HT_OP报警，输出百分比清零、继电器断开。
V1.0.6版本 














