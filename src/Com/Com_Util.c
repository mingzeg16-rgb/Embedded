#include "Com_Util.h"
#include<INTRINS.H>

/**
 * @brief STC89C52RC 毫秒软件延时
 * @param ms 延时毫秒 0~65535
 * @note Fosc = 11.0592MHz，Keil C51，内含_nop_()
 */
void Com_Util_Delay1ms(u16 ms)
{
    unsigned char i, j;
    while(ms--)
    {
        _nop_();
        i = 2;
        j = 198;
        do
        {
            while(--j);
        } while(--i);
    }
}