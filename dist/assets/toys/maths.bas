ALG = 0
xScale = 0.34
yScale =10.

def isHover(x,y,w,h)
    return mouse_X > x AND mouse_Y > y AND mouse_X < x + w AND mouse_Y < y + h
enddef

def clamp(v,min,max)
    IF v < min THEN return min ENDIF
    IF v > max THEN return max ENDIF
    return v
enddef
    
def slider(v,x,y,w,max)

    v = w * v/max
    cb = 2
    col = 4
	h=10
	
    mx = x+v+1
    my = y+1

    hover = isHover(x,y,w+10,h)
    IF hover THEN
        col = 14
        cb = 3
        
        vMul = 1
        IF w/max > 1 THEN
            vMul = w/max
        ENDIF
        v = v + (mouse_wheel() * vMul)* 0.01;
    ENDIF

    IF mouse_down(0) AND hover THEN
        v = v + (mouse_x - mx - 4*0.01);
    ENDIF
    
    v = (v/w * max)
    v = clamp(v,0, max)

    rect(x,y,w + 10,h,0,0)
    rect(x,y,w + 10,h-1,1,cb)
    rect(mx,my,8,h-3,0,col)
    textPos = formatText("%.02f",v+0.001)
    text(textPos, x - textsize(textPos, 1)-2, y, 1, cb)

    return v
enddef

def button(x,y,txt)
    colB = 1
    colH = 2
    bw = 50
    hover = isHover(x,y,bw,14)

    if hover then 
        colB = 4
        colH = 3 
        if mouse_down(0) then 
            colH = colB
            colB = 1
        endif
    endif

    rect(x,y,bw,14,0,colB)
    rect(x,y,bw,14,1,colH)
    xc = bw/2 - (textSize(txt,1) * 0.5)
    text(txt,x + xc,y+3,1,colH)

    return (mouse_released(0) AND hover)
enddef

def process(i,h)

	if ALG = 0 then
		return abs(i)
	endif

	if ALG = 1 then
		return sgn(i)*h
	endif
	
	
	if ALG = 2 then
		return sqr(i)*H
	endif	
	
	if ALG = 3 then	
		return floor(i)*H
	endif	
	
	if alg = 4 then	
		return fix(i)*H
	endif
	
	if alg = 5 then	
		return rnd(-i,i)*H	
	endif		
	
				
	if alg = 6 then	
		return exp(i)	* h	
	endif			
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
	
	
	



	
enddef

def drawGraph(x,y)

	width = 300
	height = 100
	
	rect(x,y,width,height,0,0)
	
	line(x,height/2+y,width+x,height/2+y,1,1)
	line(x+width/2,y,x+width/2,height+y,1,1)
	
	text(formatText("%03.1f",yScale+0.0001),x+5,y+5,1,2)
	text(formatText("%03.1f",-yScale+0.0001),x+5,y+height-9,1,2)
	text(formatText("%03i",round(-width/2*xScale)),x,y+height+2,1,2)
	text(formatText("%03i",round(width/2*xScale)),x+width-21,y+height+2,1,2)
	
	text("0",x-9,y+height/2-3,1,2)
	text("0",x+width/2-4,y+height+2,1,2)
	
	for i = 0 to width step width*xScale
		xL = (i)
		line(xL+x,y,xL+x,height+y,1,5)	
	next
	
	for i = -width/2 to width/2

		newY = y + height/2
		newY = newY - process(i*xScale,height/2/yScale)
		newX = i+x+width/2

		if newY > y + height-1 then 
			newY = y + height-1
		endif
		if fix(newY) < y then 
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
	drawGraph(12,10)
	
	if button(10,130,"ABS") then 
		ALG = 0
		xScale = 0.34
		yScale = 10
	endif
	
	if button(10,144,"SGN") then 
		ALG = 1
		xScale = 1.0
		yScale = 1.1
	endif
	
	if button(10,158,"SQR") then 
		ALG = 2
		xScale = 0.64
		yScale = 10.0
	endif
	
	if button(10,172,"FLOOR") then 
		ALG = 3
		xScale = 0.05
		yScale = 7.0
	endif

	if button(60,130,"FIX") then 
		ALG = 4
		xScale = 0.02
		yScale = 3
	endif	
	
	if button(60,144,"RND") then 
		ALG = 5
		xScale = 0.2
		yScale = 15	
	endif	

	if button(60,158,"EXP") then 
		ALG = 6
		xScale = 0.02
		yScale = 15	
	endif

	xScale = slider(xScale,240,125,60,2)
	yScale = slider(yScale,240,145,60,100)
	sprite(0,mouse_x+1,mouse_y+1,0)
	sprite(0,mouse_x,mouse_y,3)
enddef