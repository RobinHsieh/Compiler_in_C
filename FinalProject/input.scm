; 計算階乘
(define factorial
  (fun (n)
    (if (= n 0)
      1
      (* n (factorial (- n 1))))))

; 計算二項式係數
(define binomial-coefficient
  (fun (n k)
    (/ (factorial n)
      (* (factorial k) (factorial (- n k))))))

; 計算二項式係數固定n
(define binomial-coefficient-fixed-n-10
  (fun (k)
    (binomial-coefficient 10 k)))

; 求和a to b
(define sum-a-to-b
  (fun (function-to-sum a b)
    (if (= a b)
      (function-to-sum a)
      (+ (function-to-sum a) (sum-a-to-b function-to-sum (+ a 1) b)))))

(print-num (sum-a-to-b (fun (k) (binomial-coefficient 10 k)) 0 5))
(print-num (sum-a-to-b binomial-coefficient-fixed-n-10 0 5))

