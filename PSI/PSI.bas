
; RAM Allocation
; 0 	 	-	for loop counter
; 1		-	led states
; 2		-	calculation value 0
; 3		-	calculation value 1	
; 4		-	sequence counter 0
; 5		-	sequence counter 1
; 6		-	sequence counter 2
; 7		-	index of sequence to play
; 8-9		-	speed modifier
; 10		-	invert color modifier
; 11		- 	flip direction modifier
; 12-13	-	random number

#define i 			b0
#define leds		b1
#define calc0		b2
#define calc1		b3
#define seq0		b4
#define seq1		b5
#define seq2		b6
#define mod_index		b7
#define mod_speed		w4
#define mod_invert	b10
#define mod_flip		b11
#define rand		w6

dirsB = %11111111 ; set all portB to outputs
pinsB = %00000000 ; set all portB LOW
seq0 = 0
seq1 = 0
seq2 = 0
mod_index = 0
mod_speed = 0
mod_invert = 0
mod_flip = 0

main:

; seed random number with timer
rand = timer

main_loop:

	if seq0 = 0 then
		gosub randomise
		pause 1000
	endif

	select mod_index
		case 0
			gosub prog_bar
		case 1
			gosub wipe
		case 2
			gosub mini_wipe
		case 3
			gosub knightrider
		case 4
			gosub johnny5
		case 5
			gosub police
	endselect
	
	select mod_speed
		case 0
			pause 250
		case 1
			pause 500
		case 2
			pause 750
		case 3
			pause 1000
	endselect

	if mod_invert > 0 then
		leds = NOT leds
	endif

	if mod_flip > 0 then
		gosub flip
	endif
 
 	; update leds
	pinsB = leds
 
	goto main_loop
end

randomise:

	random rand	
	;mod_index = rand % 6
	mod_index = 1
	random rand
	;mod_speed = rand % 3
	random rand
	mod_invert = rand % 2
	random ranD
	mod_flip = rand % 2
	
return

flip:
	calc0 = leds
	bit8 	= bit23
	bit9 	= bit22
	bit10 = bit21
	bit11 = bit20
	bit12 = bit19
	bit13 = bit18
	bit14 = bit17
	bit15 = bit16
return

prog_bar:
	
	if seq0 <= 4 then
		seq1 = seq0
	else
		seq1 = seq0 - 4
		seq1 = 4 - seq1
	endif

	for i = 0 to 4
		
		calc0 = 2 * i
		calc1 = calc0 + 1
		
		if i >= seq1 then
			setbit leds, calc0
			clearbit leds, calc1
		else
			clearbit leds, calc0
			setbit leds, calc1
		endif
		
	next i

	seq0 = seq0 + 1
	seq0 = seq0 % 9

return

wipe:

	seq1 = seq0 % 4
	
	for i = 0 to 4	
				
		calc0 = 2 * i
		calc1 = calc0 + 1
		
		if i >= seq1 then
			setbit leds, calc0
			clearbit leds, calc1
		else
			clearbit leds, calc0
			setbit leds, calc1
		endif
	
	next i

	if seq0 > 4 then
		leds = NOT leds
	endif

	seq0 = seq0 + 1
	seq0 = seq0 % 9

return

mini_wipe:

return

knightrider:

return

johnny5:

return

police:

return