#include <REGX52.H>
#include "OneWire.h"
#include "LCD1602.h"
#include <stdlib.h>

//DS18B20指令
#define DS18B20_SKIP_ROM			0xCC
#define DS18B20_CONVERT_T			0x44
#define DS18B20_READ_SCRATCHPAD 	0xBE


/**
  * @brief  DS18B20开始温度变换
  * @param  无
  * @retval 无
  */
void DS18B20_ConvertT(void) 
{
	OneWire_Init();
	OneWire_SendByte(DS18B20_SKIP_ROM);//跳过ROM等高级操作，直接开始温度转换
	OneWire_SendByte(DS18B20_CONVERT_T);
}

/**
  * @brief  DS18B20读取温度
  * @param  无
  * @retval 温度数值
  */
float DS18B20_ReadT(void)
{
	unsigned char TLSB,TMSB;
	int Temp;
	float Tempreature;
	OneWire_Init();                     //初始化单总线，为通信做准备
	OneWire_SendByte(DS18B20_SKIP_ROM);//跳过ROM，直接读暂存器
	OneWire_SendByte(DS18B20_READ_SCRATCHPAD);//连续的读操作
	TLSB=OneWire_ReceiveByte();     //接收低8位数据
	TMSB=OneWire_ReceiveByte();     //接收高8位数据
	Temp=(TMSB<<8)|TLSB;            //合并数据进行处理
	Tempreature=Temp/16.0;          //由于DS18B20传过来的数据中，低四位是小数部分
                                    //因此除以16以将其转换成正确的数据
	return Tempreature;
}

void ShowTemperature()
{
    
    float temperature;
    static float T;
    DS18B20_ConvertT(); // 转换温度
    temperature = DS18B20_ReadT(); // 读取温度
    if (abs(temperature - DS18B20_ReadT()) < 2)
    {
        T = temperature;
    }
    if (T < 0) // 如果温度小于0
    {
        LCD_ShowChar(2, 12, '-'); // 显示负号
        T = -T; // 将温度变为正数
    }
    else // 如果温度大于等于0
    {
        LCD_ShowChar(2, 12, '+'); // 显示正号
    }
    LCD_ShowNum(2, 13, (unsigned long)T, 2); // 显示温度整数部分
    LCD_ShowChar(2, 15, '.'); // 显示小数点
    LCD_ShowNum(2, 16, (unsigned long)(T * 10) % 10, 1); // 显示温度小数部分

}