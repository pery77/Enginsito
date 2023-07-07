let c = 0

def init()
	c = 1000
enddef

def tick()
	c = c + 1
enddef

def draw()
	cls(0)
	rect(0,0,320,200,2,2)
	text("Running" 10,10,2,3)
	text("Press ESC to pause.", 10,30,1,3)
	text(inttotext("Counter: %i",c), 10,180,1,3)
	circle(sin(frame * 0.05)*160+160,130,20,0,6)
enddef

def pause()
	rect(0,0,320,70,0,1)
	text("Game is paused", 10,10,2,3)	
	text("Press ESC again to end.", 10,30,1,3)
	text("Another key to resume.", 10,50,1,3)
enddef

def close()
	print inttotext("Exit counter: %i",c)
enddef