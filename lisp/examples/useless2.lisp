;; This program takes two integer arguments and adds them.
(defn add [a b]
      (+ a b))

(defn main [a b]
      (add (if (= 1 2) 0 a)
	   b))
