#include"clock.h"

// Clock methods
Clock* Clock_Create()
{
    Clock *clock_new = (Clock*)malloc(sizeof(Clock));
    Clock_Reset(clock_new);

    return clock_new;
}

void Clock_Reset(Clock*r)
{
    Clock_ms_def(r, 0);
    Clock_sec_def(r, 0);
    Clock_min_def(r, 0);
    Clock_hour_def(r, 0);

    Clock_yday_def(r, 1);
    Clock_year_def(r, 1);
}

void Clock_Sum(Clock*r, ABS_T_SEC time_s)
{
    ABS_T_SEC sec;
    ABS_T_MIN min;
    ABS_T_HOUR hour;
    ABS_T_YDAY yday, year_days;
    ABS_T_YEAR year;

    sec = Clock_sec(r) + time_s;
    min = Clock_min(r) + sec / 60;
    hour = Clock_hour(r) + min / 60;
    yday = Clock_yday(r) +  hour / 24;
    year = Clock_year(r);

    while(yday >
            (year_days = Calendar_year_days(year))
            ){
        yday -= year_days;
        ++year;
    }

    Clock_sec_def(r, sec % 60);
    Clock_min_def(r, min % 60);
    Clock_hour_def(r, hour % 24);
    Clock_yday_def(r, yday);
    Clock_year_def(r, year);
}

void Clock_SumMs(Clock*r, ABS_T_MS time_ms)
{
    ABS_T_MS ms;
    ABS_T_SEC sec;

    ms = Clock_ms(r) + time_ms;
    sec = ms / 1000;

    if(sec){
      Clock_Sum(r, sec);
    }
    Clock_ms_def(r, ms % 1000);
}

void Clock_sprintf(Clock*r, char t[], char f[])
{
    char clock_printf[] = CLOCK_PRINTF;
    char data[CLOCK_PRINTF_RESULT_MAX];
    char buff[CLOCK_PRINTF_MAX];

    int c;
    size_t i;

    str_clean(buff);
    str_copy(t, f);
    for(i=0; (c = clock_printf[i]) != '\0'; ++i){
        if(c != '\n'){
            str_add_char(buff, c);
            continue;
        }

        if(is_substring(t, buff) != -1){
          Clock_DataFromFormat(r, data, buff);
          str_replace(t, buff, data);
        }
        str_clean(buff);
    }
}


void Clock_DataFromFormat(Clock*r, char to[], char f[])
{
    char format[CLOCK_PRINTF_MAX];

    if(!strcmp(f, PRINT_MS)){
        str_copy(format, T_MS_F);
        sprintf(to, format, Clock_ms(r));
    }

    else if(!strcmp(f, PRINT_SEC)){
        str_copy(format, T_SEC_F);
        sprintf(to, format, Clock_sec(r));
    }
    else if(!strcmp(f, PRINT_MIN)){
        str_copy(format, T_MIN_F);
        sprintf(to, format, Clock_min(r));
    }
    else if(!strcmp(f, PRINT_HOUR)){
        str_copy(format, T_HOUR_F);
        sprintf(to, format, Clock_hour(r));
    }

    else if(!strcmp(f, PRINT_YDAY)){
        str_copy(format, T_YDAY_F);
        sprintf(to, format, Clock_yday(r));
    }
    else if(!strcmp(f, PRINT_WDAY)){
        str_copy(format, T_WDAY_F);
        sprintf(to, format, Clock_wday(r));
    }
    else if(!strcmp(f, PRINT_WDAY_NAME)){
        Clock_wday_name(r, to);
    }
    else if(!strcmp(f, PRINT_MDAY)){
        str_copy(format, T_MDAY_F);
        sprintf(to, format, Clock_mday(r));
    }

    else if(!strcmp(f, PRINT_WEEK)){
        str_copy(format, T_WEEK_F);
        sprintf(to, format, Clock_week(r));
    }
    else if(!strcmp(f, PRINT_MONTH)){
        str_copy(format, T_MONTH_F);
        sprintf(to, format, Clock_month(r));
    }
    else if(!strcmp(f, PRINT_MONTH_NAME)){
        Clock_month_name(r, to);
    }
    else if(!strcmp(f, PRINT_YEAR)){
        str_copy(format, T_YEAR_F);
        sprintf(to, format, Clock_year(r));
    }
}


