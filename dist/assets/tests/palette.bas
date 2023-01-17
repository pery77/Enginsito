def drawpalette(o)
    for n = 0 to 16
        draw.rect(n*20, 0+o,16,8,1,n+o)
        draw.rect(n*20, 0+o,16,8,0,10)
        draw.text(textformat("%i",n+o), n*20, 7+o, 1, 9)
    next
enddef

def draw()
    cls(0)
    for i = 0 to 3
        drawpalette(i*16)
    next
enddef