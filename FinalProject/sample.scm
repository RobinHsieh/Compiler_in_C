; 最大公因數
(define gcd
        (fun (a b)
          (if (= b 0)
            a
            (gcd b (mod a b)))))
; 互為素數的檢查
(define coprime
  (fun (a b)
    (if (= (gcd a b) 1) ; Note: use the GCD function defined in previous examples
        #t
        #f)))
(print-bool (coprime 100 27))

; 二進位轉十進位
(define binary-to-decimal
  (fun (n)
    (if (= n 0)
        0
        (+ (mod n 10) (* 2 (binary-to-decimal (/ n 10)))))))
(print-num (binary-to-decimal 1100100))

; 十進位轉二進位
(define decimal-to-binary
  (fun (n)
    (if (= n 0)
        0
        (+ (mod n 2) (* 10 (decimal-to-binary (/ n 2)))))))
(print-num (decimal-to-binary 100))

; 判斷一個數字是否為回文數
(define reverse-num
  (fun (n)
    (if (= n 0)
        0
        (+ (mod n 10) (* 10 (reverse-num (/ n 10)))))))
(define is-palindrome
  (fun (n)
    (= n (reverse-num n))))
(print-bool (is-palindrome 12321))

; 求一個數字的所有位數之和
(define sum-of-digits
  (fun (n)
    (if (= n 0)
        0
        (+ (mod n 10) (sum-of-digits (/ n 10))))))
(print-num (sum-of-digits 12345))

; 求一個數字的所有位數之積
(define product-of-digits
  (fun (n)
    (if (= n 0)
        1
        (* (mod n 10) (product-of-digits (/ n 10))))))
(print-num (product-of-digits 12345))

; 最小公倍數
(define lcm
        (fun (a b)
          (/ (* a b) (gcd a b))))
(print-num (lcm 10 5))

; 階乘
(define factorial
  (fun (n)
    (if (= n 0)
      1
      (* n (factorial (- n 1))))))
(print-num (factorial 10))

; 費波那契數列
(define fibonacci
  (fun (n)
    (if (or (= n 0) (= n 1))
      1
      (+ (fibonacci (- n 1)) (fibonacci (- n 2))))))
(print-num (fibonacci 10))

; 二項式係數
(define binomial-coefficient
  (fun (n k)
    (if (or (= k 0) (= k n))
      1
      (+ (binomial-coefficient (- n 1) (- k 1))
        (binomial-coefficient (- n 1) k)))))
(print-num (binomial-coefficient 10 5))

; 二項式係數 v2
(define binomial-coefficient-v2
  (fun (m k)
    (/ (factorial m) (* (factorial k) (factorial (- m k))))))
(print-num (binomial-coefficient-v2 10 5))

; 梅森數
(define mersenne
  (fun (n current-power)
    (if (= n 1)
      (- current-power 1)
      (mersenne (- n 1) (* current-power 2)))))
(print-num (mersenne 10 2))

; Ackermann函數
(define ackermann
  (fun (m n)
    (if (= m 0)
      (+ n 1)
      (if (= n 0)
        (ackermann (- m 1) 1)
        (ackermann (- m 1) (ackermann m (- n 1)))))))
(print-num (ackermann 3 3))

; a的n次方
(define power
  (fun (a n)
    (if (= n 0)
      1
      (* a (power a (- n 1))))))
(print-num (power 3 10))

