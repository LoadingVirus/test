#ifndef _SCIO_H
#define _SCIO_H  
/***************************************************
函   数: SCI0_Init()
功   能: 串口1初始化函数
说   明: 波特率 9600
****************************************************/
void SCI0_Init(void)
{
  SCI0BD  = 521;     
  SCI0CR1 = 0X00;    /*normal,no parity */
  SCI0CR2 = 0X2C;    /*RIE=1,TE=1,RE=1, */
}
/************************************************** 
函    数: SCI0_Transmit()
功    能: 串口发送数据
输    入: 无 
输    出: 无 
说    明：  
***************************************************/
void SCI0_Transmit(unsigned char a)
{
  SCI0DRL = a;         
  while (!(SCI0SR1&0x80));
}


void Send_speed(unsigned int x)
{

    char i = 0;
    unsigned char a[3];
    a[0] = (unsigned char)x/100;   //百位
    a[1] = (unsigned char)x/10%10; //十位
    a[2] = (unsigned char)x%10;    //个位
    for(i=0;i<3;i++) 
    {
      SCI0_Transmit(a[i]+48);
    }
    
    SCI0_Transmit(10);  //发送空格
}



#endif