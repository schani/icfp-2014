
;;;;;;; -------------------------------------------------------------
;;;;;;; calculate vector and generic direction
;;;;;;; -------------------------------------------------------------
;;;;;;; subroutine
;;;;;;; 	a ... p1 x-ordinate
;;;;;;; 	b ... p1 y-ordinate
;;;;;;; 	c ... p2 x-ordinate
;;;;;;; 	d ... p2 y-ordinate
;;;;;;; returns:
;;;;;;; 	e ... delta x
;;;;;;; 	f ... delta y
;;;;;;; 	g ... direction
;;;;;;; cost:
;;;;;;; 	18+ret/8+ret/12+ret


$calc_vector:
	jgt $calc_vector$e,c,a		; p2 east of p1?
	mov e,a				; calculate delta
	sub e,c
	mov g,DIR_LEFT			; 
	bra $calc_vector$w
$calc_vector$e:
	mov e,c				; calculate delta
	sub e,a
	mov g,DIR_RIGHT			; 
$calc_vector$w:
	jgt $calc_vector$s,d,b		; p2 south of p1?
	mov f,b				; calculate delta
	sub f,d
	jgt $calc_vector$n,e,f		; delta x > delta y?
	mov g,DIR_UP			;
	bra $calc_vector$n
$calc_vector$s:
	mov f,d				; calculate delta
	sub f,b
	jgt $calc_vector$n,e,f		; delta x > delta y?
	mov g,DIR_DOWN			; 
$calc_vector$n:
	ret
