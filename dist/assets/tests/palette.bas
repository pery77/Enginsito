def drawpalette()
    for n = 0 to 16
        draw.rect(n*20, 0,18,7,1,n)
        draw.rect(n*20, 0,18,16,0,n)
        draw.text(inttotext("%i",n), 2+n*20, 7, 1, 9)
    next
enddef

def draw()
    cls(0)
    drawpalette()
enddef

def end()
print "fucking legend"
enddef

