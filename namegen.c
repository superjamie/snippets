/* Name Generator
 *
 * Place First Names in firstname.txt and surnames in surname.txt
 * 
 * firstnames: http://deron.meranda.us/data/census-derived-all-first.txt
 *             awk '{print $1}' census-derived-all-first.txt > firstname.txt
 *
 * surnames:   http://www.census.gov/genealogy/www/data/1990surnames/dist.all.last
 *             awk '{print $1}' dist.all.last > surname.txt
 *
 */

#include <stdio.h>
#include <stdlib.h>     // rand, atoi
#include <time.h>       // time
#include <ctype.h>	// isspace
#include <string.h>	// strcmp
#include <error.h>	// errno
#include <unistd.h>	// readlink
#include <libgen.h>	// basename

// header

int is_numeric(const char*);
int countlines(FILE*);
void get_filename(FILE*, char*);
#define MAX_NAME_LEN 255

// source

int is_numeric (const char * s) // http://rosettacode.org/wiki/Determine_if_a_string_is_numeric#C
{
    if (s == NULL || *s == '\0' || isspace(*s))
      return 0;
    char * p;
    strtod (s, &p);
    return *p == '\0';
}

int countlines(FILE *fp)
{
    int count = 0;
    int character = 0;
    while ((character = fgetc(fp)) != EOF)
    {
	if (character == '\n')
	{
	    count++;
	}
    }
    if (count == 0)
    {
	char filename[0xFFF];
	get_filename(fp, filename);
	printf("%s doesn't contain any lines!\n", basename(filename));
	exit(1);
    }
    return count;
}

void get_filename(FILE *fp, char *filename)
{
    // http://stackoverflow.com/questions/11221186/how-do-i-find-a-filename-given-a-file-pointer
    int fno;
    char proclnk[0xFFF];
    ssize_t r = 0;
    fno = fileno(fp);
    sprintf(proclnk, "/proc/self/fd/%d", fno);
    r = readlink(proclnk, filename, 0xFFF);
    if (r < 0)
    {
	printf("Error with filename.txt and surname.txt, failed to readlink.\n");
	exit(1);
    }
    filename[r] = '\0';
    //printf("fp -> fno -> filename: %p -> %d -> %s\n", fp, fno, filename);
}

char *remove_newline(char *s) {

  size_t len = strlen(s);

  if (len > 0 && s[len-1] == '\n')  // if there's a newline
    s[len-1] = '\0';                // truncate the string

  return s;
}

int main(int argc, char *argv[])
{
    int numnames = 3;

    int argnum;
    for (argnum = 1; argnum < argc; argnum++)
    {
	if (strncmp(argv[argnum], "-h", 2) == 0 || strncmp(argv[argnum], "--help", 6) == 0)
	{
	    printf("usage: %s [number of names]\n", argv[0]);
	    printf("Name Generator\nPlace First Names in firstname.txt and surnames in surname.txt\nA numerical parameter can specify the number of names to generate.\nThe default number of names is 3.\n");
	    return 1;
	}
	if (argc > 2 || !(is_numeric(argv[argnum])))
	{
	    printf("usage: %s [number of names]\n", argv[0]);
	    return 1;
	}
	numnames = atoi(argv[argnum]);
    }

    // open files and report error if there's a problem
    FILE *firstnamep = fopen("firstname.txt", "r");
    if (firstnamep == NULL)
    {
	perror("Error opening firstname.txt");
	return 1;
    }

    FILE *surnamep   = fopen("surname.txt", "r");
    if (surnamep == NULL)
    {
	perror("Error opening surname.txt");
	return 1;
    }

    // count number of lines in each file

    int firstnametotal = 0, surnametotal = 0;

    firstnametotal = countlines(firstnamep);
    surnametotal = countlines(surnamep);
    // i had a check for (firstnametotal == 0 || surnametotal == 0) here, but instead countlines() exits 1 after printing error

    // we just read through the files to the end to count the lines, so rewind back to the start
    rewind(firstnamep);
    rewind(surnamep);

    srand(time(NULL)); //seed the rng

    char *firstnameresult = NULL;
    char *surnameresult = NULL;
    firstnameresult = (char *)malloc(MAX_NAME_LEN * sizeof(char));
    surnameresult = (char *)malloc(MAX_NAME_LEN * sizeof(char));

    // generate this number of name results
    int name;
    for (name = 0; name < numnames; name++)
    {
	int surnum = 0, firstnum = 0;
	firstnum = rand()%firstnametotal;
	surnum = rand()%surnametotal;

	int i;
	for (i = 0; i < firstnum; i++)
	{
	    fgets(firstnameresult, 255, firstnamep);
	}
	for (i = 0; i < surnum; i++)
	{
	    fgets(surnameresult, 255, surnamep);
	}
	firstnameresult = remove_newline(firstnameresult);
	surnameresult = remove_newline(surnameresult);
	printf("%s %s\n", firstnameresult, surnameresult);
	rewind(firstnamep);
	rewind(surnamep);
    }

    printf("\n Returned %d names out of %d firstnames and %d surnames\n", numnames, firstnametotal, surnametotal);

    // close files
    fclose(firstnamep);
    fclose(surnamep);
    
    return 0;
}
