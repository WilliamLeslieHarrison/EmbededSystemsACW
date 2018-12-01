
#include<xc.h>
#include"LCD.h"
#include"Thermometer.h"
                   
unsigned char  TLV=0 ;                      //temperature high byte                     
unsigned char  THV=0;                       //temperature low byte                      
unsigned char TZ=0;                         //temperature integer after convert         
unsigned char TX=0;                         //temperature decimal  after convert        
unsigned int wd;                            //temperature BCD code  after convert       
                                                                                         
unsigned char tenbit;                           //integer ten bit                           
unsigned char intent;                            //integer Entries bit                       
unsigned char tencent;                        //ten cent bit                              
unsigned char hundredcent;                        //hundred cent bit                          
unsigned char thousandcent;                       //thousand cent bit                         
unsigned char countlesscent;                        //myriad cent bit                           
unsigned char table2[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
                              
//-----------------------------------------------------------
//delay function              
void delay(char x,char y) 
{
  char z;
  do{
      z=y;
      do{;}while(--z);
     }while(--x);
 }
//instruction time:7+£¨3*£¨Y-1£©+7£©*£¨X-1£©if add call instruction,set page , seven instruction of  transfer parameter 
                                                                                                                        
//is 14+£¨3*£¨Y-1£©+7£©*£¨X-1£©¡£                                                                                       
                                                                                                                        
//--------------------------------------------------                                                                    
//display function
void display()
{
    LCD_busy();    
    LCD_senddata(table2[tenbit]);
    LCD_busy();
    LCD_senddata(table2[intent]);
    LCD_busy();
    LCD_senddata('.');
    LCD_busy();
    LCD_senddata(table2[tencent]);
    LCD_busy();
    LCD_senddata(table2[hundredcent]);
    LCD_busy();
    LCD_senddata(table2[thousandcent]);
    LCD_busy();
    LCD_senddata(table2[countlesscent]);
    LCD_busy();
    LCD_senddata(0xDF);
    LCD_busy();
    LCD_senddata('C');
}

//------------------------------------------------
//system initialize function           
void ThermometerInit()
{
  ADCON1=0X07;                                //set A PORT general data PORT   
  TRISA=0X00;                                 //set A PORT direct OUTPUT       
  TRISD=0X00;                                 //set D PORT direct OUTPUT       
}

//-----------------------------------------------
//reset DS18B20 function   
reset(void)
{
  char presence=1;
  while(presence)
  { 
    DQ_LOW() ;                                //MAIN MCU PULL LOW                                                                       
    delay(2,70);                              //delay 503us                                                                             
    DQ_HIGH();                                //release general line and wait for resistance pull high general line and keep 15~60us    
    delay(2,8);                               //delay 70us                                                                              
    if(DQ==1) presence=1;                     // not receive responsion signal,continue reset                                           
    else presence=0;                          //receive responsion signal                                                               
    delay(2,60);                              //delay 430us                                                                             
   }
  }

//-----------------------------------------------
//write 18b20 one byte function     
void write_byte(uch val)
{
 uch i;
 uch temp;
 for(i=8;i>0;i--)
 {
   temp=val&0x01;                             //shift the lowest bit                   
   DQ_LOW();                                                                           
   NOP();                                                                              
   NOP();                                                                              
   NOP();                                                                              
   NOP();                                                                              
   NOP();                                     //pull high to low,produce write time    
   if(temp==1)  DQ_HIGH();                    //if write 1,pull high                   
   delay(2,7);                                //delay 63us                             
   DQ_HIGH();                                                                          
   NOP();                                                                              
   NOP();                                                                              
   val=val>>1;                                //right shift a bit                      
  }
}

//------------------------------------------------
//18b20 read a byte function  
uch read_byte(void)
{
 uch i;
 uch value=0;                                 //read temperature         
 static char j;
 for(i=8;i>0;i--)
 {
   value>>=1; 
   DQ_LOW();
   NOP();
   NOP();
   NOP();
   NOP();
   NOP();
   NOP();                                    //6us              
   DQ_HIGH();                                // pull high       
   NOP();                                                       
   NOP();                                                       
   NOP();                                                       
   NOP();                                                       
   NOP();                                   //4us               
   j=DQ;                                                        
   if(j) value|=0x80;                                           
   delay(2,7);                              //63us              
  }
  return(value);
}

//-------------------------------------------------
//start temperature convert function   
void get_temp()
{ 
int i;
DQ_HIGH();
reset();                              //reset,wait for  18b20 responsion                                                                                                               
write_byte(0XCC);                     //ignore ROM matching                                                                                                                            
write_byte(0X44);                     //send  temperature convert command                                                                                                              
/*for(i=20;i>0;i--)                                                                                                                                                                      
    {                                                                                                                                                                                  
                                                                                                                                                                                       
        display();                    //call some display function,insure the time of convert temperature                                                                              
    }           */
display();
delay(10,70);
reset();                              //reset again,wait for 18b20 responsion                                                                                                          
write_byte(0XCC);                     //ignore ROM matching                                                                                                                            
write_byte(0XBE);                     //send read temperature command                                                                                                                  
TLV=read_byte();                      //read temperature low byte                                                                                                                      
THV=read_byte();                      //read temperature high byte                                                                                                                     
DQ_HIGH();                            //release general line   +                                                                                                                        
TZ=(TLV>>4)|(THV<<4)&0X3f;            //temperature integer                                                                                                                            
TX=TLV<<4;                            //temperature decimal                                                                                                                            
if(TZ>100) TZ/100;                    //not display hundred bit                                                                                                                        
intent=TZ%10;                     //integer Entries bit                                                                                                                            
tenbit=TZ/10;                    //integer ten bit                                                                                                                                
wd=0;                                                                                                                                                                                  
if (TX & 0x80) wd=wd+5000;
if (TX & 0x40) wd=wd+2500;
if (TX & 0x20) wd=wd+1250;
if (TX & 0x10) wd=wd+625;                //hereinbefore four instructions are turn  decimal into BCD code                         
tencent=wd/1000;                          //ten cent bit                                                                           
hundredcent=(wd%1000)/100;                    //hundred cent bit                                                                       
thousandcent=(wd%100)/10;                     //thousand cent bit                                                                      
countlesscent=wd%10;                            //myriad cent bit                                                                        
NOP();                                                                                                                            
}            
