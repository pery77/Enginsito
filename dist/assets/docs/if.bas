' Conditional Example

' Define two boolean variables for demonstration purposes
true1 = 0
true2 = 1

' Check the first condition using an 'if' statement
if true1 then
	print "true1"  ' If true1 is true (non-zero), this block will be executed.
' Check the second condition using 'elif' (else if) statement
elif true2 then
	print "true2"  ' If true1 is false and true2 is true (non-zero), this block will be executed.
' Handle the case when all conditions are false
else
	print "all false"  ' If both true1 and true2 are false (zero), this block will be executed.
endif