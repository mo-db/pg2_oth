#include <iostream>
#include <vector>
#include <functional>

template <class T>
std::vector<T> filter(const std::vector<T> &input_elements,
                      const std::function<bool(T)> filter_func) {
  std::vector<T> result;
  for (const T &element : input_elements) {
    if (filter_func(element)) {
      result.push_back(element);
    }
  }
  return result;
};

int main() {
  std::vector<std::string> names = { "Moritz", "Hannah", "Love" };

  auto result1 = filter<std::string>(
      names, [](const std::string &str) { return str[0] == 'M'; });
	std::cout << "result1:" << std::endl;
  for (auto &str : result1) {
    std::cout << str << std::endl;
  }

  auto result2 = filter<std::string>(
      names, [](const std::string &str) { return str.size() == 4; });
	std::cout << "result2:" << std::endl;
  for (auto &str : result2) {
    std::cout << str << std::endl;
  }
  return 0;
}
