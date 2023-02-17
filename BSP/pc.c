/*
   *********************************************************************************************************
   *
   *                                          PC SUPPORT FUNCTIONS for WIN32
   *
   *                          (c) Copyright 2004-2005, Werner.Zimmermann@hs-esslingen.de
   *                                           All Rights Reserved
   *
   * File : PC.C
   * By   : Werner Zimmermann
   *********************************************************************************************************
 */
//#define _CRT_SECURE_NO_WARNINGS

#include "pc.h"
#include <conio.h>
#include <stdio.h>


BOOLEAN lock=FALSE;
HANDLE  hStdOut = NULL;



/*
   *********************************************************************************************************
   *                                        CHECK AND GET KEYBOARD KEY
   *
   * Description: This function checks to see if a key has been pressed at the keyboard and returns TRUE if
   *              so.  Also, if a key is pressed, the key is read and copied where the argument is pointing
   *              to.
   *
   * Arguments  : c     is a pointer to where the read key will be stored.
   *
   * Returns    : TRUE  if a key was pressed
   *              FALSE otherwise
   *********************************************************************************************************
 */
BOOLEAN PC_GetKey(int16_t*c)
{

    if (PC_CHECK_RECURSIVE_CALLS && lock)                       // Check and avoid recursive calls
    {   MessageBox(NULL, (LPCWSTR) "Recursive call in PC_GetKey", (LPCWSTR) "FreeRTOS", MB_OK);
        exit(-1);
    } else if (lock)
    {   return FALSE;
    } else
    {   lock = TRUE;
    }

    if (_kbhit())                                               // See if a key has been pressed
    {   *c = _getch();                                          // Get key pressed
        lock = FALSE;
        return (TRUE);
    } else
    {   *c = 0x00;                                              // No key pressed
        lock = FALSE;
        return (FALSE);
    }
}

/*
   *********************************************************************************************************
   *                                 DISPLAY A STRING  AT 'X' & 'Y' COORDINATE
   *
   * Description : This function writes an ASCII string anywhere on the PC's screen.  This function writes
   *               directly to video RAM instead of using the BIOS for speed reasons.  It assumed that the
   *               video adapter is VGA compatible.  Video RAM starts at absolute address 0x000B8000.  Each
   *               character on the screen is composed of two bytes: the ASCII character to appear on the
   *               screen followed by a video attribute.  An attribute of 0x07 displays the character in
   *               WHITE with a black background.
   *
   * Arguments   : x      corresponds to the desired column on the screen.  Valid columns numbers are from
   *                      0 to 79.  Column 0 corresponds to the leftmost column.
   *               y      corresponds to the desired row on the screen.  Valid row numbers are from 0 to 24.
   *                      Line 0 corresponds to the topmost row.
   *               s      Is the ASCII string to display.  You can also specify a string containing
   *                      characters with numeric values higher than 128.  In this case, special character
   *                      based graphics will be displayed.
   *               color  specifies the foreground/background color to use (see PC.H for available choices)
   *
   * Returns     : None
   *********************************************************************************************************
 */
void PC_DispStr(uint8_t x, uint8_t y, uint8_t* s, uint8_t color)
{   COORD pos;

#ifdef DEBUG_PC
    printf("PC_DispStr: %s\n", s);
    return;
#endif

    if (PC_CHECK_RECURSIVE_CALLS && lock)                       // Check and avoid recursive calls
    {   MessageBox(NULL, (LPCWSTR)"Recursive call in PC_DispStr", (LPCWSTR)"FreeRTOS", MB_OK);
        exit(-1);
    } else if (lock)
    {   return;
    } else
    {   lock = TRUE;
    }

    if (hStdOut==NULL)                                          // Get the handle for the standard output
        hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    if ((x>79) || (y>24))                                       // Check for valid cursor position
        MessageBox(NULL, (LPCWSTR) "Invalid screen position in PC_DispStr()", (LPCWSTR)"FreeRTOS", MB_OK);

    pos.X = x;                                                  // Set cursor position
    pos.Y = y;
    SetConsoleCursorPosition(hStdOut, pos);

    SetConsoleTextAttribute(hStdOut, color);                    // Set text color
    puts((const char *)s);                                                    // Display text string
    lock = FALSE;
}

