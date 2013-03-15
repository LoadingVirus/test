//*******************************************//
//串口函数
    


/***************************************************
** 函数名称: SCI0_Init
** 功能描述: 串口1初始化函数
** 说明: 
****************************************************/
void SCI0_Init(void)
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
     
      SCI0DRL = 0xAA;        //发送行标志
      while (!(SCI0SR1&0x80));   
      
    for(j = 0;j < COLUMN ;j++)      //前50列 无效数据
    { 
    
     if(Image_Data[i][j]>THRESHOLD) 
         SCI0DRL = 0x00;
      else 
         SCI0DRL = 0XFF;    //黑线

      while (!(SCI0SR1&0x80)); 
    }
    
      SCI0DRL = 0X0A;           //回车
      while (!(SCI0SR1&0x80));
      SCI0DRL = 0X0D;           //换行
      while (!(SCI0SR1&0x80));
    
  }
} 
