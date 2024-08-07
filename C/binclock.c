/* binary clock
 *
 * an accurate binary clock
 * gcc -Wall -Wextra -Wpedantic -std=gnu18 -g binclock.c -lrt -o binclock
 */

#include <stdio.h>      // printf, etc
#include <time.h>       // time, tm, localtime
#include <string.h>     // strlen
#include <stdbool.h>    // booleans
#include <signal.h>     // timer functions

// prototypes

void int_to_binary(int, char*, size_t);
void start_timer(void);
void stop_timer(void);
void timer_callback(int);

// source

volatile sig_atomic_t run = false;
timer_t global_timer;

// http://www.linuxquestions.org/questions/programming-9/c-timer_create-skeleton-922654/

/* start a timer which fires every second, sending SIGALRM to the process */
void start_timer(void)
{
	struct itimerspec value = { 0 };
	value.it_value.tv_sec = 1; // initial wait
	value.it_value.tv_nsec = 0;
	value.it_interval.tv_sec = 1; // timer interval
	value.it_interval.tv_nsec = 0;
	timer_create(CLOCK_REALTIME, NULL, &global_timer);
	timer_settime(global_timer, 0, &value, NULL);
}

void stop_timer(void)
{
	struct itimerspec value = { 0 };
	value.it_value.tv_sec = 0;
	value.it_value.tv_nsec = 0;
	value.it_interval.tv_sec = 0;
	value.it_interval.tv_nsec = 0;
	timer_settime(global_timer, 0, &value, NULL);
}

void timer_callback(__attribute__((unused)) int sig)
{
	run = true;
}

void int_to_binary(int number, char *word, size_t wordlen)
{
	/* skip the null terminator */
	wordlen -= 1;

	/* loop backwards through the string */
	/*
	for (int i = wordlen; i > 0; i--)
		if ((number >> (i - 1)) & 1)
			word[wordlen - i] = '1';
		else
			word[wordlen - i] = '0';
	*/

	/* loop forwards through the string */
	for (size_t i = 0; i < wordlen; i++)
		if ((number >> i) & 1)
			word[wordlen - i - 1] = '1';
		else
			word[wordlen - i - 1] = '0';
}

#define WORDBUF 7
int main(void)
{
	time_t rawtime = 0;
	struct tm *actualtime = NULL;
	char secstr[WORDBUF] = "000000";
	char minstr[WORDBUF] = "000000";
	char  hrstr[WORDBUF] = "000000";

	/* register a handler for SIGALRM, which runs the timer callback */
	/* TODO: as per signal(2) change this to sigaction(2) instead */
	(void) signal(SIGALRM, timer_callback);
	start_timer();

	while(true)
	{
		if(run)
		{
			time(&rawtime);
			actualtime = localtime(&rawtime);

			int_to_binary(actualtime->tm_sec, secstr, WORDBUF);
			int_to_binary(actualtime->tm_min, minstr, WORDBUF);
			int_to_binary(actualtime->tm_hour, hrstr, WORDBUF);

			// clear screen on POSIX ANSI terminals
			printf("\e[1;1H\e[2J\n");
			printf(" hr  %s (%2d)\n", hrstr, actualtime->tm_hour);
			printf(" min %s (%2d)\n", minstr, actualtime->tm_min);
			printf(" sec %s (%2d)\n", secstr, actualtime->tm_sec);

			run = false;
		}
	}

	return 0;
}
