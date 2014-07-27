;; This bot alternates between moving right and down.  The AI state is
;; a single integer: 0 when the next move is right, 1 when the next
;; move is down.

(defn step [state world]
      (if state
	  (cons 0 2)			;down
	  (cons 1 1)))			;right

(defn main [world ghost-codes]
      (cons 0 step))
