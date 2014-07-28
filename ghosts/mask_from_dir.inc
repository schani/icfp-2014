
;;;;;;; -------------------------------------------------------------
;;;;;;; create mask from direction
;;;;;;; -------------------------------------------------------------
;;;;;;; subroutine
;;;;;;; 	@a0 ... direction
;;;;;;; returns:
;;;;;;; 	@r0 ... direction mask
;;;;;;; scratch:
;;;;;;; cost:
;;;;;;; 	13+ret/3+ret/5+ret

$mask_from_dir:
	jeq $mask_from_dir|u,@a0,0
	jeq $mask_from_dir|r,@a0,1
	jeq $mask_from_dir|d,@a0,2
	jeq $mask_from_dir|l,@a0,3
	mov @r0,0
$mask_from_dir|ret:
	ret

$mask_from_dir|u:
	mov @r0,1
	bra $mask_from_dir|ret

$mask_from_dir|r:
	mov @r0,2
	bra $mask_from_dir|ret

$mask_from_dir|d:
	mov @r0,4
	bra $mask_from_dir|ret

$mask_from_dir|l:
	mov @r0,8
	bra $mask_from_dir|ret
