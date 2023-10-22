'Return formatted text 
'use %i for integer or %f for float

'06 mean 6 digits
score = 6655
print formatText("Score: %06i", score)

'03 mean 3 decimals
life = 54/256
print formatText("Life %03f", life)