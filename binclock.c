/* binary clock
 * 
 * an accurate binary clock
 * 
 */

#include <stdio.h>      // printf, etc
#include <time.h>       // time, tm, localtime
#include <string.h>     // strlen
#include <stdbool.h>    // booleans
#include <signal.h>     // timer functions

// prototypes

void int_to_binary(int, char*);
void start_timer(void);
void stop_timer(void);
void timer_callback(int);

// source

volatile sig_atomic_t run = false;
timer_t gTimerid;

// http://www.linuxquestions.org/questions/programming-9/c-timer_create-skeleton-922654/

void start_timer(void)
{
    struct itimerspec value;
    value.it_value.tv_sec = 1; // initial wait
    value.it_value.tv_nsec = 0;
    value.it_interval.tv_sec = 1; // timer interval
    value.it_interval.tv_nsec = 0;
    timer_create (CLOCK_REALTIME, NULL, &gTimerid);
    timer_settime (gTimerid, 0, &value, NULL);
}

void stop_timer(void)
{
    struct itimerspec value;
    value.it_value.tv_sec = 0;
    value.it_value.tv_nsec = 0;
    value.it_interval.tv_sec = 0;
    value.it_interval.tv_nsec = 0;
    timer_settime (gTimerid, 0, &value, NULL);
}

void timer_callback(int sig)
{
    run = true;
}

void int_to_binary(int number, char *word)
{
    int i = 0, len = 0, place = 0;
    len = strlen(word);
    for(i = len; i >= 0; i--)
    {
        place = number >> i;
        if (place & 1)
            //printf("1");
            word[len-i-1] = '1';
        else
            //printf("0");
            word[len-i-1] = '0';
    }
}

int main(int argc, char *argv[])
{
    time_t rawtime;
    struct tm * actualtime;
    char secstr[6] = "000000";
    char minstr[6] = "000000";
    char  hrstr[5] =  "00000";

    (void) signal(SIGALRM, timer_callback);
    start_timer();

    while(true)
    {
        if(run)
        {
            time(&rawtime);
            actualtime = localtime(&rawtime);

            int hour = actualtime->tm_hour;
            int minute = actualtime->tm_min;
            int second = actualtime->tm_sec;

            int_to_binary(second, secstr);
            int_to_binary(minute, minstr);
            int_to_binary(hour, hrstr);

            printf("\e[1;1H\e[2J"); // clears the screen on POSIX ANSI terminals
            printf("\n  hr  %s (%2d)\n min %s (%2d)\n sec %s (%2d)\n", hrstr, hour, minstr, minute, secstr, second);

            run = false;
        }
    }

    return 0;
}
