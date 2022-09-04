#pragma once

#include "Common/Misc.h"

#include "Net/NetCommon.h"
//#include "Net/NetConnection.h"


namespace Net {


	// Forward deklaration
	class Connection;


	// Base type for the message type enum
	typedef uint32_t MessageTypeBase;


	// MessageHeader will be send at start of each message
	struct MessageHeader
	{
		// Type of message
		MessageTypeBase type = 0;
		// Size of message body (0 if header only)
		uint32_t size = 0;
		// Checksum of type/size
		uint32_t crc_header = 0;
		// Checksum of body contents
		uint32_t crc_body = 0;
	};


	class Message
	{
	public:
		template<typename T>
		Message(T type) { SetType(type); }
		Message() : Message(0) { }
		Message(const Message&) = delete; // no copy constructor
		~Message() = default;

	public:
		// Set type value of message header
		template <typename T>
		void SetType(T value) { header.type = static_cast<MessageTypeBase>(value); }
		// Updates the message header
		void UpdateHeader();
		// Recalculates the crc_* fields of the header
		void UpdateCRC();

		// Check if message contains valid data
		bool IsValid();
		// Check if message.header contains valid data (header.crc_header is correct)
		bool IsHeaderValid();
		// Check if message.body contains valid data (header.crc_body is correct)
		bool IsBodyValid();

		// Pushes <count> data-bytes onto the body stack
		void PushData(const void* data, size_t count);
		// Pulls <count> data-bytes from the body stack
		void PullData(void* data, size_t count);

		// Pushes a size value on the body stack
		void PushSize(const size_t size);
		// Pulls a size value from the body stack
		size_t PullSize();

	public:
		// Header of message
		MessageHeader header{};
		// Data container of message
		std::vector<uint8_t> body;
		// Client-Connection
		Ref<Connection> remote;
	};

	// Creates a new message
	inline Ref<Message> NewMessage() { return CreateRef<Message>(0); }
	// Creates a new message with optional type
	template<typename T>
	inline Ref<Message> NewMessage(T type) { return CreateRef<Message>(type); }


} // namespace Net


#include "Net/NetMessageOperators.h"
