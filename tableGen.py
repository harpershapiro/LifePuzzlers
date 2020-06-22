#generates a lookup table for hyperbolic tangent

import math
tanhlist = list()
indexlist = [-128,128]

def pirange(n):
	return (n/100)*2*math.pi


for x in range(-128,129):
	print(pirange(x))
	tanhlist.append(math.tanh(pirange(x)))

print(tanhlist)
print(len(tanhlist))