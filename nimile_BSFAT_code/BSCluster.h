#ifndef _BS_CLUSTER_H_
#define _BS_CLUSTER_H_

#include <stdlib.h>
#include <stdio.h>

#include "definitions.h"
#include "BSFat.h"
#include "BSBlock.h"

/**
 * This struct contains information about a cluster
 * A cluster is implemented as a doubly linked list
 */
struct BSCluster_t {
	/// This attribute contains the data, eg a block
	BSBlock* block;

	/// This attribute represent the cluster index
	int index;

	/// This attribute is the next cluster, NULL iff the 
	BSCluster* next;

	/// This attribute is the previous cluster, NULL iff the cluster is the head
	BSCluster* prev;
};

/**
 * This method creates a new Cluster of the size specified by @size parameter.
 * @param size int Size of the cluster
 * @return BSCluster* Pointer to the cluster head
 */
BSCluster* BSCLUSTER_create_cluster(int size);

/*
 * This method finds the next free block inside the cluster.
 * @param file_system_t BSFat File system where the cluster is used
 * @return BSCluster* Cluster where the free block was found
 */
BSCluster* BSCLUSTER_get_next_cluster(BSFat* file_system_t);

/*
 * This method counts the free available blocks inside a cluster
 * @param file_system_t BSFat* Filesystem where the cluster is located
 * @return int Amount of free blocks
 */
int BSCLUSTER_count_free_blocks(BSFat* file_system_t);

/*
 * This method counts the occupied blocks inside a cluster
 * @param file_system_t BSFat* Filesystem where the cluster is located
 * @return int Amount of occupied blocks
 */
int BSCLUSTER_count_occupied_blocks(BSFat* file_system_t);

/*
 * This method counts the reserved blocks inside a cluster
 * @param file_system_t BSFat* Filesystem where the cluster is located
 * @return int Amount of reserved blocks
 */
int BSCLUSTER_count_reserved_blocks(BSFat* file_system_t);

/*
 * This method counts the occupied blocks inside a cluster
 * @param file_system_t BSFat* Filesystem where the cluster is located
 * @return int Amount of occupied blocks
 */
int BSCLUSTER_count_corrupted_blocks(BSFat* file_system_t);

/*
 * This method prints details about a cluster
 * @param start_t BSCluster* Start of the cluster
 */
int BSCLUSTER_print(BSCluster* start_t);

#endif //_BS_CLUSTER_H_
