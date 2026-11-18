#include"bills.h"
#include<math.h>
#include<stddef.h>

static Bill   bills[BILLS_KIND_COUNT];
static int    week;          /* 0 = sem contas */
static double elapsed_sec;
static double cost_mult = 1.0;
static int    failed;
static const char *failed_name;

static const char *const names[BILLS_KIND_COUNT] = {
    "Aluguel", "Luz e água", "Internet", "Alimentação"
};
static const double fractions[BILLS_KIND_COUNT] = {
    BILLS_FRACTION_RENT, BILLS_FRACTION_POWER,
    BILLS_FRACTION_INTERNET, BILLS_FRACTION_FOOD
};

static void issue_week(int w)
{
    int i;
    double growth;

    week   = w;
    growth = pow(BILLS_WEEKLY_GROWTH, w - 1);
    for(i = 0; i < BILLS_KIND_COUNT; ++i){
        bills[i].name   = names[i];
        bills[i].amount = (long)(BILLS_REFERENCE_WEEKLY_INCOME * fractions[i]
                                 * growth * cost_mult * 100.0 + 0.5);
        bills[i].paid   = 0;
    }
}

void Bills_Reset(double cost_factor)
{
    elapsed_sec = 0;
    failed      = 0;
    failed_name = NULL;
    cost_mult   = (cost_factor > 0)? cost_factor: 1.0;
    issue_week(1);
}

void Bills_Advance(double game_sec)
{
    int i;

    if(week == 0 || failed || game_sec <= 0){
        return;
    }
    elapsed_sec += game_sec;

    while(week > 0 && elapsed_sec >= week * BILLS_WEEK_SEC){
        for(i = 0; i < BILLS_KIND_COUNT; ++i){
            if(!bills[i].paid){
                failed      = 1;
                failed_name = bills[i].name;
                return;
            }
        }
        if(week >= BILLS_TOTAL_WEEKS){
            week = 0;                 /* acabaram as contas do mes */
            return;
        }
        issue_week(week + 1);
    }
}

int Bills_Week(void){ return week; }

double Bills_SecondsUntilDue(void)
{
    return (week > 0)? week * BILLS_WEEK_SEC - elapsed_sec: -1;
}

int Bills_PendingCount(void)
{
    int i, n = 0;
    if(week == 0){
        return 0;
    }
    for(i = 0; i < BILLS_KIND_COUNT; ++i){
        n += !bills[i].paid;
    }
    return n;
}

long Bills_PendingTotal(void)
{
    int i;
    long sum = 0;
    if(week == 0){
        return 0;
    }
    for(i = 0; i < BILLS_KIND_COUNT; ++i){
        if(!bills[i].paid){
            sum += bills[i].amount;
        }
    }
    return sum;
}

Bill *Bills_PendingAt(int n)
{
    int i;
    if(week == 0 || n < 0){
        return NULL;
    }
    for(i = 0; i < BILLS_KIND_COUNT; ++i){
        if(!bills[i].paid && n-- == 0){
            return &bills[i];
        }
    }
    return NULL;
}

int Bills_Failed(void){ return failed; }
const char *Bills_FailedName(void){ return failed_name; }

BillsPayResult Bills_Pay(Bill *b, int *wallet)
{
    if(!b || b->paid){
        return BILLS_PAY_NOTHING;
    }
    if(*wallet < b->amount){
        return BILLS_PAY_NO_FUNDS;
    }
    *wallet -= (int)b->amount;
    b->paid  = 1;
    return BILLS_PAY_OK;
}

BillsPayResult Bills_PayAll(int *wallet)
{
    int i;
    long total = Bills_PendingTotal();

    if(total == 0){
        return BILLS_PAY_NOTHING;
    }
    if(*wallet < total){
        return BILLS_PAY_NO_FUNDS;
    }
    for(i = 0; i < BILLS_KIND_COUNT; ++i){
        bills[i].paid = 1;
    }
    *wallet -= (int)total;
    return BILLS_PAY_OK;
}
