def init()
    print "init";
enddef

mouseX = 4

def tick()
    getMouse(mouseX, mouseY);
enddef


def draw()

    cls("1345a2")
    'getMouse(mouseX, mouseY);
    FOR i = 1 TO 10
    drawtext("BOOMMM %02i", MOUSEX + i * 10 ,MOUSEy+i * 20, 20,"ff2098", mouseX)
    NEXT
    
enddef

def end()
    print "end";
enddef

