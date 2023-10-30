'[
WHILE   -> Begins a loop that repeats a block of code while a certain condition is true.
WEND    -> Marks the end of a "WHILE" loop.
EXIT    -> Terminates a loop prematurely.
']
let i = 0

while i<10
	print i
	i = i+1
	if i = 4 then exit
wend