/**
 * wrap buffer of text to a specific length
 * gcc -std=c17 -g3 -Og -Wall -Wextra -Wpedantic -fanalyzer wrap.c -o wrap
 *
 * Portions written by me provided under GPL-2.0 license.
 * strcpl from OpenBSD provided under ISC license.
 */
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

static const char *file_name = "text.txt";

/**
 * https://github.com/openbsd/src/blob/master/lib/libc/string/strlcpy.c
 * Copyright (c) 1998, 2015 Todd C. Miller <millert@openbsd.org>
 *
 * Copy string src to buffer dst of size dsize.  At most dsize-1
 * chars will be copied.  Always NUL terminates (unless dsize == 0).
 * Returns strlen(src); if retval >= dsize, truncation occurred.
 */
static size_t strlcpy(char *dst, const char *src, size_t dsize)
{
	const char *osrc = src;
	size_t nleft = dsize;

	/* Copy as many bytes as will fit. */
	if (nleft != 0) {
		while (--nleft != 0) {
			if ((*dst++ = *src++) == '\0')
				break;
		}
	}

	/* Not enough room in dst, add NUL and traverse rest of src. */
	if (nleft == 0) {
		if (dsize != 0)
			*dst = '\0';		/* NUL-terminate dst */
		while (*src++)
			;
	}

	return(src - osrc - 1);	/* count does not include NUL */
}

/**
 * Accept a buffer of NUL-terminated text, wrapping it to the provided width.
 *
 * `data_size` includes the NUL terminator byte.
 *
 * Width must be greater than 1.
 *
 * Words longer than the wrap width are hyphenated to the width.
 *
 * Allocates a buffer with malloc, caller is responsible to free.
 *
 * On success, returns a buffer of wrapped text which is safe to print.
 *
 * On failure, returns NULL with errno set.
 *
 * @param  text       Buffer of text to be wrapped
 * @param  data_size  Number of bytes in source text, including NUL terminator
 * @param  target     Width to wrap to
 *
 * @return  Allocated buffer of wrapped text, or NULL on failure
 */
static char *text_create_wrapped(const char *text, size_t data_size, size_t target)
{
	char *buffer = NULL;
	if (target < 2) {
		errno = EINVAL;
		goto out;
	}

	size_t buffer_size = data_size;

	/* at most this many extra "-\n" for long lines, so preallocate them */
	buffer_size += (data_size / target) * 2;

	buffer = calloc(buffer_size, sizeof(*buffer));
	if (buffer == NULL)
		goto out;

	strlcpy(buffer, text, data_size);

	/* wrap text algorithm:
	 * set start to the start of the buffer
	 * set limit to start plus target wrap len
	 * count back from limit until a space or newline is reached
	 * set that character to newline
	 * set start to the next character after limit
	 * if start plus target is greater than file size, we've reached the last line
	 * if current word is longer than wrap length, hyphenate it and add a newline
	 */
	size_t start = 0;
	size_t limit = start + target;
	while (limit < data_size) {
		while (limit > start) {
			if (buffer[limit] == ' ' || buffer[limit] == '\n') {
				buffer[limit] = '\n';
				break;
			}
			limit--;
		}

		/* hyphenate word longer than wrap length */
		if (limit == start) {
			size_t hyphen = start + target - 1;
			size_t bytes_left = data_size - hyphen;
			memmove(&buffer[hyphen + 2], &buffer[hyphen], bytes_left);
			buffer[hyphen] = '-';
			buffer[hyphen + 1] = '\n';
			limit += 1;
			data_size += 2;
		}
		start = limit + 1;
		limit = start + target;
	}

out:
	return buffer;
}

int main(int argc, char **argv)
{
	int retval = 0;
	size_t target = 80;

	/* get target wrap length */
	if (argc > 1) {
		long tmp_target = strtol(argv[1], NULL, 10);
		if (tmp_target < 1) {
			printf("ERROR: Could not convert %s to positive number.\n", argv[1]);
			retval = -1;
			goto out;
		} else if (tmp_target < 20) {
			printf("INFO: wrap value %ld too small, clamping to 20\n", tmp_target);
			tmp_target = 20;
		} else if (tmp_target > 160) {
			printf("INFO wrap value %ld too large, clamping to 160\n", tmp_target);
			tmp_target = 160;
		}
		target = tmp_target;
	}

	/* open source text file */
	FILE *src_text = fopen(file_name, "r");
	if (src_text == NULL) {
		perror("fopen");
		retval = -1;
		goto out;
	}

	/* get filesize of source text */
	if (fseek(src_text, 0L, SEEK_END) == -1) {
		perror("fseek");
		retval = -1;
		goto fclose_and_out;
	}
	long ftell_result = ftell(src_text);
	if (ftell_result == -1) {
		perror("ftell");
		retval = -1;
		goto fclose_and_out;
	}
	size_t data_size = ftell_result;
	fseek(src_text, 0L, SEEK_SET); /* rewind() */

	/* add 1 so there's always a NUL terminator */
	size_t buffer_size = data_size + 1;

	/* allocate a buffer to store wrapped text */
	char *buffer = calloc(buffer_size, sizeof(*buffer));
	if (buffer == NULL) {
		perror("calloc");
		retval = -1;
		goto fclose_and_out;
	}

	/* read from file into buffer */
	size_t bytes_read = fread(buffer, sizeof(*buffer), data_size, src_text);
	if (bytes_read != (data_size)) {
		printf("ERROR: %zu bytes read, expected %zu\n", bytes_read, data_size);
		goto free_buf_and_out;
	}

	/* done with file */
	fclose(src_text);
	src_text = NULL;

	/* wrap text, including new NUL terminator */
	char *wrapped = text_create_wrapped(buffer, buffer_size, target);
	if (wrapped == NULL) {
		retval = -1;
		goto free_wrap_and_out;
	}

	/* display wrapped result with a guide ruler */
	for(size_t count = 0; count < target; count++)
		printf("=");
	printf("\n");
	printf("%s\n", wrapped);

	/* cleanup */
free_wrap_and_out:
	free(wrapped);
free_buf_and_out:
	free(buffer);
fclose_and_out:
	if (src_text != NULL)
		fclose(src_text);
out:
	return retval;
}
