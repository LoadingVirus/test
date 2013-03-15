#include"speed.h"
//-----------------------------------------------
//-----------------PWM��ʼ��------------------------------
//-------------------------------------------------------------
 void PWM_Init(void) 
{
  PWME = 0X00;
  PWMPRCLK = 0X33;  //CLOCK_A,CLOCK_B  Ƶ�� 1M 
  
  PWMCTL_CON67 = 1;   //16λ����  PWM67
  PWMCTL_CON45 = 1;   //16λ����  PWM45
  PWMCTL_CON23 = 1;   //16λ����  PWM23
  
  PWMCLK_PCLK7 = 0;   //PWM7  ʹ��  A
  PWMCLK_PCLK5 = 0;
  PWMCLK_PCLK3 = 0;
  
  PWMPER67 = 200;    //��� Ƶ�� 5000 HZ
  PWMPER45 = 10000;  //��� Ƶ��  100 HZ
  PWMPER23 = 200;    //��� Ƶ�� 5000 HZ
  
  PWMPOL_PPOL7 = 0;   //������� Ϊ��
  PWMPOL_PPOL5 = 1;   //������� Ϊ��
  PWMPOL_PPOL3 = 0;   //������� Ϊ��
  
  PWMCAE = 0X00;       //�����
  PWME_PWME5 = 1;      //ʹ��PWM45 
}

void Motor_go(unsigned int speed) 
{
  PWME_PWME7 = 1;  //ʹ��7ͨ��
  PWME_PWME3 = 0;  //��ֹ3ͨ��
  DDRP_DDRP3 = 1;  //�������
  PTP_PTP3 = 0;    //���Ϊ0
  PWMDTY67 = speed;
}
