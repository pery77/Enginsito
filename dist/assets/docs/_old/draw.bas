t = 0
def draw()
	cls(0)
	
	for x = 5 to 30
		pixel(x,100 + x mod 3, x mod 3 + (frame*0.1) mod 5)
	next
	
	for x = 0 to 320 step 10
		line(x,0,320-x,200,1,4)
	next
	
	circle(40,130,15,1,15)
	circle(40,130,10,0,4)
	circle(40,130,6,1,0)
	
	ring(0,100,80,100,0,320,20,0,8)
	ring(0,100,80,100,0,320,20,1,10)
	
	ellipse(250,80,sin(frame*0.02)*20+30,cos(frame*0.03)*20+30,1,6)

	triangle(200,100,190,110,210,110,0,11)
	
	rect(0,180,320,13,0,1)
	rectround(5,180,310,13,8,5,2,2)
	
	text("Some fancy text",320 - t,183,1,3)
	t=t+1
	if t>500 then t = 0
	
	for l = 0 to 39
		sprite(0,l*8,0,3)
		sprite(0,l*8,8,2)
	next
	
enddef