'DEF     ->  Begins the definition of a user-defined function.
'ENDDEF  ->  Marks the end of a user-defined function.
' You need to declare a function BEFORE call it.
a = 1
b = 0

def fun(d)
	d = call bar(d) ' you can use call inside a function for not declared yet.
	sin(10)
	return d ' Try comment this line
enddef

def foo(b)
	a = 2
	return a + b
enddef

def bar(c)
	return foo(c)
enddef

r = fun(2 * 5)
print r, a, b, c

'Variadic
def addtext(a, b, ...)
	return a + " " + b + " " + ... + ...
enddef

def vari(...)
	return addtext(...)
enddef

print vari("a", "b", "c", "d");