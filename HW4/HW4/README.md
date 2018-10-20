#Design overview:

Implemented approach 2 with a single free list managed by a large call to mmap during program start up.
Used a linkedlist t_block to manage memory allocation with capability to split block to fit requested size and merge neraby blocks together to reduce fragmentation


#Design decisions 
Initial implemented memory allocation using sbrk to switched to mmap for better controll over memory manipulation

#Known bugs and errors
Know that I am aware of
