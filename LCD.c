#include <pic.h>
#include "LCD.h" //Including the header in this source file



void LCD_busy(){
    TRISD    = 0xFF;    //PORTD as i
    LCD_en   = 1;
    LCD_rs   = 0;       //Selected command register
    LCD_rw   = 1;       //This time we are reading
    while(LCD_DB7)      //Read busy flag again and again
    {                   //till it becomes 0
        LCD_en  = 0;    //Enable H->L
        LCD_en  = 1;
    }
    TRISD   = 0x00;     //PORTD as o
}

void LCD_init()
{    
    TRISD = 0x00;
    TRISA = 0x00; 
    ADCON1 = 0x06;
}

void LCD_command(unsigned char c){
    LCD_rs   = 0;       //Selected command register
    LCD_rw   = 0;       //We are writing to the data register
    LCD_en   = 1;       //Enable H->L
    LCD_data = c;    //DIsplay is turned on, cursor blinking command    
    LCD_en = 0;
    LCD_busy();         //Waiting for the LCD to process the commands
}

void LCD_secondline()
{
    LCD_command(0xc0);
}

void LCD_senddata(unsigned char c){
    LCD_rs   = 1;       //Selected command register
    LCD_rw   = 0;       //We are writing to the data register
    LCD_en   = 1;       //Enable H->L
    LCD_data = c;    //DIsplay is turned on, cursor blinking command
    LCD_en = 0;
    LCD_busy();     
}

void LCD_sendstring(const char *string){
    for (int i = 0; string[i]!='\0'; ++i)           //till the string ends
    {
        LCD_senddata(string[i]);//send characters one by one       
    }
}