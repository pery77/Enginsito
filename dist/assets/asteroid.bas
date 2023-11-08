class vertex
	var x = 0
	var y = 0
endclass

class polygon

	var vList = list()
	var col= 14
	var theta = 0
	var posX = 160
	var posY = 100
	var collider = 0

	def addVertex(x, y)
		p = new(vertex)
		p.x = x
		p.y = y
		push(vList, p)
	enddef

	def lineRotated(dx1,dy1,dx2,dy2)
		x1_rotated = dx1 * cos(theta) - dy1 * sin(theta)+posX
		y1_rotated = dx1 * sin(theta) + dy1 * cos(theta)+posY
		x2_rotated = dx2 * cos(theta) - dy2 * sin(theta)+posX
		y2_rotated = dx2 * sin(theta) + dy2 * cos(theta)+posY
		line(x1_rotated, y1_rotated, x2_rotated, y2_rotated, 1, col)
	enddef

	def draw()
		pass = 0
		x = 0
		y = 0
		sX = 0
		sY = 0
		for i in vList
			if pass = 0 then 'first pass store values
				x = i.x
				y = i.y
				sX = x
				sY = y 
			else
				lineRotated(x, y, i.x, i.y)
				x = i.x
				y = i.y
			endif
			pass = pass + 1
			if pass = len(vList) then 'last pass close line from last vertex to stored
				lineRotated(x, y ,sX ,sY)
			endif
		next
		'circle(posX,posY,collider,1,15) 'uncomment to show collider
	enddef

endclass

class bullet

	var x = 0
	var y = 0
	var life = 200
	var col = 12
	var speed = 2
	var angle = 0

	def create(_x ,_y, _s, _a)
		x = _x
		y = _y
		speed = _s
		angle = _a - 1.5707
		life = 120
	enddef
	
	def borderCheck()
		if x < 0 then x = 320
		if x > 320 then x = 0
		if y < 0 then y = 200
		if y > 200 then y = 0
	enddef
	
	def tick()
		x = x + cos(angle) * speed
		y = y + sin(angle) * speed
		life = life - 1
		borderCheck()
	enddef

	def draw()
		circle(x, y, 1, 0, col)
	enddef
	
endclass

let bulletList = list()

class ship(polygon)

	var size  = 6
	var angle = 0
	var rotation_speed   = 0.2
	var rotation_inertia = 0.05
	var move_inertia = 0.015
	var acceleration = 2.5
	var direction = 0
	var posXT = 0
	var posYT = 0
	var current_speed = 0
	
	def create()
		addVertex(0,-(size*1.5))
		addVertex(-size,size)
		addVertex(0,0)
		addVertex(size,size)
		posX = 160
	 	posY = 100
	 	posXT = posX
	 	posYT = posY
	 	collider = 9
	enddef

	def impulse(a)
		a = a - 1.5707
		posXT = posXT + cos(a) * acceleration
		posYT = posYT + sin(a) * acceleration
	enddef

	def borderCheck()
		if posX < 0 then 
			posX = 320
			posXT = posXT + 320
		endif
		if posX > 320 then 
			posX = 0
			posXT = posXT - 320
		endif
		if posY < 0 then
			posY = 200
			posYT = posYT + 200
		endif
		if posY > 200 then
			posY = 0
			posYT = posYT - 200
		endif
	enddef

	def tick()

		if keydown(key_d) then angle = angle + rotation_speed
		if keydown(key_a) then angle = angle - rotation_speed
		if keydown(key_w) then impulse(direction)
		if keypressed(key_w)  then chon(0,69,127)
		if keyreleased(key_w) then choff(0)

		borderCheck()

		diffR = angle - theta
		diffD = theta - direction
		diffPX = posXT - posX
		diffPY = posYT - posY
		
		theta     = theta + diffR * rotation_inertia
		direction = direction + diffD * rotation_inertia
		posX = posX + diffPX * move_inertia
		posY = posY + diffPY * move_inertia
		
		if abs(diffR) < 0.01 then theta     = angle
		if abs(diffD) < 0.01 then direction = theta
		current_speed = sqr(diffPx^2 + diffPy^2)
	enddef

endclass

class asteroid(polygon)
	
	var radio = 0
	var maxV  = 0
	var noise = 0
	var turn  = 0
	var speed = 0
	var direction = 0
	var life = 1
	
	def create()
		
		radio = rnd(5,15)
		noise = radio * 0.3
		maxV =  rnd(7,11)
		turn =  rnd(-10,10) * 0.001
		speed = rnd(1,3) * 0.1
		life = 1
		max = 0
		min = 100
		collider = 100
		for n = 0 to maxV-1
			a = (n / maxV) * 6.2832
			x =  radio * cos(a) + rnd(-noise,noise)
			y =  radio * sin(a) + rnd(-noise,noise)
			addvertex(x,y)
			col = 5
			r = sqr(x^2 + y^2)
			if r < min then min = r
			if r > max then max = r
		next
		collider = (min+max)/2

	enddef

	def borderCheck()

		if posX < 0 then posX = 320
		if posX > 320 then posX = 0
		if posY < 0 then posY = 200
		if posY > 200 then posY = 0

	enddef

	def tick()
		theta = theta + turn
		posX = posX + cos(direction) * speed
		posY = posY + sin(direction) * speed
		bordercheck()
	enddef
	
	def debug()
		circle(posX,posY,collider,1,1)
		text(str(life),posX-4,posY-4,1,1)
	enddef

endclass
'--------------------------------------------------------------

let player = new(ship)
player.create()

let asteroidsList = list()

let totalAsteroids = 11
let noise = 30
let radio = 80

for n = 0 to totalAsteroids-1
	minR = 0
	as = new(asteroid)
	as.create()
	a = (n / totalAsteroids) * 6.2832
	as.posX = 160 + radio * cos(a) + rnd(-noise,noise)
	as.posY = 100 + radio * sin(a) + rnd(-noise,noise)
	as.direction = a

	push(asteroidsList, as)
next

def shot()
	s = new(bullet)
	bSpeed = player.current_speed * 0.02
	if bSpeed > 5 then bSpeed = 5
	s.create(player.posX, player.posY, 2+bSpeed, player.theta)
	push(bulletList, s)
	chplay(1, "@1o6c")
enddef

def getCollision(x1,y1,r1,x2,y2,r2)
	dis = sqr((x2-x1)^2 + (y2-y1)^2)
	if dis+r2 <= r1 or dis+r1 <= r2 then return 1
	return 0
enddef
	
def tick()
	player.tick()
	if keypressed(key_space) then shot()

	for b in bulletList
		b.tick()
	next
	
	for a in asteroidsList
		a.tick()
	next

	for b in bulletList
		for a in asteroidsList
			if getCollision(a.posX, a.posY, a.collider, b.x, b.y, 1) then
				remove(bulletList, index_of(bulletlist,b))
				remove(asteroidsList, index_of(asteroidsList,a))
			endif
		next
		if b.life<0 then remove(bulletList, index_of(bulletlist,b))
	next

enddef

def draw()
	cls(0)
	player.draw()
	
	for b in bulletList
		b.draw()
	next
	
	for a in asteroidsList
		a.draw()
		'a.debug()
	next

enddef