REM Global 
def playSound(s)
	ch_set(2,s)
	ch_play(2) 
enddef

currentScore = 0
lifes = 3
blocks = list()
currentLevel = 0
nextLevel = 0

MENU 	 = 0
LAUNCH 	 = 1
GAME 	 = 2
GAMEOVER = 3
WIN 	 = 4

state = MENU
launchTime = 0

def getHi()
	hc = peek(0xfe0) * 256 + peek(0xfe1)
	return hc
enddef

def setHi()
    poke(0xfe0, currentScore / 256)
    poke(0xfe1, currentScore mod 256)
enddef

def launchGame()
	launchTime = 0
	state = LAUNCH
enddef

def loseLife()
	lifes = lifes - 1
	if lifes < 0 then 
		state = GAMEOVER
	else
		launchGame()
	endif
enddef

def start()
	blocks = list()
	state = MENU
	lifes = 3
	currentScore = 0
	currentLevel = 0
	nextLevel = 0
	loadLevel()
enddef

levels = list()
push(levels, list(0,0,8190,8190,8190,8190))
push(levels, list(9417,4626,2340,1224,1560,2340,4290,9945))
push(levels, list(1008,1032,1320,1032,1320,1512,1032,1008))
push(levels, list(9417,4626,2340,1224,1560,2340,4290,9945))
push(levels, list(192,480,2772,6138,1008,1008,1224,2340))

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
				if getHi() < currentScore then setHi()
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
	sWallHit = "@0v40L16o5C>v20g"
	padHit = "@0v100L16o4gv50c"
	brickHit = "@0v100L32o7g"

	def setup()
		x = pad.x + pad.size * 4 + 4
		y = 178
		dy = -2
		dx = rnd(-10,10) * 0.1
	enddef

	def update()
		if dx < -2.5 then dx = -2.5
		if dx > 2.5 then dx = 2.5
		y = y + dy
		x = x + dx
		if y>190 then 
			loseLife()
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

gamePad = new(pad)
gameBall = new(ball)

def loadLevel()
	blocks = list()
	lvl =  levels(currentLevel)
	it = iterator(lvl)
	l = 0
	while move_next(it)
		dec = get(it)
		b = 0
		for p = 0 to 13
			r = dec mod 2
			if r = 1 then
				cb = new(block)
				cb.x = b*16+12
				cb.y = l*8+24
				cb.col = l+8
				cb.mustDelete = 0
				push(blocks,cb)
			endif
			dec = floor(dec / 2)
			b = b + 1
		next
		l = l + 1
	wend
enddef

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

REM Draws
def drawScore()
	sc =formatText("%05i00",currentScore)
	hsc =formatText("%05i00",getHi())
	text("8888888",256,16,1,1)
	text(sc,256,16,1,3)
	text("Score",256,26,1,10)
	text("8888888",256,56,1,1)
	text(hsc,256,56,1,3)
	text("h-Score",256,66,1,6)
enddef

def drawLifes()
	for l = 0 to 2
	col = 13
	if l>=lifes then col = 1
	sprite(4,260+l*16,180,col)
	sprite(4,268+l*16,180,col,8)
	next
enddef

REM main loops
def tick()
	if state = MENU then
		if key_released(257) then
			currentLevel = 0
			loadLevel()
			launchGame()
		endif
	endif

	if state = LAUNCH then
		launchTime = launchTime + 1
		gameBall.setup()
		gamePad.update()
		if launchTime > 60 then
			launchTime = 0
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
				if len(blocks) = 0 then
					nextLevel =  currentLevel + 1
				endif
			endif
		next
		if nextLevel > currentLevel then
			currentLevel = nextLevel
			if currentLevel < len(levels) then
				loadLevel()
				launchGame()
			else
				state = WIN
			endif
		endif
	endif

	if state = GAMEOVER or state = WIN then
		if key_released(257) then
			start()
		endif
	endif
enddef

def draw()
	if state = GAMEOVER or state = WIN then
		msg = "GAME OVER"
		if state = WIN then msg = "YOU WIN!!"
		text(msg,56,110,2,sin (frame*0.1)+2)
		text("Press enter",84,126,1,sin (frame*0.1)+6)
	else
		cls(0)
	endif

	if state = MENU then
		text("zNoi",112,20,3,6)
		line(0,16,320,16,3,6)
		line(0,44,320,44,3,6)
		text("Press enter",116,160,1,sin (frame*0.14)+4)
	endif

	if state = GAME or state = LAUNCH then
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