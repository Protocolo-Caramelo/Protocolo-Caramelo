/*
 * Clock module from https://github.com/CastCris/TinkerCode/blob/main/Clock/Clock.ino
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>

//
enum {
    JAN_ = 1,
    FEB_,
    MAR_,
    ABR_,
    MAI_,
    JUN_,
    JUL_,
    AGO_,
    SEP_,
    OCT_,
    NOV_,
    DEC_
};

enum {
    SUN,
    MON,
    TUE,
    WED,
    THU,
    FRI,
    SAT
};

#define LEAP_YEAR(y) \
( \
(!(y % 4) && y % 100) \
|| !(y % 400) \
)

// Set the month days
#define JAN_DAYS 31
#define FEB_DAYS(y) (LEAP_YEAR(y)?29: 28)
#define MAR_DAYS 31
#define ABR_DAYS 30
#define MAI_DAYS 31
#define JUN_DAYS 30
#define JUL_DAYS 31
#define AGO_DAYS 31
#define SEP_DAYS 30
#define OCT_DAYS 31
#define NOV_DAYS 30
#define DEC_DAYS 31

#define YEAR_DAYS(y) \
(LEAP_YEAR(y)? 366: 365)

/* Define min year and the week day of the min year */
#define REF_YEAR 1 // 2026
#define REF_WDAY MON // THU

/*
 * Some time measure
 */
#define SECS_MIN (60)
#define SECS_HOUR (60 * 60)
#define SECS_DAY ((uint32_t)60 * 60 * 24)

#define WEEK_SIZE 7

/*
 * Set the data type of each time measure
 * Data types used in storage of time measure
 */
typedef uint16_t T_MS;

typedef uint8_t T_SEC;
typedef uint8_t T_MIN;
typedef uint8_t T_HOUR;

typedef uint16_t T_YDAY;
typedef uint8_t T_WDAY;
typedef uint8_t T_MDAY;

typedef uint8_t T_WEEK;
typedef uint8_t T_MONTH;
typedef uint32_t  T_YEAR;


typedef int16_t T_SIGNED_YDAY;

/* Absolute time measure data type
 * These data types are used when a date is only represent as a single time measure.
 * Like represent 00:00 1/1/2026 as: 63902908800 seconds or 17750808 hours
 * from 1/1/1
 */
typedef uint64_t ABS_T_MS;

typedef uint64_t ABS_T_SEC;
typedef uint64_t ABS_T_MIN;
typedef uint64_t ABS_T_HOUR;

typedef uint64_t ABS_T_YDAY;

typedef double ABS_T_WEEK;
typedef double ABS_T_MONTH;
typedef double ABS_T_YEAR;

/*
 * Theses macro define the data type of each time measure for input/ouput
 * operations
 */
#define T_MS_F "%03u"

#define T_SEC_F "%02u"
#define T_MIN_F "%02u"
#define T_HOUR_F "%02u"

#define T_YDAY_F "%u"
#define T_WDAY_F "%u"
#define T_WDAY_NAME_F "%s"
#define T_MDAY_F "%02u"

#define T_WEEK_F "%u"
#define T_MONTH_F "%02u"
#define T_MONTH_NAME_F "%s"
#define T_YEAR_F "%u"


/*
 * Macros formats for Clock_sprintf
 */
#define CLOCK_PRINTF_COMMON "%"

#define PRINT_MS \
    CLOCK_PRINTF_COMMON "ms"

#define PRINT_SEC \
    CLOCK_PRINTF_COMMON "sec"
#define PRINT_MIN \
    CLOCK_PRINTF_COMMON "min"
#define PRINT_HOUR \
    CLOCK_PRINTF_COMMON "hour"

#define PRINT_YDAY \
    CLOCK_PRINTF_COMMON "yday"
#define PRINT_WDAY \
    CLOCK_PRINTF_COMMON "wday"
#define PRINT_WDAY_NAME \
    CLOCK_PRINTF_COMMON "Wday"
#define PRINT_MDAY \
    CLOCK_PRINTF_COMMON "mday"

#define PRINT_WEEK \
    CLOCK_PRINTF_COMMON "week"
#define PRINT_MONTH \
    CLOCK_PRINTF_COMMON "month"
#define PRINT_MONTH_NAME \
    CLOCK_PRINTF_COMMON "Month"
#define PRINT_YEAR \
    CLOCK_PRINTF_COMMON "year"

#define CLOCK_PRINTF \
    PRINT_MS "\n" \
    \
    PRINT_SEC "\n" \
    PRINT_MIN "\n" \
    PRINT_HOUR "\n" \
    \
    PRINT_YDAY "\n" \
    PRINT_WDAY "\n" \
    PRINT_WDAY_NAME "\n" \
    PRINT_MDAY "\n" \
    \
    PRINT_WEEK "\n" \
    PRINT_MONTH "\n" \
    PRINT_MONTH_NAME "\n" \
    PRINT_YEAR "\n"

