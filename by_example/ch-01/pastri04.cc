// Check if the pascal triangle properties are correct using assert

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

void print_triangle(std::ostream& s,
		std::vector<std::vector<int>>& triangle) {
	size_t final_row_size = triangle.back().size();
	std::string spaces(final_row_size * 3, ' ');
	for (const auto& row : triangle) {
		s << spaces; // why can i do this? i have no variable spaces
		if (spaces.size() > 3) {
			spaces.resize(spaces.size() - 3);
		}
		for (const int& data: row) {
			s << std::format("{: ^{}}", data, 6);
		}
		s << std::endl;
	}
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

int main() {
	auto triangle = generate_triangle(16);
	check_properties(triangle);
	print_triangle(std::cout, triangle);


	// auto v = std::vector{ 0, 1, 2, 3, 4, 5 };
	// auto new_end = std::remove_if(v.begin(), v.end(),
			// [](int i) { return i < 3; });
	// std::cout << '\n';
	// // for (int n : v) {
	// // 		std::cout << n << ' ';
	// // }
	// // for (auto it = v.begin(); it != new_end; ++it) {
	// // 		std::cout << *it << ' ';
	// // }
	// for (int n : std::views::drop_while(v, [](int i) { return i < 3; })) {
		 // std::cout << n << ' ';
	// }

}

