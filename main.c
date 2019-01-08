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
int day_of_week;
int isWeekDay;
int is24Hour;
int weekDayStart[2] = {06, 30};
int weekDayEnd[2] = {22, 30};
int weekEndStart[2] = {07, 00};
int weekEndEnd[2] = {23, 00};
int* timeBuffer;
//Buffer for the temperature
char tempBuffer[6] = {'0', '0', '0', '0', '0', '\0'};
//Buffer for the time to be displayed
char timeDisplayer[9] = {'0', '0', ':', '0', '0', '.','0', '0', '\0'};
//int timeBuffer[7] = {0, 0, 0, 0, 0, 0, 0};
char dateDisplayer[9] = {'0', '0', '/', '0', '0', '/', '0', '0', '\0'};
int timeHours, timeMins, timeSecs;
//Last saved temp for checking if it is decreasing
int previousTemp[3] = {0,0,0};
//Base trigger Temp for the device
int triggerTemp[2] = {28, 0};
//The new trigger to be changed by user
int triggerTempChange[2] = {0, 0};
//set the starting date
int start_date[8] = {59,42,14,8,2,2,19,0};

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
    timeSecs = RealTimeClock_get_seconds();
    timeMins = RealTimeClock_get_minutes();
    timeHours = RealTimeClock_get_hours();
    
    timeDisplayer[1] = timeHours % 10 + 48;
    timeHours /= 10;
    timeDisplayer[0] = timeHours % 10 + 48;
    
    timeDisplayer[4] = timeMins % 10 + 48;
    timeMins /= 10;
    timeDisplayer[3] = timeMins % 10 + 48;
    
    timeDisplayer[7] = timeSecs % 10 + 48;
    timeSecs /= 10;
    timeDisplayer[6] = timeSecs % 10 + 48;
    
    LCD_SendString(timeDisplayer);
}

//Displaying the date which will be on another screen
void DisplayDate(void)
{    
    //Takes in the date by integers
    int month = RealTimeClock_get_month();
    int day = RealTimeClock_get_day_of_month();
    int year = RealTimeClock_get_year();
    //Then displays them via converting to a char array to be displayed
    dateDisplayer[1] = month % 10 + 48;
    month /= 10;
    dateDisplayer[0] = month % 10 + 48;
    dateDisplayer[3] = day % 10 + 48;
    day /= 10;
    dateDisplayer[4] = day % 10 + 48;
    dateDisplayer[7] = year % 10 + 48;
    year /= 10;
    dateDisplayer[6] = year % 10 + 48;
}

//This will display the day depending which int the clock gives us
void DisplayDay(void)
{
    //This will show the user which day of the week it is
    day_of_week = RealTimeClock_get_day_of_week();
    char* Monday = ("Monday");
    char* Tuesday = ("Tuesday");
    char* Wednesday = ("Wednesday");
    char* Thursday = ("Thursday");
    char* Friday = ("Friday");
    char* Saturday = ("Saturday");
    char* Sunday = ("Sunday");
    
    switch(day_of_week)
    {
        case 1:
            LCD_SendString(Monday);
            break;
        case 2:
            LCD_SendString(Tuesday);
            break;
        case 3:
            LCD_SendString(Wednesday);
            break;
        case 4:
            LCD_SendString(Thursday);
            break;
        case 5:
            LCD_SendString(Friday);
            break;
        case 6:
            LCD_SendString(Saturday);
            break;
        case 7: 
            LCD_SendString(Sunday);
            break;
    }    
}

