#include <stdio.h> 
#include <string.h>

// function to remove newlines from string passed to it

char *remove_newline(char *s) {

  size_t len = strlen(s);

  if (len > 0 && s[len-1] == '\n')  // if there's a newline
    s[len-1] = '\0';                // truncate the string

  return s;
}

// a program to take user input and not allow it to overflow a buffer

int main(int argc, char *argv[]) {

  size_t bytes_read = 0;
  size_t number_of_bytes = 5;
  char firststring[number_of_bytes+1];
  char secondstring[number_of_bytes+1];
  char *tempstring = NULL;

  // i don't really see the need to use this, getline does realloc as needed
  // tempstring = (char *) malloc (number_of_bytes + 1);

  printf("Type something already:\n");
  getline(&tempstring, &bytes_read, stdin);
    if (bytes_read == -1) { printf("ERROR\n"); return 1; }
  remove_newline(tempstring);
  strncpy(firststring, tempstring, number_of_bytes);

  printf("Type something again:\n");
  getline(&tempstring, &bytes_read, stdin);
    if (bytes_read == -1) { printf("ERROR\n"); return 1; }
  remove_newline(tempstring);
  strncpy(secondstring, tempstring, number_of_bytes);

  printf("\n");
  printf("firststring: %s\n", firststring);
  printf("secondstring: %s\n", secondstring);

  return 0;
}
