time = 0
def init()
enddef

DEF tick()
    IF keydown(65) = 1 THEN x=x-1 ENDIF
    IF keydown(68) = 1 THEN x=x+1 ENDIF
ENDDEF


def draw()
    time = (time + delta()) mod 1000
    'IF time < 100 THEN
        cls(0)
    'ENDIF
   IF 0 THEN
    for j = 0 to 11
        for i = 0 to 25
        x=j*32
        y=i*8
           drawrect(x ,y ,10 ,8, 1,i+(j*25))
           drawtext(textformat("%03i",i+(j*25)), x+12 ,y-1 , 1, 255)
        next
    next
    ENDIF

    drawrect(0,0,319,9,1,175)
    drawtext(textformat("%06i",time), 0, 0, 1, 236)
    drawtext(textformat("%03i",keyup(65)), 0, 8, 1, 175)
    drawtext(textformat("%03i",keydown(65)), 0, 16, 1, 175)
    drawtext(textformat("%03i",keypressed(65)), 0, 24, 1, 175)
    drawtext(textformat("%03i",keyreleased(65)), 0, 32, 1, 175)

goto no;
    for n = 0 to 1
        drawpixel(rnd(0,319), rnd(0,199), rnd(0,255))
        'drawline(rnd(0,319), rnd(0,199), rnd(0,319), rnd(0,199),rnd(1,3),rnd(0,255))
        'drawcircle(rnd(0,319), rnd(0,199), rnd(1,20), rnd(0,1), rnd(0,255))
        'drawellipse(rnd(0,319), rnd(0,199), rnd(1,20), rnd(1,20),rnd(0,1), rnd(0,255))
        x = rnd(0,319)
        y = rnd(0,199)
        'drawtriangle(x, y, x + rnd(-10,10), y + rnd(-20,20), x + rnd(-20,20), y + rnd(-20,20), rnd(0,1), rnd(0,255))

    next
no:
    'drawtext(textformat("mouse: %03i x ",mouse.x()) + textformat("%03i",mouse.y()), 0 , 0 , 1, 2)
    'drawrect(mouse.x(), mouse.y(),319,199,8,0)
    'drawrect(0,0,mouse.x(), mouse.y(),6,0)
    'drawtext(textformat("%04f",7/22), 0 , 16 , 1, 26)
    'drawrect(0,0,320,200,2,0)

enddef

def end()
    print "end";
enddef