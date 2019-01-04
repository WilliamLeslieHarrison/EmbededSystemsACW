#include <pic.h>
#include "LCD.h" //Including the header in this source file



void LCD_Busy(void){
    //PORTD as 1
    TRISD    = 0xFF;    
    LCD_en   = 1;
    //Selected command register
    LCD_rs   = 0;       
    //This time we are reading
    LCD_rw   = 1;       
    //Read busy flag again and again till it becomes 0
    while(LCD_DB7)      
    {                
        //Enable H->L
        LCD_en  = 0;    
        LCD_en  = 1;
    }
    //PORTD as 0
    TRISD   = 0x00;     
}

//Initialise the LCD
void LCD_Init(void)
{    
    TRISD = 0x00;
    TRISA = 0x00; 
    ADCON1 = 0x06;
}

//Send Commands to the LCD
void LCD_Command(unsigned char c){
    //Selected command register
    LCD_rs   = 0;       
    //We are writing to the data register
    LCD_rw   = 0;       
    //Enable H->L
    LCD_en   = 1;       
    //Display is turned on, cursor blinking command   
    LCD_data = c;     
    LCD_en = 0;
    //Waiting for the LCD to process the commands
    LCD_Busy();         
}

//This allows for the cursor to move to the second line
void LCD_SecondLine(void)
{
    LCD_Command(0xc0);
}

//This sends an individual char to be written to the LCD
void LCD_SendData(unsigned char c){
    //Selected command register
    LCD_rs   = 1;      
    //We are writing to the data register
    LCD_rw   = 0;     
    //Enable H->L
    LCD_en   = 1;    
    //DIsplay is turned on, cursor blinking command
    LCD_data = c;    
    LCD_en = 0;
    LCD_Busy();     
}

//This sends a string to the LCD
void LCD_SendString(const char *string){
    //Till the string end character appears
    for (int i = 0; string[i]!='\0'; ++i)           
    {
        //Send characters one by one    
        LCD_SendData(string[i]);   
    }
}