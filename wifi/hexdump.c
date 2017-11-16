/*
 * Copyright (C) 2004 Joshua J. Drake <hexdump@qoop.org>
 *
 * DISCLAIMER:
 * This code is strictly for educational purposes only.
 * That means that it is *ABSOLUTELY* not to be used maliciously.
 *
 * Any duplication or proliferation of this code is
 * expressly prohibited and punishable by law.
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>


#define BYTES_IN_MINOR_GROUP 	4
#define MINOR_GROUPS_IN_MAJOR 	2
#define MAJOR_GROUPS_PER_LINE 	2


#ifndef OUT_FILEP
# define OUT_FILEP 	stdout
#endif


void hexdump(u_char *ptr, u_int len)
{
	u_char *line, *line_p;
	u_char *hex_p, *asc_p;
	u_int line_len = 0;
	u_int min_cnt = 0;
	u_int maj_cnt = 0;
	u_int byte_cnt = 0;

	/* can't dump 0 bytes! */
	if (!len)
		return;

	/* allocate enuff memory for our output */
	line_len = 3 * BYTES_IN_MINOR_GROUP; 	/* 3 bytes per byte */
	line_len *= MINOR_GROUPS_IN_MAJOR; 		/* this many groups of those */
	line_len += MINOR_GROUPS_IN_MAJOR; 		/* this many extra spaces */
	line_len *= MAJOR_GROUPS_PER_LINE; 		/* this many major groups */
	line_len += MAJOR_GROUPS_PER_LINE;  	       	/* this many extra spaces */
	/* ascii area */
	line_len += (BYTES_IN_MINOR_GROUP * MINOR_GROUPS_IN_MAJOR) * MAJOR_GROUPS_PER_LINE;
	line_len += 1; 				/* trailing newline */
	line_len += 4; 				/* prefix spaces */
#ifdef TEST_HEXDUMP
	printf("allocating line of %u bytes\n", line_len);
#endif
	line = (u_char *)malloc(line_len + 1);

	/* init our pointers */
	hex_p = ptr;
	asc_p = ptr;

	/* ok.. dump the amount specified */
	while (hex_p - ptr < len) {
		line_p = line;

		/* put some prefix spaces */
		*line_p++ = ' ';
		*line_p++ = ' ';
		*line_p++ = ' ';
		*line_p++ = ' ';

		/* build X bytes into X minor groups into X major groups for each line */
		for (maj_cnt = 0; maj_cnt < MAJOR_GROUPS_PER_LINE; maj_cnt++) {
			for (min_cnt = 0; min_cnt < MINOR_GROUPS_IN_MAJOR; min_cnt++) {
				for (byte_cnt = 0; byte_cnt < BYTES_IN_MINOR_GROUP; byte_cnt++) {
					if (hex_p - ptr < len) {
						sprintf((char *)line_p, "%02x ", *hex_p & 0xff);
						hex_p ++;
					} else {
						strcpy((char *)line_p, "   ");
					}
					line_p += 3;
				}
				*line_p++ = ' ';
			}
			*line_p++ = ' ';
		}

		/* now put the ascii stuff after it */
		for (byte_cnt = 0; byte_cnt < (BYTES_IN_MINOR_GROUP * MINOR_GROUPS_IN_MAJOR) * MAJOR_GROUPS_PER_LINE; byte_cnt++) {
			if (asc_p - ptr < len) {
				if (isprint(*asc_p))
					*line_p++ = *asc_p;
				else
					*line_p++ = '.';
				asc_p++;
			} else {
				*line_p++ = ' ';
			}
		}

		*line_p++ = '\n';
		*line_p++ = '\0';

		fprintf(OUT_FILEP, "%s", line);
	}

#ifdef TEST_HEXDUMP
	printf("actual line length: %d\n", strlen(line));
#endif
	free(line);
}
