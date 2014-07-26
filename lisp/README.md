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

`let` works like in Clojure.  The bindings are active immediately in
the next pair, so for example this

    (let [a 1]
	  (let [a (+ a 1)
	        b (+ a 1)]
	    b))

returns 3.

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

## Tail calls or loops

Right now we have no way to do loops without consuming stack space.
We might implement either proper tail calls, like Scheme, or `loop`
like Clojure.

## Local variable modification

    (let [a 1]
	  (set! a (+ a 1))
	  a)

Doesn't really make sense without loops.
