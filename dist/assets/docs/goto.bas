'GOTO  -> Transfers the program execution to a specified line number or label.
'Label -> A word finished with : ex. jump:

print "hello"

goto jump

'never print this because program jump to line labeled with jump
print 2+2

jump:
print "bye"