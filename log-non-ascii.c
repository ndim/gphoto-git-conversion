/*
 * Usage:
 *   something | log-non-ascii non-ascii.log | or-other
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


int main(int argc, const char *argv[])
{
  assert(argc == 2);
  const char *logfile_name = argv[1];

  /* Set stdout to unbuffered, so we push out our data in a single
   * fwrite(3) go.  This does not guarantee proper locking, but helps
   * reduce the probability of interleaved data from multiple
   * log-non-ascii processes in a single output file. */
  assert(0 == setvbuf(stdout, NULL, _IONBF, 0));

  /* read all STDIN data into buf */
  static char buf[4*1024*1024];
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
  /* buf[0..si-1] now contains the complete message */

  /* write the complete message to stdout */
  fwrite(buf, 1, si, stdout);

  /* examine buf for non-ascii characters, log buf if some found */
  if (has_non_ascii(buf, si)) {
    FILE *logfile = fopen(logfile_name, "a");
    fprintf(logfile, "LOGLOGLOGLOGLOGLOGLOGLOGLOGLOGLOGLOGLOGLOGLOGLOGLOGLOGLOGLOG\n");
    fwrite(buf, 1, si, logfile);
    fclose(logfile);
  }

  /* done, everything being OK */
  return 0;
}
