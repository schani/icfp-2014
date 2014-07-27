# Usage

## Preliminaries

Install [Leiningen](https://github.com/technomancy/leiningen).

Then, in this directory, do

    lein deps

Now you can run the compiler, like so

    lein run examples/useless1.lisp /tmp/useless1.gcc

# Basics

It's a very straightforward LISP, with Clojure syntax.  A sample program:

    (defn fib [n]
	  (if (< n 2)
	    n
		(+ (fib (- n 2) (- n 1)))))

    (defn main [playfield ghost-code]
	  (fib 10))

The program must contain a `main` function.  Order of function
declaration is irrelevant---all functions are visible in all
functions.

## Functions

Only top-level functions are allowed.

    (defn name [arg1 arg2 ...]
	  body)

## Operators

    (+ a b)
	(- a b)
	(* a b)
	(/ a b)

	(= a b)
	(> a b)
	(>= a b)

    (integer? x)

Comparisons and `integer?` return either the integer `0` or `1`.

## Conses

    (cons a b)
	(car c)
	(cdr c)

## Conditional

    (if condition
	  consequent
	  alternative)

Where `condition` is an integer, with `0` being false, everything else
true.

## Binding

    (let [a a-value
	      b b-value
		  ...]
      body)

`let` works like in Common Lisp.  The bindings are only active within
the body, not in the value expressions.  For example, this:

    (let [a 1
	      b 2]
      (let [a b
	        b a]
	    (+ a b)))

returns 3.

## Loops, or lack thereof

Like Scheme and OCaml, this LISP doesn't provide any loops.  It does,
however, provide proper tail recursion, which is essentially a goto
with parameter passing.  For example, here's how to implement a
factorial that runs in constant space:

    (defn fac-loop [so-far n]
	  (if (> n 1)
	    (fac-loop (* so-far n) (- n 1))
		so-far))

    (defn fac [n]
	  (fac-loop 1 n))

Note that this implementation will consume stack space:

    (defn fac [n]
	  (if (> n 1)
	    (* n (fac (- n 1)))
		1))

The reason is that the function call to `fac` here is not in a tail
position, because there remains work to be done after the call has
finished, namely the multiplication.

## Main CPU interface

    (dbug! x)
	(brk!)

These must not be used where their results might matter.

# Extensions

Maybe we'll implement this later.

## Negation

    (- x)

should compile to

    (- 0 x)

## Logical operators

    (and x y)
	(or x y)

These should obviously do short circuit evaluation.

## Inline functions

Some things will be more efficient if inlined:

    (definline cadr [c]
	  (car (cdr c)))

	(definline < [a b]
	  (> b a))

## Sequence operator

    (do
	  something-first
	  something-second
	  ...
	  result)

Executes `something-first`, `something-second`, etc, for side effects,
then returns `result`.  Both `let` as well as function bodies already
have an implicit `do`, but the consequent and alternative of `if`
don't.  `do` can be simulated with `let`:

    (let []
	  something-first
	  something-second
	  ...
	  result)

## Closures

Right now functions are not first class, i.e. there's no `lambda`.  Do
we need that?

## Local variable modification

    (let [a 1]
	  (set! a (+ a 1))
	  a)

Doesn't really make sense without loops.
