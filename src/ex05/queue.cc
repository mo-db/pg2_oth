#include <iostream>
#include <cstdlib>

class queue {
  private: int *data, head, tail, size, max;
  public: 
    void enqueue(int);
    int dequeue( );  
    bool empty( ) const;  
    queue(int);  ~queue( );
    int elements( ) const;
    friend queue &operator +(queue &, int);
    friend int operator -(queue &);
    friend void print(queue &);
};

void print(queue &queue)  {
   for (int i = 0;  i < queue.size;  ++i)
     std::cout << queue.data[(queue.head + i) % queue.max] << " ";
   std::cout << std::endl;
}

void queue::enqueue(int value) { 
      if (size == max)  {
        std::cerr << "queue overflow" 
                  << std::endl;
        exit(1);  }  
      data[tail] = value;  tail = (tail + 1) % max;  size++;  }

int queue::dequeue( )  {
      if (empty( ))  {
        std::cerr << "attempt to remove element from empty queue"
                  << std::endl;
        exit(1);  }  size--;  
        int element = data[head];
        head  = (head + 1) % max;
      return element;  }

inline int queue::elements( ) const {  return size; }

inline bool queue::empty( ) const {  return size == 0;  }

queue::queue(int max)  { head = tail = size = 0; data = new int[this->max=max]; }

queue::~queue( ) { delete [] data; }

inline queue &operator +(queue &queue, int item)  { 
  queue.enqueue(item);  return queue;  }
   
inline int operator -(queue &queue)  {
  return queue.dequeue( );
}

#include <vector>
int main( )  {  
  constexpr int max = 100;  queue myQueue { max };
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
  print(myQueue);
  std::cout << std::endl;  return 0;  }

