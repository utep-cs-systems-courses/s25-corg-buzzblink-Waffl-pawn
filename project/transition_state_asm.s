	.arch msp430g2553
	.text

	.global transition_stat
	.extern state
	
transition_state:
	MOV R4, R5
	ADD #1, &state
	RET
