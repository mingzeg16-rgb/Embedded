#include "Dri_UART.h"
#include "Dri_timer0.h"
#include "Com_Util.h"
#include <STC89C5xRC.H>


static bit s_is_sending = 0;//0未在发送 1正在发送
static char s_buffer [16];//接收缓冲区
static u8 s_buffer_index = 0;//接收缓冲区索引

static u8 s_idle_count;//接收空闲计数器
static bit s_is_complete = 0;//接收完成标志

void Dri_UART_Timer0Callback()
{
    s_idle_count++;
    if(s_idle_count >= 10 && s_buffer_index > 0)//接收空闲超过10ms且缓冲区有数据
    {
        s_is_complete = 1;
    }
}

void Dri_UART_Init()
{
    //1.串口工作模式
    SM0 = 0;
    SM1 = 1;

    //2.波特率
    //2.1 SMOD
    PCON &= 0x7F;
    //2.2定时器1
    TMOD &= 0x0F;
    TMOD |= 0X20;

    TH1 = 0xFD;
    TL1 = 0xFD;
    
    TR1 = 1;

    //3.串口配置
    REN = 1;
    SM2 = 0;

    //4.启动中断
    EA = 1;
    ES = 1;

    RI = 0;
    TI = 0;

    //5.注册定时器0回调函数
    Dri_Timer0_RegisterCallback(Dri_UART_Timer0Callback);
}

void Dri_UART_SendChar(char c)
{
    while (s_is_sending == 1);
    s_is_sending = 1;
    SBUF = c;
}

void Dri_UART_SendStr(char *str)
{
    while(*str != '\0'){
        Dri_UART_SendChar(*str);
        str++;
    }
}

bit Dri_UART_ReceiveStr(char *str)
{
    if(s_is_complete == 1)//接收完成
    {
        u8 i;
        for(i=0;i<s_buffer_index;i++){
            str[i] = s_buffer[i];
        }
        str[i] = '\0';
        s_buffer_index = 0;
        s_is_complete = 0;
        return 1;
    }
    return 0;
}

void Dri_UART_Handler() interrupt 4
{
    if (RI==1){
        s_buffer[s_buffer_index++] = SBUF;
        s_idle_count = 0;
        RI = 0;
    }
    if (TI==1){
        s_is_sending = 0;
        TI = 0;
    }
} 