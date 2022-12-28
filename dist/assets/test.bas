import "assets/tools.bas"
x = 0
def init()
    x = 3
enddef

DEF tick()
    IF getkey(65) = TRUE THEN x=x+1 ENDIF
    IF getkey(68) = TRUE THEN x=x-1 ENDIF
ENDDEF

def draw()
    cls("1345a2")
    FOR i = 1 TO x
        drawtext("BOOMMM %02i", MOUSEX + i * 10 ,MOUSEy+i * 20, 20,"ff2098", x)
    NEXT
enddef

def end()
    print "end";
enddef