int Clock_SameTime(Clock*r1, Clock*r2)
{
    return
      Clock_sec(r1) == Clock_sec(r2)
      && Clock_min(r1) == Clock_min(r2)
      && Clock_hour(r1) == Clock_hour(r2);
}

int Clock_SameDate(Clock*r1, Clock*r2)
{
    return
      Clock_year(r1) == Clock_year(r2)
      && Clock_month(r1) == Clock_month(r2)
      && Clock_mday(r1) == Clock_mday(r2);
}

// Clock properties
T_MS Clock_ms(Clock*r)
{
    return r->_ms;
}


T_SEC Clock_sec(Clock*r)
{
    return r->_sec;
}

T_MIN Clock_min(Clock*r)
{
    return r->_min;
}

T_HOUR Clock_hour(Clock*r)
{
    return r->_hour;
}


T_YDAY Clock_yday(Clock*r)
{
    return r->_day;
}

T_WDAY Clock_wday(Clock*r)
{
    return Calendar_week_day(Clock_yday(r), Clock_year(r));
}

void Clock_wday_name(Clock*r, char str[])
{
    Calendar_week_day_name(Clock_wday(r), str);
}

T_MDAY Clock_mday(Clock*r)
{
    return Calendar_month_day(Clock_yday(r), Clock_year(r));
}


T_WEEK Clock_week(Clock*r)
{
    return Calendar_year_week(Clock_yday(r), Clock_year(r));
}

T_MONTH Clock_month(Clock*r)
{
    return Calendar_month(Clock_yday(r), Clock_year(r));
}

void Clock_month_name(Clock*r, char str[])
{
    Calendar_month_name(Clock_month(r), str);
}

T_YEAR Clock_year(Clock*r)
{
    return r->_year;
}

// Clock setters
void Clock_ms_def(Clock*r, T_MS ms)
{
    r->_ms = ms;
}


void Clock_sec_def(Clock*r, T_SEC s)
{
    r->_sec = s;
}

void Clock_min_def(Clock*r, T_MIN m)
{
    r->_min = m;
}

void Clock_hour_def(Clock*r, T_HOUR h)
{
    r->_hour = h;
}


void Clock_yday_def(Clock*r, T_YDAY yday)
{
    r->_day = yday;
}

void Clock_wday_def(Clock*r, T_WDAY wday)
{
    T_YDAY week_yday_init;

    week_yday_init = Clock_yday(r) - Calendar_week_days_to_init(Clock_wday(r));
    Clock_yday_def(r, week_yday_init);

    while(Clock_wday(r) != wday){
        Clock_Sum(r, SECS_DAY);
    }
}

void Clock_mday_def(Clock*r, T_MDAY mday)
{
    T_YDAY mday_yday;
    mday_yday = Clock_yday(r) - Clock_mday(r);

    Clock_yday_def(r, mday_yday + mday);
}


void Clock_month_def(Clock*r, T_MONTH m)
{
    T_MONTH month;
    T_MDAY month_days;
    T_MDAY mday;

    month = JAN_;
    mday = Clock_mday(r);
    Clock_yday_def(r, 0);

    while(month < m){
        month_days = Calendar_month_days(month++, Clock_year(r));
        Clock_Sum(r, month_days * SECS_DAY);
    }

    Clock_Sum(r, mday * SECS_DAY);
}

void Clock_year_def(Clock*r, T_YEAR y)
{
    r->_year = y;
}


