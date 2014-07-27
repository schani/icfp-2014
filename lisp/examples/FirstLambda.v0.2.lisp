
(setq w 0) ; wall is value 0

;;; das ist ganz wichtig:
; hier kann man z.b. optimieren (die drei rows, die in frage kommen extra aufheben oder so

(defun get_tile (coordinate world_map) (1)) ; dummy value floor tile

;;; get tile list
;
; (cond t0 (cond t1 (cond t2 t3)))
; holds tiles according to direction

(defun calc_for_decision (curr_dir, curr_pos, world_map)
(
	(setq curr_x (first curr_pos))
	(setq curr_y (rest curr_pos))
    (cond
        ((= curr_dir 0) (setq curr_y (- curr_y 1)))
        ((= curr_dir 1) (setq curr_x (+ curr_x 1)))
        ((= curr_dir 2) (setq curr_y (+ curr_y 1)))
        ((= curr_dir 3) (setq curr_x (- curr_x 1)))
    )
	(setq c (cons curr_x curr_y) )
    (setq f (get_tile c world_map))
))


(defun get_current_direction (status) (first rest status))

(defun get_current_position (status) (first rest rest status))

(defun modadd (x, y) (% (+ x y) 4))

;;;function step: ; most primitive lambda                                              

(defun step (ai_state world_state) (
    (setq world_map (first world_state))
    (setq lambda_man_status (first rest world_state))
    (setq ghost_status (first rest rest world_state))
    (setq fruit_status_and_location (first rest rest rest world_state))

	(setq a (get_current_direction(lambda_man_status)))
	(setq b (get_current_position(lambda_man_status)))

	calc_for_decision (a, b)

    (if (= f w)
        ( ; it's a wall, need to find new direction
	    (setq a (modadd a 1))
	    calc_for_decision (a, b)
        (if (= f w)
            ( ; it's a wall, need to find new direction
            (setq a (modadd a 2))
            calc_for_decision (a, b)
            (if (= f w)
                ( ; it's a dead end: turn around
                (setq a (modadd a 3))
		        ) () )
            ) () )
        ) () )
	return a ; desired and possible direction
    )
)

; Problem: Lambda with direction 1 yields life lock (solve this later)
; #####.#
; \.....#
; ##.##.#
; ##....#
; #######
