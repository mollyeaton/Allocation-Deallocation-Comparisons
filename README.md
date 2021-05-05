# Allocation-Deallocation-Comparisons

## This is a project to simulate the allocation and deallocation of memory within an internal buffer using three main methods: best-fit, worst-fit, and first-fit. 

A master buffer is created and then filled/emptied with block node structs. By using different memory management methods for allocating and deallocating space within a buffer one can compare the resulting sizes of holes (chunks of unused memory). It can generally be said that the resulting buffer with the lowest amount of holes is the most efficient at dynamic memory management.

Each node struct stores keeps track of it's starting cell within the buffer as well as it's own size. The program also performs a number of vallidation checks and the many helper functions needed for memory management. More detailed explainations of the functions can be found within the header file.
