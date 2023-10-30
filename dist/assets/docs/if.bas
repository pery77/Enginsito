'[
IF      -> Begins a conditional statement that executes a block of code if a certain condition is met.
THEN    -> Specifies the code to be executed if the condition of an "IF" statement is true.
ELIF    -> Used within an "IF" statement to specify an additional condition to be checked if the previous condition was false.
ELSE    -> Specifies the code to be executed if none of the previous conditions in an "IF" statement were true.
ENDIF   -> Marks the end of an "IF" statement.
']
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