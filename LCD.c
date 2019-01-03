#include <pic.h>
#include "LCD.h" //Including the header in this source file



void LCD_Busy(void){
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

void LCD_Init(void)
{    
    TRISD = 0x00;
    TRISA = 0x00; 
    ADCON1 = 0x06;
}

void LCD_Command(unsigned char c){
    LCD_rs   = 0;       //Selected command register
    LCD_rw   = 0;       //We are writing to the data register
    LCD_en   = 1;       //Enable H->L
    LCD_data = c;    //DIsplay is turned on, cursor blinking command    
    LCD_en = 0;
    LCD_Busy();         //Waiting for the LCD to process the commands
}

void LCD_SecondLine(void)
{
    LCD_Command(0xc0);
}

void LCD_SendData(unsigned char c){
    LCD_rs   = 1;       //Selected command register
    LCD_rw   = 0;       //We are writing to the data register
    LCD_en   = 1;       //Enable H->L
    LCD_data = c;    //DIsplay is turned on, cursor blinking command
    LCD_en = 0;
    LCD_Busy();     
}

void LCD_SendString(const char *string){
    for (int i = 0; string[i]!='\0'; ++i)           //till the string ends
    {
        LCD_SendData(string[i]);//send characters one by one       
    }
}