/*
   *********************************************************************************************************
   *                           DISPLAY A SINGLE CHARACTER AT 'X' & 'Y' COORDINATE
   *
   * Description : This function writes a single character anywhere on the PC's screen.  This function
   *               writes directly to video RAM instead of using the BIOS for speed reasons.  It assumed
   *               that the video adapter is VGA compatible.  Video RAM starts at absolute address
   *               0x000B8000.  Each character on the screen is composed of two bytes: the ASCII character
   *               to appear on the screen followed by a video attribute.  An attribute of 0x07 displays
   *               the character in WHITE with a black background.
   *
   * Arguments   : x      corresponds to the desired column on the screen.  Valid columns numbers are from
   *                      0 to 79.  Column 0 corresponds to the leftmost column.
   *               y      corresponds to the desired row on the screen.  Valid row numbers are from 0 to 24.
   *                      Line 0 corresponds to the topmost row.
   *               c      Is the ASCII character to display.  You can also specify a character with a
   *                      numeric value higher than 128.  In this case, special character based graphics
   *                      will be displayed.
   *               color  specifies the foreground/background color to use (see PC.H for available choices)
   *
   * Returns     : None
   *********************************************************************************************************
*/
void PC_DispChar(uint8_t x, uint8_t y, uint8_t c, uint8_t color)
{   COORD pos;

#ifdef DEBUG_PC
    printf("PC_DispChar: %c\n", c);
    return;
#endif

    if (PC_CHECK_RECURSIVE_CALLS && lock)                       // Check and avoid recursive calls
    {   MessageBox(NULL, (LPCWSTR) "Recursive call in PC_DispChar", (LPCWSTR)"FreeRTOS", MB_OK);
        exit(-1);
    } else if (lock)
    {   return;
    } else
    {   lock = TRUE;
    }

    if (hStdOut==NULL)                                          // Get the handle for the standard output
        hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    if ((x>79) || (y>24))                                       // Check for valid cursor position
        MessageBox(NULL, (LPCWSTR) "Invalid screen position in PC_DispChar()", (LPCWSTR)"FreeRTOS", MB_OK);

    pos.X = x;                                                  // Set cursor position
    pos.Y = y;
    SetConsoleCursorPosition(hStdOut, pos);
    SetConsoleTextAttribute(hStdOut, color);                    // Set text color
    putchar(c);                                                 // Display character
    lock = FALSE;

}


/*
   *********************************************************************************************************
   *                                              CLEAR SCREEN
   *
   * Description : This function clears the PC's screen by directly accessing video RAM instead of using
   *               the BIOS.  It assumed that the video adapter is VGA compatible.  Video RAM starts at
   *               absolute address 0x000B8000.  Each character on the screen is composed of two bytes:
   *               the ASCII character to appear on the screen followed by a video attribute.  An attribute
   *               of 0x07 displays the character in WHITE with a black background.
   *
   * Arguments   : color   specifies the foreground/background color combination to use
   *                       (see PC.H for available choices)
   *
   * Returns     : None
   *********************************************************************************************************
 */
void PC_DispClrScr(uint8_t color)
{   COORD pos;


#ifdef DEBUG_PC
    printf("PC_DispClrScr\n");
    return;
#endif
    if (PC_CHECK_RECURSIVE_CALLS && lock)                       // Check and avoid recursive calls
    {   MessageBox(NULL, (LPCWSTR)"Recursive call in PC_DispClrScr", (LPCWSTR)"FreeRTOS", MB_OK);
        exit(-1);
    } else if (lock)
    {   return;
    } else
    {   lock = TRUE;
    }

    if (hStdOut==NULL)                                          // Get the handle for the standard output
        hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    pos.X = 0;
    pos.Y = 0;
    SetConsoleCursorPosition(hStdOut, pos);                     // Set cursor position to top of window
    SetConsoleTextAttribute(hStdOut, color);                    // Set text color

    system("cls");                                              // Clear the screen

    lock = FALSE;

}

