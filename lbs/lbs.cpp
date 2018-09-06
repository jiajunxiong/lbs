/**
*  @file    lbs.cpp
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

#include "quickfix/FileStore.h"
#include "quickfix/SocketAcceptor.h"
#include "quickfix/SocketInitiator.h"
#ifdef HAVE_SSL
#include "quickfix/ThreadedSSLSocketAcceptor.h"
#include "quickfix/SSLSocketAcceptor.h"
#endif
#include "quickfix/Log.h"
#include "quickfix/SessionSettings.h"
#include "app.h"
#include <string>
#include <iostream>
#include <fstream>

void wait() {
    std::cout << "Type Ctrl-C to quit" << std::endl;
    while(true) {
        FIX::process_sleep(1);
    }
}

void initConnection() {
	std::string file = "settings.txt";
	FIX::Acceptor * acceptor = 0;
	FIX::Initiator * initiator = 0;
	try {
		FIX::SessionSettings settings(file);

		Application application;
		FIX::FileStoreFactory storeFactory(settings);
		FIX::ScreenLogFactory logFactory(settings);
		initiator = new FIX::SocketInitiator(application, storeFactory, settings, logFactory);
		acceptor = new FIX::SocketAcceptor(application, storeFactory, settings, logFactory);
		acceptor->start();
		initiator->start();
		wait();
		initiator->stop();
		acceptor->stop();

		delete initiator;
		delete acceptor;
	}
	catch (std::exception & e) {
		std::cout << e.what() << std::endl;
		delete initiator;
		delete acceptor;
	}
}

int main( int argc, char** argv ) {
	initConnection();
	return 0;
}