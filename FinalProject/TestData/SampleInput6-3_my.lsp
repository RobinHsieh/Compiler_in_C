(define a (and #t #f #t #t))

(define b (not (> 1 2)))

(print-bool (= 6 5 6 (* 2 3)))

(print-bool (= 6 6 6 (* 2 3)))

(define three 3)

(define seven 7)

(define ten-plus-one (- (* three seven) (+ three seven)))

(print-bool (not (= 6 6 6 (* 2 3))))

(print-bool (or (not (= 6 6 6 (* 2 3))) b))

(print-bool (if (or (not (= 6 6 6 (* 2 3))) b) (or a b) (and a b)))

(print-num (* 1 2 3 4 5 6 7 8 9 10 ten-plus-one))

(print-bool (= 2 2 (mod 5 three) 2 2 (/ (* 1 1 1 1 2 seven) seven) 2 2 2 2 2 2 2))
