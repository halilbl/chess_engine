\# Mapping large numbers to relatively small numbers in an interval, collision-free



I faced this problem when I needed to map sliding piece occupancy mask subsets to indexes. Why do it this way? Why not

just store the data and traverse through the 2D array during move generation? This is because the move generation phase must

be optimized enough in terms of time complexity that the user won't have to wait a long time to get an answer from

the engine. By mapping subsets (unsigned 64-bit integers) to smaller integers (indexes), we can apply the same bitwise

operations to any subset to find its index in O(1) time, and then look up the designated value in the table in O(1) time.



\## Problem 1:

The first problem is finding the operation needed to avoid index collisions, which would cause overwrites. If there is an index collision, then some parameter of the index calculation must change. The parameters are: the magic bitboard, the subset, the number of squares on a chess board and the population count of the square's occupancy mask. Let's take a look

at what we can or can't change, and why.



\### Magic bitboard:

If we change the magic bitboard when we face a collision, we leave the other subsets that were previously mapped to an

index hanging. This can be explained with a dangling pointer analogy. When you free a block of memory that was allocated

and is still being pointed to by a pointer, that pointer turns into a dangling pointer because it no longer

points to memory that belongs to us. By analogy, when you change the magic number that a subset used

to compute its index, you can no longer reach that index with the same bitwise operation. You obtained the index with the old magic bitboard, but now that you have altered the magic bitboard, you can no longer obtain the same index with the new one.



\### Subset, number of squares, population count:

These parameters can't be controlled variables, since they depend on the features of chess and hence can't be changed.



\## Problem 2:

I tried things like going back and forth to find an empty slot and storing the offset required to move a subset into that slot,

but implementations like these caused circular dependencies over and over again.



\## Solution:

I added a brute-force magic bitboard generator algorithm. This algorithm checks for index collisions right after a magic bitboard is generated and before it is stored in the bishop\_magic array. If there is a collision, we throw everything

away and start working on a new magic bitboard.

