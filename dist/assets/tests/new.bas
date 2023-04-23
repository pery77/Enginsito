PI = 3.14159
t= 0
print PI
def draw()
	cls(0)
	draw.rect(0,0,319,199,4,15,0)
	for a = 0 to 15	
		draw.circle(a*10+mouse.x(),mouse.y()+sin(t)*a*20,a*20,1,a)
		t=t+1*0.001
		print t
	next
	
enddef





