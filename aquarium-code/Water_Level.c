#include <REGX52.H>
#include <intrins.h>
#include "Delay.h"
#define Gap_Value 300

float Pressure;

sbit ADDO = P1^1;
sbit ADSK = P1^2;

unsigned long ReadCount(void)
{
	unsigned long Count;
	unsigned char i;
	ADSK=0; 
	Count=0;
	while(ADDO); //当hx711芯片准备好后，ADDO将拉低，当未准备好时，将输出高电平，此时将让它进入循环，知道ADC准备好，才进行转换
	for (i=0;i<24;i++)//hx711通过一次通信的位数来选择增益大小，次数选择24+1位即25位，使增益为128
	{	
		ADSK=1; 
		Count=Count<<1; //通过移位，存储数据
		ADSK=0;
		if(ADDO) Count++;
	}
	ADSK=1;
//    _nop_();
    Count=Count^0x800000;
	ADSK=0;
	return(Count);
}


void Get_Pressure(void)
{ 
	 Pressure=Pressure/Gap_Value;//模数转换，经过运算，权重值约为300
}

void Check_WaterLevel(void)
{
    Pressure=ReadCount();
    Get_Pressure();
    
    if(Pressure<37400){P2_4=0;P2_3=1;P2_0=1;}//当水位（压力）低于最小水位时，红灯亮
    else if(Pressure>=37400&&Pressure<38200){P2_3=0;P2_4=1;P2_0=1;}//当水位（压力）高于最小水位，低于正常时，黄灯亮	
    else if(Pressure>=38200)
    {
        Pressure=ReadCount();
        Get_Pressure();
        if(Pressure>=38200){P2_0=0;P2_3=1;P2_4=1;}//当水位（压力）高于正常水位时，绿灯亮
    }
}
