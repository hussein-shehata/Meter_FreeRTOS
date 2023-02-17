#include "meter.h"
#include <stdlib.h>
#include <MyFreeRTOS.h>

#define MAX_IC_METER_READING_MWATT  (6000)
#define SECONDS_IN_HOUR             (3600)
#define MWATTS_TO_WATTS             (1000)
#define IC_METER_PERIOD_MS          (100)



static tMET_Config prvMET_Configuration;
static tMET_Measurement prvMET_Measurement;

static uint8_t prvMET_DisplayRow;
static uint8_t prvMET_DisplayColumn;

static float prvMET_TimeInHours;
static uint8_t prvMET_Seconds;

/* Define handles for meter timer, cofiguration data mutex, measurement data mutex */
/* Add your code here! */

/* End of your code! */


static void prvMET_Task(void* pvParameters);
static void prvMET_UpdateTime(void);
static void prvMET_UpdateMeasurement(void);
static void prvMET_UpdateMeter(void);
static void prvMET_TimerCallback(TimerHandle_t xTimerHandle);

void MET_Init(void)
{
    const TickType_t xTimerPeriod = pdMS_TO_TICKS(IC_METER_PERIOD_MS);
    
    /* Create configuration data mutex and metering measurement mutex*/
    /* Add your code here! */

    /* End of your code! */

    
    /* Protect reading configuration data from eeprom */
    /* Add your code here! */

    prvMET_Configuration.zero = 0;
    prvMET_Configuration.gain = 1;
    prvMET_Configuration.cost = 194;
    prvMET_Configuration.co2_rate = 99;
    prvMET_Configuration.bill_cycle = 90;

    /* End of your code! */

    /* Protect meter time and measurement data initialization */
    /* Add your code here! */

    
    prvMET_TimeInHours = 0;
    prvMET_Seconds = 0;

    
    /* Set initial metering data */
    prvMET_Measurement.watts = 0;
    prvMET_Measurement.watts_max = 0;
    prvMET_Measurement.watts_min = 6;
    prvMET_Measurement.kwh = 0;
    prvMET_Measurement.kwh_per_hour = 0;
    prvMET_Measurement.calc_hours = 0;
    prvMET_Measurement.float_cost = ((float)prvMET_Configuration.cost) / 1000;
    prvMET_Measurement.kwh_per_hour_cost = prvMET_Measurement.kwh_per_hour * prvMET_Measurement.float_cost;

    /* End of your code! */

    /* Create periodic timer every 1 second  */
    /* Add your code here! */

    /* End of your code! */

    /* Set initial window */
    prvMET_DisplayRow = 1;
    prvMET_DisplayColumn = 0;

    /* Initialize metering chip with gain and zero */
    // Not implemented 

    /* Create meter task */
    /* Add your code here! */

    /* End of your code! */

}

static void prvMET_Task(void* pvParameters)
{
    /* Initialization */
    (void)pvParameters;

    /* Sync all tasks start  */
    /* Add your code here! */

    /* End of your code! */

    /* Start meter timer */
    /* Add your code here! */

    /* End of your code! */

    for (;;)
    {
        prvMET_UpdateMeter();
    }
}


void MET_GetConfiguration(tMET_Config * configuration)
{
    /* Protect reading configuration data by any other task */
    /* Add your code here! */

    configuration->zero = prvMET_Configuration.zero;
    configuration->gain = prvMET_Configuration.gain;
    configuration->cost = prvMET_Configuration.cost;
    configuration->co2_rate = prvMET_Configuration.co2_rate;
    configuration->bill_cycle = prvMET_Configuration.bill_cycle;

    /* End of your code! */
}


static void prvMET_TimerCallback(TimerHandle_t xTimerHandle)
{
    xTimerHandle;

    tMET2DISP_Message message;

    prvMET_UpdateTime();
    
    prvMET_UpdateMeasurement();

    /* Send metering data to display */
    /* Add your code here! */

    /* End of your code! */

}

static void prvMET_UpdateTime(void)
{
    /* Make this function thread-safe */
    /* Add your code here! */

    prvMET_Seconds++;

    if (prvMET_Seconds == 60)
    {
        prvMET_Seconds = 0;
        prvMET_TimeInHours = prvMET_TimeInHours + ((float)1) / 60;
    }

    /* End of your code! */
}

