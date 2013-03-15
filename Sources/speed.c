#include"speed.h"
//-----------------------------------------------
//-----------------PWM初始化------------------------------
//-------------------------------------------------------------
 void PWM_Init(void) 
{
  PWME = 0X00;
  PWMPRCLK = 0X33;  //CLOCK_A,CLOCK_B  频率 1M 
  
  PWMCTL_CON67 = 1;   //16位级联  PWM67
  PWMCTL_CON45 = 1;   //16位级联  PWM45
  PWMCTL_CON23 = 1;   //16位级联  PWM23
  
  PWMCLK_PCLK7 = 0;   //PWM7  使用  A
  PWMCLK_PCLK5 = 0;
  PWMCLK_PCLK3 = 0;
  
  PWMPER67 = 200;    //电机 频率 5000 HZ
  PWMPER45 = 10000;  //舵机 频率  100 HZ
  PWMPER23 = 200;    //电机 频率 5000 HZ
  
  PWMPOL_PPOL7 = 0;   //电机极性 为负
  PWMPOL_PPOL5 = 1;   //舵机极性 为正
  PWMPOL_PPOL3 = 0;   //电机极性 为负
  
  PWMCAE = 0X00;       //左对齐
  PWME_PWME5 = 1;      //使能PWM45 
}

void Motor_go(unsigned int speed) 
{
  PWME_PWME7 = 1;  //使能7通道
  PWME_PWME3 = 0;  //禁止3通道
  DDRP_DDRP3 = 1;  //方向输出
  PTP_PTP3 = 0;    //输出为0
  PWMDTY67 = speed;
}
