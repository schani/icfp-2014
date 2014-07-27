;; This program returns 3.
(defn add [x]
  (+ (car x) (cdr x)))

(defn main []
  (add (cons 1 2)))
