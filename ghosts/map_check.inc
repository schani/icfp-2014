
;;;;;;; -------------------------------------------------------------
;;;;;;; detect possible directions by scanning the map
;;;;;;; -------------------------------------------------------------
;;;;;;; subroutine
;;;;;;; 	a ... x-ordinate
;;;;;;; 	b ... y-ordinate
;;;;;;; returns:
;;;;;;; 	c ... possible dirs
;;;;;;; 	d ... number of dirs
;;;;;;; scratch:
;;;;;;; 	g
;;;;;;; cost:
;;;;;;; 	26+ret/18+ret/26+ret

$map_check:
	clr c
	clr d
	mov g,a				; save x-ordinate
	inc a
	int INT_MAP			; map east square
	jeq $map_check$e,a,0
	or  c,2
	inc d
$map_check$e:
	mov a,g				; restore x-ordinate
	dec a
	int INT_MAP			; map west square
	jeq $map_check$w,a,0
	or  c,8
	inc d
$map_check$w:
	mov a,g				; restore x-ordinate
	inc b
	int INT_MAP			; map south square
	jeq $map_check$s,a,0
	or  c,4
	inc d
$map_check$s:
	mov a,g				; restore x-ordinate
	sub b,2
	int INT_MAP			; map north square
	jeq $map_check$n,a,0
	or  c,1
	inc d
$map_check$n:
	ret
