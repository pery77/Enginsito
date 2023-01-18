

def draw()
    cls(0)
    for i = 0 to 63
        x = i*5
        draw.rect(x,30,5,200,true,i)
        draw.text(textformat("Color: %n",mouse.x()/5), 0, 0, 20, 7)
    next
enddef