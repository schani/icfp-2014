(ns lisp.core
  (:use [clojure.core.match :only (match)])
  (:require [clojure.java.io :as io]
            [clojure.string :as string]))

(def ^:private unary-operators
  {'atom? :atom
   'car :car
   'cdr :cdr})

(def ^:private binary-operators
  {'+ :add
   '- :sub
   '* :mul
   '/ :div
   '= :ceq
   '> :cgt
   '>= :cgte
   'cons :cons})

(defn ^:private unary-operator? [op]
  (contains? unary-operators op))

(defn ^:private binary-operator? [op]
  (contains? binary-operators op))

(defn ^:private if-keyword? [x]
  (= x 'if))

(defn ^:private takes-labels? [opcode]
  (contains? #{:sel :ldf :tsel} opcode))

(defn ^:private env-lookup [env name]
  (loop [env env
         n-up 0]
    (cond
     (empty? env)
     (throw (Exception. (str "Undefined variable " name)))

     (contains? (first env) name)
     [n-up ((first env) name)]

     :else
     (recur (rest env)
            (inc n-up)))))

(defn ^:private compile-expr [env expr]
  (match expr

         ([(kw :guard if-keyword?) condition consequent alternative] :seq)
         (let [consequent-label (gensym "consequent")
               alternative-label (gensym "alternative")
               [condition-pre condition-post] (compile-expr env condition)
               [consequent-pre consequent-post] (compile-expr env consequent)
               [alternative-pre alternative-post] (compile-expr env alternative)]
           [(concat condition-pre
                   [[:sel consequent-label alternative-label]])
            (concat condition-post
                    [[:label consequent-label]]
                    consequent-pre
                    [[:join]]
                    consequent-post
                    [[:label alternative-label]]
                    alternative-pre
                    [[:join]]
                    alternative-post)])

         ([(op :guard unary-operator?) x] :seq)
         (let [[x-pre x-post] (compile-expr env x)]
           [(concat x-pre
                    [(unary-operators op)])
            x-post])

         ([(op :guard binary-operator?) a b] :seq)
         (let [[a-pre a-post] (compile-expr env a)
               [b-pre b-post] (compile-expr env b)]
           [(concat a-pre
                    b-pre
                    [[(binary-operators op)]])
            (concat a-post b-post)])

         (x :guard list?)
         (let [[function & args] x
               arg-codes (map (partial compile-expr env) args)
               arg-pres (mapcat first arg-codes)
               arg-posts (mapcat second arg-codes)
               [function-pre function-post] (compile-expr env function)]
           [(concat arg-pres
                    function-pre
                    [[:ap (count args)]])
            (apply concat function-post arg-posts)])

         (x :guard number?)
         [[[:ldc x]]
          []]

         (x :guard symbol?)
         (let [[n i] (env-lookup env x)]
           [[[:ld n i]]
            []])))

(defn ^:private compile-function-body [env expr]
  (let [[pre post] (compile-expr env expr)]
    (concat pre
            [[:rtn]]
            post)))

(defn ^:private compile-toplevel [env code]
  (let [[def-type name args body] code]
    (let [env-frame (zipmap args (range))
          env (if (= name 'main)
                (concat env [env-frame])
                (cons env-frame env))]
      (compile-function-body env body))))

(defn ^:private toplevel-labels [toplevels]
  (map (fn [toplevel]
         (assert (= (first toplevel) 'defn) "Toplevel is not a defn")
         (vector (second toplevel) (gensym "toplevel")))
       toplevels))

(defn ^:private collect-labels [asm]
  (loop [asm asm
         index 0
         labels {}]
    (if (empty? asm)
      labels
      (let [insn (first asm)]
        (if (= (first insn) :label)
          (recur (rest asm)
                 index
                 (assoc labels (second insn) index))
          (recur (rest asm)
                 (inc index)
                 labels))))))

(defn ^:private resolve-labels [asm]
  (let [labels (collect-labels asm)]
    (loop [asm asm
           resolved-asm []]
      (if (empty? asm)
        resolved-asm
        (let [insn (first asm)
              opcode (first insn)]
          (cond
           (= opcode :label)
           (recur (rest asm)
                  resolved-asm)

           (takes-labels? opcode)
           (recur (rest asm)
                  (conj resolved-asm
                        (cons opcode (map labels (rest insn)))))

           :else
           (recur (rest asm)
                  (conj resolved-asm
                        insn))))))))

(defn ^:private stringify-insn [insn]
  (let [opcode (first insn)
        args (rest insn)]
    (apply str (interpose " "
                          (cons (string/upper-case (name opcode))
                                (map str args))))))

(defn ^:private stringify-asm [asm]
  (apply str (map (fn [insn] (str (stringify-insn insn) "\n")) asm)))

(defn ^:private read-all [stream]
  (let [sexpr-seq (repeatedly (fn [] (read stream false :theend)))]
    (take-while (partial not= :theend) sexpr-seq)))

(defn ^:private compile-file [in-filename out-filename]
  (with-open [reader (java.io.PushbackReader. (io/reader in-filename))
              writer (io/writer out-filename)]
    (let [toplevels (read-all reader)
          labels (toplevel-labels toplevels)
          labels-map (into {} labels)
          toplevel-env [(zipmap (map first labels) (range))]
          asms (map (fn [[name label] toplevel]
                      (concat [[:label label]]
                              (compile-toplevel toplevel-env toplevel)))
                    labels
                    toplevels)
          init-asm (concat [[:dum (count labels)]]
                           (map (fn [[name label]]
                                  [:ldf label])
                                labels)
                           [[:ldf (labels-map 'main)]
                            [:rap (count labels)]
                            [:rtn]])
          asm (resolve-labels (apply concat init-asm asms))
          asm-string (stringify-asm asm)]
      (assert (contains? labels-map 'main) "No main function defined")
      (.write writer asm-string))))

(defn -main [& args]
  (let [[in-filename out-filename] args]
    (compile-file in-filename out-filename)))
