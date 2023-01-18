'Wellcome to peryEngine, this is a comment, is ignored and used for human info.

'This is the draw function, peryEngine call it 60 times per second.
def draw()
    'This line clear screen with the color of the parameter, in this case is 0 (black)
    cls(0) 
    'Whit that we draw a text ("Hello World") in the position X = 95, and y = 40
    '25 is the size (the minimun is 8) and 9 is the color (white), colors go from 0 to 63
    draw.text("Hello World!",95,40,20,9) 
    'Another fancy text with usefull information.
    draw.text("Press ESCAPE key to close.",85,63,8,32)
enddef