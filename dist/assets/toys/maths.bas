alg =2
xScale = 0.75
yScale =10.

def process(i,h)
	i = i
	
	if alg = 0 then
		return abs(i)
	endif

	if alg = 1 then
		return sgn(i)*ySCALE
	endif
	
	
	if alg = 2 then
		return sqr(i)*H
	endif	
	
	if alg = 3 then	
		return floor(i)*H
	endif	
	
	if alg = 666 then	endif
	
		
				
	if alg = 666 then
		return cos(i)*h
	endif
	
	if alg = 666 then
		return sin(i*0.17)*(h-3)/2
	endif
	
	if i mod 8 then return h/3
	else return -h/3
	endif
	
	return (sin(i*0.27) / cos(i*0.17))*(h-3)/mouse_X
	return sin(i*0.17+frame*0.1)*(h-3)/2
	return (mouse_x / (320/h*2))
	return log(i)*10
	return exp(i/5)
	return rnd(-i,i)
	
	
	



	
enddef

def drawGraph(x,y)

	width = 280
	height = 100
	
	rect(x,y,width,height,0,0)
	
	line(x,height/2+y,width+x,height/2+y,1,1)
	line(x+width/2,y,x+width/2,height+y,1,1)
	
	text(formatText("%03.1f",yScale),x-25,y,1,2)
	text(formatText("%03.1f",-yScale),x-25,y+height-8,1,2)
	text(formatText("%03i",round(-width/2*xScale)),x,y+height+2,1,2)
	text(formatText("%03i",round(width/2*xScale)),x+width-21,y+height+2,1,2)
	
	text("0",x-9,y+height/2-3,1,2)
	text("0",x+width/2-4,y+height+2,1,2)
	
	for i = -width/2 to width/2
		newY = y + height/2
		newY = newY - process(i*xScale,height/2/yScale)
		newX = i+x+width/2
		
		if newY > y + height-1 then 
			newY = y + height-1
		endif
		if newY < y  then 
			newY = y
		endif

		if i = -width/2 then 
			oldX=newX
			oldY=newY
		endif
		
		line(oldX,oldy,newX,newY,1,13)
		oldX=newX
		oldY=newY
	next
	
	rect(x-1,y,width + 1,height,1,1)
	
enddef

def draw()
	cls(8)
	drawGraph(30,10)
	sprite(0,mouse_x+1,mouse_y+1,0)
	sprite(0,mouse_x,mouse_y,3)
enddef