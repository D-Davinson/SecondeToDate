#include <stdio.h>
#include <time.h>

typedef enum {MON = 1, TUE = 2, WED = 3, THU = 4, FRI = 5, SAT = 6, SUN = 7} dayname;

typedef enum {JAN = 1, FEB = 2, MAR = 3,  APR = 4, MAY = 5, JUN = 6, JUL = 7,
              AUG = 8, SEP = 9, OCT = 10, NOV = 11, DEC = 12} monthname;

typedef struct {
  dayname weekday : 3;
  unsigned int day : 5;
  monthname month : 4;
  int year : 20;
} date;

char* dayname_str (dayname day) {
  static char *names[] = {"MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"};
  return names[(day-1)%7];
}

char* monthname_str (monthname month) {
  static char *names[] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN",
                          "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};
  return names[(month-1)%12];
}

#define EPOCH THU

dayname weekday (time_t when) {
  unsigned long days = when / (60*60*24);
  return (((EPOCH - 1) % 7) + (days % 7)) % 7 + 1;
}

int leapyear (unsigned int year) {
  return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

date from_time (time_t when) {
  static unsigned int month_len[] = {31, 28, 31, 30, 31, 30,
                                     31, 31, 30, 31, 30, 31};
  unsigned long days = when / (60*60*24);
  int daycount;
  unsigned int year, month;
  // compute year
  for (year=1970; 1; year++) {
    if (leapyear(year)) {
      daycount = 366;
    } else {
      daycount = 365;
    }
    if ((unsigned long)daycount <= days) {
      days -= daycount;
    } else {
      break;
    }
  }
  // compute month
  if (leapyear(year)) {
    month_len[FEB-1] = 29;
  } else {
    month_len[FEB-1] = 28;
  }
  for (month=0; 1; month++) {
    if (days < month_len[month]) {
      month++;
      break;
    } else {
      days -= month_len[month];
    }
  }
  // fill date and return
  date d = {.weekday = weekday(when),
            .day = days + 1,
            .month = month,
            .year = year};
  return d;
}

int main (void) {
  date now = from_time(time(NULL));
  printf("%s %s %u %u\n",
         dayname_str(now.weekday),
         monthname_str(now.month),
         now.day,
         now.year);
}
