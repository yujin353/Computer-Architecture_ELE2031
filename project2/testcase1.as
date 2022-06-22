	lw 0 1 data1
	lw 0 2 data2
	lw 0 4 data3
	noop
	add 1 2 3
	sw 0 3 r1
	add 1 4 5
	sw 0 5 r2
	halt
data1 .fill 10
data2 .fill 20 
data3 .fill 30
r1 .fill 0
r2 .fill 0
