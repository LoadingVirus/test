/***************************************************
**                                                **
**                      中断定义区                **
**                                                **
****************************************************/
#ifndef _isr_H
#define _isr_H 
//******************参数声明***********************
unsigned char m=0,n=1,Line_C=0,Line_L,Interval;
/***************************************************
** 函数名称: TIM_Init
** 功能描述: 行场中断初始化函数
** 说明: 
****************************************************/
void TIM_Init(void) {
  TIOS = 0x00;        //定时器通道0，1 为输入捕捉
  TSCR1 = 0x80;        //定时器使能
  TCTL4 = 0x09;        //通道0 捕捉上升沿,通道1 捕捉下降沿
  TIE = 0x03;          //通道0，1 中断使能
  TFLG1 = 0xFF;        //清中断标志位
}
/***************************************************
** 函数名称: PLL_Init
** 功能描述: 时钟初始化函数
** 说明:      80  MHZ
****************************************************/ 
void PLL_Init(void)
{                                  //PLLCLK=2*OSCCLK*(SYNR+1)/(REFDV+1)
  CLKSEL &= 0x7f;                  //CLKSEL的第7位置0，选择系统时钟源为OSCCLK(外部晶振)
  PLLCTL &= 0xbf;                  //禁止锁相环PLL , PLLCTL.6(pllon)设为0;  先关闭PLL
  SYNR = 0xc0 | 0x04;              //根据需要的时钟频率设置SYNR和REFDV寄存器
  REFDV = 0x00;
  PLLCTL |= (1<<6);                //打开PLL,PLLCTL.6(pllon)设为1;开PLL
  while (( CRGFLG&0x08) == 0x00);  //通过判断LOCK位，确定PLL是否稳定
  CLKSEL |= (1<<7);                //(时钟频率稳定后，允许锁相环时钟源作为系统时钟源；
}

//---------------------中断定义---------------------
#pragma CODE_SEG NON_BANKED
/************************************************** 
** 函数名称: 中断处理函数
** 功能描述: 行中断处理函数
** 输    入: 无 
** 输    出: 无 
** 说明：  
***************************************************/ 
interrupt 8 void HREF_Count(void) 
{
  TFLG1_C0F = 1;    //清行中断
  m++;
  if ( m < 8 || m > 264 ) return;               
  if ( ( m  % 8 ) == 0 ) 
  {
    for ( Line_L = 0 ; Line_L < 31 ; Line_L++ ) asm( nop );                 
    for ( Line_L = 0 ; Line_L < COLUMN ; Line_L++ ) Image_Data[Line_C][Line_L]=PORTA;
    Line_C++;
  }
} 
/************************************************** 
函数名称: 中断处理函数
功能描述: 场中断处理函数
输    入: 无 
输    出: 无 
说    明：  
***************************************************/
interrupt 9 void VSYN_Interrupt(void)
{
 /* n = n ^ 0x01;
  if ( n ) {
    TIE = TIE & 0xFE;
    return;
  } else {
    TIE = TIE | 0x03;
  } */  
  TFLG1_C1F = 1;  //清场中断
  TFLG1_C0F = 1;  //清行中断
  Line_C = 0; //行计数器
   
 VSYN_flag++; //场中断标志置1
  
}
/*****************************************************************
函    数：interrupt VectorNumber_Vpit0 void PIT_ISR(void)
功    能：PIT通道0中断服务子程序
说    明：
******************************************************************/
interrupt VectorNumber_Vpit0 void PIT_ISR(void)
{
   
     CurrentVelocity = PACNT;          //读取脉冲累加器当前数值	  
     PACNT = 0x0000;                   //重新装初值
     g_Flag|= HAVE_NEW_VELOCITY;                   
     PITTF_PTF0 = 1;						//清中断标志
    // PORTB=~PORTB;
    time++;     
}
/***************************************************
** 函数名称: SCI0_ISR() 串口中断程序
** 功能描述: 
** 说明: 
****************************************************/
interrupt VectorNumber_Vsci0 void SCI0_ISR(void) 
{
     PORTB =~PORTB;
  if(SCI0SR1_RDRF) 
  {
   g_SCIO_int_RDRF_flag = 1;    //串口中断标志位
   g_REV_DATA = SCI0DRL;        //接收数据
  }
  
   if(SCI0SR1_TC) 
   {
    ; //等待
   }
}
#pragma CODE_SEG DEFAULT

#endif
