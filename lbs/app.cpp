/**
*  @file    app.cpp
*  @author  Jiajun Xiong (jjxiong.hk@gmail.com)
*  @date    8/21/2018
*  @version 1.0
*
*  @brief fix 4.2 trading system application using QuickFIX FIX Engine
*
*  @section DESCRIPTION
*
*/

#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 4267 )
#else
#include "config.h"
#endif

#include "app.h"
#include "quickfix/Session.h"

#include <iostream>

void Application::onCreate( const FIX::SessionID& sessionID ) {}
void Application::onLogon( const FIX::SessionID& sessionID ) {}
void Application::onLogout( const FIX::SessionID& sessionID ) {}
void Application::toAdmin( FIX::Message& message,
                           const FIX::SessionID& sessionID ) {}
void Application::toApp( FIX::Message& message,
                         const FIX::SessionID& sessionID )
throw( FIX::DoNotSend ) {}

void Application::fromAdmin( const FIX::Message& message,
                             const FIX::SessionID& sessionID )
throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon ) {}

void Application::fromApp( const FIX::Message& message,
                           const FIX::SessionID& sessionID )
throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType ) {
    // const_cast const FIX::Message to non const FIX::Message
    if (const_cast<FIX::Message&>(message).isEmpty()) return;

    try {

        const std::string senderCompID = message.getHeader().getField( FIX::FIELD::SenderCompID );
        const std::string targetCompID = message.getHeader().getField( FIX::FIELD::TargetCompID );

        std::cout << "sessionID:" << sessionID;
        std::cout << " MsgType:" << message.getHeader().getField( FIX::FIELD::MsgType ) << std::endl;

        if (senderCompID == "ts") {
            onClientMessage(message, sessionID);
            std::cout << "Message:" << message << std::endl;
        } else {
            onExchangeMessage(message, sessionID);
            std::cout << "Message:" << message << std::endl;
        }
        std::cout << "Message handled!" << std::endl;
    } catch ( std::exception & e ) {
        std::cout << e.what() << std::endl;
    }
}

void Application::onClientMessage( const FIX::Message& message, const FIX::SessionID& sessionID ) {
    if ( FIX::MsgType_NewOrderSingle == message.getHeader().getField( FIX::FIELD::MsgType ) )
	{
        std::cout<< "New order single" <<std::endl;
		FIX::Symbol symbol = message.getField(FIX::FIELD::Symbol);  //str
		FIX::Side side = message.getField(FIX::FIELD::Side)[0];  //char
		FIX::OrdType ordType = message.getField(FIX::FIELD::OrdType)[0];  //char
		std::cout << "symbol: " << symbol << "side: " << side << "ordType: " << ordType << std::endl;
		FIX42::NewOrderSingle newOrderSingle(FIX::ClOrdID("1"), FIX::HandlInst('1'), symbol, side, FIX::TransactTime(), ordType);
		newOrderSingle.set(FIX::OrderQty(10));
		newOrderSingle.set(FIX::Price(10));
		newOrderSingle.getHeader().setField(FIX::SenderCompID("lbs"));
		newOrderSingle.getHeader().setField(FIX::TargetCompID("ex"));
		newOrderSingle42(newOrderSingle);
	}
	else if (FIX::MsgType_OrderCancelRequest == message.getHeader().getField(FIX::FIELD::MsgType))
	{
		std::cout << "Order cancel request" << std::endl;
		orderCancelRequest42(message);
	}
	else if (FIX::MsgType_OrderCancelReplaceRequest == message.getHeader().getField(FIX::FIELD::MsgType))
	{
		std::cout << "Order cancel replace request" << std::endl;
		orderCancelReplaceRequest42(message);
	}
    else
        std::cout<< "Unknow message" <<std::endl;
}

void Application::newOrderSingle42(const FIX::Message& message)
{
	// sendToTarget(exchange) NewOrderSingle.
	if (const_cast<FIX::Message&>(message).isEmpty()) return;
	FIX::Session::sendToTarget(const_cast<FIX::Message&>(message));
}

void Application::orderCancelRequest42(const FIX::Message& message)
{
	// sendToTarget(exchange) OrderCancelRequest.
	if (const_cast<FIX::Message&>(message).isEmpty()) return;
	//FIX::Session::sendToTarget(const_cast<FIX::Message&>(message));
}

void  Application::orderCancelReplaceRequest42(const FIX::Message& message)
{
	// sendToTarget(exchange) OrderCancelReplaceRequest.
	if (const_cast<FIX::Message&>(message).isEmpty()) return;
	//FIX::Session::sendToTarget(const_cast<FIX::Message&>(message));
}

void Application::onExchangeMessage( const FIX::Message& message, const FIX::SessionID& sessionID ) {
    if ( FIX::MsgType_ExecutionReport == message.getHeader().getField( FIX::FIELD::MsgType ) )
        std::cout<< "ExecutionReport" <<std::endl;
    else if ( FIX::MsgType_OrderCancelReject == message.getHeader().getField( FIX::FIELD::MsgType ) )
        std::cout<< "Order cancel reject" <<std::endl;
    else
        std::cout<< "Unknow message" <<std::endl;
}

void Application::executionReport42(const FIX42::ExecutionReport&, const FIX::SessionID&) {
    // TODO sendToTarget(client) ExecutionReport.
}

void Application::orderCancelReject42(const FIX42::OrderCancelReject &, const FIX::SessionID &)
{
	// TODO sendToTarget(clinet) OrderCancelReject
}