x=0;
def init()
    x = 0
enddef

DEF tick()
    IF getkeydown(65) = 1 THEN x=x-1 ENDIF
    IF getkeydown(68) = 1 THEN x=x+1 ENDIF
ENDDEF

print(7/22);

def draw()
    cls(0)
   
    for j = 0 to 11
        for i = 0 to 25
        x=j*32
        y=i*8
           drawrect(x ,y ,10 ,8,i+(j*25),1)
           drawtext(textformat("%03i",i+(j*25)), x+12 ,y-1 , 1, 255)
        next
    next

    'drawtext(textformat("mouse: %03i x ",mouse.x()) + textformat("%03i",mouse.y()), 0 , 0 , 1, 2)
    'drawrect(mouse.x(), mouse.y(),319,199,8,0)
    'drawrect(0,0,mouse.x(), mouse.y(),6,0)
    'drawtext(textformat("delta: %04i ms",delta()), 0 , 8 , 1, 26)
    'drawtext(textformat("%04f",7/22), 0 , 16 , 1, 26)
    'drawrect(0,0,320,200,2,0)

enddef

def end()
    print "end";
enddef