/*
 * Usage:
 *   something | grep-non-ascii
 */


#include <assert.h>
#include <stdbool.h>
#include <stdio.h>


bool has_non_ascii(const char *buf, const size_t size)
{
  for (size_t i=0; i<size; i++) {
    const unsigned char ch = buf[i];
    if (ch >= 128) {
      return true;
    }
  }
  return false;
}


int main()
{
  /* read all STDIN data into buf */
  static char buf[32*1024*1024];
  size_t si = 0;
  while (1) {
    assert(sizeof(buf) >  si);
    const size_t read_size = fread(&buf[si], 1, sizeof(buf) - si, stdin);
    if (read_size > 0) {
      /* Our buffer must always be larger than the message */
      assert(si + read_size < sizeof(buf));
      /* advance index */
      si += read_size;
    }
    if (feof(stdin)) {
      /* end of stdin, so we are done reading here */
      break;
    } else {
      int e = ferror(stdin);
      if (e) {
	/* error abort */
	fprintf(stderr, "log-non-ascii: Error %d reading from STDIN.\n", e);
	return 13;
      } else {
	/* no error, no eof, try reading more data */
      }
    }
  }
  /* we now have a buf[0..si-1] full of characters */
  // fprintf(stderr, "grep-non-ascii: BUF FILL: %u (0x%x)\n", si, si);

  /* Go through data line by line. If non-ascii characters are found
   * in line, write line to stdout. Lines are terminated by '\n'. */
  size_t line_begin = 0;
  size_t line_end = 0;
  size_t line_no = 1;
  size_t lines_printed = 0;
  while (line_end < si) {
    if (buf[line_end] == '\n') {
      line_end++;
      const size_t line_length = line_end - line_begin;
      if (has_non_ascii(&buf[line_begin], line_length)) {
	lines_printed++;
	fwrite(&buf[line_begin], 1, line_length, stdout);
      }
      /* advance to next line */
      line_begin = line_end;
      line_no++;
    } else {
      line_end++;
    }
  }

  /* check for last line possibly not terminated with \n */
  if (line_begin < line_end) {
    // fprintf(stderr, "grep-non-ascii: FINAL DUMP\n");
    lines_printed++;
    fwrite(&buf[line_begin], 1, line_end-line_begin, stdout);
  }

  fprintf(stderr, "grep-non-ascii: %d of %d lines contain non-ascii chars.\n",
	  lines_printed, line_no-1);

  /* done, everything being OK */
  return 0;
}
