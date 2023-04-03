def drawpalette(o)
    for n = 0 to 16
        draw.rect(n*20, 0+o,18,7,1,n+o)
        draw.rect(n*20, 0+o,18,16,0,n)
        draw.text(inttotext("%i",n+o), 2+n*20, 7+o, 1, 9)
    next
enddef

def xdraw()
    cls(0)
    for i = 0 to 3
        drawpalette(i*16)
        print i
    next
enddef
print "fucking legend"
cls(4)
fere = 3
print fere
