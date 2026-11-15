#include"Dri_UART.h"
#include"Com_Util.h"
#include"Dri_timer0.h"
#include<STC89C5xRC.H>
#include<STRING.H>
void main()
{
    char str[16];
    Dri_Timer0_Init();//初始化定时器0
    Dri_UART_Init();//初始化串口
    P0 = 0xFF;//初始化LED为关闭状态
    while(1){ 
        if(Dri_UART_ReceiveStr(str)){
            if(strcmp(str, "on") == 0){
                P0 = 0x00;
                Dri_UART_SendStr("LED ON");
            }else if(strcmp(str, "off") == 0){
                P0 = 0xFF;
                Dri_UART_SendStr("LED OFF");
            }else{
                Dri_UART_SendStr("Invalid Command");
            }
        }
    }
}