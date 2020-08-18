#include "../include/masterbootrecord.h"

Partition *MasterBootRecord::partition(int index){
    if(index >= 4){
        std::cout << utils::COLOR_RED << "[MBR] Requested index " << index << " is not in bound of the MBR partition table" << utils::COLOR_RESET << std::endl;
        last_error_m = INDEX_OUT_OF_BOUNDS;
        return nullptr;
    }
    last_error_m = OPERATION_SUCCEED;
    return partitionTable[index];
}


MasterBootRecord::MasterBootRecord(){
    partitionTable[0] = nullptr;
    partitionTable[1] = nullptr;
    partitionTable[2] = nullptr;
    partitionTable[3] = nullptr;
}

int MasterBootRecord::lastError(){
    return last_error_m;
}

void MasterBootRecord::addPartition(Partition *partition, int index){
    if(index >= 4){
        std::cout << utils::COLOR_RED << "[MBR] Requested index " << index << " is not in bound of the MBR partition table" << utils::COLOR_RESET << std::endl;
        last_error_m = INDEX_OUT_OF_BOUNDS;
        return;
    }
    partitionTable[index] = partition;
    last_error_m = OPERATION_SUCCEED;
}

void MasterBootRecord::removePartition(int index){
    if(index >= 4){
        std::cout << utils::COLOR_RED << "[MBR] Requested index " << index << " is not in bound of the MBR partition table" << utils::COLOR_RESET << std::endl;
        last_error_m = INDEX_OUT_OF_BOUNDS;
        return;
    }
    delete partitionTable[index];
    partitionTable[index] = nullptr;
    last_error_m = OPERATION_SUCCEED;
}

int MasterBootRecord::findFreePartition(){
    // Iterates over all MBRs partition table. If an empty spot is found the
    //  Index of it will be returned, otherwise MBR_FULL is returned.
    for(int i = 0; i < 4; ++i){
        if(partitionTable[i] == nullptr){
            return i;
        }
    }
    std::cout << utils::COLOR_RED << "[MBR] No free partition available" << utils::COLOR_RESET << std::endl;

    return MBR_FULL;
}

