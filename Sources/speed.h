#ifndef _speed_H
#define _speed_H 
/*************************************************************************
函    数：void PIT_init(void)
功    能：初始化PIT
说    明：10MS 触发定时器中断，进行速度检测
*************************************************************************/
void PIT_init(void)
{
    PITCFLMT = 0x00;			//禁止PIT模块
    PITCE_PCE0 = 1;		  	//使能定时器通道0
    PITMUX = 0x00;		  	//定时器通道0使用Base Timer0 (微定时基准0)
    PITMTLD0 = 0x7F;			//设置8位微定时装载寄存器0初值（8位计数器初值）
    PITLD0 = 0xC34;		   	//设置16位装载寄存器0初值（16位计数器初值）
							            //定时周期=($PITMTLD0+1)*($PITLD0+1) *(1/fBus) 
							            //定时10 MS 检测一次速度
    PITINTE = 0x01;		  	//使能PIT定时器通道0中断
    PITCFLMT = 0x80;			//使能PIT模块
}
/******************************************************************
函    数：void PAC_Init()
功    能；PAC 初始化
说    明：PAC 相关设置，只是利用其他脉冲累加器
********************************************************************/
void PAC_init() 
{
  PACTL = 0X40;  //脉冲累加器A允许
  PACNT = 0X00;  //脉冲累加器A计数初值
}

/*******************************************************
函   数:  PWM_Init()
功   能： PWM  初始化
说   明：
*********************************************************/
 void PWM_Init(void) 
{
  PWME = 0X00;
  PWMPRCLK = 0X33;    //CLOCK_A,CLOCK_B  80MHZ  分频为 10MHZ 
  PWMSCLA = 5;        //CLOCK_SA  1M  
  PWMSCLB = 5;        //CLOCK_SB  1MHZ
  
  PWMCTL_CON67 = 1;   //16位级联  PWM67
  PWMCTL_CON45 = 1;   //16位级联  PWM45
  PWMCTL_CON23 = 1;   //16位级联  PWM23
  
  PWMCLK_PCLK7 = 1;   //PWM7  使用  SA
  PWMCLK_PCLK5 = 1;
  PWMCLK_PCLK3 = 1;
  
  PWMPER67 = 1000;    //电机 频率 10 KHZ
  PWMPER45 = 5000;    //舵机 频率 200 HZ
  PWMPER23 = 1000;    //电机 频率 10 KHZ
  
  PWMPOL_PPOL5 = 1;   //舵机极性 为正
  PWMPOL_PPOL7 = 1;   //电机极性 为正
  PWMPOL_PPOL3 = 1;   //电机极性 为正
  
  PWMCAE = 0X00;      //左对齐
  PWME_PWME5 = 1;     //使能PWM45 舵机  
 // PWME_PWME7 = 1;     //使能PWM67 电机
}

/*************** 反转减速程序 ***************************
函   数：Motor_back()
功   能：设置占空比,调速  
说   明：占空比  ==  speed / 1000（调节速度 0 <speed<1000 ）
*****************************************************/
void Motor_back(unsigned int speed) 
{
  PWME_PWME7 = 0;   //禁止7通道
  PWME_PWME3 = 1;   //使能3通道
  DDRP_DDRP7 = 1;   //方向输出
  PTP_PTP7 = 0;     //输出为0
  PWMDTY23 = speed;
}
/*************** 正转前进程序 ***************************
函   数：Motor_go()
功   能：设置占空比,调速  
说   明：占空比  ==  speed / 1000（调节速度 0 <speed<1000 ）
*****************************************************/
void Motor_go(unsigned int speed) 
{
  PWME_PWME7 = 1;   //使能7通道
  PWME_PWME3 = 0;   //禁止3通道
  DDRP_DDRP3 = 1;   //方向输出
  PTP_PTP7 = 0;     //输出为0
  PWMDTY67 = speed;
}
/**********************************************************
函  数Motor_stop() 
功  能: 关闭PWM，停车

**********************************************************/
void Motor_stop(void) 
{
 PWME_PWME7 = 0;   //禁止7通道
 PWME_PWME3 = 0;   //禁止3通道
 DDRP_DDRP7 = 1;   //方向输出
 DDRP_DDRP3 = 1;   //方向输出
 PTP_PTP7 = 0;     //输出为0
 PTP_PTP3 = 0;     //输出为0
}
/****************** 舵机函数 ************************
函   数: Direction_control()
左   死 ：1200 
中   间 ：1575
右   死 ：1950
注   意 : 1200<degree<1950 (超出会打坏舵机)
*****************************************************/ 
void Direction_control(int degree) 
{
   if(degree > 375)
         PWMDTY45 = 1950;  //死舵
    else if(degree < -375)
         PWMDTY45 = 1200;  //死舵
       else 
         PWMDTY45 = 1575 + degree; 
    
}

/****************平均偏差计算********************
函   数：char ave_count()
参   数： 
返回 值：平均偏差 average
************************************************/
 int ave_count(void) 
 {
    char i = 0;
    int sum = 0;
    for(i=10;i<26;i++) 
    {
        sum += centre[i];
    }
    return (sum/15); 
 }

/****************有效行的计算********************
函   数：valid_count()
参   数： 
返回值：char 型 
        有效行数 valid_line
************************************************/
char valid_count(void) 
  {
     char r = ROW-1;
    while ( mid[r--] != 0xFF ) 
      {
        if ( r == 0 ) break;    
      }   
     return ROW-r;
  
  }

//-----------------------------------------
//PID参数初始化
//-----------------------------------------
void PID_Init(void)
{
  sptr->LastError = 0;         //Error[-1]
  sptr->PrevError = 0;         //Error[-2]
  sptr->Proportion =P_DATA;    //比例常数Proportional Const
  sptr->Integral   =I_DATA;    //积分常数Integral Const
  sptr->Derivative =D_DATA;    //微分常数Derivative Const
  sptr->SetPoint =0;           //初始速度
}

//-----------------------------------------
//增量式PID控制设计
//-----------------------------------------
int IncPIDCalc(int NextPoint)
{
   int iError, iIncpid;                  //当前误差
    iError = sptr->SetPoint - NextPoint;            //增量计算
    
    iIncpid =  (int)(sptr->Proportion * iError           //E[k]项
                   - sptr->Integral * sptr->LastError    //E[k－1]项
                   + sptr->Derivative * sptr->PrevError); //E[k－2]项
     sptr->PrevError = sptr->LastError;  //存储误差，用于下次计算
     sptr->LastError = iError;           //存储误差，用于下次计算
    return(iIncpid);                    //返回增量值
}
/***************************************************************
函  数 ；停车检测 char stop(void);
返回值 ：满足停车 返回1
         不满足   返回0
*****************************************************************/
char stop_text(void)   
{
  char stop_i = 0,s_flag_24 = 0,s_flag_25 = 0;
  
  for(stop_i = 0;stop_i<60;stop_i++) 
  {
    if(Image_Bin[24][stop_i] != Image_Bin[24][stop_i+1]) 
    {
      s_flag_24++; 
    }
    
    if(Image_Bin[25][stop_i] != Image_Bin[25][stop_i+1]) 
    {
      s_flag_25++; 
    }
    
  }
   if(s_flag_25 > s_flag_24) 
     {
      return s_flag_25; 
     }
   else 
     {
       return s_flag_24;
     }

}
#endif


