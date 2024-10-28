(import (chicken time))  ; Import the time module

(define start (current-milliseconds))  ; Record the start time
(define x 0.0)                    ; Initialize x

(do ((i 0 (+ i 1)))               ; Loop from 0 to 1,000,000
    ((>= i 1000000))              ; Loop condition
  (set! x (+ x 1.0)))             ; Increment x by 1.0 each iteration

(define end (current-milliseconds))     ; Record the end time
(print (- end start))              ; Print the time difference