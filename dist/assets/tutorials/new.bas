time = 0
pi = 3.1415
input "What is your name: ", n$

def draw()

	cls(0)
	r=60
	n = 12
	for a = 0 to n
		an = 2 * pi * a / n + sin(time*0.01)
		x = r * cos(an)+160
		y = r * sin(an)+100
		draw.circle(x,y,sin(time*0.01)*5+11,1,4)
	next
	
	time = time + 1

enddef







