; -*- clojure -*-
; first try 

;; this bot is still extremely stupid and will life-lock in a up/down szenario
;; still I tried to comment this well so it is clear what happens where.

;; in this setting, if fed to the official simulator and map,
;; lambdaman will run to and stay in the upper rightmost vertical
;; segment and remain there. He will die once, and then the game 
;; enters an infinite loop as 3 ghosts get stuck in center and
;; ghost 1 enters a long closed loop that never reaces lambda.
;; (tested beyond 100k steps)

;; standard includes for this project, nothing fancy here

;; (include "../ucl/stdlib.ucl") ; automatic by means of map.ucl
(include "../ucl/map.ucl")
(include "../ucl/gamedata.ucl")

;; basic next_x field pos (as of emp idiot bot)

(defn next_x [direction x]
  (if (= direction 1)
    (+ x 1)
    (if (= direction 3)
      (- x 1)
      x)))

;; basic next_y field pos (as of emp idiot bot)

(defn next_y [direction y]
  (if (= direction 2)
    (+ y 1)
    (if (= direction 0)
      (- y 1)
      y)))

;; my dumb direction selector (utilizes surround list)

(defn newdir [direction surround] 
  (if (= 0 (car surround)) 
    (if (= 0 (car (cdr surround))) 
      (if (= 0 (car (cdr (cdr surround)))) 
        3
        2
      )
      1
    )
    0
  )
)

;; construct a new state list (proof of concept, numbers of turns in respective directions)

(defn calc_ai_state [state direction] 
  (let [nr_up (car state)
        nr_right (car (cdr state))
        nr_down (car (cdr (cdr state)))
        nr_left (cdr (cdr (cdr state)))
       ]
    (let [nr_up (if (= direction 0) (+ nr_up 1) nr_up)
          nr_right (if (= direction 1) (+ nr_right 1) nr_right)
          nr_down (if (= direction 2) (+ nr_down 1) nr_down)
          nr_left (if (= direction 3) (+ nr_left 1) nr_left)
         ]
      (cons nr_up (cons nr_right (cons nr_down nr_left)))
    )
  )
)

;; the stepper function

(defn lambdastep (ai_state world_state)
  ;; primary setup (knd of always need to do this)
  (let [
        world_map (worldstate_get_map world_state)          ; get world map
        lman_state (worldstate_get_lman_state world_state)  ; get lambda man state tuple
       ]
    (let [
          mypos (lman_get_location lman_state)              ; get lambda man current position tuple
          olddirection (lman_get_direction lman_state)      ; get current ("old") direction
         ]
      (let [
            myposx (car mypos)                              ; extract "x" position from current position tuple
            myposy (cdr mypos)                              ; extract "x" position from current position tuple
           ]
        ;;
        ;; now for some 'specials':
        ;; 
        (let [
              ; get content of potential next tile (if current direction is kept)
              tile (map_get (next_x olddirection myposx) (next_y olddirection myposy) world_map)
              ; get 4-tuple of surrounding tiles (slow now, might be optimized, if cycles are a problem)
              surroundings (map_get_surroundings myposx myposy world_map)
             ]
          ;;
          ;; almost there: now determine new direction
          ;;
          (let [ 
                the_new_direction (if (= tile 0)            ; test whether next tile is wall
                      (newdir olddirection surroundings)    ; yes then find new direction
                      olddirection)                         ; no then keep old direction
               ]
            ;;
            ;; cleanup and return: update status list and return
            ;;
            (let [
                  ai_state (calc_ai_state ai_state the_new_direction)   ; create new status list
                 ]
              (cons ai_state the_new_direction)                         ; create return value (2-tuple)
            )
          )
        )
      )
    )
  )
)

;;
;; required main function
;;
(defn main [world_state undoc] 
  (let [
        ai_status (cons 0 (cons 0 (cons 0 0))) ; remember nr of counts for each direction
       ]
    (cons ai_status lambdastep)                ; return initializer 2-tuple (status and step function)
  )
)

