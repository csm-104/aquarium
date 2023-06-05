#include <REGX52.H>
#include "LCD1602.h"
#include "DS1302.h"
#include "Key.h"
#include "Timer0.h"
#include "Water_Level.h"
#include "DS18B20.h"
#include "Delay.h"

unsigned char KeyNum,MODE,TimeSetSelect,FeedTimeSelect1,FeedTimeSelect2,LedOpenTime,LedCloseTime,TimeSetFlashFlag,FeedFlag,LightFlag,Days;
char FeedTime1[]={13,00,10};//第一次喂食预定时间
char FeedTime2[]={14,00,10};//第二次喂食预定时间
char LedOpenSet[]={7,00,00};//补光灯打开预定时间
char LedCloseSet[]={19,00,00};//补光灯关闭预定时间

void TimeShow(void)//时间显示功能
{
	
	DS1302_ReadTime();//读取时间
	LCD_ShowNum(1,1,DS1302_Time[0],2);//显示年
	LCD_ShowNum(1,4,DS1302_Time[1],2);//显示月
	LCD_ShowNum(1,7,DS1302_Time[2],2);//显示日
	LCD_ShowNum(2,1,DS1302_Time[3],2);//显示时
	LCD_ShowNum(2,4,DS1302_Time[4],2);//显示分
	LCD_ShowNum(2,7,DS1302_Time[5],2);//显示秒
	LCD_ShowNum(1,16,DS1302_Time[6],1);//显示星期
    LCD_ShowString(1,11,"Days:");
	LCD_ShowString(2,10,"T:");
}

