#include <iostream>
#include <vector>
#include <numeric>

void generate_triangle() {
	const std::vector<int> data { 1, 2, 3 };
	for (auto number : data) {
		// std::cout << number << " ";
	}
	for (auto position = data.begin(); position!=data.end(); ++position)
		std::cout << *position << ' ';
	std::cout << std::endl;
	std::cout << std::numeric_limits<int>::max() << std::endl;
}

// calculate the row from the sum of the 2 values from last row
// 1 at begin, 1 at end
// return vector row
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

// auto return type -> std::vector<std::vector<int>>
auto generate_triangle(int rows)
{
    std::vector<int> data; // vector that holds row
    std::vector<std::vector<int>> triangle; // vector for the triangle
    for (int row = 0; row < rows; ++row)
    {
				// send data vector to function, then replace with new data vecotr
        data = get_next_row(data);
        triangle.push_back(data);
    }
    return triangle;
}

int main() {
}
