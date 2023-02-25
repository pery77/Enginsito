frame = 0 

def draw()
    cls(0)
    draw.text("Hello", 140,10,1,3)
    poke(9,frame)
    poke(10,frame * 0.5)
    poke(11,frame * 0.25)
    frame = frame + 2
    if frame > 255 then frame = 0 endif
    draw.meta(0,100,100)
    draw.meta(1,160,130)
enddef