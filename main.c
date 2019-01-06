/*
 * File:   MainLCD.c
 * Author: Reece Thompson & Will Harrison
 
 * Created on 29 October 2018, 15:41
 */

#include <xc.h>
#include "LCD.h"
#include "KeyMatrix.h"
#include "Thermometer.h"
#include "realtimeclock.h"
#include "Buzzer.h"

//begin config
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF /* Low-Voltage (Single-Supply) In-Circuit Serial
Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for
programming)*/
//end config

//Two ints serving as bools for checking if heating is on or alarm is 
//disabled
int IsHeatingOn;
int DisableAlarm;
//Buffer for the temperature
char tempBuffer[6] = {'0', '0', '0', '0', '0', '\0'};
//Buffer for the time to be displayed
char timeDisplayer[9] = {'0', '0', ':', '0', '0', '.','0', '0', '\0'};
//int timeBuffer[7] = {0, 0, 0, 0, 0, 0, 0};
int timeHours, timeMins, timeSecs;
//Last saved temp for checking if it is decreasing
int previousTemp[3] = {0,0,0};
//Base trigger Temp for the device
int triggerTemp[2] = {28, 0};
//The new trigger to be changed by user
int triggerTempChange[2] = {0, 0};

//Delay within the main code
void Main_Delay(int k)
{
    int i,j;
    for (i= 0; i < k; i++)
        for(j = 0; j < 100; j++);
}

//Displaying the time
void DisplayTime()
{
    //Sets the returned int array to our main one for displaying
    /*timeBuffer = RealTimeClock_get_burst_time();
    //This get the seconds
    int sec = timeBuffer[0];
    //This gets the minutes
    int min = timeBuffer[1];
    //This gets the hour
    int hour = timeBuffer[2];*/
    timeMins = RealTimeClock_get_minutes();
    timeSecs = RealTimeClock_get_seconds();
    //This gets the last digit and sets it into the end of the char array
    timeDisplayer[6] = timeSecs % 10 + 48;
    timeSecs /= 10;
    timeDisplayer[5] = timeSecs % 10 + 48;
    timeDisplayer[4] = timeMins % 10 + 48;
    timeMins /= 10;
    timeDisplayer[3] = timeMins % 10 + 48;
    LCD_SendData(timeDisplayer[3]);
    LCD_Busy();
    LCD_SendData(timeDisplayer[4]);
    LCD_Busy();
    LCD_SendData(timeDisplayer[5]);
    LCD_Busy();
    LCD_SendData(timeDisplayer[6]);
}

//Displaying the date which will be on another screen
void DisplayDate(int* dateBuffer)
{
    
}

//Displaying the temp
void DisplayTemp(char* tempBuffer)
{
    //This gets the temp and sets it to the tempBuffer
    Get_Temp(tempBuffer);
    //This then sends it to the LCD as a string
    LCD_SendString(tempBuffer);
    //This checks if the LCD is busy
    LCD_Busy();
    //This is the hex value for the degree sign for displaying temperature
    LCD_SendData(0xDF);
    LCD_Busy();
    //As we are doing in celcius we are showing that by having C
    LCD_SendData('C');
}

//The base screen for this device
void MainScreen(void)
{
	//These strings give the user what each digit is
    char* Time = "Time:";
    char* Temp = "Temp:";   
    LCD_SendString(Time);  
	//Move the cursor along by one space
    LCD_Command(0x14);
    DisplayTime();
    //Jump to second line of the LCD
    LCD_SecondLine();
    //Send a string to show that this next set of numbers are the temperature
    LCD_SendString(Temp);    
    //Display the current temperature
    DisplayTemp(tempBuffer);    
    //Move the cursor along by one
    LCD_Command(0x14);
}

