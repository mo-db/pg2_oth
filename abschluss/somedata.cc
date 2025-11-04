#include <iostream>
using namespace std;
class someData {
	private: 
		const char *data;
		int len;
	public:
		class reverse {
			private:
				const char *step;
			public:
				char operator *() { return *step; }
				reverse operator ++() {
					--step;
					return *this;
				}
				bool operator !=(reverse o) {
					return step != o.step;
				}
				reverse(const char *p) : step(p) {};
		};
		typedef reverse iterator;
		iterator begin() {
			return data + len - 1; 
		}
		iterator end() {
			return data - 1;
		}
		someData(const char *d, int l) : data(d), len(l) {};
};

int main() {
	someData chars("Hallo Welt", 10);
	// range based for loop -> does the same as ptr{begin}, ptr{end}
	for (char c : chars)
		cout << c;
	cout << endl;
	return 0;
}
