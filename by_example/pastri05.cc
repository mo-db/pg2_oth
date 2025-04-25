// Drawing a sirepinsky triangle using ranges::views

#include <iostream>
#include <vector>
#include <numeric> // accumulate
#include <algorithm> // ranges::copy
#include <iterator> // ostream_iterator
#include <format>
#include <ranges>
//#define NDEBUG // must be defined bevore the cassert include
#include <cassert> // assert macro


// calculate the row from the sum of the 2 values from last row
// 1 at begin, 1 at end
// return vector row, the return is a rvalue
std::vector<int> get_next_row(const std::vector<int> & last_row)
{
    std::vector next_row{ 1 };
    if (last_row.empty())
    {
        return next_row;
    }
    for (size_t idx = 0; idx+1 < last_row.size(); ++idx)
    {
        next_row.emplace_back(last_row[idx] + last_row[idx + 1]);
    }
    next_row.emplace_back(1);
    return next_row;
}

// performance improvement:
// instead of copying the unnamed vector that get_next_row returns into 
// the data vector, here we give the last element directly
// the copy was a lvalue, triangle.back() is a rvalue
// rvalues are nameless
// giving an rvalue is perfect forwarding
auto generate_triangle(int rows)
{
    std::vector<std::vector<int>> triangle{ {1} };
    for (int row = 1; row < rows; ++row)
    {
        triangle.push_back(get_next_row(triangle.back()));
    }
    return triangle;
}

bool is_palindrome(const std::vector<int>& v)
{
    auto forward = v | std::views::take(v.size() / 2);
    auto backward = v | std::views::reverse
                      | std::views::take(v.size() / 2);
    return std::ranges::equal(forward, backward);
}

// we do some property checking to show some c++ features
void check_properties(const std::vector<std::vector<int>> & triangle)
{
	size_t row_number = 1;
	int expected_total = 1;
	for (const auto& row : triangle) {
		// to not stop after the first error:
		// sum a string of error messages and assert if this string is empty
		assert(row.back() == 1);
		assert(row.front() == 1);
		assert(row.size() == row_number++);

		// sum of numbers in row of pascal triangle is a power of 2
		assert(std::accumulate(row.begin(), row.end(), 0) == expected_total);
		std::cout << expected_total << std::endl;
		expected_total *= 2;

		// check if all values of row are positive (overflow can lead to neg)
		// ranges save us from typing begin() and end()
		assert(std::ranges::all_of(row, [](auto x) { return x >= 0; }));

		// doing stuff using ranges::views, and using algorithm pipes
		// auto negative = [](int x) { return x < 0; };
		// auto negatives = row | std::views::filter(negative);
		auto negatives = row | std::views::filter([](int x) { return x < 0; });
		assert(negatives.empty());
		assert(is_palindrome(row));
	}
}

// print sierpinsky triangle
void show_view(std::ostream &s, const std::vector<std::vector<int>> &v) {
  std::string spaces(v.back().size(), ' ');
  for (const auto &row : v) {
    s << spaces;
    if (spaces.size())
      spaces.resize(spaces.size() - 1);
    auto odds =
        row | std::views::transform([](int x) { return x % 2 ? '*' : ' '; });
    for (const auto &data : odds) {
      s << data << ' ';
    }
    s << '\n';
  }
}

int main() {
	auto triangle = generate_triangle(33);
	check_properties(triangle);
	show_view(std::cout, triangle);

}

