#include <hidef.h> /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
#include <MC9S12XS128.h>
/**********************************************************
                停车参数定义区
***********************************************************/
int time = 0;
/**********************************************************
                串口参数定义区
**********************************************************/
unsigned char g_SCIO_int_RDRF_flag = 0; //串口中断标志位
unsigned char g_REV_DATA = 0;   //接收数据
/**********************************************************
               舵机参数定义区
***********************************************************/ 
char error_now = 0;   //当前偏差
char error_last = 0;  //上次偏差
char error_flag = 0;  //赛道标定

float Direction_P = 0; 
float Direction_D  = 0;

int Deviation = 0; //控制量

/**********************************************************
               电机参数定义区
***********************************************************/
//---------定义电机 PID结构体-------------------------------
typedef struct PID
{
    int SetPoint;        //设定目标Desired Value
    double Proportion;      //比例常数Proportional Const
    double Integral;        //积分常数Integral Const
    double Derivative;      //微分常数Derivative Const
    int LastError;       //Error[-1]
    int PrevError;       //Error[-2]
} PID;
//----------声明PID实体-------------------------------------
static PID sPID;
static PID *sptr = &sPID;

//----------定义相关宏--------------------------------------
#define MAX_speed 800           //最大速度
#define MIN_speed 0             //最小速度

#define P_DATA 17                  //速度P参数
#define I_DATA 4                   //速度D参数
#define D_DATA 1.2                 //速度I参数

#define HAVE_NEW_VELOCITY 0X01  //标志
int  CurrentVelocity=0;         //采集速度存储变量
int  PWM_Value = 0;
int  speed = 0;
int  text_speed = 0;

/**********************************************************
               图像处理参数定义区
***********************************************************/
#define ROW 30       //行数
#define COLUMN 120   //列数

static unsigned char Image_Data[ROW][COLUMN];
static unsigned char Image_Bin[ROW][COLUMN/2];
static unsigned char mid[ROW];  //空白区域中心线
static char centre[ROW];   //拟合中心线
static unsigned char line_side[ROW][2];
static char const line_centre[ROW]={4,5,6,7,8,9,11,12,14,15,16,17,18,19,20,20,21,22,23,24,25,25,26,27,28,29,29,30,30,30};
static unsigned char THRESHOLD=0x95;    //95
char g_Flag;     //标志位
char VSYN_flag = 0;   //场中断标志

/**********************************************************
               函数库包含区
***********************************************************/
#include "SCIO.h"
#include "image.h"
#include "speed.h"
#include "isr.h"
#include "1_delay.h"
#include "3_lcd.h"

/***************************************************
** 函数名称: main
** 功能描述: 主函数
** 说    明：     
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
  PIT_init();  //定时器初始化
  PAC_init();  //脉冲累加初始化 
  PID_Init();  //电机 PID 初始化 
  EnableInterrupts;  
    
  for(;;)
   {  
 /*******************************************************************
                     蓝牙测试
 ********************************************************************/
        /*   if(g_REV_DATA == 49) //接收到 '1' 开始,转成ASCII码
              {
                 sptr->SetPoint = 50;
              }
              if(g_REV_DATA == 48) //接收到 '0' 停止
              {
                sptr->SetPoint = 0;
              } 
           display(g_REV_DATA);  
        */ 
  /*****************************************************************
                   图像处理程序
  *****************************************************************/       
        image2bin();     //二值化
        QueryCentre();   //查询中心点
      //  Lcd2bin();       //显示赛道

 /****************************************************************
                    赛道的标定
 ****************************************************************/
        error_flag = centre[3]-30;      //用于赛道的判断 阀值6
          
      // display(error_flag+100);
    
/****************************************************************
                 电机    舵机    控制
*****************************************************************/       
         error_last = error_now;        //存储上次偏差
         error_now = centre[7]-30;      //存储当前偏差 
        
         if(error_flag>=-7&&error_flag<=7)  //直道
           {
             Direction_P = -10;          //控制量
             Direction_D = -25;
   
             Deviation = (int)(Direction_P*error_now+Direction_D*(error_now-error_last));  //舵机PD控制
           }
         
         if(error_flag>7||error_flag<-7)   //检测弯道 
            {   
                Direction_P = -23;          //控制量
                Direction_D = -10;
              Deviation = (int)(Direction_P*error_now+Direction_D*(error_now-error_last));  //舵机PD控制
            }
               
         Direction_control(Deviation); //舵机控制
               
        
            sptr->SetPoint = 120-error_flag*error_flag/2;
       
/**********************************************************
                     电机PID调节
***********************************************************/
    if(g_Flag& HAVE_NEW_VELOCITY)   //速度控制
       {
          PWM_Value=IncPIDCalc(CurrentVelocity);
          speed += PWM_Value;
           
           if(speed>MAX_speed)  speed = MAX_speed;  
           if(speed<MIN_speed)  speed = MIN_speed; 
           
           Motor_go(speed);            
          g_Flag&=~ HAVE_NEW_VELOCITY;
       }
/***********************************************************
                 检测    停车
************************************************************/
/*  if(error_flag>=-6 && error_flag<=6 && time >200)  //直道
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
       串口发速度,发送编码器速度值，调节速度PID
************************************************************/
      /*  SCI0_Transmit(0xff); 
          SCI0_Transmit(0xff);  //发送帧头，让上位机知道你发的是什么数据
        
          SCI0_Transmit((char)CurrentVelocity/10);
          SCI0_Transmit(1);
          SCI0_Transmit(2);
          SCI0_Transmit(3);     
          SCI0_Transmit(4);
          Delay_ms(10);   
      */
    //  Send_speed(CurrentVelocity);
    
    //**************** for()  底线，勿超过！！！*************            
   }//end for  
}//end main