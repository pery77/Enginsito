'[
SRND     -> ' Sets the seed for the random number generator.
RND      -> ' Returns a random float number between [0, max] with
            ' RND(max), or [MIN, MAX] with RND(min,max). (INTERGERS)
']

srnd(0) 'seed for the random, un-comment this line for force same random each run

'this loop generate 6 random numbers between 1 and 6
for n = 0 to 5
	'print rnd(5)+1 ' from 0 to 5 + 1 at end so, (1,6)
	print rnd(1,6)	' same of above (1,6) inclusive
next