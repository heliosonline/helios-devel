#include "pch.h"

#include "config/version.h"
#include "Net/NetServer.h"


enum class LoginMessages : Net::MessageTypeBase
{
	None = 0,      // uninitialized

	ServerAccept,  // server -> client
	ServerDeny,    // server -> client
	ServerPing,    // bidirectional
	MessageAll,    // client -> server
	ServerMessage, // server -> client
};


class LoginServer : public Net::Server
{
public:

	bool OnConnect(Ref<Net::Connection> client) override
	{
		std::cout << "[" << client->GetID() << "] App::OnConnect()" << std::endl;

		// std::string id   = client->GetID()       // unique connection id
		// std::string ip   = client->GetAddress()  // remote adress
		// uint16_t    port = client->GetPort()     // remote port

		Ref<Net::Message> msg = Net::NewMessage(LoginMessages::ServerAccept);
		Send(client, msg);

		return true;
	}


	void OnDisconnect(Ref<Net::Connection> client) override
	{
		std::cout << "[" << client->GetID() << "] App::OnDisconnect()" << std::endl;
	}


	void OnServerPing(Ref<Net::Message> msg)
	{
		std::cout << "[" << msg->remote->GetID() << "] App::OnServerPing()" << std::endl;
		// Simply bounce message back
		Send(msg->remote, msg);
	}


	void OnMessageAll(Ref<Net::Message> msg)
	{
		std::cout << "[" << msg->remote->GetID() << "] App::OnMessageAll()" << std::endl;
		Ref<Net::Message> msgOut = Net::NewMessage(LoginMessages::ServerMessage);
		msgOut << msg->remote->GetID();
		Broadcast(msgOut, msg->remote);
	}


	void OnMessage(Ref<Net::Message> msg) override
	{
		switch (static_cast<LoginMessages>(msg->header.type))
		{
		case LoginMessages::ServerPing: OnServerPing(msg); return;
		case LoginMessages::MessageAll: OnMessageAll(msg); return;
		}
		std::cout << "[" << msg->remote->GetID() << "] App::OnMessage() type: " << msg->header.type << " - Unknown!!!" << std::endl;
	}

};


int main()
{
#if 0
	int dummy = 0;
	Net::MessageQueue q;
	{
		Ref<Net::Message> msg = CreateRef<Net::Message>();
		msg->SetType(LoginMessages::LoginAccepted);
		q.PushBack(msg);
	}
	{
		Ref<Net::Message> msg;
		msg = q.GetBack();
		switch (static_cast<LoginMessages>(msg->header.type))
		{
		case LoginMessages::LoginRequest:
			dummy++;
			break;
		case LoginMessages::LoginAccepted:
			dummy++;
			break;
		default:
			dummy++;
			break;
		}
	}
	{
		Ref<Net::Message> msg;
		msg = q.PopBack();
	}
#endif


#if 0
	std::error_code ec;
	ec = Net::make_error_code(Net::ErrorCode::MsgErrorHeaderCRC);
	std::cout << ec << std::endl;
	std::cout << (ec == Net::ErrorCode::MsgErrorHeaderCRC ? "yes" : "no") << std::endl;
	std::cout << (ec == Net::ErrorCode::MsgErrorBodyCRC ? "yes" : "no") << std::endl;


	ec = Net::GetLastError();
	Net::SetLastError(Net::ErrorCode::MsgErrorHeaderCRC);
	Net::SetLastError(Net::ErrorCode::MsgErrorHeaderCRC);
	Net::SetLastError(Net::ErrorCode::MsgErrorHeaderCRC);
	ec = Net::GetLastError();
	Net::ClearLastError();
	ec = Net::GetLastError();
	ec = Net::GetLastError();
	ec = Net::GetLastError();
	ec = Net::GetLastError();
#endif


	LoginServer myServer;
	myServer.SetName("LoginServer");

	if (!myServer.StartListen(60000, "127.0.0.1"))
		return -1;

	bool bRun = true;
	while (bRun)
	{
		myServer.Update(true);
	}




	std::cout << "Helios::LoginServer [Version " << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_PATCH << "." << VERSION_BUILD << "]" << std::endl;
	std::cout << "Copyright (C) 2022, Lennart Molnar <pernicius@web.de>" << std::endl;
	std::cout << "See LICENSE.md for further details." << std::endl;
	std::cout << std::endl;
}
