
;;;;;;; -------------------------------------------------------------
;;;;;;; calculate vector and generic direction
;;;;;;; -------------------------------------------------------------
;;;;;;; subroutine
;;;;;;; 	@a0 ... p1 x-ordinate	(preserved)
;;;;;;; 	@a1 ... p1 y-ordinate	(preserved)
;;;;;;; 	@a2 ... p2 x-ordinate	(preserved)
;;;;;;; 	@a3 ... p2 y-ordinate	(preserved)
;;;;;;; returns:
;;;;;;; 	@r0 ... delta x
;;;;;;; 	@r1 ... delta y
;;;;;;; 	@r2 ... direction mask
;;;;;;; cost:
;;;;;;; 	16+ret/8+ret/10+ret


$calc_vector:
	clr @r2
	jgt $calc_vector|e,@a2,@a0	; p2 east of p1?
	jeq $calc_vector|w,@a2,@a0	; identical?

	mov @r0,@a0			; calculate delta
	sub @r0,@a2
	or  @r2,8			; 
	bra $calc_vector|w

$calc_vector|e:

	mov @r0,@a2			; calculate delta
	sub @r0,@a0
	or  @r2,2			; 

$calc_vector|w:
	jgt $calc_vector|s,@a3,@a1	; p2 south of p1?
	jeq $calc_vector|n,@a3,@a1	; identical?

	mov @r1,@a1			; calculate delta
	sub @r1,@a3
	or  @r2,1			;
	bra $calc_vector|n

$calc_vector|s:

	mov @r1,@a3			; calculate delta
	sub @r1,@a1
	or  @r2,4			; 

$calc_vector|n:
	ret
