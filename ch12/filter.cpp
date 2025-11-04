#include <iostream>
#include <vector>

template<class T> class FilterType {
public:
  virtual bool operator()(const T &) const = 0;
};

class n_letters : public FilterType<std::string> {
	size_t n{};
public:
	n_letters(const size_t n_) : n(n_) {}
	bool operator()(const std::string& string) const override {
		return string.size() == n;
	}
};

class starts_with : public FilterType<std::string> {
	char c{};
	public:
	starts_with(char c_) : c(c_) {} 
	bool operator()(const std::string& string) const override {
		return string[0] == c;
	}
};

template<class T> std::vector<T>
filter(const std::vector<T>& input_elements, const FilterType<T>& filter_type) {
	std::vector<T> result;
	for (const T& element : input_elements) {
		if (filter_type(element)) {
			result.push_back(element);
		}
	}
	return result;
};

int main() {
	std::vector<std::string> names = { "Moritz", "Hannah" , "Love"};
	// why dont i need a type here?
	auto result1 = filter(names, starts_with('M'));
	std::cout << "result1:" << std::endl;
	for (auto& str : result1) {
		std::cout << str << std::endl;
	}

	auto result2 = filter(names, n_letters(4));
	std::cout << "result2:" << std::endl;
	for (auto& str : result2) {
		std::cout << str << std::endl;
	}
	return 0;
}
