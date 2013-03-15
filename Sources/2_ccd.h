///*/*////////////////////////////////////////////////////////////////////////////////
///*主程序中写
//
//  DisableInterrupts;
//  DDRA = 0X00;
//  PLL_Init();
//  SCI0_Init();
//  TIM_Init();
//  Lcd_init();
//  EnableInterrupts;
//
//for(；；){
//	/*if(VSYN_C) 
//  {
//     
//    SCI0_Transmit();
//    
//    VSYN_C=0;
//    EnableInterrupts;
//  } */
//
///////////////////////////////////////////////////////////////////////////////////*/ */

void SCI0_Transmit(void);

#define ROW 47
#define COLUMN  120

//unsigned char Buffer[ROW][COLUMN];
unsigned char Image_Data[ROW][COLUMN];

unsigned char Point_C, VSYN_C=0, Line_Flag, m = 0,n=0;
unsigned int Line_C=0;

unsigned char Interval;
unsigned char  THRESHOLD=0x60;


/***************************************************
** 函数名称: PLL_Init
** 功能描述: 时钟初始化函数
** 说明: 
****************************************************/
void PLL_Init(void)
{
  CLKSEL=0x00; //48mhz
  
  SYNR=0XC0 | 0X05;
  REFDV=0XC0 | 0X04;
  
  PLLCTL_PLLON=1;
  POSTDIV=0X00;
  asm(nop);
  asm(nop);
  while(0==CRGFLG_LOCK); //锁相环锁定
  CLKSEL_PLLSEL=1; //选定PLL时钟
}

/***************************************************
** 函数名称: TIM_Init
** 功能描述: 行场中断初始化函数
** 说明: 
****************************************************/
void TIM_Init(void) 
{
TIOS =0x00;        //定时器通道0，1 为输入捕捉
TSCR1=0x80;        //定时器使能
TCTL4=0x09;        //通道0 捕捉上升沿通道1 捕捉下降沿
TIE=0x03;          //通道0，1 中断使能
TFLG1=0xFF;        //清中断标志位
}

/***************************************************
** 函数名称: SCI0_Init
** 功能描述: 串口1初始化函数
** 说明: 
****************************************************/
void SCI0_Init()
{
SCI0BDL = (byte)((48000000 /* OSC freq *//2) / 57600 /* baud rate */ / 16 /*factor*/);
SCI0CR1 = 0X00;                                      /*normal,no parity*/
SCI0CR2 = 0X0C;                                      /*RIE=1,TE=1,RE=1, */
}