/*
   *********************************************************************************************************
   *                                       ELAPSED TIME INITIALIZATION
   *
   * Description : This function initialize the elapsed time module by determining how long the START and
   *               STOP functions take to execute.  In other words, this function calibrates this module
   *               to account for the processing time of the START and STOP functions.
   *               Needs to be called only once before any of the timers is started with PC_ElapsedStart().
   *
   * Arguments   : None.
   *
   * Returns     : None.
   *********************************************************************************************************
 */
#define NTIMERS 16
static uint16_t PC_ElapsedOverhead=0;
static LARGE_INTEGER PC_startTime[NTIMERS], PC_stopTime[NTIMERS], PC_frequency;

void PC_ElapsedInit(void)
{   static BOOLEAN initDone=FALSE;
    if (initDone)
        return;

    QueryPerformanceFrequency(&PC_frequency);                   // Get the CPU frequency

    PC_ElapsedOverhead = 0;                                     // Measure the overhead of PC_ElapsedStart
    PC_ElapsedStart(0);                                         // ... and PC_ElapsedStop
    PC_ElapsedOverhead = (uint16_t) PC_ElapsedStop(0);
    initDone=TRUE;
}

/*
   *********************************************************************************************************
   *                            START A TIMER FOR EXECUTION TIME MEASUREMENT
   *
   * Description : Trigger the timer to be used to measure the time between events.
   *               Timer will be running when the function returns.
   *               Time measurement needs to be initalized before by calling PC_ElapsedInit, only
   *               needed once for all timers together.
   *
   * Arguments   : n=0...NTIMERS-1 number of timer
   *
   * Returns     : None.
   *********************************************************************************************************
 */
void PC_ElapsedStart(uint8_t n)
{   if (n >= NTIMERS)
        return;
    QueryPerformanceCounter(&PC_startTime[n]);                  // Read the CPU counter and store it
}

/*
   *********************************************************************************************************
   *                             STOP A TIMER FOR EXECUTION TIMER MEASUREMENT
   *
   * Description : This function stops the the timer for execution time measurement and computes the
   *               time in microseconds since the timer was started with PC_ElapsedStart.
   *
   * Arguments   : n=0...NTIMERS-1 number of timer.
   *
   * Returns     : The number of micro-seconds since the timer was last started.
   *
   * Notes       : - The returned time accounts for the processing time of the START and STOP functions.
   *
   *********************************************************************************************************
 */
uint32_t PC_ElapsedStop(uint8_t n)
{   LARGE_INTEGER PC_diffTime;

    if (n >= NTIMERS)
        return 0;
    QueryPerformanceCounter(&PC_stopTime[n]);                   // Read the CPU counter and store it
    PC_diffTime.QuadPart = PC_stopTime[n].QuadPart - PC_startTime[n].QuadPart; //Compute the difference and
                                                                // ... convert it into microseconds
    return (uint32_t) (PC_diffTime.QuadPart * (__int64) 1000000 / PC_frequency.QuadPart - PC_ElapsedOverhead);
}

/*
   *********************************************************************************************************
   *                                       GET THE CURRENT DATE AND TIME
   *
   * Description: This function obtains the current date and time from the PC.
   *
   * Arguments  : s     is a pointer to where the ASCII string of the current date and time will be stored.
   *                    You must allocate at least 21 bytes (includes the NUL) of storage in the return
   *                    string.  The date and time will be formatted as follows:
   *
   *                        "YYYY-MM-DD  HH:MM:SS"
   *
   * Returns    : none
   *********************************************************************************************************
 */
void PC_GetDateTime(char *s)
{   SYSTEMTIME now;

    GetLocalTime(&now);                                         // Get the local date and time

    sprintf(s, "%04d-%02d-%02d  %02d:%02d:%02d",                // Convert into a string
            now.wYear,
            now.wMonth,
            now.wDay,
            now.wHour,
            now.wMinute,
            now.wSecond);
}





