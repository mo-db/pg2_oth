#include <iostream>
#include <vector>
#include <forward_list>

// class Subject;

// could also be a funciton
class Observer {
public:
	~Observer() {}
	virtual void on_notify() = 0;
};

class Subject {
	// maybe weak_ptr here? maybe no vector?
	std::forward_list<Observer*> observers;
public:
	Subject() {}
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

class SomeObserver : public Observer {
	Subject& subject;
	int id;
public:
	explicit SomeObserver(Subject& subject_, int id_) : subject(subject_), id(id_) {
		subject.add_observer(this);
	}
	~SomeObserver() {
		subject.remove_observer(this);
	}

	void on_notify() override {
		std::cout << "watcher: " << id << std::endl;
	}
};

class SomeSubject : public Subject {
	// maybe weak_ptr here? maybe no vector?
	std::forward_list<Observer*> observers;
public:
};

int main() {
	SomeSubject subject;
	SomeObserver o0{subject, 0};
	SomeObserver o1{subject, 1};
	SomeObserver o2{subject, 2};


	subject.notify();

	// subject.remove_observer(&w2);

	subject.notify();

	return 0;
}
