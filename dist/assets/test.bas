x=0;
def init()
    x = 7
enddef

DEF tick()
    IF getkeydown(65) = 1 THEN x=x+1 ENDIF
    IF getkeydown(68) = 1 THEN x=x-1 ENDIF
ENDDEF

def draw()
    cls(0)
   
    'for i = 0 to 100 step 2
    ''    for j = 0 to 10
    ''    drawrect(i*2 ,j*2 ,2 ,2,(i+j) mod 255)
    ''    drawrect(i*2 ,j*2+32 ,2 ,2,(i+j) mod 255)
    ''    drawrect(i*2 ,j*2+64 ,2 ,2,(i+j) mod 255)
    ''    next
    'next
    
    drawtext(textformat("mouse: %03i x ",mouse.x()) + textformat("%03i",mouse.y()), 0 , 0 , 1, 2)
    drawrect(mouse.x(), mouse.y(),319,199,8,1)
    drawrect(0,0,mouse.x(), mouse.y(),6,0)
enddef

def end()
    print "end";
enddef