'[ 
Songs are in MML format
This is from https://archeagemmllibrary.com/castlevania-bloody-tears-master/
Credit: NethulMeow
']
chset(0,"@0t126a+16f16>f16<f16>d+16<f16>c+16<f16>c16<f16>c+16<f16>c16<f16a+16f16>c16<f16>c+16<f16>d+16<f16>c+16<f16>c16<f16g+16f16>c16<f16a+16f16a+16f16>f16<f16>d+16<f16>c+16<f16>c16<f16>c+16<f16>c16<f16a+16f16>c16<f16>c+16<f16>d+16<f16>c+16<f16>c16<f16g+16f16>c16<f16a+16f16>d+8g+16f2&f16d+8c+8d+8.g+8.f4.d+8c+8d+8g+16f2&f16d+4f+8.g+4&g+16f8.f+4&f+16d+8g+16f2&f16d+8c+8d+8.g+8.f4.d+8c+8d+8g+16f2&f16d+8f8f+8.g+4&g+16f8g8a8>c8<c8.<a+8.>a+8c8.<a+8.>a+8c8.<a+8.>a+8c+16>c+16<c16>c16<<a+16>a+16<g+16>g+16c8<a+16>a+4&a+16c8<a+16>a+4&a+16c8<a+16>a+4&a+16>c+8d+8c16c+8")
chset(1,"@1o3a+1>f1o3a+1>f1<<a+8>a+16<a+8a+16>a+8<a+8>a+8<a+8>a+8<g+8>g+16<g+8g+16>g+8<g+8>g+8<g+8>g+8<f+8>f+16<f+8f+16>f+8<f+8>f+8<f+8>f+8<f8>f16<f8f16>f8<f8>f8<f8>f8<a+8>a+16<a+8a+16>a+8<a+8>a+8<a+8>a+8<g+8>g+16<g+8g+16>g+8<g+8>g+8<g+8>g+8<f+8>f+16<f+8f+16>f+8<f+8>f+8<f+8>f+8<f8>f16<f8f16>f8<f8><g8><a8>c8><<a+8.a+8.a+8><g+8.g+8.g+8><f+8.f+8.f+8><g+8.g+8.g+8><a+8.a+8.a+8><g+8.g+8.g+16g+16f+8.f+8.f+16>f+16<g+8>g+8<g+16g+16>g+8")

chpreset(3,4)

jump = "@2v30d32v80f8"
plof = "@3v90c32o2c"


playing = 0

def isHover(x,y,w,h)
    return mouse_X > x AND mouse_Y > y AND mouse_X < x + w AND mouse_Y < y + h
enddef

def button(x,y,txt)
    colB = 1
    colH = 2
    hover = isHover(x,y,40,14)

    if hover then 
        colB = 9
        colH = 3 
        if mousedown(0) then 
            colH = colB
            colB = 1
        endif
    endif

    rect(x,y,40,14,0,colB)
    rect(x,y,40,14,1,colH)
    xc = 20 - (textSize(txt,1) * 0.5)
    text(txt,x + xc,y+3,1,colH)

    return (mousereleased(0) AND hover)
enddef

def drawOSC(ch,x,y)
	text(formatText("Chn:%i",ch), x,y-44,1,3)
	rect(x,y-32,63,64,0,1)
	rect(x,y-32,63,64,1,4)
	for i = 0 to 62
		yf = chframe(ch,i)* 0.25
		
		newY = (y + yf)-32
		newX = i+x
		
		if i = 0 then 
			oldX=newX
			oldY=newY
		endif
		
		line(oldX,oldy,newX,newY,1,13)
		oldX=newX		
		oldY=newY

	next
enddef

def draw()
	cls(0)
	if button(10,10,"Play") then 
		chplay(0) 
		chplay(1)
	endif
	if button(55,10,"Stop") then 
		chstop(0)
		chstop(1)
	endif
	n=0
	for n= 0 to 1
		text(formatText("(%04i/%04i)",chpos(n),chsize(n)), 120,10+n*9,1,3)
		text(chgetnote(n), 220,10+n*9,1,3)		
		'text(chgetnoteNum(n), 260,10+n*9,1,3)
	next
	if button(55,40,"Jump") then 
		chset(2,jump)
		chplay(2) 	
	endif
	
	if button(10,40,"Boom") then 
		chset(2,plof)
		chplay(2) 		
	endif
	
	circle (60,87,6,1-playing,6)
	circle (60,87,6,1,2)
	if button(10,80,"SFX") then
		if playing then 
			choff(3)
		else 
			chon(3,40,127)
		endif
		playing = not playing	
	endif
	
	if playing then
		poke(0xd63, sin(frame*0.08)*30+50)
	endif
	
	for o = 0 to 3
		drawOSC(o,o*70+20,160)	
	next
	sprite(0,mouse_x+1,mouse_y+1,1)
	sprite(0,mouse_x,mouse_y,3)
enddef