/** @file analyze_test.h 
 @brief Tests for analyze.h
 */

#include "graphpass.h"
#include "unity.h"
#include <math.h>
void setUp(void) {
}

void tearDown(void) {
}

void TEST_DEGREE_ALGORITHM() {
  calc_degree(&g, 'd');
  igraph_vector_t deg;
  igraph_vector_init(&deg, 0);
  VANV(&g, "Degree", &deg);
  TEST_ASSERT_EQUAL_FLOAT(roundf(10 * VECTOR(deg)[0])/10, 1.00);
  TEST_ASSERT_EQUAL_FLOAT(roundf(10 * VECTOR(deg)[10])/10, 1.00);
  TEST_ASSERT_EQUAL_FLOAT(roundf(10 * VECTOR(deg)[100])/10, 2.00);
  igraph_vector_destroy(&deg);
}

void TEST_INDEGREE_ALGORITHM() {
  calc_degree(&g, 'i');
  igraph_vector_t deg;
  igraph_vector_init(&deg, 0);
  VANV(&g, "Indegree", &deg);
  TEST_ASSERT_EQUAL_FLOAT(roundf(10 * VECTOR(deg)[0])/10, 1.00);
  TEST_ASSERT_EQUAL_FLOAT(roundf(10 * VECTOR(deg)[10])/10, 1.00);
  TEST_ASSERT_EQUAL_FLOAT(roundf(10 * VECTOR(deg)[100])/10, 2.00);
  igraph_vector_destroy(&deg);
}

void TEST_OUTDEGREE_ALGORITHM() {
  calc_degree(&g, 'o');
  igraph_vector_t deg;
  igraph_vector_init(&deg, 0);
  VANV(&g, "Outdegree", &deg);
  TEST_ASSERT_EQUAL_FLOAT(roundf(10 * VECTOR(deg)[0])/10, 0.00);
  TEST_ASSERT_EQUAL_FLOAT(roundf(10 * VECTOR(deg)[10])/10, 0.00);
  TEST_ASSERT_EQUAL_FLOAT(roundf(10 * VECTOR(deg)[100])/10, 0.00);
  igraph_vector_destroy(&deg);
}

void TEST_BETWEENNESS_ALGORITHM() {
  calc_betweenness(&g);
  igraph_vector_t bet;
  igraph_vector_init(&bet, 0);
  VANV(&g, "Betweenness", &bet);
  TEST_ASSERT_EQUAL_FLOAT(roundf(100000 * VECTOR(bet)[0])/100000, 0.00);
  TEST_ASSERT_EQUAL_FLOAT(roundf(100000 * VECTOR(bet)[10])/100000, 0.00);
  TEST_ASSERT_EQUAL_FLOAT(roundf(100000 * VECTOR(bet)[100])/100000, 0.00);
  igraph_vector_destroy(&bet);
}

void TEST_AUTHORITY_ALGORITHM() {
  calc_authority(&g);
  igraph_vector_t aut;
  igraph_vector_init(&aut, 0);
  VANV(&g, "Authority", &aut);
  TEST_ASSERT_EQUAL_FLOAT(roundf(100000 * VECTOR(aut)[0])/100000, 0.07685);
  TEST_ASSERT_EQUAL_FLOAT(roundf(100000 * VECTOR(aut)[10])/100000, 0.00);
  TEST_ASSERT_EQUAL_FLOAT(roundf(100000 * VECTOR(aut)[100])/100000, 0.08232);
  igraph_vector_destroy(&aut);
}

void TEST_HUB_ALGORITHM() {
  calc_hub(&g);
  igraph_vector_t hub;
  igraph_vector_init(&hub, 0);
  VANV(&g, "Hub", &hub);
  TEST_ASSERT_EQUAL_FLOAT(roundf(100000 * VECTOR(hub)[0])/100000, 0.00);
  TEST_ASSERT_EQUAL_FLOAT(roundf(100000 * VECTOR(hub)[10])/100000, 0.00);
  TEST_ASSERT_EQUAL_FLOAT(roundf(100000 * VECTOR(hub)[100])/100000, 0.00);
  igraph_vector_destroy(&hub);
}

void TEST_EIGENVECTOR_ALGORITHM() {
  calc_eigenvector(&g);
  igraph_vector_t eig;
  igraph_vector_init(&eig, 0);
  VANV(&g, "Eigenvector", &eig);
  TEST_ASSERT_EQUAL_FLOAT(roundf(100000 * VECTOR(eig)[0])/100000, 0.0);
  TEST_ASSERT_EQUAL_FLOAT(roundf(100000 * VECTOR(eig)[10])/100000, 0.00);
  TEST_ASSERT_EQUAL_FLOAT(roundf(100000 * VECTOR(eig)[100])/100000, 0.00);
  igraph_vector_destroy(&eig);
}

void TEST_PAGERANK_ALGORITHM() {
  calc_pagerank(&g);
  igraph_vector_t pr;
  igraph_vector_init(&pr, 0);
  VANV(&g, "PageRank", &pr);
  TEST_ASSERT_EQUAL_FLOAT(roundf(100000 * VECTOR(pr)[0])/100000, 0.00424);
  TEST_ASSERT_EQUAL_FLOAT(roundf(100000 * VECTOR(pr)[10])/100000, 0.00419);
  TEST_ASSERT_EQUAL_FLOAT(roundf(100000 * VECTOR(pr)[100])/100000, 0.00447);
  igraph_vector_destroy(&pr);
}

void TEST_MODULARITY() {
  calc_modularity(&g);
  igraph_vector_t mod;
  igraph_vector_init(&mod, 0);
  VANV(&g, "WalkTrapModularity", &mod);
  TEST_ASSERT_EQUAL_FLOAT(VECTOR(mod)[0], 5);
  TEST_ASSERT_EQUAL_FLOAT(VECTOR(mod)[10], 7);
  TEST_ASSERT_EQUAL_FLOAT(VECTOR(mod)[100], 5);
  igraph_vector_destroy(&mod);
}

void TEST_RANKORDER() {
  igraph_vector_t test, test2, ranks;
  igraph_vector_init(&test, 10);
  igraph_vector_init(&test2, 10);
  igraph_vector_init(&ranks, 10);
  VECTOR(test)[0] = 10; VECTOR(test)[1] = 2;
  VECTOR(test)[2] = 4;  VECTOR(test)[3] = 7;
  VECTOR(test)[4] = 10; VECTOR(test)[5] = 100;
  VECTOR(test)[6] = 11; VECTOR(test)[7] = 99;
  VECTOR(test)[8] = 2;  VECTOR(test)[9] = 10;
  VECTOR(ranks)[0] = 4; VECTOR(ranks)[1] = 9;
  VECTOR(ranks)[2] = 8;  VECTOR(ranks)[3] = 7;
  VECTOR(ranks)[4] = 4; VECTOR(ranks)[5] = 1;
  VECTOR(ranks)[6] = 3; VECTOR(ranks)[7] = 2;
  VECTOR(ranks)[8] = 9;  VECTOR(ranks)[9] = 4;
  produceRank(&test, &test2);
  TEST_ASSERT_TRUE(igraph_vector_all_e(&test2, &ranks));
  igraph_vector_destroy(&test);
  igraph_vector_destroy(&test2);
  igraph_vector_destroy(&ranks);
}


