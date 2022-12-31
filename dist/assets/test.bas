x=0;
def init()
    x = 7
enddef

DEF tick()
    IF getkeydown(65) = 1 THEN x=x+1 ENDIF
    IF getkeydown(68) = 1 THEN x=x-1 ENDIF
ENDDEF

    print(7/22)
def draw()
    cls(0)
   
    for i = 0 to 128
        drawrect(i*2 ,30 ,2 ,2,i,0)
    next
    for i = 0 to 128
        drawrect(i*2 ,32 ,2 ,2,i+128,0)
    next
    
    drawtext(textformat("mouse: %03i x ",mouse.x()) + textformat("%03i",mouse.y()), 0 , 0 , 1, 2)
    drawrect(mouse.x(), mouse.y(),319,199,8,1)
    drawrect(0,0,mouse.x(), mouse.y(),6,0)
    drawtext(textformat("delta: %04i ms",delta()), 0 , 8 , 1, 26)
    drawtext(textformat("%04f",7/22), 0 , 16 , 1, 26)

enddef

def end()
    print "end";
enddef