//This will allow the user to change the trigger temp
void ChangeTrigger(char Key)
{
    //Clear the entire display
    LCD_Command(0x01);
    //Int for allowing to see what digit you want to change
    int i = 0;    
    char* Trigger = "Trigger:";
    char* TriggerSet = "Trigger Set";
    while(1)
        {   
            //To display decimal place
            char decimal;
            //To display the whole number
            char digits[3];         
            //Set to home
            LCD_Command(0x03);
            //Allowing to user to see what the new trigger will be
            LCD_SendString(Trigger);
            //Check to see what key has been pressed
            Key = Keypad_Scan();
            //If changing triggerTemp button is repressed exit this function
            if(Key == 1)
            {
                LCD_Command(0x01);
                LCD_Command(0x03);
                LCD_SendString(TriggerSet);
                Main_Delay(200);
                LCD_Command(0x01);
                break;
            }
            //Switch with all keys
            switch(Key)
            {
                //If the second button is pressed you can swap between decimal or whole
                case 2:
                    if(i == 0)
                        i = 1;
                    else
                        i = 0;                    
                    break;
                case 3:
                    //Decrement selected number
                    if(i == 1 && triggerTemp[1] == 0)
                    {
                        triggerTemp[1] = 9;
                        triggerTemp[0] = triggerTemp[0] - 1;
                    }
                    else
                        triggerTemp[i] = triggerTemp[i] - 1;
                    break;                   
                case 4:
                    //Increment selected number
                    if(i == 1 && triggerTemp[1] == 9)
                    {
                        triggerTemp[1] = 0;
                        triggerTemp[0] = triggerTemp[0] + 1;
                    }
                    else
                        triggerTemp[i] = triggerTemp[i] + 1;
                    break;
            } 
            //Set the temp to the whole number
            int i = triggerTemp[0];    
            //Put each digit of the whole number into a char array for printing
            digits[1] = i % 10 + 48;
            i /= 10;
            digits[0] = i % 10 + 48;                 
            //Add the ending  character to the array
            digits[2] = '\0';
            //Set char into the the decimal number
            decimal = triggerTemp[1] + 48;
            //Send the string to the LCD
            LCD_SendString(digits);  
            LCD_Busy();
            //Send a decimal point
            LCD_SendData('.');
            LCD_Busy();
            //Send the triggerTemp decimal to the LCD
            LCD_SendData(decimal);
            LCD_Busy();
            //Send a ending 0 to the LCD for the triggerTemp temp
            LCD_SendData('0');
        }
}

//Swaps to the heating display
void SwapToHeatingDisplay(int Key)
{
    char* Heat = "Heating:";
	//Clears the current screen
    LCD_Command(0x01);
    char* On = "On";
    char* Off = "Off";
    while(1)
    {
		//Scanning the keymatrix for a button to be pressed
        Key = Keypad_Scan();
        if(Key == 7)
        {
            LCD_Command(0x01);
            break;
        }
        LCD_Command(0x03);
        LCD_SendString(Heat);
		//These check if the heating is set to on or not and displays on or off
        if(IsHeatingOn == 1)
        {
            LCD_Command(0x14);
            LCD_SendString(On);
        }
        else if(IsHeatingOn == 0)
        {
            LCD_Command(0x14);
            LCD_SendString(Off);
        }
    }
}

//This is for the heating control to be off
void HeatingControlOff(int Key, int temp, int temp2, int temp3)
{
	//This disables any alarm that might be happening
    DisableAlarm = 1;
    SoundOff();
    char* Off = "Heating Off";
    char* On = "Heating On";
    //Clear LCD    
    LCD_Command(0x01);
    //Set cursor to home
    LCD_Command(0x03);
    //Tell the user the heating is off as the temperature is at the current trigger
    LCD_SendString(Off); 
    //Have a delay for the user to see that the heating is now off
    Main_Delay(200);
    //Clear the screen
    LCD_Command(0x01);
    while(1)
    {            
        LCD_Command(0x03);
        //Carry on showing the mainscreen to see that temp is still rising
        MainScreen();
        Key = Keypad_Scan();
        //If the first button is pressed go to the changetrigger screen
        if(Key == 1)
        {
           ChangeTrigger(Key);
        }
        //check if the heating is on
        if(Key == 7)
        {
            SwapToHeatingDisplay(Key);
        }
        //Keep checking the temp
        Get_Temp(tempBuffer);
        //Convert it back into an int
        temp = tempBuffer[0] - 48;
        temp2 = tempBuffer[1] - 48;    
        temp3 = (temp* 10) + temp2; 
        //If the current temp drops below the trigger temp clear the LCD
        if(triggerTemp[0] > temp3)
        {
            LCD_Command(0x01);
            LCD_Command(0x03);
            LCD_SendString(On);
            //A quick delay to say the heating is back on as it is under 
            //the trigger temp
            Main_Delay(200);
            LCD_Command(0x01);
            //And put heating back on
            IsHeatingOn = 1;
            //Alarms are now reenabled
            DisableAlarm = 0;
            break;
        }
    }
}

