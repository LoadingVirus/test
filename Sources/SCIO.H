#ifndef _SCIO_H
#define _SCIO_H  
/***************************************************
��   ��: SCI0_Init()
��   ��: ����1��ʼ������
˵   ��: ������ 9600
****************************************************/
void SCI0_Init(void)
{
  SCI0BD  = 521;     
  SCI0CR1 = 0X00;    /*normal,no parity */
  SCI0CR2 = 0X2C;    /*RIE=1,TE=1,RE=1, */
}
/************************************************** 
��    ��: SCI0_Transmit()
��    ��: ���ڷ�������
��    ��: �� 
��    ��: �� 
˵    ����  
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
    a[0] = (unsigned char)x/100;   //��λ
    a[1] = (unsigned char)x/10%10; //ʮλ
    a[2] = (unsigned char)x%10;    //��λ
    for(i=0;i<3;i++) 
    {
      SCI0_Transmit(a[i]+48);
    }
    
    SCI0_Transmit(10);  //���Ϳո�
}



#endif