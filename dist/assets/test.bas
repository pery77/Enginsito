x=0;
def init()
    x = 3
enddef


DEF tick()
    IF getkeydown(65) = TRUE THEN x=x+1 ENDIF
    IF getkeydown(68) = TRUE THEN x=x-1 ENDIF
ENDDEF

def draw()
    cls("1345a2")
    drawtext(textformat("mouse: %03i x ",mousex()) + textformat("%03i",mousey()), 0 , 0 , 20, "ff9098")
enddef

def end()
    print "end";
enddef