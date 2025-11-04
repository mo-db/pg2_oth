#include <iostream>
#include <vector>
#include <forward_list>

// could also be a funciton
class Observer {
	int id;
public:
	Observer(int id_) : id(id_) {}
	void on_notify() {
		std::cout << "observer: " << id << std::endl;
	}
};

class Subject {
	// maybe weak_ptr here? maybe no vector?
	std::forward_list<Observer*> observers;
public:
	void add_observer(Observer* observer) {
		observers.push_front(observer);
	}
	void remove_observer(Observer* observer) {
		observers.remove(observer);
	}
	void notify() {
		for (auto& observer : observers) {
			observer->on_notify();
		}
	}
};

int main() {
	Subject subject;
	Observer ob0{0};
	Observer ob1{1};

	// this leads to UB
	{
		Observer ob2{2};
		subject.add_observer(&ob0);
		subject.add_observer(&ob1);
		subject.add_observer(&ob2);
	}

	subject.notify();

	// subject.remove_observer(&ob2);

	subject.notify();

	return 0;
}
