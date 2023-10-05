(define factorial (fun (n)
  (if (= n 1)
      1
      (* n (factorial (- n 1))))))
(print-num (factorial 10)) ; 3628800


(define fib (fun (n)
  (if (< n 2)
      n
      (+ (fib (- n 1)) (fib (- n 2))))))
(print-num (fib 10)) ; 55


(define sum-to-n (fun (n)
  (if (= n 1)
      1
      (+ n (sum-to-n (- n 1))))))
(print-num (sum-to-n 100)) ; 5050


(define power (fun (base exp)
  (if (= exp 0)
      1
      (* base (power base (- exp 1))))))
(print-num (power 2 10)) ; 1024


(define power-then-times-what (fun (base exp n)
  (* n (power base exp))))
(print-num (power-then-times-what 2 10 5)) ; 5120


(print-num
  (+ 
    (if (= 1 1) 1 2) (if (= 1 2) 1 2)) ; 3
)
