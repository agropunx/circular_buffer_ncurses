/*
 *
 * ----------------------- cirBuff.h---------------------
 *
 */

#include <cstdio>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <sstream>
template <class T>
class cirBuff {
public:
	// using a smart pointer is safer (and we don't have to implement a destructor)
	explicit cirBuff(size_t size) :
		buffer(std::unique_ptr<T[]>(new T[size])),
		max_size(size)
	{

	}
	

	// For maximum usefulness we need to provide two versions of operator[];
	// one 'normal' version for non-const objects, and one for const objects.
	
	T &operator[](size_t idx)
		{
			if (idx >= max_size) {
			std::cout << "Array index out of bound, exiting";
			exit(0);
		}
	
		return buffer[idx];
	}

	const T &operator[](size_t idx) const
		{
                if (idx >= max_size) {
                        std::cout << "Array index out of bound, exiting";
                        exit(0);
                }

                return buffer[idx];
        }
	

	//---------- print whole buffer
        void print_buffer()
	{
		std::cout << "Buffer values ------------------------------------>"; 
		for (int i = 0 ; i < size(); i++)
		{
			std::cout << " " << buffer[i] << " ";
		}
		std::cout << "\n";
	}

	//---------- right & left shift buffer elements
	// pass the buffer argument by reference, std::unique_ptr<T[]> cannot be copied - moveonly type
	void right_shift(std::unique_ptr<T[]> const &b, size_t max){
		// shift elements forward
                for (int i = max ; i > 0 ; i--)
                {
			b[i] = b[i - 1];
                }
	}		

	void left_shift(std::unique_ptr<T[]> const &b, size_t max){

                // shift elements backwards
                for (int i = 0 ; i < max ; i++)
                {
                        b[i] = b[i + 1];
                }
        }
	
	//---------- insert an element at buffer back
	void push_back(T item)
	{
		//std::lock_guard<std::mutex> lock(mutex);

		if(full)
		{
			left_shift(buffer, max_size);
			buffer[max_size-1] = item;
		}
		else
		{
			buffer[tracker] = item;
			tracker += 1;
		}
		
		if ( tracker < max_size )
		{
			full = 0;
		}
		else
		{
			full = 1;
		}
	}

	//----------  insert an element at buffer front
        void push_front(T item)
        {
                //std::lock_guard<std::mutex> lock(mutex);
		
                if(full)
                {
			right_shift(buffer, max_size);
			buffer[0] = item;
                }
		else
		{
			right_shift(buffer, tracker);
			buffer[0] = item;
			tracker += 1;
		}

		
		// follwoing if/else could be directly inserted into isfull() method,
		// but it should be updated to be not const in order to be reassigned
		// and could lead to user modyfing it not adequately		
		if (tracker < max_size)
                {
                        full = 0;
                }
                else
                {
                        full = 1;
                }

        }

	//---------- reset buffer to initial state
	void reset_buffer()
	{
		//std::lock_guard<std::mutex> lock(mutex);
		tracker = 0;
		full = false;
	}
	
	//---------- buffer empty state 
	bool isempty() const
	{
		return (!full && (tracker == 0));
	}

	//---------- buffer full state
	bool isfull() const
	{
		return full;
	}
	
	//---------- buffer maximum capacity
	size_t capacity() const
	{
		return max_size;
	}

	//---------- current buffer size
	size_t size() const
	{
		return tracker;
	}
	
	std::unique_ptr<T[]> buffer;
	//std::unique_ptr<T[]> buffer;
private:
	size_t tracker = 0;
//	std::mutex mutex;
//	std::unique_ptr<T[]> buffer;
	const size_t max_size = 8;
	bool full = 0;
};

