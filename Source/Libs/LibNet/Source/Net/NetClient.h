#pragma once

#include "Common/Misc.h"

#include "Net/NetCommon.h"
//#include "Net/NetMessage.h"
#include "Net/NetMessageQueue.h"
//#include "Net/NetConnection.h"


namespace Net {


	// Forward deklaration
	class Message;
//	class MessageQueue;
	class Connection;


	class Client
	{
	public:
		Client() {}
		Client(const std::string host, const uint16_t port);
		~Client();

	public:
		// Connect to server
		bool Connect();
		// Connect to server with hostname/ip-address and port
		bool Connect(const std::string host, const uint16_t port);
		// Disconnect from server
		void Disconnect();

		// Send message to server
		void Send(Ref<Message> msg);

		// Process incoming messages
		void Update(bool wait = false);
		// Process incoming messages
		void Update(size_t maxMessages = -1, bool wait = false);

		// Check if client is actually connected to a server
		bool IsConnected();

	protected:
		// Should be overridden by the application
		// Will be called if client connects
		virtual void OnConnect() {};
		// Should be overridden by the application
		// Will be called if client disconnects
		virtual void OnDisconnect() {};
		// Should be overridden by the application
		// Will be called for each arriving message
		virtual void OnMessage(Ref<Message> msg) {};

	private:
		// Thread safe queue for incoming message packets
		MessageQueue m_MessagesIn;

		// The single connection instance
		Ref<Connection> m_Connection;

		// Cache for remote host
		std::string m_host = "";
		// Cache for remote port
		uint16_t m_port = 0;

	private:
		// asio context handles the data transfer...
		asio::io_context m_asioContext;
		// ...but needs a thread of its own to execute its work commands
		std::thread threadContext;
	};


} // namespace Net
