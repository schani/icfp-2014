
;;;;;;; -------------------------------------------------------------
;;;;;;; detect possible directions by scanning the map
;;;;;;; -------------------------------------------------------------
;;;;;;; subroutine
;;;;;;; 	a ... x-ordinate	(lost)
;;;;;;; 	b ... y-ordinate	(lost)
;;;;;;; returns:
;;;;;;; 	@r0 ... possible dirs
;;;;;;; 	@r1 ... number of dirs
;;;;;;; scratch:
;;;;;;; 	@s0
;;;;;;; cost:
;;;;;;; 	26+ret/18+ret/26+ret

$map_check:
	clr @r0
	clr @r1

	mov @s0,a			; save x-ordinate
	inc a
	int INT_MAP			; map east square
	jeq $map_check|e,a,0

	or  @r0,2
	inc @r1
$map_check|e:
	mov a,@s0			; restore x-ordinate
	dec a
	int INT_MAP			; map west square
	jeq $map_check|w,a,0

	or  @r0,8
	inc @r1
$map_check|w:
	mov a,@s0			; restore x-ordinate
	inc b
	int INT_MAP			; map south square
	jeq $map_check|s,a,0

	or  @r0,4
	inc @r1
$map_check|s:
	mov a,@s0			; restore x-ordinate
	sub b,2
	int INT_MAP			; map north square
	jeq $map_check|n,a,0

	or  @r0,1
	inc @r1
$map_check|n:
	ret
