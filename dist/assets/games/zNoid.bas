REM Global 
def playSound(s)
	ch_set(2,s)
	ch_play(2) 
enddef

currentScore = 0
lifes = 3

MENU = 0
GAME = 1
GAMEOVER = 2

state = MENU

REM Classes
class pad
	x = 108
	y = 180
	col = 14
	size = 3
	
	speed = 0
	acc = 0.5
	dec = 0.93
	
	def update()
		s = (1+size)*8
		'Ball collision
		if gameBall.y > y-2 then
			if gameBall.x > x and gameBall.x < s + x then
				v = (gameBall.x - x - s/2)/7
				gameBall.hit(v)
				playSound(gameBall.padHit)
			endif
		endif
		'Inputs
		if key_down(65) or key_down(263) then
			speed = speed - acc
		endif
		if key_down(68) or key_down(262) then
			speed = speed + acc
		endif
		
		x = x + speed 
		
		'walls collision
		xlimit = 232 - (8 * size)
 
		if x < 8 then 
			x = 8
			speed = abs(speed * 0.5)
		endif
		if x > xlimit then 
			x = xlimit
			speed = -abs(speed * 0.5)
		endif
		
		speed = speed * dec
	enddef
	
	def draw()
		rect(x,y+1,(1+size)*8,6,0,15)
		sprite(0,x,y,col)
		for i = 0 to size-2
			sprite(1,x+(i*8)+8,y,col)
		next
		sprite(0,x+8*size,y,col,8)
	enddef
endclass

class block
	x = 0
	y = 0
	col = 8
	mustDelete = 0
	def update()
		'Ball collision
		if gameBall.x > x and gameBall.x < 17 + x then
			if gameBall.y > y and gameBall.y < y+8 then
				v = (gameBall.x - x - 8)/20
				gameBall.hit(v)
				playSound(gameBall.brickHit)
				currentScore = currentScore + 1
				mustDelete = 1
			endif
		endif

	enddef
	def draw()
		rect(x+1,y+1,14,6,0,col)
		sprite(5,x,y,2)
		sprite(5,x+8,y,2,2)
	enddef
endclass

class ball
	x = 124
	y = 160
	dy = -2
	dx = 0.0
	sWallHit = "@0v100L16o5C>g"
	padHit = "@0v100L16o4gv50c"
	brickHit = "@0v100L32o7g"
	def update()
		if dx < -2.5 then dx = -2.5
		if dx > 2.5 then dx = 2.5
		y = y + dy
		x = x + dx
		if y>190 then 
			dy = dy * -1
		endif
		if y < 10 then 
			dy = dy * -1
			playSound(sWallHit)
		endif
		if x < 10 then
			dx = dx * -1
			playSound(sWallHit)
		endif
		if x>238 then
			dx = dx * -1
			playSound(sWallHit)
		endif
	enddef
	def draw()
		circle(x,y,2,0,3)
	enddef
	
	def Hit(vx)
		dy = dy * -1
		dx = dx + vx
	enddef
	
endclass

REM Draw UI
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

def background()
	for x = 0 to 14
		for y = 0 to 11
			meta(0,x*16,y*16)
		next
	next
enddef 

gamePad = new(pad)
gameBall = new(ball)


blocks = list()
for l = 0 to 5
	for b = 0 to 13
		cb = new(block)
		cb.x = b*16+12
		cb.y = l*8+32
		cb.col = l+8
		push(blocks,cb)
	next
next

REM Draws
def drawScore()
	sc =formatText("%05i00",currentScore)
	text("8888888",256,16,1,1)
	text(sc,256,16,1,3)
	text("Score",256,26,1,10)
	text("8888888",256,116,1,1)
	text("h-Score",256,126,1,6)
enddef

def drawLifes()
	for l=0 to lifes-1
	sprite(4,260+l*16,180,13)
	sprite(4,268+l*16,180,13,8)
	next
enddef

REM main loops
def tick()
	if state = MENU then
		if key_released(257) then
			state = GAME
		endif
	endif 
	if state = GAME then
		gamePad.update()
		gameBall.update()
		for b in blocks
			b.update()
			if b.mustDelete then
				remove(blocks, index_of(blocks, b))
			endif
		next
	endif
enddef

def draw()
	cls(0)

	if state = MENU then
		text("zNoi",112,20,3,6)
		line(0,16,320,16,3,6)
		line(0,44,320,44,3,6)
		text("Press enter",116,160,1,sin (frame*0.14)+4)
	endif

	if state = GAME then
		background()
		drawBorder()
		drawScore()
		drawBlocks()
		drawLifes()
		gameBall.draw()
		gamePad.draw()
		
		for b in blocks
			b.draw()
		next
	endif

enddef