void Clock_TimeSet(
        Clock*r,
        T_HOUR h, T_MIN m, T_SEC s
        )
{
    Clock_sec_def(r, s);
    Clock_min_def(r, m);
    Clock_hour_def(r, h);
}

void Clock_DateSet(
        Clock*r,
        T_MDAY mday, T_MONTH month, T_YEAR year
        )
{
    Clock_year_def(r, year);
    Clock_month_def(r, month);
    Clock_mday_def(r, mday);
}

// Calendar
int Calendar_year_leap(T_YEAR y)
{
    return LEAP_YEAR(y);
}

T_YDAY Calendar_year_days(T_YEAR y)
{
    return YEAR_DAYS(y);
}

T_YEAR Calendar_yearsLeap_util_year(T_YEAR y)
{
  return (y / 4 - y / 100 + y / 400);
}


ABS_T_WEEK Calendar_year_weeks(T_YEAR y)
{
    if(Calendar_year_leap(y))
        return 52.0 + 2.0 / 7.0;
    return 52 + 1.0 / 7.0;
}

T_WEEK Calendar_year_week(T_YDAY yday, T_YEAR year)
{
    ABS_T_YDAY days;
    ABS_T_WEEK week;
    T_YEAR y;

    days = Calendar_days_from_dates(
            1, 1,
            yday, year
            );

    week = days / 7.0;
    y = 1;

    // printf("days: %lli\n", days);
    while(week > Calendar_year_weeks(y)){
        week -= Calendar_year_weeks(y++);
        // printf("week: %lf\n", week);
    }

    return week + 1;
}


T_WDAY Calendar_week_day(T_YDAY yday, T_YEAR year)
{
    ABS_T_YDAY days_since_yref;

    days_since_yref = Calendar_days_from_dates(
            1, REF_YEAR,
            yday, year
            );

    if(year >= REF_YEAR){
        // We plus the REF_WDAY for define sunday
        // as the first day of week
        return ( days_since_yref + REF_WDAY ) % 7;
    }
    else {
        return (7 - (days_since_yref % 7) + REF_WDAY) % 7;
    }
}

void Calendar_week_day_name(T_WDAY wday, char str[])
{
    switch(wday){
        case SUN:
            str_copy(str, "SUN");
            break;

        case MON:
            str_copy(str, "MON");
            break;

        case TUE:
            str_copy(str, "TUE");
            break;

        case WED:
            str_copy(str, "WED");
            break;

        case THU:
            str_copy(str, "THU");
            break;

        case FRI:
            str_copy(str, "FRI");
            break;

        case SAT:
            str_copy(str, "SAT");
            break;

        default:
            break;
    }
}

T_WDAY Calendar_week_days_to_init(T_WDAY wday)
{
    return wday - SUN;
}


T_MONTH Calendar_month(T_YDAY yday, T_YEAR year)
{
    T_MDAY mday;
    T_MONTH month;

    month = JAN_;
    while(yday >
            (mday = Calendar_month_days(month, year))
            ){
        yday -= mday;
        ++month;
    }

    return month;
}

T_MDAY Calendar_month_day(T_YDAY yday, T_YEAR year)
{
    T_MDAY mday;
    T_MONTH month;

    month = JAN_;
    while(yday >
            (mday = Calendar_month_days(month, year))
            ){
        yday -= mday;
        ++month;
    }

    return yday;
}

void Calendar_month_name(T_MONTH month, char str[])
{
    switch(month){
        case JAN_:
            str_copy(str, "JAN");
            break;

        case FEB_:
            str_copy(str, "FEB");
            break;

        case MAR_:
            str_copy(str, "MAR");
            break;

        case ABR_:
            str_copy(str, "ABR");
            break;

        case MAI_:
            str_copy(str, "MAI");
            break;

        case JUN_:
            str_copy(str, "JUN");
            break;

        case JUL_:
            str_copy(str, "JUL");
            break;

        case AGO_:
            str_copy(str, "AGO");
            break;

        case SEP_:
            str_copy(str, "SEP");
            break;

        case OCT_:
            str_copy(str, "OCT");
            break;

        case NOV_:
            str_copy(str, "NOV");
            break;

        case DEC_:
            str_copy(str, "DEC");
            break;

        default:
            break;
    }
}