/*void Image_Binaryzation(unsigned int row)  //二值化程序 
{ 
  unsigned char *p_Image; 
  unsigned char *q_Image; 
  q_Image=&Buffer[row][0]; 
  
  for(p_Image=&Image_Data[row][0];p_Image<=&Image_Data[row][COLUMN-1];p_Image++)      
    {*(q_Image++)=*p_Image; }  
}*/


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
  TFLG1_C0F = 1;
  Line_Flag = 0;
  m++;
  if ( m<12 || m>288 ) 
  {
    return;  //判断是否从新的一场开始
  } 
  /*if(m<=144)              Interval=12;
  else if(m>144&&m<=200)    Interval=8;
  else if(m>200&&m<=252)    Interval=6;
  else                      Interval=4;  */
  
  Interval=6;          
  if(m%Interval==0)
  {
      
Image_Data[Line_C][0] = PORTA;
Image_Data[Line_C][1] = PORTA;
Image_Data[Line_C][2] = PORTA;
Image_Data[Line_C][3] = PORTA;
Image_Data[Line_C][4] = PORTA;
Image_Data[Line_C][5] = PORTA;
Image_Data[Line_C][6] = PORTA;
Image_Data[Line_C][7] = PORTA;
Image_Data[Line_C][8] = PORTA;
Image_Data[Line_C][9] = PORTA;
Image_Data[Line_C][10] = PORTA;
Image_Data[Line_C][11] = PORTA;
Image_Data[Line_C][12] = PORTA;
Image_Data[Line_C][13] = PORTA;
Image_Data[Line_C][14] = PORTA;
Image_Data[Line_C][15] = PORTA;
Image_Data[Line_C][16] = PORTA;
Image_Data[Line_C][17] = PORTA;
Image_Data[Line_C][18] = PORTA;
Image_Data[Line_C][19] = PORTA;
Image_Data[Line_C][20] = PORTA;
Image_Data[Line_C][21] = PORTA;
Image_Data[Line_C][22] = PORTA;
Image_Data[Line_C][23] = PORTA;
Image_Data[Line_C][24] = PORTA;
Image_Data[Line_C][25] = PORTA;
Image_Data[Line_C][26] = PORTA;
Image_Data[Line_C][27] = PORTA;
Image_Data[Line_C][28] = PORTA;
Image_Data[Line_C][29] = PORTA;
Image_Data[Line_C][30] = PORTA;
Image_Data[Line_C][31] = PORTA;
Image_Data[Line_C][32] = PORTA;
Image_Data[Line_C][33] = PORTA;
Image_Data[Line_C][34] = PORTA;
Image_Data[Line_C][35] = PORTA;
Image_Data[Line_C][36] = PORTA;
Image_Data[Line_C][37] = PORTA;
Image_Data[Line_C][38] = PORTA;
Image_Data[Line_C][39] = PORTA;
Image_Data[Line_C][40] = PORTA;
Image_Data[Line_C][41] = PORTA;
Image_Data[Line_C][42] = PORTA;
Image_Data[Line_C][43] = PORTA;
Image_Data[Line_C][44] = PORTA;
Image_Data[Line_C][45] = PORTA;
Image_Data[Line_C][46] = PORTA;
Image_Data[Line_C][47] = PORTA;
Image_Data[Line_C][48] = PORTA;
Image_Data[Line_C][49] = PORTA;
Image_Data[Line_C][50] = PORTA;
Image_Data[Line_C][51] = PORTA;
Image_Data[Line_C][52] = PORTA;
Image_Data[Line_C][53] = PORTA;
Image_Data[Line_C][54] = PORTA;
Image_Data[Line_C][55] = PORTA;
Image_Data[Line_C][56] = PORTA;
Image_Data[Line_C][57] = PORTA;
Image_Data[Line_C][58] = PORTA;
Image_Data[Line_C][59] = PORTA;
Image_Data[Line_C][60] = PORTA;
Image_Data[Line_C][61] = PORTA;
Image_Data[Line_C][62] = PORTA;
Image_Data[Line_C][63] = PORTA;
Image_Data[Line_C][64] = PORTA;
Image_Data[Line_C][65] = PORTA;
Image_Data[Line_C][66] = PORTA;
Image_Data[Line_C][67] = PORTA;
Image_Data[Line_C][68] = PORTA;
Image_Data[Line_C][69] = PORTA;
Image_Data[Line_C][70] = PORTA;
Image_Data[Line_C][71] = PORTA;
Image_Data[Line_C][72] = PORTA;
Image_Data[Line_C][73] = PORTA;
Image_Data[Line_C][74] = PORTA;
Image_Data[Line_C][75] = PORTA;
Image_Data[Line_C][76] = PORTA;
Image_Data[Line_C][77] = PORTA;
Image_Data[Line_C][78] = PORTA;
Image_Data[Line_C][79] = PORTA;
Image_Data[Line_C][80] = PORTA;
Image_Data[Line_C][81] = PORTA;
Image_Data[Line_C][82] = PORTA;
Image_Data[Line_C][83] = PORTA;
Image_Data[Line_C][84] = PORTA;
Image_Data[Line_C][85] = PORTA;
Image_Data[Line_C][86] = PORTA;
Image_Data[Line_C][87] = PORTA;
Image_Data[Line_C][88] = PORTA;
Image_Data[Line_C][89] = PORTA;
Image_Data[Line_C][90] = PORTA;
Image_Data[Line_C][91] = PORTA;
Image_Data[Line_C][92] = PORTA;
Image_Data[Line_C][93] = PORTA;
Image_Data[Line_C][94] = PORTA;
Image_Data[Line_C][95] = PORTA;
Image_Data[Line_C][96] = PORTA;
Image_Data[Line_C][97] = PORTA;
Image_Data[Line_C][98] = PORTA;
Image_Data[Line_C][99] = PORTA;
Image_Data[Line_C][100] = PORTA;
Image_Data[Line_C][101] = PORTA;
Image_Data[Line_C][102] = PORTA;
Image_Data[Line_C][103] = PORTA;
Image_Data[Line_C][104] = PORTA;
Image_Data[Line_C][105] = PORTA;
Image_Data[Line_C][106] = PORTA;
Image_Data[Line_C][107] = PORTA;
Image_Data[Line_C][108] = PORTA;
Image_Data[Line_C][109] = PORTA;
Image_Data[Line_C][110] = PORTA;
Image_Data[Line_C][111] = PORTA;
Image_Data[Line_C][112] = PORTA;
Image_Data[Line_C][113] = PORTA;
Image_Data[Line_C][114] = PORTA;
Image_Data[Line_C][115] = PORTA;
Image_Data[Line_C][116] = PORTA;
Image_Data[Line_C][117] = PORTA;
Image_Data[Line_C][118] = PORTA;
Image_Data[Line_C][119] = PORTA; 
    
    Line_C++;
  }
  
 // Image_Binaryzation(Line_C);
}

/************************************************** 
** 函数名称: 中断处理函数
** 功能描述: 场中断处理函数
** 输    入: 无 
** 输    出: 无 
** 说明：  
***************************************************/
interrupt 9 void VSYN_Interrupt(void)
{
  TFLG1_C1F = 1; //清场中断
  TFLG1_C0F = 1; //清行中断
  Point_C = 0;
  Line_C = 0; //行计数器
  VSYN_C = ++VSYN_C;
    
  
}

#pragma CODE_SEG DEFAULT

/************************************************** 
** 函数名称: 串口发射端程序
** 功能描述: 发送赛道信息 1为黑线 0为白板
** 输    入: 无 
** 输    出: 无 
** 说明：  
***************************************************/
void SCI0_Transmit(void)
{
  byte temp;
  unsigned char i,j;
  temp = SCI0SR1; //清零
  SCI0DRH = 0;
  
  for(i = 0;i< ROW-8;i++) 
  {
      SCI0DRL = i;           
      while (!(SCI0SR1&0x80)); 
      
    for(j = 0;j < COLUMN ;j=j+2) 
    { 
      //if(Image_Data[i][j]>THRESHOLD && Image_Data[i][j+1]>THRESHOLD && Image_Data[i][j-1]>THRESHOLD)  SCI0DRL = 0x30;
     if(Image_Data[i][j]>THRESHOLD) 
           SCI0DRL = 0x00;    //白
        else 
           SCI0DRL = 0XFF;    //黑线

      while (!(SCI0SR1&0x80)); 
    }
  
      SCI0DRL = 0XAA;           //回车
        while (!(SCI0SR1&0x80));
      SCI0DRL = 0X0D;           //换行
        while (!(SCI0SR1&0x80));
  }
}