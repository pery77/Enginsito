class pad
	x = 8
	y = 180
	col = 13
	size = 3
	
	def update()
	enddef
	
	def draw()
		rect(x,y+1,(1+size)*8,6,0,15)
		sprite(0,x,y,col)
		for i = 0 to size-2
			sprite(1,x*i+16,y,col)
		next
		sprite(0,x+8*size,y,col,8)
	enddef
endclass

class block
	x = 0
	y = 0
	col = 4
	def draw()
		sprite(5,x,y,col)
	enddef
endclass

class ball
	x = 100
	y = 60
	dy = 1
	def update()
		y = y + dy
		if y>190 then dy = -1
		if y < 8 then dy = 1
	enddef
	def draw()
		circle(x,y,2,0,3)
	enddef
endclass

def drawBorder()
	col = 7
	for x = 1 to 29
		sprite(2,x*8,0,col,2)
		sprite(2,x*8,192,col)
	next
	for y = 1 to 23
		sprite(2,0,y*8,col,1)
		sprite(2,240,y*8,col,3)
	next
	sprite(3,0,0,col)
	sprite(3,0,192,col,3)
	sprite(3,240,0,col,1)
	sprite(3,240,192,col,2)
enddef

gamePad = new(pad)
currentScore = 0
lifes = 3
gameBall = new(ball)


blocks = list()
for b = 0 to 10
	cb = new(block)
	cb.x = b*8 + 16
	cb.y = 32
next

def drawScore()
	sc =formatText("%05i00",currentScore)
	text("8888888",256,16,1,1)
	text(sc,256,16,1,3)
	text("Score",256,26,1,10)
enddef

def drawLifes()
	for l=0 to lifes-1
	sprite(4,260+l*16,180,13)
	sprite(4,268+l*16,180,13,8)
	next
enddef

def tick()
	gamePad.update()
	gameBall.update()
	currentScore = frame
enddef

def draw()
	cls(0)
	drawBorder()
	drawScore()
	drawBlocks()
	drawLifes()
	gameBall.draw()
	gamePad.draw()

enddef