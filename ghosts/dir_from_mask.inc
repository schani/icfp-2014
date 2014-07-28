
;;;;;;; -------------------------------------------------------------
;;;;;;; create direction from mask
;;;;;;; -------------------------------------------------------------
;;;;;;; subroutine
;;;;;;; 	@a0 ... directions mask
;;;;;;; returns:
;;;;;;; 	@r0 ... direction
;;;;;;; scratch:
;;;;;;; cost:
;;;;;;; 	13+ret/3+ret/5+ret

$dir_from_mask:
	jgt $dir_from_mask|l,@a0,7
	jgt $dir_from_mask|d,@a0,3
	jgt $dir_from_mask|r,@a0,1
	jgt $dir_from_mask|u,@a0,0
	mov a,-1
$dir_from_mask|ret:
	ret

$dir_from_mask|u:
	mov @r0,DIR_UP
	bra $dir_from_mask|ret

$dir_from_mask|r:
	mov @r0,DIR_RIGHT
	bra $dir_from_mask|ret

$dir_from_mask|d:
	mov @r0,DIR_DOWN
	bra $dir_from_mask|ret

$dir_from_mask|l:
	mov @r0,DIR_LEFT
	bra $dir_from_mask|ret
