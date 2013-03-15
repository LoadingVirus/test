#include <hidef.h> /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
#include <MC9S12XS128.h>
/**********************************************************
                ͣ������������
***********************************************************/
int time = 0;
/**********************************************************
                ���ڲ���������
**********************************************************/
unsigned char g_SCIO_int_RDRF_flag = 0; //�����жϱ�־λ
unsigned char g_REV_DATA = 0;   //��������
/**********************************************************
               �������������
***********************************************************/ 
char error_now = 0;   //��ǰƫ��
char error_last = 0;  //�ϴ�ƫ��
char error_flag = 0;  //�����궨

float Direction_P = 0; 
float Direction_D  = 0;

int Deviation = 0; //������

/**********************************************************
               �������������
***********************************************************/
//---------������ PID�ṹ��-------------------------------
typedef struct PID
{
    int SetPoint;        //�趨Ŀ��Desired Value
    double Proportion;      //��������Proportional Const
    double Integral;        //���ֳ���Integral Const
    double Derivative;      //΢�ֳ���Derivative Const
    int LastError;       //Error[-1]
    int PrevError;       //Error[-2]
} PID;
//----------����PIDʵ��-------------------------------------
static PID sPID;
static PID *sptr = &sPID;

//----------������غ�--------------------------------------
#define MAX_speed 800           //����ٶ�
#define MIN_speed 0             //��С�ٶ�

#define P_DATA 17                  //�ٶ�P����
#define I_DATA 4                   //�ٶ�D����
#define D_DATA 1.2                 //�ٶ�I����

#define HAVE_NEW_VELOCITY 0X01  //��־
int  CurrentVelocity=0;         //�ɼ��ٶȴ洢����
int  PWM_Value = 0;
int  speed = 0;
int  text_speed = 0;

/**********************************************************
               ͼ�������������
***********************************************************/
#define ROW 30       //����
#define COLUMN 120   //����

static unsigned char Image_Data[ROW][COLUMN];
static unsigned char Image_Bin[ROW][COLUMN/2];
static unsigned char mid[ROW];  //�հ�����������
static char centre[ROW];   //���������
static unsigned char line_side[ROW][2];
static char const line_centre[ROW]={4,5,6,7,8,9,11,12,14,15,16,17,18,19,20,20,21,22,23,24,25,25,26,27,28,29,29,30,30,30};
static unsigned char THRESHOLD=0x95;    //95
char g_Flag;     //��־λ
char VSYN_flag = 0;   //���жϱ�־

/**********************************************************
               �����������
***********************************************************/
#include "SCIO.h"
#include "image.h"
#include "speed.h"
#include "isr.h"
#include "1_delay.h"
#include "3_lcd.h"

/***************************************************
** ��������: main
** ��������: ������
** ˵    ����     
****************************************************/  
void main(void)
{ 
  DisableInterrupts;
  DDRA = 0X00;
  DDRB = 0XFF;
  PLL_Init();
  SCI0_Init();
  TIM_Init();                 
  PWM_Init();
  LCD_Init();
  PIT_init();  //��ʱ����ʼ��
  PAC_init();  //�����ۼӳ�ʼ�� 
  PID_Init();  //��� PID ��ʼ�� 
  EnableInterrupts;  
    
  for(;;)
   {  
 /*******************************************************************
                     ��������
 ********************************************************************/
        /*   if(g_REV_DATA == 49) //���յ� '1' ��ʼ,ת��ASCII��
              {
                 sptr->SetPoint = 50;
              }
              if(g_REV_DATA == 48) //���յ� '0' ֹͣ
              {
                sptr->SetPoint = 0;
              } 
           display(g_REV_DATA);  
        */ 
  /*****************************************************************
                   ͼ�������
  *****************************************************************/       
        image2bin();     //��ֵ��
        QueryCentre();   //��ѯ���ĵ�
      //  Lcd2bin();       //��ʾ����

 /****************************************************************
                    �����ı궨
 ****************************************************************/
        error_flag = centre[3]-30;      //�����������ж� ��ֵ6
          
      // display(error_flag+100);
    
/****************************************************************
                 ���    ���    ����
*****************************************************************/       
         error_last = error_now;        //�洢�ϴ�ƫ��
         error_now = centre[7]-30;      //�洢��ǰƫ�� 
        
         if(error_flag>=-7&&error_flag<=7)  //ֱ��
           {
             Direction_P = -10;          //������
             Direction_D = -25;
   
             Deviation = (int)(Direction_P*error_now+Direction_D*(error_now-error_last));  //���PD����
           }
         
         if(error_flag>7||error_flag<-7)   //������ 
            {   
                Direction_P = -23;          //������
                Direction_D = -10;
              Deviation = (int)(Direction_P*error_now+Direction_D*(error_now-error_last));  //���PD����
            }
               
         Direction_control(Deviation); //�������
               
        
            sptr->SetPoint = 120-error_flag*error_flag/2;
       
/**********************************************************
                     ���PID����
***********************************************************/
    if(g_Flag& HAVE_NEW_VELOCITY)   //�ٶȿ���
       {
          PWM_Value=IncPIDCalc(CurrentVelocity);
          speed += PWM_Value;
           
           if(speed>MAX_speed)  speed = MAX_speed;  
           if(speed<MIN_speed)  speed = MIN_speed; 
           
           Motor_go(speed);            
          g_Flag&=~ HAVE_NEW_VELOCITY;
       }
/***********************************************************
                 ���    ͣ��
************************************************************/
/*  if(error_flag>=-6 && error_flag<=6 && time >200)  //ֱ��
   {
      if(stop_text()>4)
      {
        Motor_back(210);
        Delay_s(30);
        Motor_stop(); 
        for(;;);
      }
     // display(stop_text());
   }   */
   
/***********************************************************
       ���ڷ��ٶ�,���ͱ������ٶ�ֵ�������ٶ�PID
************************************************************/
      /*  SCI0_Transmit(0xff); 
          SCI0_Transmit(0xff);  //����֡ͷ������λ��֪���㷢����ʲô����
        
          SCI0_Transmit((char)CurrentVelocity/10);
          SCI0_Transmit(1);
          SCI0_Transmit(2);
          SCI0_Transmit(3);     
          SCI0_Transmit(4);
          Delay_ms(10);   
      */
    //  Send_speed(CurrentVelocity);
    
    //**************** for()  ���ߣ��𳬹�������*************            
   }//end for  
}//end main