T_YDAY Calendar_month_days(T_MONTH m, T_YEAR y)
{
    switch(m){
        case JAN_:
            return JAN_DAYS;
            break;

        case FEB_:
            return FEB_DAYS(y);
            break;

        case MAR_:
            return MAR_DAYS;
            break;

        case ABR_:
            return ABR_DAYS;
            break;

        case MAI_:
            return MAI_DAYS;
            break;

        case JUN_:
            return JUN_DAYS;
            break;

        case JUL_:
            return JUL_DAYS;
            break;

        case AGO_:
            return AGO_DAYS;
            break;

        case SEP_:
            return SEP_DAYS;
            break;

        case OCT_:
            return OCT_DAYS;
            break;

        case NOV_:
            return NOV_DAYS;
            break;

        case DEC_:
            return DEC_DAYS;
            break;

        default:
            break;
    }

    return 0;
}

ABS_T_YDAY Calendar_days_from_dates(
        T_YDAY y1day, T_YEAR y1,
        T_YDAY y2day, T_YEAR y2
        )
{
    if(y1 > y2){
        y1 += y2;
        y2 = y1 - y2;
        y1 -= y2;

        y1day += y2day;
        y2day = y1day - y2day;
        y1day -= y2day;
    }

    T_YEAR yearsLeap_betw_y1_y2;
    yearsLeap_betw_y1_y2 =
        Calendar_yearsLeap_util_year(y2 - 1) - Calendar_yearsLeap_util_year(y1 - 1)
        ;

    return
        (y2 - y1) * 365
        + yearsLeap_betw_y1_y2
        + ((T_SIGNED_YDAY)y2day - (T_SIGNED_YDAY)y1day)
        ;
}


//
void str_copy(char t[], char f[])
{
    size_t i, j;

    i = j = 0;
    while((t[i++] = f[j++]) != '\0');
}

void str_clean(char s[])
{
    s[0] = '\0';
}

void str_add_char(char s[], int c)
{
    size_t i;

    i = 0;
    while(s[i] != '\0')
        ++i;

    s[i] = c;
    s[i + 1] = '\0';
}

void str_replace(char str[], char sub[], char nsub[])
{
    char result[STR_REPLACE_MAX];
    int i, j, t;

    str_clean(result);
    i = t = 0;
    while((i = str_search(str, sub, t)) != -1){
        for(j=t; j < i; ++j)
            str_add_char(result, str[j]);

        t = i + strlen(sub);
        strcat(result, nsub);

      /*
    Serial.print(sub);
        Serial.print(" ");
        Serial.print(i);
        Serial.print(" ");
        Serial.print(j);
        Serial.print(" ");
        Serial.println(t);
      // */
        // printf("%i %i %i\n", i, j, t);
    }

    strcat(result, str + t);
    str_copy(str, result);
}

int str_search(char str[], char sub[], int index)
{
    int result;
    return (
            (result = is_substring(str + index, sub)) != -1
           )? result + index: -1;
}

int is_substring(char str[], char sub[])
{
    size_t str_len, sub_len;
    size_t remain_sub_chars;
    size_t i, j, t;

    str_len = strlen(str);
    sub_len = strlen(sub);
    i = j = 0;

    while(i < str_len){
        remain_sub_chars = sub_len;
        j = 0;
        t = i;

        while(
            remain_sub_chars
                && t < str_len && j < sub_len
                && str[t] == sub[j]
            ){
            ++j;
            ++t;

            --remain_sub_chars;
        }
        // printf("%li %li\n", t, j);

        if(!remain_sub_chars)
            return i;

        i = t + !j;
    }

    return -1;
}
