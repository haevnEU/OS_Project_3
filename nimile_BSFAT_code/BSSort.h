#ifndef _BS_SORT_H_
#define _BS_SORT_H_

#include <stdlib.h>
#include <stdio.h>

#include "definitions.h"
#include "BSFat.h"
#include "BSCluster.h"
/**
 * This method swaps two cluster entries.
 * @param cluster1 BSCluster* first cluster to swap
 * @param cluster2 BSCluster* second cluster to swap
 */
void swap(BSCluster* cluster1, BSCluster* cluster2);

void BSSORT_heap(BSCluster* cluster_t);

void BSSORT_merge(BSCluster* cluster_t);

void BSSORT_quick(BSCluster* cluster_t);

void BSSORT_insertion(BSCluster* cluster_t);

/**
 * This method sorts a BSCluster using bubble sort algorithm.
 * @param cluster_t BSCluster* Head of the cluster
 * @param size_t int Size of the cluster
 */
void BSSORT_bubble(BSCluster* cluster_t, int size_t);

#endif