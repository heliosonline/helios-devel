#include "Net/NetServer.h"


namespace Net {


	// ========================================================================
	Server::Server()
		: m_asioAcceptor(m_asioContext)
	{
	}


	// ========================================================================
	Server::Server(uint16_t port)
		: m_asioAcceptor(m_asioContext)
	{
		m_port = port;
	}


	// ========================================================================
	Server::Server(uint16_t port, const std::string& host)
		: m_asioAcceptor(m_asioContext)
	{
		m_host = host;
		m_port = port;
	}


	// ========================================================================
	Server::~Server()
	{
		if (IsListening())
			StopListening();
	}


	// ========================================================================
	// Starts the server
	bool Server::StartListen()
	{
		return StartListen(m_port, m_host);
	}


	// ========================================================================
	// Starts the server, listening on the specified port and optional address
	bool Server::StartListen(uint16_t port, const std::string& host)
	{
		// Cache host/port
		m_host = host;
		m_port = port;

		std::string addr;
		try
		{
			// Setup asio acceptor
			asio::ip::tcp::endpoint ep(m_host.empty() ? asio::ip::address_v6::any() : asio::ip::address::from_string(m_host), m_port);
			addr = ep.address().to_string();
			m_asioAcceptor.open(ep.protocol());
			m_asioAcceptor.bind(ep);
			m_asioAcceptor.listen();

			// Issue a task to the asio context - This is important as it will
			// prime the context with "work", and stop it from exiting immediately.
			// Since this is a server, we want it primed ready to handle clients
			// trying to connect.
			ASYNC_WaitForConnection();

			// Launch the asio context in its own thread
			m_threadContext = std::thread([this]() { m_asioContext.run(); });
		}
		catch (std::exception& e)
		{
			// Error
std::cerr << "[" << m_name << "] Exception: " << e.what() << std::endl;
			return false;
		}
		m_IsListening = true;
		// Log
std::cout << "[" << m_name << "] Started, listening on IP(" << addr << ") Port(" << port << ")" << std::endl;
		return true;
	}


	// ========================================================================
	// Stops the server
	void Server::StopListening()
	{
		if (!IsListening())
			return;

		// Request the context to close...
		m_asioContext.stop();
		// ...adnd wait for its thread to exit
		if (m_threadContext.joinable())
			m_threadContext.join();
		m_IsListening = false;

		// Log
std::cout << "[" << m_name << "] Stopped!" << std::endl;
	}


	// ========================================================================
	// Disconnects the spezified client
	void Server::Disconnect(Ref<Connection> client)
	{
		// Inform app and reset pointer
		OnDisconnect(client);
		client.reset();
		// Remove the client
		m_Connections.erase(
			std::remove(m_Connections.begin(), m_Connections.end(), nullptr),
			m_Connections.end());
	}


	// ========================================================================
	// Send a message to a single client
	void Server::Send(Ref<Connection> client, Ref<Message> msg)
	{
		// Check client is legitimate...
		if (client && client->IsConnected())
		{
			// ...and post the message via the connection
			client->Send(msg);
		}
		else
		{
			// If we cant communicate with client then we may as 
			// well remove the client - let the server know, it may
			// be tracking it somehow
			OnDisconnect(client);

			// Off you go now, bye bye!
			client.reset();

			// Then physically remove it from the container
			m_Connections.erase(
				std::remove(m_Connections.begin(), m_Connections.end(), nullptr),
				m_Connections.end());
		}
	}


	// ========================================================================
	// Send message to all connected clients
	void Server::Broadcast(Ref<Message> msg, Ref<Connection> ignore)
	{
		bool bInvalidClientExists = false;

		// Iterate through all clients in container
		for (auto& client : m_Connections)
		{
			// Check client is connected...
			if (client && client->IsConnected())
			{
				// ..it is!
				if (client != ignore)
					client->Send(msg);
			}
			else
			{
				// The client couldnt be contacted, so assume it has
				// disconnected.
				OnDisconnect(client);
				client.reset();

				// Set this flag to then remove dead clients from container
				bInvalidClientExists = true;
			}
		}

		// Remove dead clients, all in one go - this way, we dont invalidate the
		// container as we iterated through it.
		if (bInvalidClientExists)
			m_Connections.erase(
				std::remove(m_Connections.begin(), m_Connections.end(), nullptr),
				m_Connections.end());
	}


	// ========================================================================
	// Process incoming messages
	void Server::Update(bool wait)
	{
		Update(-1, wait);
	}


	// ========================================================================
	// Process incoming messages
	void Server::Update(size_t maxMessages, bool wait)
	{
		if (wait) m_MessagesIn.Wait();

		// Process as many messages as we can up to the value specified
		size_t nMessageCount = 0;
		while (nMessageCount < maxMessages && !m_MessagesIn.IsEmpty())
		{
			// Grab the front message
			auto msg = m_MessagesIn.PopFront();

			// Pass to message handler
			OnMessage(msg);

			nMessageCount++;
		}

		// Search for dead clients...
		UpdateDeadClients();
	}


	// ========================================================================
	// Search for and remove dead clients
	void Server::UpdateDeadClients()
	{
		// Search for dead clients...
		bool bInvalidClientExists = false;
		for (auto& client : m_Connections)
		{
			// Check client is disconnected
			if (!client || !client->IsConnected())
			{
				// Inform app and reset pointer
				OnDisconnect(client);
				client.reset();
				// Set this flag to then remove dead clients from container
				bInvalidClientExists = true;
			}
		}
		// Remove dead clients, all in one go
		// This way, we dont invalidate the container as we iterated through it.
		if (bInvalidClientExists)
			m_Connections.erase(
				std::remove(m_Connections.begin(), m_Connections.end(), nullptr),
				m_Connections.end());
	}


	// ========================================================================
	// ASYNC - Instruct asio to wait for incomming connection
	void Server::ASYNC_WaitForConnection()
	{
		// Prime context with an instruction to wait until a socket connects. This
		// is the purpose of an "acceptor" object. It will provide a unique socket
		// for each incoming connection attempt
		m_asioAcceptor.async_accept(
			[this](std::error_code ec, asio::ip::tcp::socket socket)
			{
				// Triggered by incoming connection request
				if (!ec)
				{
					// Display some useful(?) information
std::cout << "[" << m_name << "] New Connection: " << socket.remote_endpoint() << std::endl;

					// Create a new connection to handle this client 
					Ref<Connection> newconn =
						CreateRef<Connection>(true, m_asioContext, std::move(socket), m_MessagesIn);
					newconn->SetID(++m_IDCounter);

					// Give the user server a chance to deny connection
					if (OnConnect(newconn))
					{
						// Connection allowed, so add to container of new connections
						m_Connections.push_back(newconn);

						// And very important! Issue a task to the connection's
						// asio context to sit and wait for bytes to arrive!
						m_Connections.back()->ConnectToClient();

std::cout << "[" << m_Connections.back()->GetID() << "] Connection Approved" << std::endl;
					}
					else
					{
std::cout << "[" << m_Connections.back()->GetID() << "] Connection Denied" << std::endl;
						// Connection will go out of scope with no pending tasks, so will
						// get destroyed automagically due to the wonder of smart pointers
					}
				}
				else
				{
					// Error has occurred during acceptance
std::cout << "[" << m_name << "] New Connection Error: " << ec.message() << std::endl;
				}

				// Prime the asio context with more work
				// again simply wait for another connection...
				ASYNC_WaitForConnection();
			});
	}


} // namespace Net
