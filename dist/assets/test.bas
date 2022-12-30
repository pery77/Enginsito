x=0;
def init()
    x = 3
enddef


DEF tick()
    IF getkeydown(65) = 1 THEN x=x+1 ENDIF
    IF getkeydown(68) = 1 THEN x=x-1 ENDIF
ENDDEF

def draw()
    'cls(23)
    for i = 0 to 255
        drawtext("HOLA" ,0 ,i*10 ,20 ,i)
    next
    'drawtext(textformat("mouse: %03i x ",mousex()) + textformat("%03i",mousey()), 0 , 0 , 20, 30)
enddef

def end()
    print "end";
enddef