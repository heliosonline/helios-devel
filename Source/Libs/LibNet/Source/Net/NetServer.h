#pragma once

#include "Common/Misc.h"

#include "Net/NetCommon.h"
#include "Net/NetMessage.h"
#include "Net/NetMessageQueue.h"
#include "Net/NetConnection.h"


namespace Net {


	class Server
	{
	public:
		Server();
		Server(uint16_t port);
		Server(uint16_t port, const std::string& host = {});
		~Server();

	public:
		// Starts the server
		bool StartListen();
		// Starts the server, listening on the specified port and optional address
		bool StartListen(uint16_t port, const std::string& host = {});
		// Stops the server
		void StopListening();

		// Disconnects the spezified client
		void Disconnect(Ref<Connection> client);

		// Send a message to a single client
		void Send(Ref<Connection> client, Ref<Message> msg);
		// Send message to all connected clients
		void Broadcast(Ref<Message> msg, Ref<Connection> ignore = nullptr);

		// Process incoming messages
		void Update(bool wait);
		// Process incoming messages
		void Update(size_t maxMessages = -1, bool wait = false);
		// Search for and remove dead clients
		void UpdateDeadClients();

		// Check if the server ist started and listening
		bool IsListening() { return m_IsListening; }

		// Sets a descriptive name for debugging/logging output
		void SetName(const std::string& name) { m_name = name; }

	protected:
		// Should be overridden by the application
		// Will be called if a client tries to connect
		virtual bool OnConnect(Ref<Connection> client) { return true; }
		// Should be overridden by the application
		// Will be called if a client disconnects
		virtual void OnDisconnect(Ref<Connection> client) {}
		// Should be overridden by the application
		// Will be called for each arriving message
		virtual void OnMessage(Ref<Message> msg) {}

	private:
		// ASYNC - Instruct asio to wait for incomming connection
		void ASYNC_WaitForConnection();

	private:
		// Thread safe queue for incoming messages
		MessageQueue m_MessagesIn;

		// Status of asio acceptor
		bool m_IsListening = false;

		// Container of active connections
		std::deque<Ref<Connection>> m_Connections;

		// Cached listening host
		std::string m_host = {};
		// Cached listening port
		uint16_t m_port = 0;

		// Client ID counter
		uint32_t m_IDCounter = 0;

		// Name of the server for debugging/logging output
		std::string m_name = "SERVER";

	private:
		// asio context handles the data transfer...
		asio::io_context m_asioContext;
		// ...but needs a thread of its own to execute its work commands
		std::thread m_threadContext;

		// Handles new incoming connection attempts
		asio::ip::tcp::acceptor m_asioAcceptor;
	};


} // namespace Net
