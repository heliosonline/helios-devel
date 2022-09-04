#include "Net/NetMessageQueue.h"
#include "Net/NetErrors.h"


namespace Net {


	// ========================================================================
	// Returns and maintains item at front of queue
	Ref<Message> MessageQueue::GetFront()
	{
		std::scoped_lock scoped_lock(m_mutexQueue);
		if (m_deque.empty())
		{
			// TODO... MessageQueue::GetFront()
		}
		return m_deque.front();
	}


	// ========================================================================
	// Returns and maintains item at back of queue
	Ref<Message> MessageQueue::GetBack()
	{
		std::scoped_lock scoped_lock(m_mutexQueue);
		if (m_deque.empty())
		{
			// TODO... MessageQueue::GetBack()
		}
		return m_deque.back();
	}


	// ========================================================================
	// Removes and returns item from front of queue
	Ref<Message> MessageQueue::PopFront()
	{
		std::scoped_lock scoped_lock(m_mutexQueue);
		if (m_deque.empty())
		{
			// TODO... MessageQueue::PopFront()
		}
		auto t = m_deque.front();
		m_deque.pop_front();
		return t;
	}


	// ========================================================================
	// Removes and returns item from back of queue
	Ref<Message> MessageQueue::PopBack()
	{
		std::scoped_lock scoped_lock(m_mutexQueue);
		if (m_deque.empty())
		{
			// TODO... MessageQueue::PopBack()
		}
		auto t = m_deque.back();
		m_deque.pop_back();
		return t;
	}


	// ========================================================================
	// Adds an item to front of queue
	void MessageQueue::PushFront(Ref<Message> msg)
	{
		std::scoped_lock scoped_lock(m_mutexQueue);
		m_deque.push_front(msg);

		std::unique_lock<std::mutex> unique_lock(m_mutexBlocking);
		m_condBlocking.notify_one();
	}


	// ========================================================================
	// Adds an item to back of queue
	void MessageQueue::PushBack(Ref<Message> msg)
	{
		std::scoped_lock scoped_lock(m_mutexQueue);
		m_deque.push_back(msg);

		std::unique_lock<std::mutex> unique_lock(m_mutexBlocking);
		m_condBlocking.notify_one();
	}


	// ========================================================================
	// Returns true if queue has no items
	bool MessageQueue::IsEmpty()
	{
		std::scoped_lock scoped_lock(m_mutexQueue);
		return m_deque.empty();
	}


	// ========================================================================
	// Returns number of items in queue
	size_t MessageQueue::GetCount()
	{
		std::scoped_lock scoped_lock(m_mutexQueue);
		return m_deque.size();
	}


	// ========================================================================
	// Clears queue
	void MessageQueue::Clear()
	{
		std::scoped_lock scoped_lock(m_mutexQueue);
		m_deque.clear();
	}


	// ========================================================================
	// Waits until queue has messages
	void MessageQueue::Wait()
	{
		while (IsEmpty())
		{
			std::unique_lock<std::mutex> unique_lock(m_mutexBlocking);
			m_condBlocking.wait(unique_lock);
		}
	}


} // namespace Net
