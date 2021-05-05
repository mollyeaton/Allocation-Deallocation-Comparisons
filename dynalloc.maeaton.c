#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "dynalloc.maeaton.h"

int main() {
    //initialize a random number generator
    time_t t;
    srand((unsigned) time(&t));

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~MASTER BUFFER SET UP~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // initialize the elements
    AllocInfo *master = (AllocInfo*)malloc(sizeof(AllocInfo));
    BlockListNode *holes = (BlockListNode*)malloc(sizeof(BlockListNode));
    BlockListNode *allocations;
    BlockListNode *currNode; //used for looping and tracking

    int capacity = 2100; //set the capacity to 2100
    //set the variables for the master buffer
    master -> capacity = capacity;
    master -> allocations = NULL; //there are currently no allocations
    //set the holes to include the entire buffer
    holes -> start = 0;
    holes -> size = capacity;
    holes -> next = NULL;
    master -> holes = holes; //assign the holes to the master buffer

    // pick a mean request size and initialize the failure count
    int mean = 30;
    int failureCount = 0;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~INITIAL ALLOCATIONS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //loop to add a number of initial allocations to the buffer
    for (int i = 0; i < (capacity)/mean ; i++){
        //allocate the blocks with BEST FIT
        allocateBlock(mean, ALLOC_BEST_FIT, master);
        //check if there are any errors
        if (validAllocGeneral(master) == 1){
            failureCount ++;
        }
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~DEALLOCATE-ALLOCATE CYCLES FOR BEST-FIT~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //set the amount of times you wish to allocate/deallocate blocks
    int N = 200;
    //create empty array to hold the mean hole size
    float BFholeSize[N];
    //variables needed to find the mean
    int holeSum;
    int holeCount;
    //loop through the indicated amount
    for (int i = 0; i < N; i++){
        //pick a random number for the allocation block
        int choice = rand() % (capacity/mean) -1;
        //save the allocations to reference the master buffer
        allocations = master -> allocations;
        //loop through the allocations until it is found
        for (int j = 0; j < choice; j++){
            if (allocations -> next == NULL)
                break;
            allocations = allocations -> next;
        }
        //deallocate the block
        deallocateBlock(allocations->start, master);
        //pick a random block size
        int randSize = rand() % (2*mean) + 1;
        //allocate a block of chosen size
        if (allocateBlock(randSize, ALLOC_BEST_FIT, master) == -1)
            failureCount++;
        //check if there are any errors
        validAllocGeneral(master);

        //re-initialize variables
        currNode = master->holes;
        holeSum = 0;
        holeCount = 0;
        //loop through the holes to find the sum of all holes and total count
        while (currNode != NULL) {
            holeSum = holeSum + currNode->size;
            holeCount ++;
            currNode = currNode->next;

        }
        //add the mean to the array
        BFholeSize[i] = (float)(holeSum)/holeCount;

    }
    //print the values for the array
    printf("Mean Hole Size for Best Fit:\n");
    for(int i = 0; i < N; i++)
        printf("%.2f, ", BFholeSize[i]);
    printf("\nbest fit failure count: %d\n", failureCount);

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~DEALLOCATE-ALLOCATE CYCLES FOR FIRST-FIT~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //create empty array to hold the mean hole size
    float FFholeSize[N];
    //vset variables back to 0
    failureCount = 0;
    //loop through the indicated amount
    for (int i = 0; i < N; i++){
        //pick a random number for the allocation block
        int choice = rand() % (capacity/mean) -1;
        //save the allocations to reference the master buffer
        allocations = master -> allocations;
        //loop through the allocations until it is found
        for (int j = 0; j < choice; j++){
            if (allocations -> next == NULL)
                break;
            allocations = allocations -> next;
        }
        //deallocate the block
        deallocateBlock(allocations->start, master);
        //pick a random block size
        int randSize = rand() % (2*mean) + 1;
        //allocate a block of chosen size
        if (allocateBlock(randSize, ALLOC_FIRST_FIT, master) == -1)
            failureCount ++;
        //check if there are any errors
        validAllocGeneral(master);

        //re-initialize variables
        currNode = master->holes;
        holeSum = 0;
        holeCount = 0;
        //loop through the holes to find the sum of all holes and total count
        while (currNode != NULL) {
            holeSum = holeSum + currNode->size;
            holeCount ++;
            currNode = currNode->next;

        }
        FFholeSize[i] = (float)(holeSum)/holeCount;
    }

    //print the values for the array
    printf("\nMean Hole Size for First Fit:\n");
    for(int i = 0; i < N; i++)
        printf("%.2f, ", FFholeSize[i]);
    printf("\nfirst fit failure count: %d\n", failureCount);



    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~DEALLOCATE-ALLOCATE CYCLES FOR WORST-FIT~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //create empty array to hold the mean hole size
    float WFholeSize[N];
    //vset variables back to 0
    failureCount = 0;
    //loop through the indicated amount
    for (int i = 0; i < N; i++){
        //pick a random number for the allocation block
        int choice = rand() % (capacity/mean) -1;
        //save the allocations to reference the master buffer
        allocations = master -> allocations;
        //loop through the allocations until it is found
        for (int j = 0; j < choice; j++){
            if (allocations -> next == NULL)
                break;
            allocations = allocations -> next;
        }
        //deallocate the block
        deallocateBlock(allocations->start, master);
        //pick a random block size
        int randSize = rand() % (2*mean) + 1;
        //allocate a block of chosen size
        if (allocateBlock(randSize, ALLOC_WORST_FIT, master) == -1)
            failureCount ++;
        //check if there are any errors
        validAllocGeneral(master);

        //re-initialize variables
        currNode = master->holes;
        holeSum = 0;
        holeCount = 0;
        //loop through the holes to find the sum of all holes and total count
        while (currNode != NULL) {
            holeSum = holeSum + currNode->size;
            holeCount ++;
            currNode = currNode->next;

        }
        WFholeSize[i] = (float)(holeSum)/holeCount;
    }

    //print the values for the array
    printf("\nMean Hole Size for Worst Fit:\n");
    for(int i = 0; i < N; i++)
        printf("%.2f, ", WFholeSize[i]);
    printf("\nworst fit failure count: %d\n", failureCount);
    return 0;
}

int allocateBlock(int size, AllocType allocType, AllocInfo *allocInfo) {
    switch (allocType) {
        case ALLOC_BEST_FIT: {
            //if the allocation list is empty you simply allocate to the beginning
            if (allocInfo -> allocations == NULL){
                addAllocation(0, size, allocInfo);
                updateHole(size, allocInfo -> holes, allocInfo);
                return 0;
            }
            //save temporary node for looping
            BlockListNode *currNode = allocInfo->holes;
            //save temporary node for finding the best hole to use
            BlockListNode *bestSoFar = NULL;
            //set the initial difference as larger than the capacity
            int smallestDiff = 3000;
            //loop through the nodes until you run out
            while (currNode != NULL) {
                //if the size of the node could accomodate the requested block
                if (currNode->size >= size) {
                    int diff = currNode->size - size; //find the difference is size
                    //if the difference is smaller than the previous minimum save it as the best node
                    if (diff < smallestDiff) {
                        bestSoFar = currNode;
                        smallestDiff = diff;
                    }
                }
                //move down the list
                currNode = currNode->next;
            }
            //if the best hole does not exist, exit and do not create an allocation
            if (bestSoFar == NULL) {
                return -1;
            }
                //otherwise go forward
            else {
                //add an alocation of the required specs
                addAllocation(bestSoFar->start, size, allocInfo);
                //update the hole used so the start/size line up
                updateHole(size, bestSoFar, allocInfo);

                return (bestSoFar->start);

            }
        }

        case ALLOC_FIRST_FIT: {
            //if the allocation list is empty you simply allocate to the beginning
            if (allocInfo -> allocations == NULL){
                addAllocation(0, size, allocInfo);
                updateHole(size, allocInfo -> holes, allocInfo);
                return 0;
            }
            //save temporary node for looping
            BlockListNode *currNode = allocInfo->holes;
            //save temporary node for finding the first hole to use
            BlockListNode *bestSoFar = NULL;
            //loop through the nodes until you find one that fits
            while (currNode != NULL ) {
                //if the size of the node could accommodate the requested block
                if (currNode->size >= size) {
                    bestSoFar = currNode;
                    break; //exit the while loop
                }
                //move down the list
                currNode = currNode->next;
            }
            //if hole is not big enough return an error and do not allocate
            if (bestSoFar == NULL) {
                return -1;
            }
            //otherwise go forward
            else {
                //add an alocation of the required specs
                addAllocation(bestSoFar->start, size, allocInfo);
                //update the hole used so the start/size line up
                updateHole(size, bestSoFar, allocInfo);

                return (bestSoFar->start);
            }
        }

        case ALLOC_WORST_FIT: {
            //if the allocation list is empty you simply allocate to the beginning
            if (allocInfo -> allocations == NULL){
                addAllocation(0, size, allocInfo);
                updateHole(size, allocInfo -> holes, allocInfo);
                return 0;
            }
            //save temporary node for looping
            BlockListNode *currNode = allocInfo->holes;
            //save temporary node for finding the worst hole to use
            BlockListNode *worstSoFar = NULL;
            //set the initial difference as 0 to keep track of the largest hold
            int biggestDifference = 0;
            //loop through the nodes until you run out
            while (currNode != NULL) {
                //if the size of the node could accomodate the requested block
                if (currNode->size >= size) {
                    int diff = currNode->size - size; //find the difference is size
                    //if the difference is greater than the previous max save it as the best node
                    if (diff > biggestDifference) {
                        worstSoFar = currNode;
                        biggestDifference = diff;
                    }
                }
                //move down the list
                currNode = currNode->next;
            }
            //if a hole is not big enough, exit and do not create an allocation
            if (worstSoFar == NULL) {
                return -1;
            }
                //otherwise go forward
            else {
                //add an alocation of the required specs
                addAllocation(worstSoFar->start, size, allocInfo);
                //update the hole used so the start/size line up
                updateHole(size, worstSoFar, allocInfo);

                return (worstSoFar->start);
            }
        }
    }
}


int deallocateBlock(int location, AllocInfo *allocInfo){
    //create variable to store the size of the node
    int size;
    //variable to track if an allocation is found
    bool found = false;
    //exit if the master buffer or the allocations list are null
    if (allocInfo != NULL) {
        if (allocInfo -> allocations != NULL) {
            //create variable to hold the alocations in allocInfo
            BlockListNode *allocationList = allocInfo->allocations;
            //check if there is actually a block with the start value
            while (allocationList != NULL){
                if (allocationList -> start == location){
                    found = true;
                    break;
                }
                allocationList = allocationList -> next;
            }
            //if there is no allocation in the list with the start value return an error
            if(!found){
                return -1;
            }
            allocationList = allocInfo -> allocations;
            //if the top node is the node we are looking for
            if (allocationList->start == location) {
                size = allocationList -> size;
                //move the list down which removes the first node
                allocInfo->allocations = allocInfo->allocations->next;
                //make the next pointer point to null removing it from the list
                allocationList->next = NULL;
                //create a hole to take the previous location
                addHole(location, size, allocInfo);
                return 0;
            }

            //loop through until the next node matches the start time
            while (allocationList->next->start != location) {
                //exit loop if the next next is null
                if (allocationList -> next -> next == NULL){
                    break;
                }
                //move down the list
                allocationList = allocationList->next;
            }
            //create an object to hold the node that needs to be removed
            BlockListNode *removeNode;
            removeNode = allocationList -> next;
            size = removeNode -> size;
            //have the original list point to the removal nodes next
            allocationList->next = removeNode->next;
            //have the removed node point to null
            removeNode->next = NULL;
            //create a hole to take the place of the block
            addHole(location, size, allocInfo);
            return 0;
        }
        return -1;
    }
    return -1;
}

int addHole(int start, int size, AllocInfo *allocInfo){
    //create a temporary node to hold all of the info
    BlockListNode *temp = (BlockListNode*)malloc(sizeof(BlockListNode));
    //create a temporary node for checking adjacent holes
    BlockListNode *holeTemp = (BlockListNode*)malloc(sizeof(BlockListNode));
    //create a temporary node to hold info of node if found to be adjacent
    BlockListNode *leftHole = (BlockListNode*)malloc(sizeof(BlockListNode));
    BlockListNode *rightHole = (BlockListNode*)malloc(sizeof(BlockListNode));

    //boolean values for checking if there are holes next to each other
    bool onLeft = false;
    bool onRight = false;

    if (allocInfo -> holes == NULL){
        //assign values for the node
        temp -> start = start;
        temp -> size = size;
        temp -> next = NULL; //have the node point to null
        //update the holes list with the new hole at the beginning
        allocInfo -> holes = temp;

        return 0;
    }
    else{
        //loop through the holes to see if there is one to the left of the proposed
        leftHole = allocInfo -> holes;
        //check if the current node's finish spot matches the proposed starting location
        while(leftHole -> start + leftHole -> size != start){
            //if the next node is null break the loop
            if (leftHole -> next == NULL){
                break;
            }
            leftHole = leftHole -> next;
        }
        //check if the values align (this allows the last node to also be checked)
        if(leftHole -> start + leftHole -> size == start){
            onLeft = true;
        }

        //loop through the holes to see if there is one to the right of the proposed
        rightHole = allocInfo -> holes;
        //check if the current node's start spot matches the proposed ending location
        while(rightHole -> start !=  start + size){
            //if the next node is null break the loop
            if (rightHole -> next == NULL){
                break;
            }
            rightHole = rightHole -> next;
        }
        //check if the values align (this allows the last node to also be checked)
        if(rightHole -> start == start + size){
            onRight = true;
        }
        //if there is a hole on the left but not the right
        if(onLeft && !onRight){
            //increase the size and keep the starting point (fills in the proposed gap)
            leftHole -> size = leftHole -> size + size;
        }
        //if there is a hole on the right but not on the left
        else if(onRight && !onLeft){
            //move back the starting point and increase the size
            rightHole -> start = start;
            rightHole -> size = rightHole -> size + size;
        }
        //if there are holes on both left and right
        else if(onLeft && onRight){
            //expand the left hole to cover the proposed gap and the right hole
            leftHole -> size = leftHole -> size + size + rightHole -> size;
            //delete the right hole
            deleteHole(rightHole -> start, allocInfo);
        }
        //otherwise there is no conjoined holes
        else {
            //assign values for the node
            temp -> start = start;
            temp -> size = size;
            temp -> next = allocInfo -> holes; //have the node point to the previous holes object
            //update the holes list with the new hole at the beginning
            allocInfo -> holes = temp;
        }
        return 0;
    }
}

int deleteHole(int start, AllocInfo *allocInfo){
    //exit the function if the master buffer or the holes list is null
    if (allocInfo != NULL) {
        if (allocInfo -> holes != NULL) {
            //create variable to hold the holes in allocInfo
            BlockListNode *holeList = allocInfo->holes;
            //if the top node is the node we are looking for
            if (holeList->start == start) {
                //move the list down which removes the first node
                allocInfo->holes = allocInfo->holes->next;
                //make the next pointer point to null removing it from the list
                holeList->next = NULL;
                return 0;
            }

            //loop through until the next node matches the start time
            while (holeList->next->start != start) {
                if (holeList -> next == NULL) {
                    break;
                }
                holeList = holeList->next;
            }

            //create an object to hold the node that needs to be removed
            BlockListNode *removeNode = holeList->next;
            //have the original list point to the removal nodes next
            holeList->next = removeNode->next;
            //have the removed node point to null
            removeNode->next = NULL;
            return 0;
        }
        return -1;
    }
    return -1;
}


int addAllocation(int start, int size, AllocInfo *allocInfo) {
    //create a temporary node to hold all of the info
    BlockListNode *temp = (BlockListNode*)malloc(sizeof(BlockListNode));
    //save the current allocInfo in case adding the hole creates a problem
    AllocInfo *current = allocInfo;
    if (current -> allocations == NULL){
        //assign values for the node
        temp -> start = start;
        temp -> size = size;
        temp -> next = NULL; //have the node point to the previous holes object
        //update the holes list with the new hole at the beginning
        allocInfo -> allocations = temp;
        return 0;
    }
    else{
        //assign values for the node
        temp -> start = start;
        temp -> size = size;
        temp -> next = allocInfo -> allocations; //have the node point to the previous holes object
        //update the holes list with the new hole at the beginning
        allocInfo -> allocations = temp;
        return 0;
    }

}

void updateHole(int requestSize, BlockListNode *holeNode, AllocInfo *info){
    //create a temporary node to hold all of the info
    BlockListNode *holeTemp = info -> holes;
    //loop until you find the right hole
    while (holeTemp != holeNode ){
        holeTemp = holeTemp -> next;
    }
    //store the new start and size values
    int newStart = (holeTemp -> start) + requestSize; //start moves down by the size
    int newSize= (holeTemp -> size) - requestSize; //size decreases by the size

    //set the values to the node
    holeNode -> start = newStart;
    holeNode -> size = newSize;

    //if the new node size is 0 just delete it
    if(holeTemp -> size == 0) {
        deleteHole(holeTemp -> start, info);
    }

}

int validAllocGeneral(AllocInfo *allocInfo) {
    int allocSum, holeSum, idx, found;
    BlockListNode *currNode, *holeNode, *allocNode;
    int lookingFor;
    int HOLE = 0;
    int ALLOC = 1;
    int EITHER = 2;

    allocSum = 0;
    holeSum = 0;

    currNode = allocInfo->allocations;
    while (currNode != NULL) {
        allocSum = allocSum + currNode->size;
        currNode = currNode->next;
    }

    currNode = allocInfo->holes;
    while (currNode != NULL) {
        holeSum = holeSum + currNode->size;
        currNode = currNode->next;
    }

    if (allocSum + holeSum != allocInfo->capacity) {
        printf("ERROR: capacity = %d, allocSum = %d, holeSum = %d\n",
               allocInfo->capacity, allocSum, holeSum);
        return 1;
    }

    idx = 0;

    found = 0;
    holeNode = allocInfo->holes;
    allocNode = allocInfo->allocations;

    // try to find idx=0 in either the holes list or the allocs list
    currNode = allocInfo->holes;

    while (currNode != NULL && currNode->start != idx)
        currNode = currNode->next;

    if ( currNode != NULL ) {
        // found idx=0 in holes list; next thing better be an alloc
        lookingFor = ALLOC;
        idx = idx + currNode->size;
    } else {
        // idx=0 is not in holes list; better be in allocs list!
        currNode = allocInfo->allocations;
        while (currNode != NULL && currNode->start != idx)
            currNode = currNode->next;
        if ( currNode != NULL ) {
            // found idx=0 in alloc list; next thing can be a hole
            // or another allocation
            lookingFor = EITHER;
            idx = idx + currNode->size;
        } else {
            printf("ERROR: cannot find idx %d in either holes or allocations\n", idx);
            return 1;
        }
    }

    while (idx < allocInfo->capacity) {
        if (lookingFor == HOLE) {
            currNode = allocInfo->holes;
            while (currNode != NULL && currNode->start != idx)
                currNode = currNode->next;
            if (currNode == NULL) {
                printf("ERROR: can't find expected idx %d as start value in holes list \n", idx);
                return 1;
            } else {
                // only alloc can follow hole
                lookingFor = ALLOC;
                idx = idx + currNode->size;
            }
        } else if (lookingFor == ALLOC) {
            currNode = allocInfo->allocations;
            while (currNode != NULL && currNode->start != idx)
                currNode = currNode->next;
            if (currNode == NULL) {
                printf("ERROR: can't find expected idx %d as start value in allocations list \n", idx);
                return 1;
            } else {
                // hole or another alloc can follow alloc
                lookingFor = EITHER;
                idx = idx + currNode->size;
            }
        } else {
            // look for either one: must look in one list and then the other
            currNode = allocInfo->holes;
            while (currNode != NULL && currNode->start != idx)
                currNode = currNode->next;
            if (currNode != NULL) {
                // only alloc can follow hole
                lookingFor = ALLOC;
                idx = idx + currNode->size;
            } else {
                // didn't find in holes; look in allocs
                currNode = allocInfo->allocations;
                while (currNode != NULL && currNode->start != idx)
                    currNode = currNode->next;
                if (currNode == NULL) {
                    printf("ERROR: expected either a hole or another allocation starting at idx = %d\n", idx);
                    return 1;
                } else {
                    // hole or another alloc can follow alloc
                    lookingFor = EITHER;
                    idx = idx + currNode->size;
                }
            }
        }
    } // while we haven't gone all the way through the list

    // separate test: check for consecutive holes
    currNode = allocInfo->holes;
    while (currNode != NULL) {
        idx = currNode->start + currNode->size;
        if (idx < allocInfo->capacity) {
            holeNode = allocInfo->holes;
            found = 0;
            while ( ! found && holeNode != NULL) {
                if (holeNode->start == idx)
                    found = 1;
                else
                    holeNode = holeNode->next;
            }
            if (found) {
                printf("ERROR: found two adjacent holes: (%d, %d) and (%d, %d)\n",
                       currNode->start, currNode->size, holeNode->start, holeNode->size);
                return 1;
            }
        }
        currNode = currNode->next;
    }

    // note: it's OK to have two adjacent allocations

    return 0;
}
