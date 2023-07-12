class star
	x = 0
	y = 0
	col = 4
	speed = 3
	def draw()
		x = x - speed * 0.01
		if x < 0  then x = 320
		pixel(x, y, col)
	enddef
endclass

class fire
	x = 0
	y = 0
	col = 1
	speed = 10
	size = 4
	
	def draw()
		x = x - speed * 0.06
		size = size - 0.1
		circle(x, y, size,0, size+5)
	enddef
	
endclass

starfield = list()
enginefire = list()

for a = 0 to 500
	s = new (star)
	s.x = rnd(0,320)
	s.y = rnd(0,200)
	s.col = rnd(1,6)
	s.speed = rnd(1,40)
	push(starfield, s)
next

def createFire(x,y)
	s = new (fire)
	s.x = x
	s.y = y
	s.size = rnd(2,5)
	push(enginefire, s)
enddef

for a = 0 to 100
createFire(rnd(100,120),a)
next
def drawShip(x,y)

	for s in engineFire
		s.draw()	
	next
	
	rect(x,y,20,8,1,2)
	if len(enginefire) then 
	    f = get(enginefire, 0) 
	    if f.size < 0 then
	        remove(enginefire, 0)
			createFire(x-2,y+4)
	    endif            
	endif
	
enddef

def draw()
	cls(0)
	for s in starfield
		s.draw()
	next
	
	drawShip(120,80)
enddef