frame = 0 
setColor(0,3,3,3)
setColor(3,250,250,250)
def draw()
    cls(0)
    'draw.text("Hello", 140,10,1,3)
    'poke(9,frame)
    'poke(10,frame * 0.5)
    'poke(11,frame * 0.25)
    'frame = frame + 2
    'if frame > 255 then frame = 0 endif
    'draw.meta(0,100,100)
    'draw.meta(1,160,130)
    
    draw.rect(20,20,MOUSE.x(),mouse.y,101,rnd(0,15))
    draw.text("EVANGELION",20 + rnd(-10,10),34,4,3)
    for p = 0 to 160
        for j = 0 to 100
            draw.pixel(p*2,j*2,rnd(0,15))
        next
    next
enddef