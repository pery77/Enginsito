'[
DO      -> Begins a loop that repeats a block of code until a certain condition is true.
UNTIL   -> Specifies the condition for exiting a "DO" loop.
EXIT    -> Terminates a loop prematurely.
']

let i = 0
do
	print i
	i = i+1
until i>10

i = 0
do
	print i
	i = i+1
	if i = 4 then exit
until i>10