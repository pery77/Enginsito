x=0;
def init()
    x = 7
enddef

DEF tick()
    IF getkeydown(65) = 1 THEN x=x+1 ENDIF
    IF getkeydown(68) = 1 THEN x=x-1 ENDIF
ENDDEF

def draw()
    cls(x)
    for i = 0 to 255
        drawtext(textformat("color: %03i ",i) ,0 ,i*10 ,20 ,i)
    next
    drawtext(textformat("mouse: %03i x ",mouse.x()) + textformat("%03i",mouse.y()), 0 , 0 , 20, 30)
enddef

def end()
    print "end";
enddef