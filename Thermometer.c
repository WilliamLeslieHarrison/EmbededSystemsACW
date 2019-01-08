#include<xc.h>
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
//A simple number table to allow for accurate representation of the temp
unsigned char numberTable[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
//A simple array for the temp to be stored
char Temp[5];
                              
//Delay function              
void Delay(char x,char y) 
{
  char z;
  do{
      z=y;
      do{;}while(--z);
     }while(--x);
 }                                                                                  
//System initialize function           
void Thermometer_Init(void)
{
  ADCON1=0X07;                                //set A PORT general data PORT   
  TRISA=0X00;                                 //set A PORT direct OUTPUT       
  TRISD=0X00;                                 //set D PORT direct OUTPUT       
}

//Reset thermometer   
void Reset(void)
{
  //char presence=1;
    for(int i = 0; i < 100; ++i)
  { 
     //Pull low
    DQ_LOW();  
    //Delay for 503 us
    Delay(2,70); 
    //Release general line and wait for resistance pull high general line and keep 15-60us
    DQ_HIGH();                                
    //Delay for 70us
    Delay(2,8);                                                                  
    //We did not receive response signal so continue reset 
    //We have received the response signal
    if(!DQ) break;
    //Delay 430us
    Delay(2,60);                                                                                                       
   }
  }

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
   //If write is equal to 1 pull high
   if(temp==1)  DQ_HIGH();        
   //Delay for 63 us
   Delay(2,7);                                                             
   DQ_HIGH();                                                                          
   NOP();                                                                              
   NOP();           
   //Right shift a bit from val
   val=val>>1;                                                  
  }
}

//18b20 read a byte function  
uch Read_Byte(void)
{
 uch i;
 uch value=0;                                      
 static char j;
 for(i=8;i>0;i--)
 {
   value>>=1; 
   //Pull Low
   DQ_LOW();
   //Delay of 6 us
   NOP();
   NOP();
   NOP();
   NOP();
   NOP();
   NOP();                                        
   //Pull high
   DQ_HIGH();                                     
   //Delay of 4us
   NOP();                                                       
   NOP();                                                       
   NOP();                                                       
   NOP();                                                       
   NOP();                                                 
   j=DQ;                                                        
   if(j) value|=0x80;     
   //Delay of 63 us
   Delay(2,7);                                           
  }
  return(value);
}

//start temperature convert function   
void Get_Temp(char* buffer)
{ 
int i;
DQ_HIGH();
//Reset and wait for thermometer response 
Reset(); 
//Ignore the ROM matching
Write_Byte(0XCC);    
//Send the conversion command
Write_Byte(0X44);                              
//Put each digit of the temperature into a char array
Temp[0] = numberTable[tenbit];
Temp[1] = numberTable[intent];
Temp[2] = '.';
Temp[3] = numberTable[tenthbit];
Temp[4] = numberTable[hundredthbit];
//Loop through the temp array and put it into the parameter buffer array
for(i = 0; i < 5; i++)
{
    buffer[i] = Temp[i];
}
Delay(10,70);
//Reset and wait for thermometer
Reset();                     
//Ignore the ROM matching
Write_Byte(0XCC);        
 //Send the read temperature command 
Write_Byte(0XBE);                      
//Read the low byte/ least significant byte
TLB = Read_Byte();                      
//Read the high byte/ high significant byte
THB = Read_Byte();                     
//release the general line
DQ_HIGH();                           
//Get the temperature integer
TempInt=(TLB>>4)|((THB<<4)&(0X3f));            
//Get the temperature decimal
TempDec=TLB<<4;                            
//Do not display the 100 bit
if(TempInt>100) TempInt = TempInt/100;            
//Integer entries bit
intent=TempInt%10;                   
//Integer ten bit
tenbit=TempInt/10;      
//Get the bcd code
TempBCD=0;        
//Four instructions to turn decimal into BCD code
if (TempDec & 0x80) TempBCD=TempBCD+5000;
if (TempDec & 0x40) TempBCD=TempBCD+2500;
if (TempDec & 0x20) TempBCD=TempBCD+1250;
if (TempDec & 0x10) TempBCD=TempBCD+625;                
//Tenthbit
tenthbit=TempBCD/1000;                                          
//Hundredthbit
hundredthbit=(TempBCD%1000)/100;                                                                                                                                                              
NOP();                                                                                                                            
}           
