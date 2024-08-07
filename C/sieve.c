/* Sieve of Eratosthenes
 *
 * A method to calculate prime numbers
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main (int argc, char *argv[])
{
        /* limit = the amount of numbers to check are prime or not */
        int limit, i, j;

        if (argc != 2 || ((limit = atoi(argv[1]))) <= 0) {
                printf("Usage: %s NUMBERS\n", argv[0]);
                exit(1);
        }

        limit = limit-1;

        /* allocate an array to store numbers
         * we can't store this on the stack because it could get huge! */
        int *numbers;

        numbers = (int *)malloc(sizeof(int)*limit);

        if (!numbers) {
                perror("Could not allocate memory");
                exit(1);
        }

        /* fill the array with all numbers to check */
        for(i = 0; i < limit; i++) {
                numbers[i] = i+2;
        }

        for (i = 0; i < limit; i++) {
                /* if we've already crossed this one out, skip it */
                if (numbers[i] != 0) {
                        /* square the number, if it's smaller than the limit, find its array address and zero it */
                        for (j = 2 * numbers[i]-2; j < limit; j += numbers[i])
                                numbers[j]=0;
                }
        }

        /* one is always prime */
        printf("1 ");

        /* walk the number array. if the number is not zero, print it */
        for (i = 0; i < limit; i++) {
                if (numbers[i] != 0) {
                        printf("%d ", numbers [i]);
                }
        }

        /* clean up */
        printf("\n");
        free(numbers);
        return 0;
}
