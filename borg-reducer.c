#define ASSETS_PATH "assets/"
#define MAX_METHODS 9
#define MAX_FILESIZE 100

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "Include/borg-reducer.h"


typedef enum { false, true } bool;

int verbose_flag;
bool save;
bool report;
char* methods;
char* filepath;
double percentile;
char* output;

int main (int argc, char *argv[]) {

  int c;
  while (1)
    {
      static struct option long_options[] =
        {
          /* These options set a flag. */
          {"verbose", no_argument,       &verbose_flag, 1},
          {"brief",   no_argument,       &verbose_flag, 0},
          /* These options don’t set a flag.
             We distinguish them by their indices. */
          {"no-save", no_argument,       0, 'n'},
          {"report",  no_argument,       0, 'r'},
          {"file",    required_argument, 0, 'f'},
          {"percent", required_argument, 0, 'p'},
          {"methods", required_argument, 0, 'm'},
          {"output",  required_argument, 0, 'o'},
          {0, 0, 0, 0}
        };
      /* getopt_long stores the option index here. */
      int option_index = 0;

      c = getopt_long (argc, argv, "nrf:p:m:o:",
                       long_options, &option_index);

      /* Detect the end of the options. */
      if (c == -1)
        break;

      switch (c)
        {
        case 0:
          /* If this option set a flag, do nothing else now. */
          if (long_options[option_index].flag != 0)
            break;

        case 'n':
          save = false;
          break;

        case 'r':
          report = true;
          break;

        case 'p':
          percentile = optarg ? atof(optarg) : 0.1;
          break;

        case 'm':
          methods = optarg ? optarg : "d";
          break;

        case 'f':
          filepath = optarg ? optarg : "miserables.graphml";
          break;

        case 'o':
          output = optarg ? optarg : "OUT/";
          break;

        case '?':
          /* getopt_long already printed an error message. */
          break;

        default:
          abort ();
        }
    }

  /* Instead of reporting ‘--verbose’
     and ‘--brief’ as they are encountered,
     we report the final status resulting from them. */
  if (verbose_flag)
    puts ("verbose flag is set");

  /* Print any remaining command line arguments (not options). */
  if (optind < argc)
    {
      printf ("non-option ARGV-elements: ");
      while (optind < argc)
        printf ("%s ", argv[optind++]);
      putchar ('\n');
    }
percentile = percentile ? percentile : 0.1;
methods = methods ? methods : "d";

load_graph((filepath ? "assets/miserables.graphml" : strcat(ASSETS_PATH, filepath)));

filter_graph(percentile, methods, (filepath ? "assets/miserables.graphml" : strcat(ASSETS_PATH, filepath)));
  return 0;
}
