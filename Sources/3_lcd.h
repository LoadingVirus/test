///////////////////////////////////////////////////////////////////////
//Lcd2ccd(); ��ʾ����������������ѭ��ɨ��
//�÷���1����ʼ��   Lcd_init();         2.while(1){Lcd2ccd();}
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
		
/*****************************��������***********************************/
extern void Lcd_init(void);                                                      //Һ����ʼ��
extern void Lcd_clear(void);                                                     //Һ������              
extern void Lcd_write_byte(unsigned char dat, unsigned char command);            //дһ���ֽ�
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
  DDR_LCD_SCLK  = 1  ;     //�ڷ���Ĵ��������У�"1"����I/O���
  DDR_LCD_SDIN  = 1  ;     //�ڷ���Ĵ��������У�"1"����I/O���
  DDR_LCD_DC    = 1  ;     //�ڷ���Ĵ��������У�"1"����I/O���
  DDR_LCD_CE    = 1  ;     //�ڷ���Ĵ��������У�"1"����I/O���
  DDR_LCD_RST   = 1  ;     //�ڷ���Ĵ��������У�"1"����I/O���
  DDR_LCD_LED   = 1  ;     //�ڷ���Ĵ��������У�"1"����I/O���
	  
      	 
      	  LCD_LED = 1;             //�ر�Nokia5110�ı���
      	   
      	  LCD_RST = 0;             // ����һ����LCD��λ�ĵ͵�ƽ����
         	Delay_ms(13);            //��ʱ13ms
                                  
      	  LCD_RST = 1;
          Delay_us(30);
        
          LCD_CE = 0;		  // ʹ��LCD
      	  Delay_ms(13);
      	     		
      	  LCD_CE = 1;	  	  // �ر�LCD
          Delay_ms(13);	  
          Lcd_write_byte(0x21, 0);//ʹ����չ��������LCDģʽ��оƬ�ǻ�ģ�ˮƽѰַ 
          Lcd_write_byte(0xba, 0);//����ƫ�õ�ѹ     C8
          Lcd_write_byte(0x06, 0);//�¶�У��
          Lcd_write_byte(0x13, 0);//1:48
          Lcd_write_byte(0x20, 0);//ʹ�û�������

      	  Lcd_clear();	          // ����      
      	  Delay_us(30);               
      	  Lcd_write_byte(0x0c, 0);// �趨��ʾģʽ����ͨģʽ��ʾ  
              
      	  LCD_CE = 0;      	  // ʹ��LCD 
      	  LCD_LED = 0;            //��Nokia5110����       
  }

void Lcd_clear(void) 
{
  unsigned int i;	
  Lcd_write_byte(0x0c, 0);      	        // �趨��ʾģʽ����ͨģʽ��ʾ		
  Lcd_write_byte(0x80, 0);      	        // ����RAM����ʼ��ַΪ0		
  for ( i = 0; i < 504; i++ ) 
  {
    Lcd_write_byte(0, 1);	            // д��0
  }			
}
void Lcd_write_byte(unsigned char dat, unsigned char command) 
{
  unsigned char i;
  LCD_CE = 0;	        			     // ʹ��LCD
      Delay_us(30);  
      
    	if (command == 0)
    	{
    		LCD_DC = 0;        		       // ��������
    	}
    	else
    	{	
    		LCD_DC = 1;        		       // ��������
    	}	
      Delay_us(30); 
        
    	for(i=0;i<8;i++)
    	{
        		if(dat&0x80)			      // �ȴ���λ		  
        		{
        			LCD_SDIN = 1;
        		}
        		else 
        		{
        			LCD_SDIN = 0;
        		}
        		Delay_us(30);
        			
        		LCD_SCLK = 0;			      // ����������ȡ����
        		dat = dat << 1;
        		LCD_SCLK = 1;
    	}	
            LCD_CE = 1;				     // �ر�LCD     
}
 
void Lcd_set_xy(unsigned char x, unsigned char y) 
{
  Lcd_write_byte(0x40 | y, 0);		      // ��
  Lcd_write_byte(0x80 | x, 0);          // ��
}


void Lcd2ccd(void) 
{
  unsigned char i,j,data;
  char k;
  for ( i = 0; i <  COLUMN/2; i++ ) 
  {
    for ( j = 0; j < ROW/8; j++) 
    {              
      Lcd_set_xy(i, j);        //X��ȡֵ��ΧΪ0-83��Y��ȡֵ��ΧΪ0-5;
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
      Lcd_set_xy(i, j);        //X��ȡֵ��ΧΪ0-83��Y��ȡֵ��ΧΪ0-5;
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