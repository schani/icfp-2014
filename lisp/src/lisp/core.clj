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

(defn ^:private let-keyword? [x]
  (= x 'let))

(defn ^:private fn-keyword? [x]
  (= x 'fn))

(defn ^:private and-keyword? [x]
  (= x 'and))

(defn ^:private or-keyword? [x]
  (= x 'or))

(defn ^:private not-keyword? [x]
  (= x 'not))

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

(declare ^:private compile-function)
(declare ^:private compile-expr)

(defn ^:private compile-condition [env tail true-label false-label expr]
  (match expr

         ([(kw :guard and-keyword?) a b] :seq)
         (let [a-true-label (gensym "and")
               [a-pre a-post] (compile-condition env tail a-true-label false-label a)
               [b-pre b-post] (compile-condition env tail true-label false-label b)]
           [(concat a-pre
                    [[:label a-true-label]]
                    b-pre)
            (concat a-post
                    b-post)])

         ([(kw :guard or-keyword?) a b] :seq)
         (let [a-false-label (gensym "or")
               [a-pre a-post] (compile-condition env tail true-label a-false-label a)
               [b-pre b-post] (compile-condition env tail true-label false-label b)]
           [(concat a-pre
                    [[:label a-false-label]]
                    b-pre)
            (concat a-post
                    b-post)])

         ([(kw :guard not-keyword?) x] :seq)
         (compile-condition env tail false-label true-label x)

         :else
         (let [[pre post] (compile-expr env false expr)]
           [(concat pre
                    [[:tsel true-label false-label]])
            post])))

(defn ^:private compile-expr [env tail expr]
  (match expr

         ([(kw :guard if-keyword?) condition consequent alternative] :seq)
         (let [consequent-label (gensym "consequent")
               alternative-label (gensym "alternative")
               continuation-label (gensym "continuation")
               [condition-pre condition-post] (compile-condition env tail consequent-label alternative-label condition)
               [consequent-pre consequent-post] (compile-expr env tail consequent)
               [alternative-pre alternative-post] (compile-expr env tail alternative)]
           [(concat condition-pre
                    [[:label consequent-label]]
                    consequent-pre
                    (if tail
                      []
                      [[:ldc 0]
                       [:tsel continuation-label continuation-label]])
                    [[:label alternative-label]]
                    alternative-pre
                    (if tail
                      []
                      [[:label continuation-label]]))
            (concat condition-post
                    consequent-post
                    alternative-post)])

         ([(kw :guard let-keyword?) bindings body] :seq)
         (let [bindings (partition 2 bindings)
               binding-names (map first bindings)
               binding-exprs (map second bindings)
               binding-codes (map (partial compile-expr env false) binding-exprs)
               binding-pres (mapcat first binding-codes)
               binding-posts (mapcat second binding-codes)
               body-code (compile-function env binding-names false body)
               body-label (gensym "let")]
           [(concat binding-pres
                    [[:ldf body-label]]
                    [[(if tail :tap :ap) (count bindings)]])
            (concat binding-posts
                    [[:label body-label]]
                    body-code)])

         ([(op :guard unary-operator?) x] :seq)
         (let [[x-pre x-post] (compile-expr env false x)]
           [(concat x-pre
                    [[(unary-operators op)]]
                    (if tail [[:rtn]] []))
            x-post])

         ([(op :guard binary-operator?) a b] :seq)
         (let [[a-pre a-post] (compile-expr env false a)
               [b-pre b-post] (compile-expr env false b)]
           [(concat a-pre
                    b-pre
                    [[(binary-operators op)]]
                    (if tail [[:rtn]] []))
            (concat a-post b-post)])

         ([(op :guard fn-keyword?) args body] :seq)
         (let [body-code (compile-function env args false body)
               body-label (gensym :fn)]
           [(concat [[:ldf body-label]]
                    (if tail [[:rtn]] []))
            (concat [[:label body-label]]
                    body-code)])

         (x :guard list?)
         (let [[function & args] x
               arg-codes (map (partial compile-expr env false) args)
               arg-pres (mapcat first arg-codes)
               arg-posts (mapcat second arg-codes)
               [function-pre function-post] (compile-expr env false function)]
           [(concat arg-pres
                    function-pre
                    [[(if tail :tap :ap) (count args)]])
            (concat function-post arg-posts)])

         (x :guard number?)
         [(concat [[:ldc x]]
                  (if tail [[:rtn]] []))
          []]

         (x :guard symbol?)
         (let [[n i] (env-lookup env x)]
           [(concat [[:ld n i]]
                    (if tail [[:rtn]] []))
            []])))

(defn ^:private compile-function [env args is-main body]
  (let [env-frame (zipmap args (range))
        env (if is-main
              (concat env [env-frame])
              (cons env-frame env))
        [pre post] (compile-expr env true body)]
    (concat pre
            post)))

(defn ^:private compile-toplevel [env code]
  (let [[def-type name args body] code]
    (compile-function env args (= name 'main) body)))

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
