#pragma once

#include "Common/Misc.h"

#include "Net/NetCommon.h"
#include "Net/NetMessage.h"


namespace Net {


	class MessageQueue
	{
	public:
		MessageQueue() = default;
		MessageQueue(const MessageQueue&) = delete; // no copy constructor
		~MessageQueue() { Clear(); }

	public:
		// Returns and maintains item at front of queue
		Ref<Message> GetFront();
		// Returns and maintains item at back of queue
		Ref<Message> GetBack();

		// Removes and returns item from front of queue
		Ref<Message> PopFront();
		// Removes and returns item from back of queue
		Ref<Message> PopBack();

		// Adds an item to front of queue
		void PushFront(Ref<Message> msg);
		// Adds an item to back of queue
		void PushBack(Ref<Message> msg);

		// Returns true if queue has no items
		bool IsEmpty();
		// Returns number of items in queue
		size_t GetCount();

		// Clears queue
		void Clear();
		// Waits until queue has messages
		void Wait();

	private:
		// Message storage
		std::deque<Ref<Message>> m_deque;
		// Notifier for Wait()
		std::condition_variable m_condBlocking;
		// Mutext for access on m_deque
		std::mutex m_mutexQueue;
		// Mutex for access on m_condBlocking
		std::mutex m_mutexBlocking;
	};


} // namespace Net
