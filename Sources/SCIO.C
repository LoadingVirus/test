//*******************************************//
//���ں���
    


/***************************************************
** ��������: SCI0_Init
** ��������: ����1��ʼ������
** ˵��: 
****************************************************/
void SCI0_Init(void)
{
SCI0BDL = (byte)((48000000 /* OSC freq *//2) / 57600 /* baud rate */ / 16 /*factor*/);
SCI0CR1 = 0X00;                                      /*normal,no parity*/
SCI0CR2 = 0X0C;                                      /*RIE=1,TE=1,RE=1, */
}


/*void Image_Binaryzation(unsigned int row)  //��ֵ������ 
{ 
  unsigned char *p_Image; 
  unsigned char *q_Image; 
  q_Image=&Buffer[row][0]; 
  
  for(p_Image=&Image_Data[row][0];p_Image<=&Image_Data[row][COLUMN-1];p_Image++)      
    {*(q_Image++)=*p_Image; }  
}*/
/************************************************** 
** ��������: ���ڷ���˳���
** ��������: ����������Ϣ 1Ϊ���� 0Ϊ�װ�
** ��    ��: �� 
** ��    ��: �� 
** ˵����  
***************************************************/
void SCI0_Transmit(void)
{
  byte temp;
  unsigned char i,j;
  temp = SCI0SR1; //����
  SCI0DRH = 0;
 

  for(i = 0;i< ROW-8;i++) 
  {   
     
      SCI0DRL = 0xAA;        //�����б�־
      while (!(SCI0SR1&0x80));   
      
    for(j = 0;j < COLUMN ;j++)      //ǰ50�� ��Ч����
    { 
    
     if(Image_Data[i][j]>THRESHOLD) 
         SCI0DRL = 0x00;
      else 
         SCI0DRL = 0XFF;    //����

      while (!(SCI0SR1&0x80)); 
    }
    
      SCI0DRL = 0X0A;           //�س�
      while (!(SCI0SR1&0x80));
      SCI0DRL = 0X0D;           //����
      while (!(SCI0SR1&0x80));
    
  }
} 