/* -*- Graphpass -- Network Graph Utilities mode: C -*-  */
/* Copyright [2018] [Ryan Deschamps]
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License. */


#ifndef LIB_GRAPHPASS_H
#define LIB_GRAPHPASS_H

#include <igraph.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <getopt.h>

typedef enum { false, true } bool;

igraph_t g;
igraph_attribute_table_t att;
char* FILENAME; /**< The filename from -f flag. */
char* DIRECTORY; /**< Directory to access FILENAME and put REPORT */
char* METHODS;  /**< METHODS to filter */
char* OUTPUT;  /**< Folder to output new graphs */
char* OUTPATH; /**< Path to output folder (DIRECTORY + OUTPUT) */
igraph_integer_t NODESIZE; /**< Number of Nodes in original graph */
float PERCENT; /**< Filtering percentage 0.0 by default */
bool REPORT; /**< Include a report? */
bool GFORMAT; /**< Graph format - true is "GEXF" false is "GRAPHML" */
bool QUICKRUN; /**< Lightweight visualization run */
bool SAVE; /**< If false, does not save graphs at all (for reports) */
bool CALC_WEIGHTS;
igraph_vector_t WEIGHTED; /**< If greater than 0, conducts weighted analysis */

/* Required External libraries */

#define PROGRAM_NAME "Graph Pass"
#define BUG_REPORT "ryan.deschamps@gmail.com"
#define GIT_REPOSITORY "https://www.github.com/archivesunleashed/graphpass"

/* Color Presets */

#define COLOUR_SET_PASTEL "pastel.h"
#define COLOUR_SET_PRIMARY "primary.h"
#define COLOUR_SET_DAMPENED "dampened.h"

/* Visualization Presets */

#define VIZ_SET_SPACIOUS "viz_spacious.h"
#define VIZ_SET_LARGE "viz_large.h"
#define VIZ_SET_SMALL "vis_small.h"

/* Required External libraries */
#define MAX_METHODS 9
#define ALL_METHODS 'abcdehioprw'
#define SIZE_DEFAULT "Degree"
#define SIZE_DEFAULT_CHAR 'd'
#define COLOR_BASE "WalkTrapModularity"
#define PAGERANK_DAMPING 0.85 /**< chance random walk will not restart */
#define LAYOUT_DEFAULT_CHAR 'f'

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

struct Node {
  char* abbrev;
  igraph_real_t val;
  struct Node *next;
};

/** @struct RankNode
 @brief unimplemented struct for holding the top 20 rankids for the graph.
 */
struct RankNode {
  int rankids[20];
  struct RankNode *next;
};

struct stat;
struct Node* asshead;
struct Node* edges;
struct Node* density;
struct Node* betcent;
struct Node* reciprocity;
struct Node* degcent;
struct Node* idegcent;
struct Node* odegcent;
struct Node* eigcent;
struct Node* pagecent;
struct Node* diameter;
struct Node* pathlength;
struct Node* clustering;
struct RankNode* ranks;

int shuffle(int *array, int n);
/** adds a new value to a Node **/
int push(struct Node** head_ref, igraph_real_t value, char* attr);

/** adds a new value to a RankNode **/
int pushRank (struct RankNode** head_ref, int rankids[20]);
int igraph_i_xml_escape(char* src, char** dest);

int igraph_write_graph_gexf(const igraph_t *graph, FILE *outstream,
                            igraph_bool_t prefixattr);

/** Writes the report **/
int write_report(igraph_t *graph);
int colors (igraph_t *graph);
int layout_graph(igraph_t *graph, char layout);
int set_size(igraph_t *graph, igraph_vector_t *v, int max);

int strip_ext(char *fname);
int load_graph (char* filename);
int write_graph(igraph_t *graph, char *attr);
int produceRank(igraph_vector_t *source, igraph_vector_t *vector);
int calc_betweenness(igraph_t *graph);
int calc_authority(igraph_t *graph);
int calc_hub(igraph_t *graph);
int calc_pagerank(igraph_t *graph);
int calc_eigenvector(igraph_t *graph);
/* extern int calc_clustering(igraph_t *graph); */
/* extern int calc_random(igraph_t *graph); */
int calc_degree(igraph_t *graph, char type);
int calc_modularity(igraph_t *graph);
int centralization(igraph_t *graph, char* attr);
int analysis_all (igraph_t *graph);
int quickrunGraph();

float fix_percentile();
int create_filtered_graph(igraph_t *graph, double cutoff, int cutsize, char* attr);
int shrink (igraph_t *graph, int cutsize, char* attr);
int runFilters (igraph_t *graph, int cutsize);
int filter_graph();

#endif
