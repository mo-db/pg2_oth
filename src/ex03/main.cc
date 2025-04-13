#include <typeinfo>
#include <iostream>
#include <complex>

using namespace std;
using namespace std::literals;

int main() {
  auto a = exp(M_PI * 1i) + 1.;
  cout << typeid(a).name() << endl << "value: " << a << endl;
  return 0;
}
