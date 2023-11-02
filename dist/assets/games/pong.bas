REM ------ TOOLS ------
def drawField()
	line(0,0,320,0,4,1)
	line(0,200,320,200,4,1)
	line(160,0,160,200,2,1)
enddef

def clamp(v,min,max)
	if v<= min then return min
	if v>= max then return max
	return v
enddef

def collision(rx,ry,rw,rh,cx,cy,cr)
	px = clamp(cx, rx, rx+rw)
	py = clamp(cy, ry, ry+rh)
	dis = sqr((px - cx))^2 + (py - cy)^2)
	return dis <= cr
enddef

REM **********************************
REM ***       CLASSES             ****
REM **********************************


REM --- BAL ---
class ball

	var x = 160
	var y = 100
	var dx = 3.2
	var dy = 1.5
	
	def update()
	
		x = x + dx
		y = y + dy
		
		if x > 314 then hitX()
		if x < 6 then hitX()

		if y > 194 then hitY()
		if y < 6 then hitY()
		
	enddef
	
	def draw()
		circle(x,y,3,0,3)
		circle(x,y,2,0,1)
	enddef
	
	def hitX()
		dx = dx * -1
	enddef
	
	def hitY()
		dy = dy * -1
	enddef
	
endclass

REM --- PAD ---
class pad
	var x = 0
	var y = 0
	var col = 3
	var upKey   = 81
	var downKey = 65
	
	def init(_x, _y)
		x = _x
		y = _y
	enddef
	
	def update()
	
		if keydown(upKey) then y = y - 3 
		if keydown(downKey) then y = y + 3
		
		if collision(x,y,6,32,theBall.x,theBall.y,6) then theBall.HitX()
			
		
	enddef
	
	def draw()
		rect(x,y,6,32,0,col)
	enddef
endclass


REM **********************************
REM ***     GLOBAL VARIABLES       ***
REM **********************************

theBall = new(ball)
pad1 = new(pad)
pad2 = new(pad)

pad1.init(4+30,84)
pad2.init(312-30,84)
pad2.upKey = 80
pad2.downKey =76




REM *********************************
REM ***        MAIN LOOPS         ***
REM *********************************

def tick()

	theBall.update()
	pad1.update()
	pad2.update()
	
enddef

def draw()

	cls(0)

	drawField()

	theBall.draw()
	pad1.draw()
	pad2.draw()
	
enddef