void TimeSet(void)//时间设置功能
{
	if(KeyNum==2)//按键2按下
	{
		TimeSetSelect++;//设置选择位加1
		TimeSetSelect%=17;//越界清零
	}
	if(KeyNum==3)//按键3按下
	{
		TimeSetSelect--;//设置选择位加1
		if(TimeSetSelect==255)
		{
			TimeSetSelect=16;//越界清零	
		}
	}
	if(KeyNum==4)//按键4按下
	{
		if(TimeSetSelect<7)
		{	
			DS1302_Time[TimeSetSelect]++;//时间设置位数值加1
			if(DS1302_Time[0]>99){DS1302_Time[0]=0;}//年越界判断
			if(DS1302_Time[1]>12){DS1302_Time[1]=1;}//月越界判断
			if( DS1302_Time[1]==1 || DS1302_Time[1]==3 || DS1302_Time[1]==5 || DS1302_Time[1]==7 || 
				DS1302_Time[1]==8 || DS1302_Time[1]==10 || DS1302_Time[1]==12)//日越界判断
			{
				if(DS1302_Time[2]>31){DS1302_Time[2]=1;}//大月
			}
			else if(DS1302_Time[1]==4 || DS1302_Time[1]==6 || DS1302_Time[1]==9 || DS1302_Time[1]==11)
			{
				if(DS1302_Time[2]>30){DS1302_Time[2]=1;}//小月
			}
			else if(DS1302_Time[1]==2)
			{
				if(DS1302_Time[0]%4==0)
				{
					if(DS1302_Time[2]>29){DS1302_Time[2]=1;}//闰年2月
				}
				else
				{
					if(DS1302_Time[2]>28){DS1302_Time[2]=1;}//平年2月
				}
			}
			if(DS1302_Time[3]>23){DS1302_Time[3]=0;}//时越界判断
			if(DS1302_Time[4]>59){DS1302_Time[4]=0;}//分越界判断
			if(DS1302_Time[5]>59){DS1302_Time[5]=0;}//秒越界判断
			if(DS1302_Time[6]>7){DS1302_Time[6]=0;}//星期越界判断
		}
		else if(TimeSetSelect>6&&TimeSetSelect<10)
		{
			FeedTimeSelect1=TimeSetSelect-7;
			FeedTime1[FeedTimeSelect1]++;
			if(FeedTime1[0]>23){FeedTime1[0]=0;}//时越界判断
		    if(FeedTime1[1]>59){FeedTime1[1]=0;}//分越界判断
	  	    if(FeedTime1[2]>59){FeedTime1[2]=0;}//秒越界判断		
		}
		else if(TimeSetSelect>9&&TimeSetSelect<13)
		{
			FeedTimeSelect2=TimeSetSelect-10;
			FeedTime2[FeedTimeSelect2]++;
			if(FeedTime2[0]>23){FeedTime2[0]=0;}//时越界判断
		    if(FeedTime2[1]>59){FeedTime2[1]=0;}//分越界判断
	  	    if(FeedTime2[2]>59){FeedTime2[2]=0;}//秒越界判断		
		}
		else if(TimeSetSelect>12&&TimeSetSelect<15)
		{
			LedOpenTime=TimeSetSelect-13;
			LedOpenSet[LedOpenTime]++;
			if(LedOpenSet[0]>23){LedOpenSet[0]=0;}//时越界判断
		    if(LedOpenSet[1]>59){LedOpenSet[1]=0;}//分越界判断
//	  	    if(LedOpenSet[2]>59){LedOpenSet[2]=0;}//秒越界判断		
		}
		else if(TimeSetSelect>14&&TimeSetSelect<17)
		{
			LedCloseTime=TimeSetSelect-15;
			LedCloseSet[LedCloseTime]++;
			if(LedCloseSet[0]>23){LedCloseSet[0]=0;}//时越界判断
		    if(LedCloseSet[1]>59){LedCloseSet[1]=0;}//分越界判断
//	  	    if(LedCloseSet[2]>59){LedCloseSet[2]=0;}//秒越界判断		
		}
	}
	if(KeyNum==5)//按键5按下
	{
		if(TimeSetSelect<7)
		{	
			DS1302_Time[TimeSetSelect]--;//时间设置位数值减1
			if(DS1302_Time[0]<0){DS1302_Time[0]=99;}//年越界判断
			if(DS1302_Time[1]<1){DS1302_Time[1]=12;}//月越界判断
			if( DS1302_Time[1]==1 || DS1302_Time[1]==3 || DS1302_Time[1]==5 || DS1302_Time[1]==7 || 
				DS1302_Time[1]==8 || DS1302_Time[1]==10 || DS1302_Time[1]==12)//日越界判断
			{
				if(DS1302_Time[2]<1){DS1302_Time[2]=31;}//大月
				if(DS1302_Time[2]>31){DS1302_Time[2]=1;}
			}
			else if(DS1302_Time[1]==4 || DS1302_Time[1]==6 || DS1302_Time[1]==9 || DS1302_Time[1]==11)
			{
				if(DS1302_Time[2]<1){DS1302_Time[2]=30;}//小月
				if(DS1302_Time[2]>30){DS1302_Time[2]=1;}
			}
			else if(DS1302_Time[1]==2)
			{
				if(DS1302_Time[0]%4==0)
				{
					if(DS1302_Time[2]<1){DS1302_Time[2]=29;}//闰年2月
					if(DS1302_Time[2]>29){DS1302_Time[2]=1;}
				}
				else
				{
					if(DS1302_Time[2]<1){DS1302_Time[2]=28;}//平年2月
					if(DS1302_Time[2]>28){DS1302_Time[2]=1;}
				}
			}
			if(DS1302_Time[3]<0){DS1302_Time[3]=23;}//时越界判断
			if(DS1302_Time[4]<0){DS1302_Time[4]=59;}//分越界判断
			if(DS1302_Time[5]<0){DS1302_Time[5]=59;}//秒越界判断
			if(DS1302_Time[6]<0){DS1302_Time[6]=7;}//星期越界判断
		}
		else if(TimeSetSelect>6&&TimeSetSelect<10)
		{
			FeedTimeSelect1=TimeSetSelect-7;
			FeedTime1[FeedTimeSelect1]--;
			if(FeedTime1[0]<0){FeedTime1[0]=23;}//时越界判断
		    if(FeedTime1[1]<0){FeedTime1[1]=59;}//分越界判断
            if(FeedTime1[2]<0){FeedTime1[2]=59;}//秒越界判断		
		}
		else if(TimeSetSelect>9&&TimeSetSelect<13)
		{
			FeedTimeSelect2=TimeSetSelect-10;
			FeedTime2[FeedTimeSelect2]--;
			if(FeedTime2[0]<0){FeedTime2[0]=23;}//时越界判断
		    if(FeedTime2[1]<0){FeedTime2[1]=59;}//分越界判断
            if(FeedTime2[2]<0){FeedTime2[2]=59;}//秒越界判断		
		}
		else if(TimeSetSelect>12&&TimeSetSelect<15)
		{
			LedOpenTime=TimeSetSelect-13;
			LedOpenSet[LedOpenTime]--;
			if(LedOpenSet[0]<0){LedOpenSet[0]=23;}//时越界判断
		    if(LedOpenSet[1]<0){LedOpenSet[1]=59;}//分越界判断
//	  	    if(LedOpenSet[2]<0){LedOpenSet[2]=59;}//秒越界判断		
		}
		else if(TimeSetSelect>14&&TimeSetSelect<17)
		{
		    LedCloseTime=TimeSetSelect-15;
			LedCloseSet[LedCloseTime]--;
			if(LedCloseSet[0]<0){LedCloseSet[0]=23;}//时越界判断
		    if(LedCloseSet[1]<0){LedCloseSet[1]=59;}//分越界判断
//	  	    if(LedCloseSet[2]<0){LedCloseSet[2]=59;}//秒越界判断
		}
	}
	//更新显示，根据TimeSetSelect和TimeSetFlashFlag判断可完成闪烁功能
	if(TimeSetSelect<7)
	{
		LCD_ShowString(2,10," ReTIME");//LCD显示实时时间ReTIME（Real TIME）
        if(TimeSetSelect==0 && TimeSetFlashFlag==1){LCD_ShowString(1,1,"  ");}//通过设置中断标志位，使标志位TimeSetFlashFlag在0和1之间来回切换，且在1时显示空字符，即可实现选中数据闪烁功能
	    else {LCD_ShowNum(1,1,DS1302_Time[0],2);}
	    if(TimeSetSelect==1 && TimeSetFlashFlag==1){LCD_ShowString(1,4,"  ");}
	    else {LCD_ShowNum(1,4,DS1302_Time[1],2);}
	    if(TimeSetSelect==2 && TimeSetFlashFlag==1){LCD_ShowString(1,7,"  ");}
        else {LCD_ShowNum(1,7,DS1302_Time[2],2);}	
        if(TimeSetSelect==3 && TimeSetFlashFlag==1){LCD_ShowString(2,1,"  ");}
        else {LCD_ShowNum(2,1,DS1302_Time[3],2);}
        if(TimeSetSelect==4 && TimeSetFlashFlag==1){LCD_ShowString(2,4,"  ");}
        else {LCD_ShowNum(2,4,DS1302_Time[4],2);}
        if(TimeSetSelect==5 && TimeSetFlashFlag==1){LCD_ShowString(2,7,"  ");LCD_ShowNum(1,16,DS1302_Time[6],1);}
        else {LCD_ShowNum(2,7,DS1302_Time[5],2);}
        if(TimeSetSelect==6 && TimeSetFlashFlag==1){LCD_ShowString(1,16," ");}
        else {LCD_ShowNum(1,16,DS1302_Time[6],1);}
	}
	else if(TimeSetSelect>6&&TimeSetSelect<10)//时钟设置过后，切换到喂食时间设置，一天喂食两次，因此有两个时间可以设置
	{
        LCD_ShowString(2,10," F1TIME");//LCD切换显示为第一次喂食时间设置F1TIME（Feed1 TIME）
        if(TimeSetSelect==7 && TimeSetFlashFlag==1){LCD_ShowString(2,1,"  ");LCD_ShowNum(1,16,DS1302_Time[6],1);}
        else {LCD_ShowNum(2,1,FeedTime1[0],2);}
        if(TimeSetSelect==8 && TimeSetFlashFlag==1){LCD_ShowString(2,4,"  ");}
        else {LCD_ShowNum(2,4,FeedTime1[1],2);}
        if(TimeSetSelect==9 && TimeSetFlashFlag==1){LCD_ShowString(2,7,"  ");}
        else {LCD_ShowNum(2,7,FeedTime1[2],2);}
		}
	else if(TimeSetSelect>9&&TimeSetSelect<13)
	{
	    LCD_ShowString(2,10," F2TIME");//LCD切换显示为第二次喂食时间设置F2TIME（Feed2 TIME）
        if(TimeSetSelect==10 && TimeSetFlashFlag==1){LCD_ShowString(2,1,"  ");}
        else {LCD_ShowNum(2,1,FeedTime2[0],2);}
        if(TimeSetSelect==11 && TimeSetFlashFlag==1){LCD_ShowString(2,4,"  ");}
        else {LCD_ShowNum(2,4,FeedTime2[1],2);}
        if(TimeSetSelect==12 && TimeSetFlashFlag==1){LCD_ShowString(2,7,"  ");}
        else {LCD_ShowNum(2,7,FeedTime2[2],2);}
	}
	else if(TimeSetSelect>12&&TimeSetSelect<15)	//喂食之后切换到led补光时间设置，开启时间设置
	{
	    LCD_ShowString(2,10," LOTIME");//LCD切换显示为LCTIME（LightLED Open Time）
        if(TimeSetSelect==13 && TimeSetFlashFlag==1){LCD_ShowString(2,1,"  ");}
        else {LCD_ShowNum(2,1,LedOpenSet[0],2);}
        if(TimeSetSelect==14 && TimeSetFlashFlag==1){LCD_ShowString(2,4,"  ");}
        else {LCD_ShowNum(2,4,LedOpenSet[1],2);}
//	    if(TimeSetSelect==14 && TimeSetFlashFlag==1){LCD_ShowString(2,7,"  ");}
        LCD_ShowNum(2,7,LedOpenSet[2],2);
	}
	else if(TimeSetSelect>14&&TimeSetSelect<17)	//喂食之后切换到led补光时间设置，关闭时间设置
	{
	    LCD_ShowString(2,10," LCTIME");//LCD切换显示为LCTIME（LightLED Close Time）
        if(TimeSetSelect==15 && TimeSetFlashFlag==1){LCD_ShowString(2,1,"  ");}
        else {LCD_ShowNum(2,1,LedCloseSet[0],2);}
        if(TimeSetSelect==16 && TimeSetFlashFlag==1){LCD_ShowString(2,4,"  ");}
        else {LCD_ShowNum(2,4,LedCloseSet[1],2);}
//		if(TimeSetSelect==17 && TimeSetFlashFlag==1){LCD_ShowString(2,7,"  ");}
        LCD_ShowNum(2,7,LedCloseSet[2],2);
	}
}

