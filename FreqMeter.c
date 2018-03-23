/*
htop - TemperatureMeter.c
(C) 2013 Ralf Stemmer
(C) 2014 Blair Bonnett
Released under the GNU GPL, see the COPYING file
in the source distribution for its full text.
*/

#include "FreqMeter.h"

#include "ProcessList.h"
#include "CRT.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/*{
#include "Meter.h"
}*/

int FreqMeter_attributes[] = {
   TEMPERATURE_COOL,
   TEMPERATURE_MEDIUM,
   TEMPERATURE_HOT,
};

static void FreqMeter_setValues(Meter* this, char* buffer, int len) {
   ProcessList* pl = this->pl;
   this->total = pl->totalTasks;
   this->values[0] = pl->runningTasks;
   snprintf(buffer, len, "%d/%d", (int) this->values[0], (int) this->total);
}

static void FreqMeter_display(Object* cast, RichString* out) {
int textColor   = CRT_colors[METER_TEXT];
   int coolColor   = CRT_colors[TEMPERATURE_COOL];
   int mediumColor = CRT_colors[TEMPERATURE_MEDIUM];
   int hotColor    = CRT_colors[TEMPERATURE_HOT];
   char *line[100];
   char *tend[100];

   int  freq;
   


FILE* file = fopen("/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq", "r");
         char* ok = fgets(line, 100, file);
fclose(file);

 // line contains the freq, we just need to do some math on it.

 // convert the string into an integer, this is necessary for further steps
      freq = strtol(line, &tend, 10);
	freq = freq / 1000;

      // choose the color for the temperature
      int tempColor;
      if      (freq < 1100)                      tempColor = coolColor;
      else if (freq >= 1200 && freq < 1400) tempColor = mediumColor;
      else                                             tempColor = hotColor;

      // output the temperature
      char buffer[20];
      sprintf(buffer, "%d", freq);
      RichString_append(out, tempColor, buffer);
      RichString_append(out, textColor, " MHz ");

   //free(line);
}

MeterClass FreqMeter_class = {
   .super = {
      .extends = Class(Meter),
      .display = FreqMeter_display,
      .delete = Meter_delete,
   },
   .updateValues = FreqMeter_setValues,
   .defaultMode = TEXT_METERMODE,
   .total = 100.0,
   .attributes = FreqMeter_attributes,
   .name = "FreqMeter",
   .uiName = "FreqMeter",
   .caption = "CPU Freq.: "
};
