#include <cmath>
#include <vector>
#include <iostream> 
#include <chrono>

void printNumbers( )  { 
  for (int i = 1;  i <= 7;  i++) {
    for (int j = 1;  j <= i;  ++j)
      std::cout << j;                   
    for (int k = 7 - i;  k >= 1;  k--)  
      std::cout << "*";                  
    std::cout << std::endl;  }
} 

std::vector<int> sieve(int n) {
    std::vector<int> primes = std::vector<int>( );
    if (n < 2) return primes;
    std::vector<bool> candidates(n + 1, true);
    int sqrtN = (int)sqrt(n);
    for (int i = 2;  i <= sqrtN;  i++) {
        if (!candidates[i])  continue;
        for (int j = i * i; j <= n; j += i) 
            candidates[j] = false;
    }
    primes.push_back(2);
    for (int i = 3; i <= n; i += 2) 
        if (candidates[i]) 
            primes.push_back(i);
    return primes;
}

std::vector<int> sieveOld(int n)  {  int sqrtN = (int)sqrt(n);
    std::vector<int> primes = std::vector<int>( );
    std::vector<bool> candidates(n + 1, true);
    for (int i = 2;  i <= sqrtN; ++i) {
        for (int j = i; i * j <= n;  ++j)  
            candidates[i * j] = false;  }
    for (int i = 2; i <= n; ++i)    
        if (candidates[i])  primes.push_back(i);
    return primes; }

int main()  {
  std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
  std::vector<int> primes = sieve(10000);
  std::cout << std::endl;
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  std::cout << "Computation time: "
           << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
              << "us.\n";
  return 0;
}
