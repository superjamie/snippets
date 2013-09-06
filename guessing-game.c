#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// random number guess program
// try to guess the number the computer is thinking of in three turns or less
// my first "dos game"
// gnu gpl v3 lololol

int main(int argc, char*argv[]){
  int max_guesses = 3;  // maximum number of guesses allowed
  int current_guess = 0;  // how many guesses we've had
  int answer = 0;  // the number the computer is thinking of
  int user_guess = 0; // the number the user has guessed

  srand(time(NULL));  // seed the random number generator with the current time (in seconds)
  // debug int r = rand();  // print a random number
  answer = ((rand() % 9) +1);  // modulo the random number by 9 to get a number between 0 and 8, then add one to get a number between 1 and 9
  // debug printf("%d\n", answer);  // print the answer
  printf("I'm thinking of a number between 1 and 9, can you guess it?\n");  // tell the user what input we're asking for

  // loop to ask for user guesses until the max guesses are reached
  do{
    if (user_guess != 0){ printf("Guess again.\n"); }  // if the user has already had a guess, ask for another one
    scanf("%d", &user_guess);  //get a number from the user

    if (user_guess == answer){  // if they got it
      printf("You got it! You truly are a being to be reckoned with.\n");  // tell them
      break;  // exit the guessing loop
      }
    else if (user_guess > answer){  // if they guessed lower
      printf("My number is lower than that.\n"); // tell them, and wait to re-loop
      }
    else if (user_guess < answer){  // if they guessed higher
      printf("My number is higher than that.\n"); // tell them, and wait to re-loop
      }
    current_guess++;  // increment the loop counter
  } while(current_guess < max_guesses);  // check if the user has run out of guesses

  if (user_guess != answer){  // if the user didn't win
    printf("\nLooks like I win today, human. Better luck next time!\n");  // taunt them
  }
  return 0;  // return 0 to main
}
