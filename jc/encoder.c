s16 getLength()
{
	static int32_t count;
	s16 result;
	if(tims>=0){
	    count = __HAL_TIM_GET_COUNTER(&htim3) + tims*0xffff;
	}else{		  
		count = __HAL_TIM_GET_COUNTER(&htim3) - 0xffff;
		if(tims<-1){
		  count += (tims+1)*0xffff;   
		}
	}
	result = count/10;
	return result;
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(__HAL_TIM_IS_TIM_COUNTING_DOWN(htim)){
		tims--;
	}else{
		tims++;
	}
	
}
