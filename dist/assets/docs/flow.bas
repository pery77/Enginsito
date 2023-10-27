'[
INIT()     ->  When the program start
TICK()     ->  In each frame if not paused
DRAW()     ->  In each frame after tick if not paused
PAUSE()    ->  In each frame only in pause mode
CLOSE()    ->  When the program end

frame       ->  number of frame since game started
delta       ->  return the frame duration in seconds
']
let c = 0
fps = 0

def init()
	c = 1000
enddef

def tick()
	c = c + 1
	fps = 1/delta
enddef

def draw()
	cls(0)
	rect(0,0,320,200,2,2)
	text("Running" 10,10,2,3)
	text("Press ESC to pause.", 10,30,1,3)
	text(formatText("Counter: %i",c), 10,180,1,3)
	text(formatText("FPS %.2f", fps), 150,180,1,3)
	circle(sin(frame * 0.05)*160+160,130,20,4,6)
enddef

def pause()
	rect(0,0,320,70,0,1)
	text("Game is paused", 10,10,2,3)	
	text("Press ESC again to end.", 10,30,1,3)
	text("Another key to resume.", 10,50,1,3)
enddef

def close()
	print formattext("Exit counter: %i",c)
enddef