//This sounds an alarm for when the temp keeps dropping
void SoundAlarm(int Key, int temp, int temp2, int temp3)
{
    //Shows the user heating is not working
    char* Alarm = "Heating Failure";
    DisableAlarm = 0;
    LCD_Command(0x01);
    while(1)
    {
        LCD_Command(0x03);
        LCD_SendString(Alarm);
        //This alerts the user by the use of the buzzer
        SoundOn();
        //This continues to get the temp
        Get_Temp(tempBuffer);
        temp = tempBuffer[0] - 48;
        temp2 = tempBuffer[1] - 48;    
        temp3 = (temp* 10) + temp2; 
        Key = Keypad_Scan();
        //Allow the user to check or change the trigger
        if(Key == 1)
        {
            ChangeTrigger(Key);
        }
        //See if heating is on
        if(Key == 7)
        {
            SwapToHeatingDisplay(Key);
        }
        //Disable the alarm
        if(Key == 5)
        {                
            DisableAlarm = 1;
            SoundOff();
            LCD_Command(0x01);
            break;
        }
        //If temp is over the trigger then clear the alarm
        if(temp3 > triggerTemp[0])
        {
            LCD_Command(0x01);
            HeatingControlOff(Key, temp, temp2, temp3);
            break;
        }
    }
}

//This shows the date and current day
void DateDayScreen(int Key)
{
    //Here will be the code for displaying the date and day by pressing a button
}

//This will allow the user to change the day
void SetDate(int Key)
{
    //This will set the new date as well as the day of the week
}

void get_date_time(char* date_time) {
    unsigned char time[7];
    //RealTimeClock_get_time(time);
    date_time[0] = 48 + (char)((time[0] & 0b01110000) >> 4);
    date_time[1] = 48 + (char)(time[0] & 0b00001111);
    //date_time[1] = ((int)((time[1] & 0b01110000) >> 4) * 10) + (int)(time[1] & 0b00001111);    
}

//If the saved temp is higher than the current temp turn alarm on have button to turn it off
//Weekdays the heating should be on between 6:30 - 10:30 and off otherwise
//Weekends it is between 7:00 - 11:00
//If it is outside of this time don't do anything.
//Have a separate screen for Day and Date
void main() {      
    //Init_Buzzer(&PORTB);
    //TRISB = 0x00;
    LCD_Init();
    Init_Keypad();
    Thermometer_Init();
    RealTimeClock_init();
    Buzzer_Init();
    int Key = 0;
    DisableAlarm = 0;
    int temp, temp2, temp3, tempdec, tempdectenth;
    IsHeatingOn = 1;
    //Set Display on without cursor
    LCD_Command(0xc);
    //Clear display
    LCD_Command(0x01);
    //Set cursor to be at the start
    LCD_Command(0x03);   
    //Set LCD to 2 line mode
    LCD_Command(0x38);
    SoundOff();
    Main_Delay(100);
    while(1)
    {    
    //Each time the loop starts LCD will set back to home
    LCD_Command(0x03);    
    //To allow for checking the temperature against the trigger I am converting
    //the characters of the temp array to some integers
    temp = tempBuffer[0] - 48;
    temp2 = tempBuffer[1] - 48;    
    temp3 = (temp* 10) + temp2;    
    previousTemp[0] = temp3;
    //Setting the first decimal place of the temperature to an int for more
    //accurate checking
    tempdec = tempBuffer[2];
    tempdectenth = tempBuffer[3];
    previousTemp[1] = tempdec;
    previousTemp[2] = tempdectenth;
    //Sets Key to the button that has been pressed or to 0 if none have been pressed
    Key = Keypad_Scan();    
    MainScreen();
    //To allow for checking the temperature against the trigger I am converting
    //the characters of the temp array to some integers
    temp = tempBuffer[0] - 48;
    temp2 = tempBuffer[1] - 48;    
    temp3 = (temp* 10) + temp2;  
    //Setting the first decimal place of the temperature to an int for more
    //accurate checking
    tempdec = tempBuffer[3];
    tempdectenth = tempBuffer[4];
    //If the first button (the one for changing the trigger) is pressed
    if(Key == 1)
    {
        ChangeTrigger(Key);
    }
    //Switch to the date and day screen
    if(Key == 6)
    {
        
    }
    //Switches to the screen that can tell the user if the heating display has been set
    if(Key == 7)
    {
        SwapToHeatingDisplay(Key);
    }
    //If the 9th button is pressed this will go into set the date function
    if(Key == 9)
    {
        
    }
    //If the current temp is equal to the whole number of the trigger temp set heating off
    if(temp3 >= triggerTemp[0])
    {        
        IsHeatingOn = 0;        
        HeatingControlOff(Key, temp, temp2, temp3);
    }
    if(temp3 <= triggerTemp[0] && temp3 < previousTemp[0] && IsHeatingOn == 1 && DisableAlarm == 0)
    {        
       SoundAlarm(Key, temp, temp2, temp3);
    }
    //If time is outside of the time needed for heating to be on then turn off
    }
}           
