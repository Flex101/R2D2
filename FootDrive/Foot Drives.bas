SETFREQ m64						; 64 Mhz Clock
HSERSETUP B115200_64, %00001			; 115200 Baud Rate
HPWM PWMDIV4, 0, pwmHHHH, %1000, 199, 0	; 20Khz PWM

; RAM Allocation
; 0 - 7 	-	serial processing buffer
; 8 		-	processed flag
; 9		-	ramp value
; 10		-	ramp counter
; 11		-	dir (temp)
; 12		-	dir (ramp)
; 13		-	dir
; 14 - 15	-	speed (temp)
; 16 - 17	-	speed (ramp) 
; 18 - 19	-	speed 
; 20 - 21	-	calc

#define dir_pin		B.3
#define processed		b8
#define ramp		b9
#define ramp_count	b10
#define dir_temp		b11
#define dir_ramp		b12
#define dir			b13
#define speed_temp	w7
#define speed_temp_l	b14
#define speed_temp_h	b15
#define speed_ramp	w8
#define speed_ramp_l	b16
#define speed_ramp_h	b17
#define speed		w9
#define calc		w10

main:

ptr = 0
hserptr = 0
bptr = 0

speed_temp = 0
speed_ramp = 0
speed = 0
ramp = 1
ramp_count = 0

gosub reset_watchdog

main_loop:

	if hserinflag > 0 then
		
		do while hserptr <> ptr
		
			@bptrinc = @ptr
			
			if @ptr = 13 then 
				gosub process_msg
			else if bptr > 7 then
				gosub flush
				goto main_loop
			end if
		
			ptr = ptr + 1
			
		loop
	
		hserinflag = 0
		
	end if

	; watchdog timer triggered due to lack of coms
	if timer > 0 then
		speed = 0
	end if

	; PWM is updated every [ramp] cycles
	ramp_count = ramp_count + 1
	if ramp_count = ramp then
		gosub update_pwm
		ramp_count = 0
	end if

	goto main_loop
end
	
process_msg:
	
	processed = 0	;reset procesed flag
	bptr = 0
	
	; CC - Connection check
	if processed = 0 and @bptrinc = "C" and @bptrinc = "C" and @bptrinc = 13 then
		hserout 0,("CCOK",13)
		processed = 1
	end if

	bptr = 0
	
	; ID - Read ID
	if processed = 0 and @bptrinc = "I" and @bptrinc = "D" and @bptrinc = 13 then
		hserout 0,("LFOOT",13)
		processed = 1
	end if

	bptr = 0
	
	; SS - Set speed
	if processed = 0 and @bptrinc = "S" and @bptrinc = "S" then
		
		dir_temp = @bptrinc - 43
		speed_temp = 0;
		calc = @bptrinc - 48;
		calc = calc * 1000
		speed_temp = speed_temp + calc
		calc = @bptrinc - 48;
		calc = calc * 100
		speed_temp = speed_temp + calc	
		calc = @bptrinc - 48;
		calc = calc * 10
		speed_temp = speed_temp + calc	
		calc = @bptrinc - 48;
		speed_temp = speed_temp + calc	
		
		; completion check
		if @bptr = 13 then
			dir = dir_temp
			speed = speed_temp
			hserout 0,("SSOK",13)
			processed = 1
		endif 
	
	end if
	
	bptr = 0	
	
	if processed = 0 then
		; Unrecognised command
		hserout 0,("?",13)
	else
		gosub reset_watchdog	
	endif

	bptr = 0
	
return

flush:

	bptr = 0
	ptr = 0
	hserptr = 0
	hserout 0,("!buffer overflow",13)
	
return

update_pwm:

	if dir_ramp != dir then
		
		if speed_ramp > 0 then			
			speed_ramp = speed_ramp - 1;
		else
			dir_ramp = dir
		endif
	
	else
	
		if speed_ramp < speed then
			speed_ramp = speed_ramp + 1
		else if speed_ramp > speed then
			speed_ramp = speed_ramp - 1
		end if
	
	endif

	;hserout 0,(#speed_ramp,13)

	if dir_ramp > 0 then : HIGH dir_pin : else : LOW dir_pin : end if
	
	HPWMDUTY speed_ramp

return;

reset_watchdog:
	
	; 0.1 second watchdog
	SETTIMER 40535 
	timer = 0

return;