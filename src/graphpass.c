/* -*- mode: C -*-  */

/** Copyright [2018] [Ryan Deschamps]
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License. 
*/


/** @file graphpass.c
 @brief The main file for GraphPass.
 
 Graphpass accepts a file, a percentage and a series of characters that represent
 methods of filtering a network graph, and outputs new graph files with the filtered
 graphs and optionally, a report showing how those filters affected the graph
 */


#define _GNU_SOURCE
#define ASSETS_PATH "assets/"
#define MAX_METHODS 9
#define MAX_FILESIZE 100


#include "igraph.h"
#include "graphpass.h"

bool SAVE = true;
bool GFORMAT = false;
bool REPORT = false;
bool QUICKRUN = false;

int verbose_flag;

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
          {"gexf",    no_argument,       0, 'g'},
          {"quick",   no_argument,       0, 'q'},
          {"weighted",no_argument,       0, 'w'},
          {"file",    required_argument, 0, 'f'},
          {"percent", required_argument, 0, 'p'},
          {"methods", required_argument, 0, 'm'},
          {"output",  required_argument, 0, 'o'},
          {"dir",     required_argument, 0, 'd'},
          {0, 0, 0, 0}
        };
      /* getopt_long stores the option index here. */
      int option_index = 0;
      c = getopt_long (argc, argv, "nrgqwf:p:m:o:d",
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
          SAVE = !SAVE;
          break;
        case 'g':
          GFORMAT = !GFORMAT;
          break;
        case 'd':
          DIRECTORY = optarg ? optarg : "assets/";
          break;
        case 'r':
          REPORT = !REPORT;
          break;
        case 'p':
          PERCENT = optarg ? atof(optarg) : 0.0;
          break;
        case 'm':
          METHODS = optarg ? optarg : "d";
          break;
        case 'f':
          FILENAME = optarg ? optarg : "cpp2.graphml";
          break;
        case 'q':
          QUICKRUN = !QUICKRUN;
          break;
        case 'o':
          /*check size of output text */
          OUTPUT = optarg ? optarg : "OUT/";
          break;
        case 'w':
          CALC_WEIGHTS = !CALC_WEIGHTS;
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
  OUTPUT = OUTPUT ? OUTPUT : "OUT/";
  PERCENT = PERCENT ? PERCENT : 0.00;
  METHODS = METHODS ? METHODS : "d";
  DIRECTORY = DIRECTORY ? DIRECTORY : "assets/";
  FILENAME = FILENAME ? FILENAME : "cpp2.graphml";
  char path[strlen(DIRECTORY)+1];
  strncpy(path, DIRECTORY, strlen(DIRECTORY)+1);
  printf(">>>>>>>  GRAPHPASSING >>>>>>>> \n");
  printf("DIRECTORY: %s \nSTRLEN PATH: %li \n", DIRECTORY, strlen(path));
  printf("OUTPUT DIRECTORY: %s\nPERCENTAGE: %f\n", OUTPUT, PERCENT);
  printf("FILE: %s\nMETHODS STRING: %s\n", FILENAME, METHODS);
  printf("QUICKRUN: %i\nREPORT: %i\nSAVE: %i\n", QUICKRUN, REPORT, SAVE);
  if (FILENAME[0] == '/' && DIRECTORY[strlen(DIRECTORY)] == '/' ){
    printf("Removing redundant slashes from filename.\n");
    path[strlen(path)+1] = '\0';
  }
  else if (FILENAME[0] != '/' && DIRECTORY[strlen(DIRECTORY)-1] != '/') {
    printf("Adding slash separator.\n");
    strncat(path, "/", 1);
    printf ("Current path: %s.\n", path);
  }
  int sizeOfPath = (strlen(path)+1);
  int sizeOfFile = (strlen(FILENAME)+1);
  int filepathsize = sizeOfPath + sizeOfFile;

  FILEPATH = malloc(filepathsize + 1);
  snprintf(FILEPATH, filepathsize, "%s%s", path, FILENAME);
  printf("Running graphpass on file: %s\n", FILEPATH);
  load_graph(FILEPATH);
  filter_graph();
  printf(">>>>  SUCCESS!");
  if (SAVE) {
    printf("- Files output to %s\n", OUTPUT);
  }
  free(FILEPATH);
  return 0;
}
