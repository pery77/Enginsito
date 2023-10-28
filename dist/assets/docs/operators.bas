'[
Operator Precedence

Level Operation
1 		^
2 		*, /, MOD
3 		+, - (subtraction)
4 		<, >, <=, >=, <>, = (comparison)
5 		NOT
6 		AND, OR
7 		- (negation)
8 		= (assignment)

Boolean
a   b	|	and		or
----------------------
0	0	|	0		0
1	0	|	0		1
0	1	|	0		1
1	1	|	1		1

a	not
-------
0	1
1	0
']

' Basic Operations
print 3 + 5 - 2 * 2   '-> 4
print (3 + 5 - 2) * 2 '-> 12

' Exponential Operation
print 9^3 ' -> 9*9*9 729

' Modulus Operation
print 10 mod 3 ' 10 / 3 = 9 + (1) -> 1

' NOT Operation
print not 0 '-> 1 (true), not of any other non-zero value is 0 (false)

' OR Operation
print 6 or 0 '-> 1 because 6 is true (not 0)

' Comparison Operation
print 8 > 19 '-> 0 (false) because 8 is not greater than 19