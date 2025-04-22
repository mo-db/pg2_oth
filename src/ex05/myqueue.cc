#include <iostream>

using namespace std;

class Queue {
	private:
		int *data, max, head, tail, size;
	public:
		// mutators
		// member functions
		void enqueue(int); 
		int dequeue();
		int queueEmpty() const;
		Queue(int); // constructor declaration
		~Queue(); // destructor
		// friend functions
		// friend is no member function, grants acces to private members
		friend Queue &operator +(Queue &, int); 
		friend int operator -(Queue &);
		// accessors
		friend void queuePrint(Queue &queue);
		int elements() const;
		bool empty() const;
		// void queuePrint(Queue &queue);

};

int main() {
	// Queue q; // stack ram allocated here
  constexpr int max = 100;
	Queue myQueue { max };
  myQueue + 4 + 7 + 1 + 1 + 4 + 2;
  int limit = myQueue.elements( );
  for (int i = 0;  i < limit;  ++i)   {
    int a = -myQueue;
    for (int j = 0;  j < limit - 1;  ++j)  {
      int b = -myQueue;
      if (a < b)  myQueue + b;
      else myQueue + a, a = b;
    }
    myQueue + a;
  }
  queuePrint(myQueue);
  std::cout << std::endl;
	return 0;
}

Queue::Queue(int max) {
	head = tail = size = 0;
	data = new int[this->max = max];
}

Queue::~Queue() {
	delete [] data;
}

void Queue::enqueue(int value) {
	if (size == max) {
		cerr << "Queue overflow" << endl;
		exit(1); // isnt this bac? return from just somewhere in the code
						 // should rather use assert or return value right
	}
	data[tail] = value;
	tail++;
	size++; // shouldnt i rather use one value for size and tail?
}

int Queue::dequeue() {
	if (empty()) {
		cerr << "cannot dequeue empty queue" << endl;
		exit(1);
	}
	// i dont get this here.. so the queue will just be empty in the front?
	size--;
	int element = data[head];
	head++;
	return element;
}

// i dont know why to use or dont use inline
inline int Queue::elements() const { return size; }

inline bool Queue::empty () const { return size == 0;}

void queuePrint(Queue &queue) {
	for (int i = 0; i < queue.size; i++) {
		cout << queue.data[queue.head + i] << " ";
	}
	cout << endl;
}

// binary overloaded +
inline Queue &operator +(Queue &queue, int item) {
	queue.enqueue(item);
	return queue; // ?? so i return the reference to queue -> to where?
}

// unary overloaded -
inline int operator -(Queue &queue) {
	return queue.dequeue();
}

// void Queue::queuePrint(Queue &queue) {
// 	for (int i = 0; i < size; i++) {
// }
