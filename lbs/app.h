/**
*  @file    app.h
*  @author  Jiajun Xiong (jjxiong.hk@gmail.com)
*  @date    8/21/2018
*  @version 1.0
*
*  @brief fix 4.2 trading system application using QuickFIX FIX Engine
*
*  @section DESCRIPTION
*
*/

#ifndef LBS_APP_H
#define LBS_APP_H

#include "quickfix/Application.h"
#include "quickfix/Values.h"
#include "quickfix/Utility.h"
#include "quickfix/Mutex.h"

#include "quickfix/fix42/NewOrderSingle.h"
#include "quickfix/fix42/ExecutionReport.h"
#include "quickfix/fix42/OrderCancelRequest.h"
#include "quickfix/fix42/OrderCancelReject.h"
#include "quickfix/fix42/OrderCancelReplaceRequest.h"

class Application
	: public FIX::Application {
public:
	Application() : m_orderID(0), m_execID(0) {}

	// Application overloads
	void onCreate(const FIX::SessionID&);
	void onLogon(const FIX::SessionID& sessionID);
	void onLogout(const FIX::SessionID& sessionID);
	void toAdmin(FIX::Message&, const FIX::SessionID&);
	void toApp(FIX::Message&, const FIX::SessionID&)
		throw(FIX::DoNotSend);
	void fromAdmin(const FIX::Message&, const FIX::SessionID&)
		throw(FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon);
	void fromApp(const FIX::Message& message, const FIX::SessionID& sessionID)
		throw(FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType);


	// Do not use this message to cancel the remaining quantity of
	// an outstanding order, use the Cancel Request message for this purpose.
	void onClientMessage(const FIX::Message&, const FIX::SessionID&);
	void newOrderSingle42(const FIX::Message& message);
	void orderCancelRequest42(const FIX::Message& message);
	void orderCancelReplaceRequest42(const FIX::Message& message);

	// The Order Cancel Reject <9> message is issued by the broker upon receipt of a cancel request
	// or cancel/replace request message which cannot be honored.
	void onExchangeMessage(const FIX::Message&, const FIX::SessionID&);
	void executionReport42(const FIX42::ExecutionReport&, const FIX::SessionID&);
	void orderCancelReject42(const FIX42::OrderCancelReject&, const FIX::SessionID&);


	std::string genOrderID() {
		std::stringstream stream;
		stream << ++m_orderID;
		return stream.str();
	}
	std::string genExecID() {
		std::stringstream stream;
		stream << ++m_execID;
		return stream.str();
	}
private:
	int m_orderID, m_execID;
};

#endif

