l = ""
l2 = ""
def tick()
	'get the last key char pressed and add to l text
    k = key_char()
    IF k <> 0 THEN
    	l = l+CHR(k)
    ENDIF
    
    k = key_get()
    IF k <> 0 THEN
    	print k 'for debug, see the number
	ENDIF
	'press enter to down the text
    IF key_pressed(257) THEN
        l2 = l + CHR(0)
        l = ""
    ENDIF
    'if you release enter, the text 2 is cleared
    IF key_released(257) THEN
        l2 = ""
     ENDIF 
enddef

def draw()
	cls(0)
	text(">"+l+"_",10,10,1,3)
	text(l2,10,30,1,6)
enddef