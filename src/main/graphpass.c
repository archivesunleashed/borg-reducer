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
#define ASSETS_PATH "src/resources"
#define MAX_METHODS 9
#define MAX_FILESIZE 100


#include "igraph.h"
#include <stdlib.h>
#include "graphpass.h"

char* FILEPATH; /**< The filepath (ug_DIRECTORY + ug_FILENAME) */

/** Whether to save the graph **/
bool ug_save = true;
/** Graph format true for GEXF; false for GRAPHML **/
bool ug_gformat = false;
/** Produce a report analyzing effect of filtering on graph **/
bool ug_report = false;
/** Provide a quickrun with simple sizing, positioning and coloring **/
bool ug_quickrun = false;
/** Print out helper messages **/
bool ug_verbose = false;

int main (int argc, char *argv[]) {
  /* Experiments here

   End experiments */

  int c;
  while (1)
    {
      static struct option long_options[] =
        {
          /* These options have no required argument */
          {"gexf",    no_argument,       0, 'g'},
          {"no-save", no_argument,       0, 'n'},
          {"verbose", no_argument,       0, 'v'},
          {"quick",   no_argument,       0, 'q'},
          {"report",  no_argument,       0, 'r'},

          /* These options require an argument */
          {"dir",     required_argument, 0, 'd'},
          {"file",    required_argument, 0, 'f'},
          {"methods", required_argument, 0, 'm'},
          {"output",  required_argument, 0, 'o'},
          {"percent", required_argument, 0, 'p'},
          {"max-nodes", required_argument, 0, 'x'},
          {"max-edges", required_argument, 0, 'y'},
          {0, 0, 0, 0}
        };
      /* getopt_long stores the option index here. */
      int option_index = 0;
      c = getopt_long (argc, argv, "gnvqrd:f:m:o:p:x:y:",
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
        case 'v':
          ug_verbose = true;
          break;
        case 'n':
          ug_save = !ug_save;
          break;
        case 'g':
          ug_gformat = !ug_gformat;
          break;
        case 'd':
          ug_DIRECTORY = optarg ? optarg : "src/resources/";
          break;
        case 'r':
          ug_report = !ug_report;
          break;
        case 'p':
          ug_percent = optarg ? atof(optarg) : 0.0;
          break;
        case 'm':
          ug_methods = optarg ? optarg : "d";
          break;
        case 'f':
          ug_FILENAME = optarg ? optarg : "cpp2.graphml";
          break;
        case 'q':
          ug_quickrun = !ug_quickrun;
          break;
        case 'o':
          /*check size of output text */
          ug_OUTPUT = optarg ? optarg : "OUT/";
          break;
        case 'w':
          CALC_WEIGHTS = !CALC_WEIGHTS;
          break;
        case 'x':
          ug_maxnodes = optarg ? (long)strtol(optarg, (char**)NULL, 10) : MAX_NODES;
          break;
        case 'y':
          ug_maxedges = optarg ? (long)strtol(optarg, (char**)NULL, 10) : MAX_EDGES;
          break;
        case '?':
          /* getopt_long already printed an error message. */
          break;
        default:
          abort ();
        }
    }

  /* Print any remaining command line arguments (not options). */
  if (optind < argc)
    {
      printf ("non-option ARGV-elements: ");
      while (optind < argc)
        printf ("%s ", argv[optind++]);
      putchar ('\n');
    }

  /** set default values if not included in flags **/
  ug_maxnodes = ug_maxnodes ? ug_maxnodes : MAX_NODES;
  ug_maxedges = ug_maxedges ? ug_maxedges : MAX_EDGES;
  ug_OUTPUT = ug_OUTPUT ? ug_OUTPUT : "OUT/";
  ug_percent = ug_percent ? ug_percent : 0.00;
  ug_methods = ug_methods ? ug_methods : "d";
  ug_DIRECTORY = ug_DIRECTORY ? ug_DIRECTORY : "src/resources/";
  ug_FILENAME = ug_FILENAME ? ug_FILENAME : "cpp2.graphml";

  /** setup directory path ug_DIRECTORY + ug_FILENAME **/
  char path[strlen(ug_DIRECTORY)+1];
  strncpy(path, ug_DIRECTORY, strlen(ug_DIRECTORY)+1);

  /** start output description **/
  if (ug_verbose == true) {
    printf(">>>>>>>  GRAPHPASSING >>>>>>>> \n");
    printf("DIRECTORY: %s \nSTRLEN PATH: %li \n", ug_DIRECTORY, strlen(path));
    printf("OUTPUT DIRECTORY: %s\nPERCENTAGE: %f\n", ug_OUTPUT, ug_percent);
    printf("FILE: %s\nMETHODS STRING: %s\n", ug_FILENAME, ug_methods);
    printf("QUICKRUN: %i\nREPORT: %i\nSAVE: %i\n", ug_quickrun, ug_report, ug_save);
  }
  /** try to be nice if user leaves out a '/' **/
  if (ug_FILENAME[0] == '/' && ug_DIRECTORY[strlen(ug_DIRECTORY)] == '/' ){
    path[strlen(path)+1] = '\0';  // remove end slash
  }
  else if (ug_FILENAME[0] != '/' && ug_DIRECTORY[strlen(ug_DIRECTORY)-1] != '/') {
    strncat(path, "/", 1); // add a slash.
  }

  /** set up FILEPATH to access graphml file **/
  int sizeOfPath = (strlen(path)+1);
  int sizeOfFile = (strlen(ug_FILENAME)+1);
  int filepathsize = sizeOfPath + sizeOfFile;
  FILEPATH = malloc(filepathsize + 1);
  snprintf(FILEPATH, filepathsize, "%s%s", path, ug_FILENAME);
  if (ug_verbose == true) {
    printf("Running graphpass on file: %s\n", FILEPATH);
  }
  load_graph(FILEPATH);
  free(FILEPATH);
  if (igraph_vcount(&g) > ug_maxnodes || igraph_ecount(&g) > ug_maxedges){
    printf ("FAIL >>> Graphpass can only conduct analysis on graphs with \
fewer than %li nodes and %li edges.\n", ug_maxnodes, ug_maxedges);
    printf ("FAIL >>> Exiting...\n");
    exit(EXIT_FAILURE);
  }

  /** start the filtering based on values and methods **/
  filter_graph();
  printf("\n\n>>>>  SUCCESS!");
  if (ug_save) {
    printf("- Files output to %s\n", ug_OUTPUT);
  }
  else {
    printf("- NO_SAVE requested, so no output.\n\n\n");
  }
  return 0;
}
