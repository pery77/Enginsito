

def draw()
    cls(0)
    draw.TRIANGLE(mouse.x(), 29,mouse.x()-4, 20,mouse.x()+4, 20, 0, 20)
    draw.rect(120,3,180,17,true, floor(mouse.x()/10))
    for i = 0 to 32
        x = i*10
        draw.rect(x,30,9,200,true,i)
        draw.text(textformat("Color: %i",floor(mouse.x()/10)), 2, 2, 20, 20)
    next
enddef