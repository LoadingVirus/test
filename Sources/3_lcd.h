///////////////////////////////////////////////////////////////////////
//Lcd2ccd(); 显示赛道，在主程序中循环扫描
//用法：1。初始化   Lcd_init();         2.while(1){Lcd2ccd();}
///////////////////////////////////////////////////////////////////////////////////*/
#define LCD_SCLK  PTM_PTM4		
#define LCD_SDIN  PTM_PTM3		 
#define LCD_DC    PTM_PTM5		
#define LCD_CE    PTM_PTM2		
#define LCD_RST   PTM_PTM1
#define LCD_LED   PTM_PTM0

#define DDR_LCD_SCLK    DDRM_DDRM4
#define DDR_LCD_SDIN    DDRM_DDRM3
#define DDR_LCD_DC      DDRM_DDRM5
#define DDR_LCD_CE      DDRM_DDRM2
#define DDR_LCD_RST     DDRM_DDRM1
#define DDR_LCD_LED     DDRM_DDRM0
		
/*****************************函数声明***********************************/
extern void Lcd_init(void);                                                      //液晶初始化
extern void Lcd_clear(void);                                                     //液晶清屏              
extern void Lcd_write_byte(unsigned char dat, unsigned char command);            //写一个字节
extern void Lcd_set_xy(unsigned char x, unsigned char y);
extern unsigned char numb[10][6] = {
  { 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
  { 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
  { 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
  { 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
  { 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
  { 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
  { 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
  { 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
  { 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
  { 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9  
};

void LCD_Init(void)
{               	                  
  DDR_LCD_SCLK  = 1  ;     //在方向寄存器设置中，"1"代表I/O输出
  DDR_LCD_SDIN  = 1  ;     //在方向寄存器设置中，"1"代表I/O输出
  DDR_LCD_DC    = 1  ;     //在方向寄存器设置中，"1"代表I/O输出
  DDR_LCD_CE    = 1  ;     //在方向寄存器设置中，"1"代表I/O输出
  DDR_LCD_RST   = 1  ;     //在方向寄存器设置中，"1"代表I/O输出
  DDR_LCD_LED   = 1  ;     //在方向寄存器设置中，"1"代表I/O输出
	  
      	 
      	  LCD_LED = 1;             //关闭Nokia5110的背光
      	   
      	  LCD_RST = 0;             // 产生一个让LCD复位的低电平脉冲
         	Delay_ms(13);            //延时13ms
                                  
      	  LCD_RST = 1;
          Delay_us(30);
        
          LCD_CE = 0;		  // 使能LCD
      	  Delay_ms(13);
      	     		
      	  LCD_CE = 1;	  	  // 关闭LCD
          Delay_ms(13);	  
          Lcd_write_byte(0x21, 0);//使用扩展命令设置LCD模式，芯片是活动的，水平寻址 
          Lcd_write_byte(0xba, 0);//设置偏置电压     C8
          Lcd_write_byte(0x06, 0);//温度校正
          Lcd_write_byte(0x13, 0);//1:48
          Lcd_write_byte(0x20, 0);//使用基本命令

      	  Lcd_clear();	          // 清屏      
      	  Delay_us(30);               
      	  Lcd_write_byte(0x0c, 0);// 设定显示模式，普通模式显示  
              
      	  LCD_CE = 0;      	  // 使能LCD 
      	  LCD_LED = 0;            //打开Nokia5110背光       
  }

void Lcd_clear(void) 
{
  unsigned int i;	
  Lcd_write_byte(0x0c, 0);      	        // 设定显示模式，普通模式显示		
  Lcd_write_byte(0x80, 0);      	        // 设置RAM的起始地址为0		
  for ( i = 0; i < 504; i++ ) 
  {
    Lcd_write_byte(0, 1);	            // 写入0
  }			
}
void Lcd_write_byte(unsigned char dat, unsigned char command) 
{
  unsigned char i;
  LCD_CE = 0;	        			     // 使能LCD
      Delay_us(30);  
      
    	if (command == 0)
    	{
    		LCD_DC = 0;        		       // 传送命令
    	}
    	else
    	{	
    		LCD_DC = 1;        		       // 传送数据
    	}	
      Delay_us(30); 
        
    	for(i=0;i<8;i++)
    	{
        		if(dat&0x80)			      // 先传高位		  
        		{
        			LCD_SDIN = 1;
        		}
        		else 
        		{
        			LCD_SDIN = 0;
        		}
        		Delay_us(30);
        			
        		LCD_SCLK = 0;			      // 在上升沿提取数据
        		dat = dat << 1;
        		LCD_SCLK = 1;
    	}	
            LCD_CE = 1;				     // 关闭LCD     
}
 
void Lcd_set_xy(unsigned char x, unsigned char y) 
{
  Lcd_write_byte(0x40 | y, 0);		      // 列
  Lcd_write_byte(0x80 | x, 0);          // 行
}


void Lcd2ccd(void) 
{
  unsigned char i,j,data;
  char k;
  for ( i = 0; i <  COLUMN/2; i++ ) 
  {
    for ( j = 0; j < ROW/8; j++) 
    {              
      Lcd_set_xy(i, j);        //X的取值范围为0-83；Y的取值范围为0-5;
      data = 0x00;
      for ( k = 7; k >= 0; k-- ) 
      {
        if ( Image_Data[j*8+k][i*2] > THRESHOLD ) 
        {
          data = ( data << 1 );
        } else 
        {
          data = ( data << 1 ) | 0x01;
        }
      }   
      Lcd_write_byte(data , 1);
    }                            
  }
}

   
void Lcd2bin(void) 
{
  char k,i,j,data;
  for ( i = 0; i < COLUMN/2; i++) 
  {
    for ( j = 0; j < 4; j++ ) 
    {              
      Lcd_set_xy(i, j);        //X的取值范围为0-83；Y的取值范围为0-5;
      data = 0x00;
      for ( k = 7; k >= 0; k--) 
        {
          if( j*8+k >= ROW-1) 
              {
                data = data << 1;
                continue;
              }
          data = ( data << 1 ); 
         data = data | Image_Bin[j*8+k][i];
           if ( centre[j*8+k] == i ) data = data | 0x01;
           // if ( mid[j*8+k] == i ) data = data | 0x01; 
          //if ( line_side[j*8+k][0] == i ) data = data | 0x01;
          //if ( line_side[j*8+k][1] == i ) data = data | 0x01;          
      }   
         Lcd_write_byte(data, 1);
    }                            
  }
}


void display(int n)
{
  unsigned char aa;
  for ( aa = 0; aa < 6; aa++ )
    {
      Lcd_set_xy(24+aa, 5);
      Lcd_write_byte(numb[n/1000][aa], 1); 
    }
  for ( aa = 0; aa < 6; aa++ )
    {
      Lcd_set_xy(30+aa, 5);
      Lcd_write_byte(numb[n%1000/100][aa], 1); 
    }
  for ( aa = 0; aa < 6; aa++ )
    {       
      Lcd_set_xy(36+aa, 5);
      Lcd_write_byte(numb[n%1000%100/10][aa], 1); 
    }
  for ( aa = 0; aa < 6; aa++ )
    {
      Lcd_set_xy(42+aa, 5);
      Lcd_write_byte(numb[n%10][aa], 1); 
    }  
}  