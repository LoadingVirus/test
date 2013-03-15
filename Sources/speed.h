#ifndef _speed_H
#define _speed_H 
/*************************************************************************
��    ����void PIT_init(void)
��    �ܣ���ʼ��PIT
˵    ����10MS ������ʱ���жϣ������ٶȼ��
*************************************************************************/
void PIT_init(void)
{
    PITCFLMT = 0x00;			//��ֹPITģ��
    PITCE_PCE0 = 1;		  	//ʹ�ܶ�ʱ��ͨ��0
    PITMUX = 0x00;		  	//��ʱ��ͨ��0ʹ��Base Timer0 (΢��ʱ��׼0)
    PITMTLD0 = 0x7F;			//����8λ΢��ʱװ�ؼĴ���0��ֵ��8λ��������ֵ��
    PITLD0 = 0xC34;		   	//����16λװ�ؼĴ���0��ֵ��16λ��������ֵ��
							            //��ʱ����=($PITMTLD0+1)*($PITLD0+1) *(1/fBus) 
							            //��ʱ10 MS ���һ���ٶ�
    PITINTE = 0x01;		  	//ʹ��PIT��ʱ��ͨ��0�ж�
    PITCFLMT = 0x80;			//ʹ��PITģ��
}
/******************************************************************
��    ����void PAC_Init()
��    �ܣ�PAC ��ʼ��
˵    ����PAC ������ã�ֻ���������������ۼ���
********************************************************************/
void PAC_init() 
{
  PACTL = 0X40;  //�����ۼ���A����
  PACNT = 0X00;  //�����ۼ���A������ֵ
}

/*******************************************************
��   ��:  PWM_Init()
��   �ܣ� PWM  ��ʼ��
˵   ����
*********************************************************/
 void PWM_Init(void) 
{
  PWME = 0X00;
  PWMPRCLK = 0X33;    //CLOCK_A,CLOCK_B  80MHZ  ��ƵΪ 10MHZ 
  PWMSCLA = 5;        //CLOCK_SA  1M  
  PWMSCLB = 5;        //CLOCK_SB  1MHZ
  
  PWMCTL_CON67 = 1;   //16λ����  PWM67
  PWMCTL_CON45 = 1;   //16λ����  PWM45
  PWMCTL_CON23 = 1;   //16λ����  PWM23
  
  PWMCLK_PCLK7 = 1;   //PWM7  ʹ��  SA
  PWMCLK_PCLK5 = 1;
  PWMCLK_PCLK3 = 1;
  
  PWMPER67 = 1000;    //��� Ƶ�� 10 KHZ
  PWMPER45 = 5000;    //��� Ƶ�� 200 HZ
  PWMPER23 = 1000;    //��� Ƶ�� 10 KHZ
  
  PWMPOL_PPOL5 = 1;   //������� Ϊ��
  PWMPOL_PPOL7 = 1;   //������� Ϊ��
  PWMPOL_PPOL3 = 1;   //������� Ϊ��
  
  PWMCAE = 0X00;      //�����
  PWME_PWME5 = 1;     //ʹ��PWM45 ���  
 // PWME_PWME7 = 1;     //ʹ��PWM67 ���
}

/*************** ��ת���ٳ��� ***************************
��   ����Motor_back()
��   �ܣ�����ռ�ձ�,����  
˵   ����ռ�ձ�  ==  speed / 1000�������ٶ� 0 <speed<1000 ��
*****************************************************/
void Motor_back(unsigned int speed) 
{
  PWME_PWME7 = 0;   //��ֹ7ͨ��
  PWME_PWME3 = 1;   //ʹ��3ͨ��
  DDRP_DDRP7 = 1;   //�������
  PTP_PTP7 = 0;     //���Ϊ0
  PWMDTY23 = speed;
}
/*************** ��תǰ������ ***************************
��   ����Motor_go()
��   �ܣ�����ռ�ձ�,����  
˵   ����ռ�ձ�  ==  speed / 1000�������ٶ� 0 <speed<1000 ��
*****************************************************/
void Motor_go(unsigned int speed) 
{
  PWME_PWME7 = 1;   //ʹ��7ͨ��
  PWME_PWME3 = 0;   //��ֹ3ͨ��
  DDRP_DDRP3 = 1;   //�������
  PTP_PTP7 = 0;     //���Ϊ0
  PWMDTY67 = speed;
}
/**********************************************************
��  ���MMotor_stop() 
��  ��: �ر�PWM��ͣ��

**********************************************************/
void Motor_stop(void) 
{
 PWME_PWME7 = 0;   //��ֹ7ͨ��
 PWME_PWME3 = 0;   //��ֹ3ͨ��
 DDRP_DDRP7 = 1;   //�������
 DDRP_DDRP3 = 1;   //�������
 PTP_PTP7 = 0;     //���Ϊ0
 PTP_PTP3 = 0;     //���Ϊ0
}
/****************** ������� ************************
��   ��: Direction_control()
��   �� ��1200 
��   �� ��1575
��   �� ��1950
ע   �� : 1200<degree<1950 (������򻵶��)
*****************************************************/ 
void Direction_control(int degree) 
{
   if(degree > 375)
         PWMDTY45 = 1950;  //����
    else if(degree < -375)
         PWMDTY45 = 1200;  //����
       else 
         PWMDTY45 = 1575 + degree; 
    
}

/****************ƽ��ƫ�����********************
��   ����char ave_count()
��   ���� 
���� ֵ��ƽ��ƫ�� average
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

/****************��Ч�еļ���********************
��   ����valid_count()
��   ���� 
����ֵ��char �� 
        ��Ч���� valid_line
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
//PID������ʼ��
//-----------------------------------------
void PID_Init(void)
{
  sptr->LastError = 0;         //Error[-1]
  sptr->PrevError = 0;         //Error[-2]
  sptr->Proportion =P_DATA;    //��������Proportional Const
  sptr->Integral   =I_DATA;    //���ֳ���Integral Const
  sptr->Derivative =D_DATA;    //΢�ֳ���Derivative Const
  sptr->SetPoint =0;           //��ʼ�ٶ�
}

//-----------------------------------------
//����ʽPID�������
//-----------------------------------------
int IncPIDCalc(int NextPoint)
{
   int iError, iIncpid;                  //��ǰ���
    iError = sptr->SetPoint - NextPoint;            //��������
    
    iIncpid =  (int)(sptr->Proportion * iError           //E[k]��
                   - sptr->Integral * sptr->LastError    //E[k��1]��
                   + sptr->Derivative * sptr->PrevError); //E[k��2]��
     sptr->PrevError = sptr->LastError;  //�洢�������´μ���
     sptr->LastError = iError;           //�洢�������´μ���
    return(iIncpid);                    //��������ֵ
}
/***************************************************************
��  �� ��ͣ����� char stop(void);
����ֵ ������ͣ�� ����1
         ������   ����0
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