static void prvMET_UpdateMeasurement(void)
{
    /* Make this function thread-safe */
    /* Add your code here! */

    /* Read metering IC */
    prvMET_Measurement.watts = ((float)(rand() % MAX_IC_METER_READING_MWATT))/ MWATTS_TO_WATTS;
    
    /* Update metering data */
    prvMET_Measurement.kwh += prvMET_Measurement.watts / SECONDS_IN_HOUR;
    
    if (prvMET_Measurement.watts > prvMET_Measurement.watts_max)
    {
        prvMET_Measurement.watts_max = prvMET_Measurement.watts;
    }

    if (prvMET_Measurement.watts < prvMET_Measurement.watts_min)
    {
        prvMET_Measurement.watts_min = prvMET_Measurement.watts;
    }

    prvMET_Measurement.calc_hours = prvMET_TimeInHours; 
    if (prvMET_Measurement.calc_hours != 0)
    {
        prvMET_Measurement.kwh_per_hour = prvMET_Measurement.kwh / prvMET_Measurement.calc_hours;
    }
    else
    {

    }
    
    prvMET_Measurement.float_cost = ((float)prvMET_Configuration.cost) / 1000;
    prvMET_Measurement.kwh_per_hour_cost = prvMET_Measurement.kwh_per_hour * prvMET_Measurement.float_cost;

    /* End of your code! */
}

/* Updates configuration, display window, clears metering data */
static void prvMET_UpdateMeter(void)
{
    char mbMessage = '\0';
    tMET2DISP_Message message;
    /* Wait for a message from oush button task on the message buffer */
    /* Add your code here! */

    /* End of your code! */
    if (mbMessage == 'c')
    {
        /* Protect measurement data during clearing/sending */
        /* Add your code here! */
        /* Clear measurements */

        prvMET_Measurement.watts = 0;
        prvMET_Measurement.watts_max = 0;
        prvMET_Measurement.watts_min = 6;
        prvMET_Measurement.kwh = 0;
        prvMET_Measurement.kwh_per_hour = 0;
        prvMET_Measurement.calc_hours = 0;
        prvMET_Measurement.float_cost = ((float)prvMET_Configuration.cost) / 1000;
        prvMET_Measurement.kwh_per_hour_cost = prvMET_Measurement.kwh_per_hour * prvMET_Measurement.float_cost;

        /* Send metering data to display */
        message.type = MET2DISP_UpdateMetertingData;
        message.data.measurement.calc_hours = prvMET_Measurement.calc_hours;
        message.data.measurement.float_cost = prvMET_Measurement.float_cost;
        message.data.measurement.kwh = prvMET_Measurement.kwh;
        message.data.measurement.kwh_per_hour = prvMET_Measurement.kwh_per_hour;
        message.data.measurement.kwh_per_hour_cost = prvMET_Measurement.kwh_per_hour_cost;
        message.data.measurement.watts = prvMET_Measurement.watts;
        message.data.measurement.watts_max = prvMET_Measurement.watts_max;
        message.data.measurement.watts_min = prvMET_Measurement.watts_min;

        /* End of your code! */

        /* Send measurement data to display */
        /* Add your code here! */

        /* End of your code! */

        /* Reset Time, do not forget to protect it */
        /* Add your code here! */

        /* End of your code! */
    } 
    else if (mbMessage == 'f')
    {
        prvMET_DisplayRow++;

        if ((prvMET_DisplayRow == 6))
        {
            prvMET_DisplayRow = 1;
        }
        prvMET_DisplayColumn = 0;

        /* Send row and coulmn to display */
        message.type = MET2DISP_UpdateWindow;
        message.data.window.row = prvMET_DisplayRow;
        message.data.window.column = prvMET_DisplayColumn;
        /* Add your code here! */

        /* End of your code! */
    }
    else if (mbMessage == 'r')
    {
        if (prvMET_DisplayRow < 6)
        {
            prvMET_DisplayColumn++;

            /* Send row and coulmn to display */
            message.type = MET2DISP_UpdateWindow;
            message.data.window.row = prvMET_DisplayRow;
            message.data.window.column = prvMET_DisplayColumn;
            /* Add your code here! */

            /* End of your code! */
        }
    }
    else if (mbMessage == 'l')
    {
        if (prvMET_DisplayRow < 6)
        {
            prvMET_DisplayColumn--;
			/* Send row and coulmn to display */
            message.type = MET2DISP_UpdateWindow;
            message.data.window.row = prvMET_DisplayRow;
            message.data.window.column = prvMET_DisplayColumn;
            /* Add your code here! */

            /* End of your code! */
        }
    }
}




   