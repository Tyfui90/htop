/*
htop - TemperatureMeter.c
(C) 2013 Ralf Stemmer
(C) 2014 Blair Bonnett
Released under the GNU GPL, see the COPYING file
in the source distribution for its full text.
*/

#include "TemperatureMeter.h"

#include "ProcessList.h"
#include "CRT.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/*{
#include "Meter.h"
}*/

int TemperatureMeter_attributes[] = {
   TEMPERATURE_COOL,
   TEMPERATURE_MEDIUM,
   TEMPERATURE_HOT,
};

static void TemperatureMeter_setValues(Meter* this, char* buffer, int len) {
   ProcessList* pl = this->pl;
   this->total = pl->totalTasks;
   this->values[0] = pl->runningTasks;
   snprintf(buffer, len, "%d/%d", (int) this->values[0], (int) this->total);
}

static void TemperatureMeter_display(Object* cast, RichString* out) {
int textColor   = CRT_colors[METER_TEXT];
   int coolColor   = CRT_colors[TEMPERATURE_COOL];
   int mediumColor = CRT_colors[TEMPERATURE_MEDIUM];
   int hotColor    = CRT_colors[TEMPERATURE_HOT];
   char *line[100];
   char *tend[100];

   int  temperature;
   


FILE* file = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
         char* ok = fgets(line, 100, file);
fclose(file);

 // line contains the temprature, we just need to do some math on it.

 // convert the string into an integer, this is necessary for further steps
      temperature = strtol(line, &tend, 10);
	temperature = temperature / 1000;

      // choose the color for the temperature
      int tempColor;
      if      (temperature < 65)                      tempColor = coolColor;
      else if (temperature >= 65 && temperature < 75) tempColor = mediumColor;
      else                                            tempColor = hotColor;

      // output the temperature
      char buffer[20];
      sprintf(buffer, "%d", temperature);
      RichString_append(out, tempColor, buffer);
      RichString_append(out, textColor, "°C ");

   //free(line);
}

MeterClass TemperatureMeter_class = {
   .super = {
      .extends = Class(Meter),
      .display = TemperatureMeter_display,
      .delete = Meter_delete,
   },
   .updateValues = TemperatureMeter_setValues,
   .defaultMode = TEXT_METERMODE,
   .total = 100.0,
   .attributes = TemperatureMeter_attributes,
   .name = "Temperature",
   .uiName = "Temperature Sensors",
   .caption = "Temperature: "
};
