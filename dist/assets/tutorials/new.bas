time = 0
pi = 3.1415

def draw()

	cls(0)
	r=30
	n = mouse.x()/16
	for a = 0 to n
		an = 2 * pi * a / n + sin(time*0.01)
		x = r * cos(an)+160
		y = r * sin(an)+100
		draw.circle(x,y,sin(time*0.01)*5+11,1,a)
	next
	
	time = time + 10

enddef