//Displaying the temp
void DisplayTemp(char* tempBuffer)
{
    //This gets the temp and sets it to the tempBuffer
    Get_Temp(tempBuffer);
    //This then sends it to the LCD as a string
    LCD_SendString(tempBuffer);
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
    DisplayTime();
    //Jump to second line of the LCD
    LCD_SecondLine();
    //LCD_SendString(dateDisplayer);
    //Send a string to show that this next set of numbers are the temperature
    LCD_SendString(Temp);    
    //Display the current temperature
    DisplayTemp(tempBuffer);    
    //Move the cursor along by one
    //LCD_Command(0x14);
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

//This switches between 24 hour and 12
void Switch24Hour(int Key)
{
    LCD_Command(0x01);
    char c = is24Hour % 10;
    while(1)
    {
        LCD_Command(0x03);
        LCD_SendData(c);
        if (Key == 8) 
        {
            LCD_Command(0x01);
            break;
        }
    }
}

//This shows the date and current day
void DateDayScreen(int Key)
{
    LCD_Command(0x01);
    char* date = ("Date: ");
    char* day = ("Day: ");
    while(1)
    {
        Key = Keypad_Scan();
        LCD_Command(0x03);        
        LCD_SendString(date);
        DisplayDate();
        LCD_SecondLine();
        LCD_SendString(day);
        DisplayDay(); 
        if (Key == 1) 
        {
            ChangeTrigger(Key);
        }
        if (Key == 6) 
        {
            LCD_Command(0x01);
            break;
        }
        if (Key == 7) 
        {
            SwapToHeatingDisplay(Key);
        }   
    }
}

//Setting the time of the clock
void SetTime(int Key)
{
    //Clear lcd
    LCD_Command(0x01);
    //Int to determine the digit we are changing
    int i = 0;
    //Seperate ints to allow for showing of changing time
    int hour, min, sec;
    char* Time = "Time:";
    char* TimeSet = "Time Set";
    //Get the time from the clock
    timeSecs = RealTimeClock_get_seconds();
    timeMins = RealTimeClock_get_minutes();
    timeHours = RealTimeClock_get_hours();
    //Array for new time to be changed
    int setTime[3];    
    setTime[0] = timeHours;
    setTime[1] = timeMins;
    setTime[2] = timeSecs;
    while(1)
    {
        //Allow new time to be displayed like normal clock
        char digits[9] = {'0', '0', ':', '0', '0', '.', '0', '0', '\0'};
        //Send cursor home
        LCD_Command(0x03);        
        LCD_SendString(Time);
        //Scan for button press
        Key = Keypad_Scan();
        //if the init button is pressed return home with new time set
        if(Key == 9)
        {
            //setting new time
            RealTimeClock_set_hours(setTime[0]);
            RealTimeClock_set_minutes(setTime[1]);
            RealTimeClock_set_seconds(setTime[2]);
            //Clear lcd
            LCD_Command(0x01);
            LCD_Command(0x03);
            //Tell user time has been set
            LCD_SendString(TimeSet);            
            Main_Delay(200);
            LCD_Command(0x01);
            break;
        }
        switch(Key)
        {
            //If this button is pressed swap to the next digit
            case 10:
            //if the digit is already the seconds move back to the hours
            if(i == 2)
                i = 0;
            //Otherwise increment
            else
                i++;
            break;
            //This decrements
            case 11:              
                //Do different calculations if we are dealing with the hours
                if(i == 0)
                {   //if the clock is 24 hours allow for change between 0 to 23
                    if(is24Hour == 1)
                    {
                        if(setTime[i] == 0)
                        {
                            setTime[i] = 23;
                        }
                        //Else just decrement it like normal
                        else
                        {
                            setTime[i] = setTime[i] - 1;
                        }
                    }
                    else if(is24Hour == 0)
                    {
                        //Do the 12 hour equilavent
                    } 
                    else
                        setTime[i] = setTime[i] - 1;
                }
                //Other wise if it is minutes or seconds move from 0 to 59
                else if(setTime[i] == 0)
                {
                    setTime[i] = 59;
                }          
                //Else decrement
                else                
                    setTime[i] = setTime[i] - 1;               
                break;
                //This case increments
            case 12:                
                //If hours do different calculations
                if(i == 0)
                {
                    //If 24 and at 23 allow for movement to 0
                    if(is24Hour == 1)
                    {
                        if(setTime[i] == 23)
                        {
                            setTime[i] = 0;
                        }
                        //Else just increment
                        else
                        {
                            setTime[i] = setTime[i] + 1;
                        }
                    }
                    else if(is24Hour == 0)
                    {
                        //Do the 12 hour equilavent
                    }
                }
                //Else move from 59 to 0
                else if(setTime[i] == 59) 
                {
                    setTime[i] = 0;
                }                 
                else
                    setTime[i] = setTime[i] + 1;
                break;                
        }
        //This allows for the user to see what digits and what time is currently
        //being changed to
        hour = setTime[0];
        min = setTime[1];
        sec = setTime[2];
        digits[1] = hour % 10 + 48;
        hour /= 10;
        digits[0] = hour % 10 + 48;
        digits[4] = min % 10 + 48;
        min /= 10;
        digits[3] = min % 10 + 48;
        digits[7] = sec % 10 + 48;
        sec /= 10;
        digits[6] = sec % 10 + 48;
        LCD_SendString(digits);
    }
}

//This checks to see which day it is and where it is a weekend or weekday
void CheckDay(void)
{    
    switch(day_of_week)
    {
        //Since the clock gives us days in numbers this goes through them and 
        //changes depending what day it is
        case 1:
            isWeekDay = 1;
            break;
        case 2:
            isWeekDay = 1;
            break;
        case 3:
            isWeekDay = 1;
            break;
        case 4:
            isWeekDay = 1;
            break;
        case 5:
            isWeekDay = 1;
            break;
        case 6:
            isWeekDay = 0;
            break;
        case 7:
            isWeekDay = 0;
    }
}

//This will allow the user to change the day
void SetDate(int Key)
{
    //This will set the new date as well as the day of the week    
    LCD_Command(0x01);    
    int i = 0;
    char* Day = ("Day:");
    char* Date = ("Date:");
    char* DateSet = ("Date/Day Set");
    char* Monday = ("Mon");
    char* Tuesday = ("Tue");
    char* Wednesday = ("Wed");
    char* Thursday = ("Thu");
    char* Friday = ("Fri");
    char* Saturday = ("Sat");
    char* Sunday = ("Sun");
    int dayInt;    
    dayInt = RealTimeClock_get_day_of_week();
    while(1)
    {
        LCD_Command(0x03);
        LCD_SendString(Date);
        Key = Keypad_Scan();
        if(Key == 13)
        {
            LCD_Command(0x01);
            LCD_Command(0x03);
            //Checks the new day to allow for checking the trigger time
            CheckDay();
            //Changing the global day to the new day
            day_of_week = dayInt;
            //Sets the day to the selected day
            RealTimeClock_set_day_of_week(dayInt);
            //Tell user time has been set            
            LCD_SendString(DateSet);            
            Main_Delay(200);
            LCD_Command(0x01);
            break;
        }
        switch(Key)
        {
            case 14:
                break;
            case 15:
                if(i == 0)
                {
                    if(dayInt == 1)
                        dayInt = 7;
                    else
                        dayInt = dayInt - 1;                    
                }
                break;
            case 16:
                if(i == 0)
                {
                    if(dayInt == 7)
                        dayInt = 1;
                    else
                        dayInt = dayInt + 1;   
                }
                break;
        }
        LCD_SecondLine();
        LCD_SendString(Day);
        switch(dayInt)
        {            
            case 1:
                LCD_SendString(Monday);
                break;
            case 2:
                LCD_SendString(Tuesday);
                break;
            case 3:                
                LCD_SendString(Wednesday);
                break;
            case 4:
                LCD_SendString(Thursday);
                break;
            case 5:
                LCD_SendString(Friday);
                break;
            case 6:
                LCD_SendString(Saturday);
                break;
            case 7:
                LCD_SendString(Sunday);
                break;
        }
    }
}

//This checks the current time against the chosen start and end time
void CheckTime(void)
{
    //If it is a weekday the time must be inbetween 06:30 and 22:30
    //or the heating will not be on
    if (isWeekDay == 1) 
    {
        timeMins = RealTimeClock_get_minutes();
        timeHours = RealTimeClock_get_hours();        
        if (timeHours < weekDayStart[0]) 
        {
            if(timeMins < weekDayStart[1])
                IsHeatingOn = 0;
            else
                IsHeatingOn = 1;
        }
        else if(timeHours > weekDayEnd[0])
        {
            if(timeMins > weekDayEnd[1])
                IsHeatingOn = 0;
            else
                IsHeatingOn = 1;
        }
        else
            IsHeatingOn = 1;
    }
    //Otherwise the time must be inbetween 07:00 and 23:00 or the heating will
    //be off
    else if (isWeekDay == 0) 
    {
        
        timeMins = RealTimeClock_get_minutes();
        timeHours = RealTimeClock_get_hours();
        if (timeHours < weekEndStart[0]) 
        {
            IsHeatingOn = 0;
        }
        else if(timeHours > weekEndEnd[0])
        {
            IsHeatingOn = 0;
        }
        else
            IsHeatingOn = 1;
    }
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
    //RealTimeClock_set_burst_time(start_date);
    /*RealTimeClock_set_seconds(1);
    RealTimeClock_set_day_of_month(2);*/
    Buzzer_Init();
    int Key = 0;
    DisableAlarm = 0;
    isWeekDay = 1;
    int temp, temp2, temp3, tempdec, tempdectenth;
    IsHeatingOn = 1;
    is24Hour = 1;
    //Set Display on without cursor
    LCD_Command(0xc);
    //Clear display
    LCD_Command(0x01);
    //Set cursor to be at the start
    LCD_Command(0x03);   
    //Set LCD to 2 line mode
    LCD_Command(0x38);
    SoundOff();
    //Checks day and time to allow us to set certain conditions
    day_of_week = RealTimeClock_get_day_of_week();
    CheckDay();   
    CheckTime();
    Main_Delay(50);
    while(1)
    {
        day_of_week = RealTimeClock_get_day_of_week();
        //Each time the loop starts LCD will set back to home
        LCD_Command(0x03);
        //To allow for checking the temperature against the trigger I am converting
        //the characters of the temp array to some integers
        temp = tempBuffer[0] - 48;
        temp2 = tempBuffer[1] - 48;
        temp3 = (temp * 10) + temp2;
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
        temp3 = (temp * 10) + temp2;
        //Setting the first decimal place of the temperature to an int for more
        //accurate checking
        tempdec = tempBuffer[3];
        tempdectenth = tempBuffer[4];
        //If the first button (the one for changing the trigger) is pressed
        if (Key == 1) {
            ChangeTrigger(Key);
        }
        //Switch to the date and day screen
        if (Key == 6) {
            DateDayScreen(Key);
        }
        //Switches to the screen that can tell the user if the heating display has been set
        if (Key == 7) {
            SwapToHeatingDisplay(Key);
        }
        //If the 9th button is pressed this will go into set the time function
        if (Key == 9) {
            SetTime(Key);
        }
        //This key will allow the user to change from 24 hour to 12
        if(Key == 8)
        {
            Switch24Hour(Key);
        }
        //This key will allow the user to change the day and date
        if(Key == 13)
        {
            SetDate(Key);
        }
        //If the current temp is equal to the whole number of the trigger temp set heating off
        if (temp3 >= triggerTemp[0]) {
            IsHeatingOn = 0;
            HeatingControlOff(Key, temp, temp2, temp3);
        }
        if (temp3 <= triggerTemp[0] && temp3 < previousTemp[0] && IsHeatingOn == 1 && DisableAlarm == 0) {
            SoundAlarm(Key, temp, temp2, temp3);
        }        
        //If time is outside of the time needed for heating to be on then turn off                    
        CheckTime();
    }
}           
