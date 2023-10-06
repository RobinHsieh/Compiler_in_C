; This is a Scheme program that computes the factorial of a number.
; It is a recursive program.

(define factorial (fun (n)
  (if (= n 0)
      1
      (* n (factorial (- n 1))))))
(print-num (factorial 5))

(print-num (= 6 6 6 (* 2 3)))

(define bar (fun (x) (+ x 1)))
(define bar-y (fun () 1))
(define bar-z (fun (x) (+ (if (= 1 1) 1 2) (if (= x (bar (bar-y))) 1 2))
))
(print-num (bar-z 1))
(print-num (bar-z 2))
(print-num (bar-z 3))