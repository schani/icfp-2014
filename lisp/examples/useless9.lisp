;; This program returns 3.
(defn main []
  (/ (let [a 2
	   b 4]
	  (if (or 0 0)
	      777
	      (if (or 1 0)
		  (if (or 0 1)
		      (if (or 0 (or 0 1))
			  (+ a b)
			  666)
		      888)
		  999)))
     2))
