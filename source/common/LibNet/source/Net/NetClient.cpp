#include "Net/NetClient.h"
#include "Net/NetConnection.h"


namespace Net {


	// ========================================================================
	Client::Client(const std::string host, const uint16_t port)
		: m_host(host), m_port(port)
	{
	}


	// ========================================================================
	Client::~Client()
	{
		// If the client is destroyed, always try to disconnect from server
		Disconnect();
	}


	// ========================================================================
	// Connect to server
	bool Client::Connect()
	{
		return Connect(m_host, m_port);
	}


	// ========================================================================
	// Connect to server with hostname/address and port
	bool Client::Connect(const std::string host, const uint16_t port)
	{
		// Cache host/port
		m_host = host;
		m_port = port;

		try
		{
			// Resolve hostname/ip-address into tangiable physical address
			asio::ip::tcp::resolver resolver(m_asioContext);
			asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(m_host, std::to_string(m_port));

			// Create connection
			m_Connection = CreateRef<Connection>(false, m_asioContext, asio::ip::tcp::socket(m_asioContext), m_MessagesIn);

			// Tell the connection object to connect to server
			m_Connection->ConnectToServer(endpoints);

			// Start context thread
			threadContext = std::thread([this]() { m_asioContext.run(); });
		}
		catch (std::exception& e)
		{
std::cerr << "Client Exception: " << e.what() << std::endl;
			return false;
		}
		return true;
	}


	// ========================================================================
	// Disconnect from server
	void Client::Disconnect()
	{
		// If connection exists, and it's connected then...
		if (IsConnected())
		{
			// ...disconnect from server gracefully
			m_Connection->Disconnect();
		}

		// Either way, we're also done with the asio context...				
		m_asioContext.stop();
		// ...and its thread
		if (threadContext.joinable())
			threadContext.join();

		// Destroy the connection object
		m_Connection.reset();
	}


	// ========================================================================
	// Send message to server
	void Client::Send(Ref<Message> msg)
	{
		if (IsConnected())
			m_Connection->Send(msg);
	}


	// ========================================================================
	// Process incoming messages
	void Client::Update(bool wait)
	{
		Update(-1, wait);
	}


	// ========================================================================
	// Process incoming messages
	void Client::Update(size_t maxMessages, bool wait)
	{
		if (!IsConnected())
			return;

		// Wait for messages
		if (wait)
			m_MessagesIn.Wait();

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
	}


	// ========================================================================
	// Check if client is actually connected to a server
	bool Client::IsConnected()
	{
		if (m_Connection)
			return m_Connection->IsConnected();
		else
			return false;
	}


} // namespace Net
