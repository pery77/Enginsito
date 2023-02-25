frame = 0 

loadMemory("bios.bin")
addmetasprite(0,0,33,0,0,2,0)
addmetasprite(0,1,34,8,0,4,0)

addmetasprite(1,0,34,0,-1,5,0)
addmetasprite(1,1,34,-1,0,6,0)
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