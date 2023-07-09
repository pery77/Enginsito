'[ 
Songs are MML format
This is from https://archeagemmllibrary.com/castlevania-bloody-tears-master/
Credit: NethulMeow
']
ch_set(0,"@0t126a+16f16>f16<f16>d+16<f16>c+16<f16>c16<f16>c+16<f16>c16<f16a+16f16>c16<f16>c+16<f16>d+16<f16>c+16<f16>c16<f16g+16f16>c16<f16a+16f16a+16f16>f16<f16>d+16<f16>c+16<f16>c16<f16>c+16<f16>c16<f16a+16f16>c16<f16>c+16<f16>d+16<f16>c+16<f16>c16<f16g+16f16>c16<f16a+16f16>d+8g+16f2&f16d+8c+8d+8.g+8.f4.d+8c+8d+8g+16f2&f16d+4f+8.g+4&g+16f8.f+4&f+16d+8g+16f2&f16d+8c+8d+8.g+8.f4.d+8c+8d+8g+16f2&f16d+8f8f+8.g+4&g+16f8g8a8>c8<c8.<a+8.>a+8c8.<a+8.>a+8c8.<a+8.>a+8c+16>c+16<c16>c16<<a+16>a+16<g+16>g+16c8<a+16>a+4&a+16c8<a+16>a+4&a+16c8<a+16>a+4&a+16>c+8d+8c16c+8")
ch_set(1,"@1o3a+1>f1o3a+1>f1<<a+8>a+16<a+8a+16>a+8<a+8>a+8<a+8>a+8<g+8>g+16<g+8g+16>g+8<g+8>g+8<g+8>g+8<f+8>f+16<f+8f+16>f+8<f+8>f+8<f+8>f+8<f8>f16<f8f16>f8<f8>f8<f8>f8<a+8>a+16<a+8a+16>a+8<a+8>a+8<a+8>a+8<g+8>g+16<g+8g+16>g+8<g+8>g+8<g+8>g+8<f+8>f+16<f+8f+16>f+8<f+8>f+8<f+8>f+8<f8>f16<f8f16>f8<f8><g8><a8>c8><<a+8.a+8.a+8><g+8.g+8.g+8><f+8.f+8.f+8><g+8.g+8.g+8><a+8.a+8.a+8><g+8.g+8.g+16g+16f+8.f+8.f+16>f+16<g+8>g+8<g+16g+16>g+8")

ch_preset(3,4)

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
        if mouse_down(0) then 
            colH = colB
            colB = 1
        endif
    endif

    rect(x,y,40,14,0,colB)
    rect(x,y,40,14,1,colH)
    xc = 20 - (measureText(txt,1) * 0.5)
    text(txt,x + xc,y+3,1,colH)

    return (mouse_released(0) AND hover)
enddef

def drawOSC(ch,x,y)
	text(formatText("Chn:%i",ch), x,y-44,1,3)
	rect(x,y-32,63,64,1,4)
	for i = 0 to 62
		yf = ch_frame(ch,i)* 0.25
		pixel(x+i,(y + yf)-32,13)
	next
enddef

def draw()
	cls(0)
	if button(10,10,"Play") then 
		ch_play(0) 
		ch_play(1)
	endif
	if button(55,10,"Stop") then 
		ch_stop(0)
		ch_stop(1)
	endif
	
	if button(55,40,"Jump") then 
		ch_set(2,jump)
		ch_play(2) 	
	endif
	
	if button(10,40,"Boom") then 
		ch_set(2,plof)
		ch_play(2) 		
	endif
	
	circle (60,87,6,1-playing,6)
	circle (60,87,6,1,2)
	if button(10,80,"SFX") then
		if playing then 
			ch_off(3)
		else 
			ch_on(3,40,127)
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