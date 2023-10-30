'[
FLOOR  ->  Returns the greatest integer not greater than a number.
CEIL   ->  Returns the least integer not less than a number.
FIX    ->  Returns the integer part of a number.
ROUND  ->  Rounds a number to the nearest integer.
']

print floor(1.8) 'round to 1
print floor(1.1)

print ceil(1.8) ' round to 2
print ceil(1.1)

print round(5.8)' round to 6
print round(5.5)

print round(5.1)' round to 5
print round(5.4999)

print fix(234.0001) 'ignore decimal part
print fix(234.9998)