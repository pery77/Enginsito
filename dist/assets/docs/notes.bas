'CHON(ch, note, volume) -> Start sound until stop it whit choff()
'CHOFF(ch)              -> Stop note in this channel

let playing = 0
CHPRESET(0,0)

def draw()
	cls(0)
	text(str(playing),10,10,1,6)
	
	if MOUSERELEASED(0) then
	
		playing = 1 - playing
		
		if playing then
			chon(0,69,127)
		else
			choff(0)
		endif
		
	endif

enddef