#pragma once


namespace Net {


	// Convenience Operator overloads - These allow to add and remove stuff from
	// the body vector as if it were a stack, so First-In, Last-Out. These are a 
	// template in itself, because the data type the app is pushing or 
	// popping is unknown, so allow them all.
	//
	// NOTE: It assumes the data type is fundamentally Plain Old Data (POD).
	// TLDR: Serialise & Unserialise onto/from a vector
	//
	// Dataformat:
	//   On push: First pushes the data bytes then pushes the count of data bytes
	//   On pull: First pull the cont of data bytes then the data bytes itself
	// 
	// NOTE: The count of data bytes is stored to allow a basic type validation


	// Pushes any POD-like data onto the message buffer
	template <typename TDataType>
	inline Ref<Message> operator << (Ref<Message> msg, const TDataType data)
	{
		// Check that the type of the data being pushed is trivially copyable
		static_assert(std::is_standard_layout<TDataType>::value, "Data is too complex to be pushed into vector");

		// Push data onto the message buffer
		msg->PushData(&data, sizeof(TDataType));

		// Push size of data onto the message buffer
		msg->PushSize(sizeof(TDataType));

		// Return the target message so it can be "chained"
		return msg;
	}


	// Pulls any POD-like data from the message buffer
	template <typename TDataType>
	inline Ref<Message> operator >> (Ref<Message> msg, TDataType& data)
	{
		// Check that the type of the data being pulled is trivially copyable
		static_assert(std::is_standard_layout<TDataType>::value, "Data is too complex to be pulled from vector");

		// assert() if stored data size != expected size
		size_t data_size = msg->PullSize();
		//assert(data_size == sizeof(TDataType));

		// Pull data from message buffer
		msg->PullData(&data, sizeof(TDataType));

		// Return the target message so it can be "chained"
		return msg;
	}


	// ---------------------------------------------------------------------------
	// -- Spezializations --------------------------------------------------------
	// ---------------------------------------------------------------------------


	// Pushes an std::string onto the message buffer
	inline Ref<Message> operator << (Ref<Message> msg, const std::string& data)
	{
		// Push data onto the message buffer
		msg->PushData(data.c_str(), data.size());

		// Push size of data onto the message buffer
		msg->PushSize(data.size());

		// Return the target message so it can be "chained"
		return msg;
	}


	// Pulls an std::string from the message buffer
	inline Ref<Message> operator >> (Ref<Message> msg, std::string& data)
	{
		// Pull size of data from message buffer
		size_t data_size = (size_t)msg->PullSize();

		// Resize std:string to required size
		data.resize(data_size);

		// Pull data from message buffer
		msg->PullData(&data[0], data_size);

		// Return the target message so it can be "chained"
		return msg;
	}


} // namespace Net
