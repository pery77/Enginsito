
def hola(n$)
    print "Hola " + n$;
enddef

def init()
enddef

def tick()
enddef
mouseX =90
def draw()
    cls("1345a2")
    FOR i = 1 TO 10
    drawtext("BOOMMM %02i", MOUSEX + i * 10 ,i * 20, 20,"ff2098", MOUSEX)
    NEXT
    
enddef

def end()
enddef

