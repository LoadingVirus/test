void Delay_us(unsigned int n_us)
{
    unsigned int loop_i,loop_j;
    
    for (loop_i=0; loop_i< n_us; loop_i++) 
    {
        for (loop_j=0; loop_j<1;loop_j++) 
        {
            asm(nop);    //����ʱ��Ϊ8MHzʱ���ӳ�1*12=12����������ԼΪ1us
   
        } 
        //end of for (loop_j,80)         
    }
    //end of for (loop_i,n_us)    
}
//end of extern void delay_us 


   
void Delay_ms(unsigned int n_ms)
{
    unsigned int loop_i,loop_j;
    
    for (loop_i=0; loop_i< n_ms; loop_i++) 
    {    
       for (loop_j=0; loop_j<667;loop_j++) 
        {
            ;    //����ʱ��Ϊ8MHzʱ���ӳ�667*12=8000����������ԼΪ1ms
    
        }
    }
    //end of for (loop_i,n_us)    
}
//end of extern void delay_us 

   
void Delay_s(unsigned int n_s)
{
    unsigned int loop_i;
    
    for (loop_i=0; loop_i< n_s; loop_i++) 
    {    
       Delay_ms(1000);          //1s = 1000ms
    }
    //end of for (loop_i,n_s)    
}