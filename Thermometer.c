#include<xc.h>
#include"LCD.h"
#include"Thermometer.h"
            
//Temperature low byte 
unsigned char  TLB=0 ;                                     
//Temperature high byte
unsigned char  THB=0;                                            
//The temperature interger after conversion 
unsigned char TempInt=0;                                 
//The temperature decimal after conversion
unsigned char TempDec=0;                                 
//Temperature binary-coded decimal after conversion
unsigned int TempBCD;                                   
//Ten bit                                                                                         
unsigned char tenbit;                                                
//Int entry bit
unsigned char intent;
//Tenth bit
unsigned char tenthbit;                        
//Hundredth bit
unsigned char hundredthbit;                                                                    
unsigned char table2[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
                              
//-----------------------------------------------------------
//delay function              
void Delay(char x,char y) 
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
void Display(void)
{
    LCD_Busy();    
    //This is the ten decimal digit for the temp
    LCD_SendData(table2[tenbit]); 
    LCD_Busy();
    //This is the one decimal digit for the temp
    LCD_SendData(table2[intent]); 
    LCD_Busy();
    //This adds a decimal point for the temp
    LCD_SendData('.'); 
    LCD_Busy();
    //This is the tenth place digit for the temp
    LCD_SendData(table2[tenthbit]); 
    LCD_Busy();
    //This is the hundredth place digit for the temp
    LCD_SendData(table2[hundredthbit]); 
    LCD_Busy();
    //This is a degree symbol for the temp
    LCD_SendData(0xDF);
    LCD_Busy();
    //This is showing it is done in Celsius
    LCD_SendData('C');
}

//------------------------------------------------
//system initialize function           
void Thermometer_Init(void)
{
  ADCON1=0X07;                                //set A PORT general data PORT   
  TRISA=0X00;                                 //set A PORT direct OUTPUT       
  TRISD=0X00;                                 //set D PORT direct OUTPUT       
}

//-----------------------------------------------
//reset thermometer   
void Reset(void)
{
  char presence=1;
  while(presence)
  { 
     //Pull low
    DQ_LOW() ;  
    //Delay for 503 us
    Delay(2,70); 
    //release general line and wait for resistance pull high general line and keep 15-60us
    DQ_HIGH();                                
    //Delay for 70us
    Delay(2,8);                                                                  
    // did not receive response signal so continue reset 
    if(DQ==1) presence=1;                     
    //have received the response signal
    else presence=0;                          
    //Delay 430us
    Delay(2,60);                                                                                                       
   }
  }

//-----------------------------------------------
//write to the thermometer   
void Write_Byte(uch val)
{
 uch i; 
 uch temp;
 for(i=8;i>0;i--)
 {
   //Shift the lowest bit
   temp=val&0x01;         
   //Pull high to low and produce write time
   DQ_LOW();                                                                           
   NOP();                                                                              
   NOP();                                                                              
   NOP();                                                                              
   NOP();                                                                              
   NOP();      
   //if write is equal to 1 pull high
   if(temp==1)  DQ_HIGH();        
   //Delay for 63 us
   Delay(2,7);                                                             
   DQ_HIGH();                                                                          
   NOP();                                                                              
   NOP();           
   //right shift a bit from val
   val=val>>1;                                //right shift a bit                      
  }
}

//------------------------------------------------
//18b20 read a byte function  
uch Read_Byte(void)
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
   Delay(2,7);                              //63us              
  }
  return(value);
}

//-------------------------------------------------
//start temperature convert function   
void Get_Temp(void)
{ 
int i;
DQ_HIGH();
Reset();                              //reset,wait for  18b20 responsion                                                                                                               
Write_Byte(0XCC);                     //ignore ROM matching                                                                                                                            
Write_Byte(0X44);                     //send  temperature convert command                                                                                                              
/*for(i=20;i>0;i--)                                                                                                                                                                      
    {                                                                                                                                                                                  
                                                                                                                                                                                       
        display();                    //call some display function,insure the time of convert temperature                                                                              
    }           */
Display();
Delay(10,70);
Reset();                              //reset again,wait for 18b20 responsion                                                                                                          
Write_Byte(0XCC);                     //ignore ROM matching                                                                                                                            
Write_Byte(0XBE);                     //send read temperature command                                                                                                                  
TLB = Read_Byte();                      //read temperature low byte                                                                                                                      
THB = Read_Byte();                      //read temperature high byte                                                                                                                     
DQ_HIGH();                            //release general line   +                                                                                                                        
TempInt=(TLB>>4)|(THB<<4)&(0X3f);            //temperature integer                                                                                                                            
TempDec=TLB<<4;                            //temperature decimal                                                                                                                            
if(TempInt>100) TempInt = TempInt/100;                    //not display hundred bit                                                                                                                        
intent=TempInt%10;                     //integer Entries bit                                                                                                                            
tenbit=TempInt/10;                    //integer ten bit                                                                                                                                
TempBCD=0;                                                                                                                                                                                  
if (TempDec & 0x80) TempBCD=TempBCD+5000;
if (TempDec & 0x40) TempBCD=TempBCD+2500;
if (TempDec & 0x20) TempBCD=TempBCD+1250;
if (TempDec & 0x10) TempBCD=TempBCD+625;                //hereinbefore four instructions are turn  decimal into BCD code                         
tenthbit=TempBCD/1000;                          //ten cent bit                                                                           
hundredthbit=(TempBCD%1000)/100;                    //hundred cent bit                                                                                                                                            
NOP();                                                                                                                            
}            
