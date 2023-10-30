' Setting variables
let radius = 10
let bx = 160
let by = 100
let gravity = 98
let velocityY = -100
let velocityX = 0
let itemPoint = 16
let itemX = 0
let itemY = 0
let itemSides = 6
let itemColor = 8
let score = 0
let time = 60

' Gamemodes
let GAME_OVER = 0
let PLAYING   = 1

let MODE = GAME_OVER

' Init and restart
def init()
	bx = 160
	by = 100
	velocityY = -100
	velocityX = 0
	score = 0
	time = 60
	itemX = rnd(20,300)
	itemY = rnd(30,170)
	itemSides = rnd(3,8)
	itemColor = rnd(8,12)
enddef

' Circle collision algorithm
def collision(chX, chY, rad)
	d = sqr((bx - chX)^2 + (by - chY)^2)
	if (d<rad) then
		return 1
	endif
	return 0
enddef

' Update tick
def tick()

	if MODE = GAME_OVER then 
		if keyget() <> 0 then 
			MODE = PLAYING
			init()
		endif
		return
	endif
	
	velocityY = velocityY + gravity * delta
	by = by + velocityY * delta
	bx = bx + velocityX * delta
	if by >= 200 - radius then
		velocityY = -velocityY
		score = score - 1
		time = time - 5
		chplay(0,"@1O2c")
	endif
	
	if bx < 0 + radius or bx > 320 - radius then
		velocityX = -velocityX
	endif
	
	time = time - delta
	if time <= 0 or score < 0 then MODE = GAME_OVER
	
enddef

' Manage item
def item()
	x = itemX
	y = itemY
	poly(x, y, itemSides, 10, frame,  2, itemColor)
	
	if itemPoint <= -1 then 
		itemPoint = -1
		text("-1",x-7,y-4,1,1)
	else 
		itemPoint = itemPoint - delta
		text(formattext("%02i",ceil(itemPoint)),x-7,y-4,1,1)
	endif
	
	' You catch it
	if collision(x,y,radius + 10) then
		score = score + ceil(itemPoint)
		itemPoint = 16
		time = time + 3
		itemX = rnd(20,300)
		itemY = rnd(30,170)
		itemSides = rnd(3,8)
		itemColor = rnd(8,12)
		chplay(1,"t200l32@2O5cf.")
		if time > 60 then time = 60
	endif
	
enddef

def draw()
	cls(0)
	
	if MODE = GAME_OVER then 
		text("Press any key to start",70,95,1,3)
	endif
	
	'this work in playing mode
	if MODE = PLAYING then
		col = 2
		' check for mouse collision
		if collision(mouse_x,mouse_y, radius)=1 then
			col = 4
			' left mouse click
			if mousepressed(0) then
				velocityX = (bx - mouse_X) * 5
				velocityY = -100
				chplay(0,"@0O3c")
			endif
		endif
		'Draw game items, ball and mouse cursor
		item()
		circle(bx,by,radius,2,col)
		sprite(224, mouse_x,mouse_y, 3)
		
	endif
	' Draw UI
	text(formattext("Score: %04i", score),10,5,1,2)
	rect(0,0,320,200,2,2)
	rect(110,4,200,10,1,1)
	rect(112,6,196*time/60,6,0,6)
	
enddef