'The CALL statement is used to get a invokable routine value.
'Be aware it requires a pair of brackets comes along with a CALL 
'statement to get a invokable value, otherwise it calls the routine instantly.
'This mechanism is useful when you are tending 
'to store a sub routine value for later invoking.
def fun(msg)
	print msg
enddef

routine = call(fun) ' Get a routine value
routine("hello")    ' Invoke a routine value