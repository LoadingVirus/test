
//--------------------------------------------------------------//
//********************图像处理程序******************************//
//--------------------------------------------------------------//
    
#include"image.h"

/***************************************************
** 函数名称: void serach_center(unsigned int row)
** 功能描述: 查找中间点
** 说明: 
****************************************************/
void serach_center(unsigned int row) 
{
   //------------------------二值化
   for(i=0;i<120;i++)
   {
     if(Image_Data[row][i]>THRESHOLD)
        Image_Data[row][i] = 1;
     else
        Image_Data[row][i] = 0;    
   }
   
   //------------直道查询边线 (视野中有两根黑线)----------------------
    while(Image_Data[row][L]+Image_Data[row][L+1]+Image_Data[row][L+2]+Image_Data[row][L+3]+Image_Data[row][L+4]>=3) 
    {
      L++;
    }
    left = L+1;  //左线边缘所在列
    R = left;
    while(Image_Data[row][R]+Image_Data[row][R+1]+Image_Data[row][R+2]+Image_Data[row][R+3]+Image_Data[row][R+4]<=2) 
    {
       R++;
    }
    right = R+2;  //右线边缘所在列
    mid[line++] =(unsigned char)(left + right)/2; 
}  
            
//********************************************************//
//** 函数名：void direction(void)
//** 功  能：方向控制
//**
//******************************************************//      
unsigned char direction(void) 
{
   for(i=0;i<line;i++)
     {
       sum = mid[i]+ sum;
     }
   ave=(unsigned char)sum/line;
   
   for(i=0;i<line;i++) 
     {
       deviation = deviation + mid[i]-ave;   
     }
     if(deviation > 0||deviation == 0) return 1;   //1 --- right
           else                        return 0;   //0 --- left
}