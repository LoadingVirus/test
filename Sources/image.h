#ifndef _image_H
#define _image_H  
/**********************************************************************/
void image2bin(void) 
{
  unsigned char i,j,last=0; 
   
  for ( j = 0; j < ROW; j++ ) 
  {    
    for ( i = 0; i < COLUMN/2; i++ )
     {
      if ( Image_Data[j][i*2] > THRESHOLD ) 
      {
        Image_Bin[j][i] = 0x00;        //white
      } else {
        Image_Bin[j][i] = 0x01;        //black
      }
    }
  }  
    
  for ( j = 0; j < ROW; j++)      //滤波
  {
    last = Image_Bin[j][0];    
    for ( i = 1; i < COLUMN/2-2; i++) 
    {
      if ( Image_Bin[j][i] != last ) 
      {
        if ( Image_Bin[j][i+1] == last ) Image_Bin[j][i] = last;
        last = Image_Bin[j][i];
      } 
    }
  }
  
  for( i = 0; i < COLUMN/2; i++ )   //滤波
  {
    last = Image_Bin[0][i];
    for ( j = 1; j < ROW-1; j++) 
    {
      if ( Image_Bin[j][i] != last )
      {
        if ( Image_Bin[j+1][i] == last ) Image_Bin[j][i] = last;
        last = Image_Bin[j][i];
      }
    }
  }
// end image2bin
} 
/**********************************************************************/
extern void Delay_s(unsigned int n_s);
#define SIDE_LEFT 0
#define SIDE_RIGHT 59
#define SIDE_CENTRE 29
#define line_left 0
#define line_right 1

extern void Lcd_write_byte(unsigned char dat, unsigned char command);            //写一个字节
extern void Lcd_set_xy(unsigned char x, unsigned char y);
extern unsigned char numb[10][6];

void QueryCentre(void)
{
  unsigned char column_right,column_left;
  char r,c;
  c = SIDE_LEFT;
  
  while ( Image_Bin[ROW-1][c++] == 1 ) 
  {                
    if ( c == SIDE_RIGHT ) 
    {
      c = SIDE_LEFT;
      break;
    }
  } 
  
  column_left = c;
  c = SIDE_RIGHT;
  while ( Image_Bin[ROW-1][c--] == 1 ) 
  {              
    if ( c == SIDE_LEFT ) 
    {
      c = SIDE_RIGHT;
      break; 
    }
  } 
  column_right = c;
  
  
  line_side[ROW-1][line_left] = column_left;    
  line_side[ROW-1][line_right] = column_right;
  centre[ROW-1] = (unsigned char)( column_left + column_right ) / 2;
  
  
  mid[ROW-1] = centre[ROW-1]; 
   
  for ( r = ROW-2; r >= 0; r-- ) 
{ 
    c = mid[r+1];
    while ( Image_Bin[r][--c] == 0 )   //向左扫描边界 
    {       
      if ( c == SIDE_LEFT ) break;
    } 
    
    column_left = c;
    
    c = mid[r+1];    
    while ( Image_Bin[r][++c] == 0 )    //向右扫描边界 
    {       
      if ( c == SIDE_RIGHT ) break;
    } 
    column_right = c;  
      
    line_side[r][line_left] = column_left;    //提取边界线
    line_side[r][line_right] = column_right; 
       
    if ( column_left == SIDE_LEFT && column_right == SIDE_RIGHT )   //计算中心线
    { 
      mid[r] =  mid[r+1]; 
    } else 
    {
      mid[r] = (unsigned char)( column_left + column_right ) / 2;
    }
    c=0;
    if ( column_left == SIDE_LEFT ) c += 1;
    if ( column_right == SIDE_RIGHT ) c += 2;  
          
    switch ( c )    //拟合实际中心线
    {    
      case 1: 
      {    
        centre[r] = mid[r] - (line_centre[r] - column_right/2) - line_centre[r]/3; 
        break;      
      }
      case 2: 
      { 
        centre[r] = mid[r] + (line_centre[r] - (SIDE_RIGHT - column_left)/2) + line_centre[r]/3;
        break;  
      }
      default:
      {
        centre[r] = mid[r];      
      }
    }
  }
  
 //*****************************************************************************  
 
/* 
  c = 0;
  if ( mid[c] == (line_side[c][line_left]+1) && mid[c] == (line_side[c][line_right]-1) ) 
  {
    while ( mid[++c] == mid[c-1] );
    if ( c >= 2 ) 
    {  
      for ( c; c >= 0; c-- ) 
      {
        mid[c] = 0xFF;
        centre[c] =125;
      }
    }
  }
  r = ROW-1;
  
  
  while ( mid[r--] != 0xFF )   //计算无效行
  {
    if ( r == 0 ) break;    
  }
  
  for ( c = 0; c < 6; c++ ) //显示有效行
  {
    Lcd_set_xy(c,5);
    Lcd_write_byte(numb[(ROW-r)/10][c] , 1);
  }
  for ( c = 0; c < 6; c++ ){
    Lcd_set_xy(c+6,5);
    Lcd_write_byte(numb[(ROW-r)%10][c] , 1);
  }
  Delay_s(5);
  
*/  

// end QueryCentre
}

/**********************************************************************/
#endif


