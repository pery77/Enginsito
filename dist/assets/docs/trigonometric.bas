'[
SIN  -> ' Returns the sine of an angle (in radians).
COS  -> ' Returns the cosine of an angle (in radians).
TAN  -> ' Returns the tangent of an angle (in radians).
ASIN -> ' Returns the arcsine of a number, resulting in an angle (in radians).
ACOS -> ' Returns the arccosine of a number, resulting in an angle (in radians).
ATAN -> ' Returns the arctangent of a number, resulting in an angle (in radians).
']

pi = 3.141593
print sin(pi/2)
print cos(pi)

def draw()
	cls(0)
	for n = 0 to 320 step 1
		x = n*10
		x1 = (n + 1)*10
		y = (cos(n+frame*0.1)*40)+100
		y1 =(cos((n+frame+1)*0.1)*80)+100
		line(x,y,x1,y1,1,3)
	next
enddef