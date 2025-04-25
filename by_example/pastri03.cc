// Print the triangle with ostream

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm> // ranges::copy
#include <iterator> // ostream_iterator
#include <format>

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

int main() {
	auto triangle = generate_triangle(16);
	print_triangle(std::cout, triangle);
}

