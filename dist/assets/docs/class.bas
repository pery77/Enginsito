'[
CLASS   -> Begins the definition of a user-defined class.
ENDCLASS-> Marks the end of a user-defined class.
ME      -> Refers to the current instance of a class.
NEW     -> Creates a new instance of a class.
VAR     -> Used to declare a variable within a class.
IS      -> Compare object with type

REFLECT -> Provides reflection capabilities to access and manipulate class members dynamically.
MEM     -> Use MEM to tell how much memory in bytes has
			been allocated by MY-BASIC.
TYPE    -> The TYPE statement tells the type of a value, or generates type
			information with a predefined type string

The GET statement can be also applied to a class instance to get a member of
 it. It results the value of a field variable or the routine object of a sub routine:

print get(foo, "A");   ' Result the value of "A"
print get(foo, "FUN"); ' Result the routine object

The SET statement can be applied to a class instance to set the value of 
 a member variable:

set(foo, "A", 42)
print get(foo, "A"); ' Result 42


']


class vehicle
	var wheels = 4
	var sound = "brrr"
	def start()
		print "I have " + str(wheels) + " wheels"
		print sound
	enddef
endclass

class motorbike(vehicle) ' inheriting
	var wheels = 2
endclass

print "motorbike"
mybike = new(motorbike)
mybike.start()

print "car"
mycar = new(vehicle)
mycar.sound = "RRRRrrrrruunnn"
mycar.start()

print "Reflection"
r = reflect(mycar)
f = iterator(r)
while move_next(f)
	k = get(f)
	v = r(k)
	print k , v;
wend
g = get(mycar, "start")
g()

print mem

print type(123), type("Hi"); 	' Get types of values
print type("INT"), type("REAL") ' Get specific types
print type(mycar)				' print CLASS
print mycar is type("CLASS") 	' true because mycar is a class