# Big_integer_Cpp
Currently this BigInt class supports all relational, arithmetical, assignment and Unary operators.


* Make sure to include the following headerfiles :- iostream, iomanip and vector.

* This class will works on C++ 14 and above.

* Karatsuba multiplication is used for multiplication, as a result if the length of the number (number of digits) is more than 2.5 * 10 ^ 8 (250000000) (approx) then it may give wrong results (look at Convert_Base function to understand why this error will be inevitable with such large numbers). There is also a function named Multiply_Naive which use O(N ^ 2) algorithm but will work with any length (Though i don't think you would ever need to multiply numbers of order 10 ^ 8 so, using the overloaded * which uses karatsuba is good enough) [Multiply_Naive will work better for small Numbers].
