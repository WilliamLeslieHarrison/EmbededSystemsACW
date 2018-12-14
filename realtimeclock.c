#include<pic.h>
#include "realtimeclock.h"


void ds1302_init()
  {
   TRISA = 0x00;                     //a port all output
   TRISD = 0X00;                     //d port all output
   ADCON1 = 0X06;                    //a port all i/o
   sclk = 0;                         //pull low clock
   rst = 0;                         //reset DS1302
   rst = 1;                          //enable DS1302
   time_write_byte(0x8e);             //send control command
   time_write_byte(0);                //enable write DS1302
   rst = 0;                          //reset
  }

//---------------------------------------------
//set time.
void set_time(char *table)
  {
   int i;                             //define the loop counter.
   rst=1;                             //enable DS1302
   time_write_byte(0xbe);                //
   for(i=0;i<8;i++)                   //continue to write 8 bytes.
     {
       time_write_byte(table[i]);        //write one byte
     }
   rst=0;                             //reset
   }

//---------------------------------------------
//get time.
void get_time(char *table)
 {
   int i;                             //set loop counter.
   rst=1;                             //enable DS1302
   time_write_byte(0xbf);                //
   for(i=0;i<7;i++)                   //continue to read 7 bytes.
     {
        table[i]=time_read_byte();      //
     }
    rst=0;                            //reset DS1302
 }

//--------------------------------------------
//write one byte
void time_write_byte(unsigned char time_tx)
 {
    int j;                            //set the loop counter.
    for(j=0;j<8;j++)                  //continue to write 8bit
      {
        i_o=0;                        //
        sclk=0;                       //pull low clk
        if(time_tx&0x01)              //judge the send bit is 0 or 1.
          {
            i_o=1;                    //is 1
          }
        time_tx=time_tx>>1;           //rotate right 1 bit.
        sclk=1;                       //pull high clk
       }
      sclk=0;                         //finished 1 byte,pull low clk
  }

//---------------------------------------------
//read one byte.
unsigned char time_read_byte()
 {
   int j;                            //set the loop counter.  
   TRISB4=1;                         //continue to write 8bit 
   for(j=0;j<8;j++)                  
      {
        sclk=0;                       //pull low clk                   
        time_rx=time_rx>>1;           //judge the send bit is 0 or 1.  
        
        if(i_o)time_rx = time_rx | 0x80;
        
   //     time_rx=time_rx | i_o;                //put the received bit into the reg's highest.
       sclk=1;                       //pull high clk                 
      }                                                              
    TRISB4=0;                        //finished 1 byte,pull low clk  
    sclk=0;                          
    return(time_rx);                 
  }
