
#include <queue>
#include <mutex>
#include <thread>

/// <summary>
	   /// A ConcurrentQueue class implements a thread-safe first in-first out (FIFO) collection. 
	   /// </summary>
template <typename T>
class ConcurrentQueue
{
public:
	/// <summary>
	/// Initializes a new instance of the ConcurrentQueue class. 
	/// </summary>
	ConcurrentQueue() : stopRequested_(false) { }

	/// <summary>
	/// Adds an object to the end of the ConcurrentQueue. 
	/// </summary>
	void Push(T value)
	{
		{
			unique_lock<mutex> lock(mutex_);
			queue_.push(value);
		}

		queueNotEmpty_.notify_one();
	}

	bool IsEmpty()
	{
		unique_lock<mutex> lock(mutex_);
		return queue_.empty();
	}

	/// <summary>
	/// Tries to remove and retrieve the object at the beginning of the ConcurrentQueue. 
	/// </summary>
	bool TryPop(T& value)
	{
		unique_lock<mutex> lock(mutex_);
		if (queue_.empty())
		{
			return false;
		}

		value = queue_.front();
		queue_.pop();

		return true;
	}

	/// <summary>
	/// Removes and retrieves the object at the beginning of the ConcurrentQueue,
	/// waiting if necessary until an object becomes available.
	/// </summary>
	bool WaitAndPop(T& value)
	{
		{
			unique_lock<mutex> lock(mutex_);
			queueNotEmpty_.wait(lock,
				[this] { return !queue_.empty() || stopRequested_; });
		}

		if (stopRequested_)
			return false;

		{
			unique_lock<mutex> lock(mutex_);

			if (!queue_.empty())
			{
				value = queue_.front();
				queue_.pop();
			}
		}

		return true;
	}

	/// <summary>
	/// Interrupts waiting for a new object. 
	/// </summary>
	void StopWait()
	{
		{
			unique_lock<mutex> lock(mutex_);
			stopRequested_ = true;
		}

		queueNotEmpty_.notify_all();
	}

private:
	bool stopRequested_;
	std::queue<T> queue_;
	mutable mutex mutex_;
	condition_variable queueNotEmpty_;
};