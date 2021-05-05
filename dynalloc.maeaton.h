#ifndef ASSIGNMENT05_DYNALLOC_MAEATON_H
#define ASSIGNMENT05_DYNALLOC_MAEATON_H

typedef struct BlockListNodeStruct {
    int size;
    int start;
    struct BlockListNodeStruct *next;
} BlockListNode;

typedef struct {
    int capacity;
    BlockListNode *allocations;
    BlockListNode *holes;
} AllocInfo;

typedef enum AllocTypeEnum {
    ALLOC_BEST_FIT,
    ALLOC_WORST_FIT,
    ALLOC_FIRST_FIT
} AllocType;

//Requires: size of the desired block, the type of allocation protocol, the master buffer
//Modifies: the allocation and holes list
//Effects: finds a hole in the buffer to meet specifications and stores a new process block there
int allocateBlock(int size, AllocType allocType, AllocInfo *allocInfo);

//Requires: the starting location of the to-be-deleted block, the master buffer
//Modifies: the allocation and holes list
//Effects: finds the desired block, removes it, adds a hole to take its place
int deallocateBlock(int location, AllocInfo *allocInfo);

//Requires: the starting location, the size of the hole, the master buffer
//Modifies: holes list
//Effects: creates a hole of desired specifications
int addHole(int start, int size, AllocInfo *allocInfo);

//Requires: the starting location, the master buffer
//Modifies: the holes list
//Effects: finds the desired hole and removes it from the list
int deleteHole(int start, AllocInfo *allocInfo);

//Requires: the starting location, the size of the hole, the master buffer
//Modifies: the allocation list
//Effects: creates an allocation of desired specifications
int addAllocation(int start, int size, AllocInfo *allocInfo);

//Requires: size to be changed, the hole to be changed, the master buffer
//Modifies: the hole node and hole list
//Effects: finds the desired node and changes its specifications (start location increases, size decreases)
void updateHole(int requestSize, BlockListNode *holeNode, AllocInfo *info);

//professor supplied validation function
int validAllocGeneral(AllocInfo *info);
#endif //ASSIGNMENT05_DYNALLOC_MAEATON_H
