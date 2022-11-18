#pragma once

#include "Common/Misc.h"

#include "Net/NetCommon.h"
#include "Net/NetMessage.h"
#include "Net/NetMessageQueue.h"


namespace Net {


	class Connection : public std::enable_shared_from_this<Connection>
	{
	public:
		Connection(bool server, asio::io_context& asioContext, asio::ip::tcp::socket socket, MessageQueue& qIn);
		~Connection()
		{
			std::cout << "~Connection()" << std::endl;
		}
	public:
		void ConnectToServer(const asio::ip::tcp::resolver::results_type& endpoints);
		void ConnectToClient();

		void Disconnect();

		// Send a message
		void Send(Ref<Message> msg);

		uint32_t GetID() const { return m_connID; }
		void SetID(uint32_t id) { m_connID = id; }
		std::string GetIP() const { return m_host; }
		uint16_t GetPort() const { return m_port; }

		// Check if connection is open
		bool IsConnected() { return m_socket.is_open(); }

	private:
		// ASYNC - Send a message, connections are one-to-one so no need to specifiy
		// the target, for a client, the target is the server and vice versa
		void ASYNC_Send(Ref<Message> msg);
		// ASYNC - Prime context to write a message header
		void ASYNC_WriteHeader();
		// ASYNC - Prime context to write a message body
		void ASYNC_WriteBody();
		// ASYNC - Prime context ready to read a message header
		void ASYNC_ReadHeader();
		// ASYNC - Prime context ready to read a message body
		void ASYNC_ReadBody();

		// Once a full message is received, add it to the incoming queue
		void AddToIncomingMessageQueue();

	private:
		// This queue holds all messages to be sent to the remote side of this connection
		MessageQueue m_MessagesOut;

		// This references the incoming queue of the parent object
		MessageQueue& m_MessagesIn;

		// Incoming messages are constructed asynchronously, so we will
		// store the partly assembled message here, until it is ready
		Ref<Message> m_msgTemporaryIn;

		// Decides how some of the connection behaves
		bool m_IsServer;

		// Cache for remote host
		std::string m_host = {};
		// Cache for remote port
		uint16_t m_port = 0;

		// Unique ID of connection
		uint32_t m_connID = 0;

	private:
		// Each connection has a unique socket to a remote 
		asio::ip::tcp::socket m_socket;

		// This context is shared with the whole asio instance
		asio::io_context& m_asioContext;
	};


} // namespace Net
