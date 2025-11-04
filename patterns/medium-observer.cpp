#include <iostream>
#include <vector>
#include <forward_list>

class Subject;

// could also be a funciton
class Observer {
public:
	~Observer() {}
	virtual void on_notify() = 0;
};

class Watcher : public Observer {
	int id;
public:
	// Observer(Subject& subject_, int id_) : subject(subject_), id(id_) {
	explicit Watcher(int id_) : id(id_) {}

	void on_notify() override {
		std::cout << "watcher: " << id << std::endl;
	}
};

class Subject {
	// maybe weak_ptr here? maybe no vector?
	std::forward_list<Observer*> observers;
public:
	// Subject() {}
	virtual ~Subject() {}
	
	virtual void add_observer(Observer* observer) {
		observers.push_front(observer);
	}
	
	virtual void remove_observer(Observer* observer) {
		observers.remove(observer);
	}
	
	void notify() {
		for (auto& observer : observers) {
			observer->on_notify();
		}
	}
};

class SomeSubject : public Subject {
	// maybe weak_ptr here? maybe no vector?
	std::forward_list<Observer*> observers;
public:
};



int main() {
	SomeSubject subject;
	Watcher w0{0};
	Watcher w1{1};

	// this leads to UB
	{
		Watcher w2{2};
		subject.add_observer(&w0);
		subject.add_observer(&w1);
		subject.add_observer(&w2);
	}

	subject.notify();

	// subject.remove_observer(&w2);

	subject.notify();

	return 0;
}