void Feed()//对比实时时间和预设喂食时间，若一样，使标志位FeedFlag置1并启动电机
{
	DS1302_ReadTime1();//调用读取时间函数以用于比较
	if(Real_Time[0]==FeedTime1[0]&&Real_Time[1]==FeedTime1[1]&&Real_Time[2]==FeedTime1[2])//当现实时间等于喂食时间
	{	  
      P2_2=0;	
      P1_3=0;
      FeedFlag=1;
	}
	if(Real_Time[0]==FeedTime2[0]&&Real_Time[1]==FeedTime2[1]&&Real_Time[2]==FeedTime2[2])//当现实时间等于喂食时间
	{
      P2_2=0;	
      P1_3=0;
      FeedFlag=1;  
	}
}

void LedOpen()//对比实时时间和预设补光时间，若一样，使标志位LightFlag置1
{
	unsigned long int real,lightopen1,lightopen2;
	DS1302_ReadTime1();
	real=Real_Time[0]*60+Real_Time[1];//以分为单位，算出区间在多少分之间，
	lightopen1=LedOpenSet[0]*60+LedOpenSet[1];
	lightopen2=LedCloseSet[0]*60+LedCloseSet[1];
	if(real>lightopen1 && real<lightopen2)
	{		
		LightFlag=1;	  
	}
	else
	{
	    LightFlag=0;
	}
	
}

