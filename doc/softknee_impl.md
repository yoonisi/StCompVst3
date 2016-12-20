# soft knee characteristics
![softknee](softknee.png)

## Getting a and p

$$ y = a(x-p)^2+q $$
$$ y = ax^2 - 2apx + 2ap^2 + q $$
$$ y'=2ax-2ap $$
$$ 1 = 2ab-2ap = 2a(b-p) $$
$$ a = 1/(2(b-p)) $$  

$$ b = (b^2 - 2bp + p^2)/(2(b-p)) + q $$
$$ 0 = p^2 - 2qp + 2bq - b^2 $$
$$ p = (2q ± sqrt((-2q)^2-4(2bq - b^2)))/(2) $$  
  
* for example
threshold = 0.8, knee = 0.6  
$$ q = 0.8 $$
$$ b = 0.48 $$
$$ p = (1.6±0.64)/2 = 1.12 or 0.48 = 1.12 $$
$$ a = -0.7825  $$
