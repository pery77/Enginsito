def drawpalette(o)
    for n = 0 to 16
        draw.rect(n*20, 0+o,18,7,1,n+o)
        draw.rect(n*20, 0+o,18,16,0,5)
        draw.text(textformat("%i",n+o), 2+n*20, 7+o, 1, 9)
    next
enddef

def draw()
    cls(0)
    for i = 0 to 3
        drawpalette(i*16)
    next
enddef