#define CLOCK_PRINTF_MAX 10
#define CLOCK_PRINTF_RESULT_MAX 55


#define PRINT_DEFAULT \
    PRINT_HOUR ":" PRINT_MIN ":" PRINT_SEC " " \
    PRINT_MDAY "/" PRINT_MONTH "/" PRINT_YEAR "\n" \
    PRINT_WDAY_NAME ", " PRINT_MONTH_NAME

#define STR_REPLACE_MAX \
    ( CLOCK_PRINTF_RESULT_MAX * 2)

/*
 * The Clock struct is a portable way(at least I try!) of storage a date.
 * This struct storage a arbitrary date in the format:
 * seconds
 * minutes
 * hours
 *
 * year's day
 * year
 *
 * From those primitive data we can get precisious infos of a time,
 * like the weekday, monthday or the year's week.
 *
 * Additionally, the Clock struct storage if the clock is set to display
 * the time in a 24 hour format or not
 */
typedef struct{
    T_MS _ms;

    T_SEC _sec;
    T_MIN _min;
    T_HOUR _hour;

    T_YDAY _day;
    T_YEAR _year;
} Clock;


// Clock methods
Clock* Clock_Create();
void Clock_Reset(Clock*);
void Clock_Sum(Clock*, ABS_T_SEC sec);
void Clock_SumMs(Clock*r, ABS_T_MS ms);

void Clock_sprintf(Clock*, char to[], char format[]);
void Clock_DataFromFormat(Clock*, char to[], char format[]);

int Clock_SameTime(Clock*, Clock*);
int Clock_SameDate(Clock*, Clock*);

// int Clock_before_date(Clock*, ABS_T_SEC time);
// int Clock_after_date(Clock*, ABS_T_SEC time);

// Clock properties / setters
T_MS Clock_ms(Clock*);

T_SEC Clock_sec(Clock*);
T_MIN Clock_min(Clock*);
T_HOUR Clock_hour(Clock*);

T_YDAY Clock_yday(Clock*);
T_WDAY Clock_wday(Clock*);
void Clock_wday_name(Clock*, char to[]);
T_MDAY Clock_mday(Clock*);

T_WEEK Clock_week(Clock*);
T_MONTH Clock_month(Clock*);
void Clock_month_name(Clock*, char to[]);
T_YEAR Clock_year(Clock*);


void Clock_ms_def(Clock*, T_MS ms);

void Clock_sec_def(Clock*, T_SEC sec);
void Clock_min_def(Clock*, T_MIN minutes);
void Clock_hour_def(Clock*, T_HOUR hour);

void Clock_yday_def(Clock*, T_YDAY yday);
void Clock_wday_def(Clock*, T_WDAY wday);
void Clock_mday_def(Clock*, T_MDAY mday);

void Clock_month_def(Clock*, T_MONTH month);
void Clock_year_def(Clock*, T_YEAR year);

void Clock_time_set(
        Clock*,
        T_HOUR, T_MIN, T_SEC
        );
void Clock_date_set(
        Clock*,
        T_MDAY, T_MONTH, T_YEAR
        );

// Calendar properties
int Calendar_year_leap(T_YEAR y);
T_YDAY Calendar_year_days(T_YEAR y);
T_YEAR Calendar_yearsLeap_util_year(T_YEAR y);

ABS_T_WEEK Calendar_year_weeks(T_YEAR y);
T_WEEK Calendar_year_week(T_YDAY yday, T_YEAR year);

T_WDAY Calendar_week_day(T_YDAY yday, T_YEAR year);
void Calendar_week_day_name(T_WDAY wday, char str[]);
T_WDAY Calendar_week_days_to_init(T_WDAY wday);

T_MONTH Calendar_month(T_YDAY yday, T_YEAR year);
void Calendar_month_name(T_MONTH month, char str[]);
T_YDAY Calendar_month_days(T_MONTH m, T_YEAR y);
T_MDAY Calendar_month_day(T_YDAY yday, T_YEAR year);

ABS_T_YDAY Calendar_days_from_dates(
        T_YDAY y1day, T_YEAR y1,
        T_YDAY y2day, T_YEAR y2
        );

// String methods
void str_copy(char to[], char from[]);
void str_clean(char str[]);
void str_add_char(char str[], int c);
void str_replace(char str[], char sub[], char nsub[]);
int str_search(char str[], char sub[], int index);

int is_substring(char str[], char sub[]);