void main()
{
	P1=0xFF;                   //P1管脚初始化
    DS18B20_ConvertT();		//上电先转换一次温度，防止第一次读数据错误	
    LCD_Init();
    ShowTemperature();
	DS1302_Init();
	Timer0Init();
	LCD_ShowString(1,1,"  -  -  ");//静态字符初始化显示
	LCD_ShowString(2,1,"  :  :  ");
	DS1302_SetTime();//设置时间
	    
	while(1)
	{
		Feed();
		LedOpen();
        Check_WaterLevel();
        KeyNum=Key();//读取键码
		if(KeyNum==1)//按键1按下
		{
			if(MODE==0){MODE=1;TimeSetSelect=0;}//功能切换
			else if(MODE==1){MODE=0;DS1302_SetTime();}
		}
		switch(MODE)//根据不同的功能执行不同的函数
		{
			case 0:TimeShow();ShowTemperature();break;
			case 1:TimeSet();break;
		}

	}
}

void Timer0_Routine() interrupt 1
{
	static unsigned int T0Count0,T0Count1,T0Count2;
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	
	T0Count0++;
	if(T0Count0>=500)//每500ms进入一次
	{
		T0Count0=0;
		TimeSetFlashFlag=!TimeSetFlashFlag;//闪烁标志位取反
	}
	
	T0Count1++;
	if(T0Count1>=20)//按键扫描
	{
		T0Count1=0;
		Key_Loop();	//20ms调用一次按键驱动函数	
	}
	
	if(FeedFlag==1)//喂食时长控制
	{
	    T0Count2++;
	    if(T0Count2>=2000)
	    {
		    T0Count2=0;
		    FeedFlag=0;	//2s（200*10ms）后关闭喂食系统
			P2_2=1;
            P1_3=1;
	    }
	}
		
    if(P1_6==1&&LightFlag==1){P2_1=0;}//当光强较低（P1_6为高电平），且时间区间满足，即标志位LightFlag为1时，打开LEd进行补光     
    else{P2_1=1;}	
}
