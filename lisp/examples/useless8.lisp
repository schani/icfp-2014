;; This program returns 3.
(defn main []
  (/ (let [a 2
	   b 4]
	  (if (and 0 1)
	      777
	      (if (and 1 0)
		  999
		  (if (and 1 1)
		      (if (and (and 1 1) 1)
			  (+ a b)
			  666)
		      888))))
     2))
