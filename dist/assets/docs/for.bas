'[
FOR     -> Begins a loop that repeats a block of code for a specified number of times.
IN      -> Used in conjunction with the "FOR" keyword to specify the range or collection to iterate over.
TO      -> Specifies the upper limit of the range in a "FOR" loop.
STEP    -> Specifies the increment or decrement value for each iteration in a "FOR" loop.
NEXT    -> Marks the end of a "FOR" loop and proceeds to the next iteration.
']

for a = 0 to 10 step 2
	print a
next

n = list("one", "dos", 3, "KVAR")
for b in n
	print b
next