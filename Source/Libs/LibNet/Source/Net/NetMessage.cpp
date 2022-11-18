#include "Net/NetMessage.h"
#include "Net/NetErrors.h"


namespace Net {


	// ========================================================================
	// Updates the message header
	void Message::UpdateHeader()
	{
		// TODO... Message::UpdateHeader()
	}


	// ========================================================================
	// Recalculates the crc_* fields of the header
	void Message::UpdateCRC()
	{
		// Calculate CRC for header
		header.crc_header = 0;
		header.crc_header += header.type;
		header.crc_header += header.size;

		// Calculate CRC for body
		header.crc_body = 0;
		for (auto x : body)
			header.crc_body += x;
	}


	// ========================================================================
	// Check if message contains valid data
	bool Message::IsValid()
	{
		// Check body, it will check header first
		if (!IsBodyValid())
			return false;

		return true;
	}


	// ========================================================================
	// Check if message.header contains valid data (header.crc_header is correct)
	bool Message::IsHeaderValid()
	{
		// Calculate CRC
		uint32_t check = 0;
		check += header.type;
		check += header.size;

		// Check
		if (check != header.crc_header)
		{
			SetLastError(ErrorCode::MsgErrorHeaderCRC);
			return false;
		}

		return true;
	}


	// ========================================================================
	// Check if message.body contains valid data (header.crc_body is correct)
	bool Message::IsBodyValid()
	{
		// Check header first
		if (!IsHeaderValid())
			return false;

		// Calculate CRC
		uint32_t check = 0;
		for (auto x : body)
			check += x;

		// Check
		if (check != header.crc_body)
		{
			SetLastError(ErrorCode::MsgErrorBodyCRC);
			return false;
		}

		return true;
	}


	// ========================================================================
	// Pushes <count> data-bytes onto the body stack
	void Message::PushData(const void* data, size_t count)
	{
		// Cache current size of vector, as this will be the point we insert the data
		size_t i = body.size();
		// Resize the vector by the size of the data being pushed
		body.resize(body.size() + count);
		// Physically copy the data into the newly allocated vector space
		std::memcpy(body.data() + i, data, count);
		// Recalculate the message size
		header.size = (uint32_t)body.size();
	}


	// ========================================================================
	// Pulls <count> data-bytes from the body stack
	void Message::PullData(void* data, size_t count)
	{
		// error check
		if (body.empty())
		{
			SetLastError(ErrorCode::MsgErrorBodyEmpty);
			return;
		}
		// Cache the location towards the end of the vector where the pulled data starts
		size_t i = body.size() - count;
		// Physically copy the data from the vector into the user variable
		std::memcpy(data, body.data() + i, count);
		// Shrink the vector to remove read bytes, and reset end position
		body.resize(i);
		// Recalculate the message size
		header.size = (uint32_t)body.size();
	}


	// ========================================================================
	// Pushes a size value on the body stack
	void Message::PushSize(const size_t size)
	{
		// Cache current size of vector, as this will be the point we insert the data
		size_t i = body.size();
		// Resize the vector by the size of sizevalue being pushed
		body.resize(body.size() + sizeof(uint32_t));
		// Physically copy the data into the newly allocated vector space
		std::memcpy(body.data() + i, &size, sizeof(uint32_t));
		// Recalculate the message size
		header.size = (uint32_t)body.size();
	}


	// ========================================================================
	// Pulls a size value from the body stack
	size_t Message::PullSize()
	{
		// error check
		if (body.empty())
		{
			SetLastError(ErrorCode::MsgErrorBodyEmpty);
			return 0;
		}

		uint32_t data_size;

		// Cache the location towards the end of the vector where the pulled data starts
		size_t i = body.size() - sizeof(uint32_t);
		// Physically copy the data from the vector into the user variable
		std::memcpy(&data_size, body.data() + i, sizeof(uint32_t));
		// Shrink the vector to remove read bytes, and reset end position
		body.resize(i);
		// Recalculate the message size
		header.size = (uint32_t)body.size();

		return data_size;
	}


} // namespace Net
