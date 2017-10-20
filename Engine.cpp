

// RooBarb
#include "XmlConfig.h"
#include "TaskEngine.h"
using namespace jdb;

// STL
#include <iostream>
#include <exception>


#include "MVA/MVA.h"
// #include "MVA/SquareCuts.h"

#define LOGURU_IMPLEMENTATION 1
#include "vendor/loguru.h"

int main( int argc, char* argv[] ) {
	loguru::add_file("everything.log", loguru::Truncate, loguru::Verbosity_MAX);


	cout << "args: ";
	for ( int i = 0; i < argc; i++ ){
		cout << "argv[" << i << "] = "<< quote( argv[i] ) << endl;
	}   
	Logger::setGlobalLogLevel( "none" );

	TaskFactory::registerTaskRunner<MVA>( "MVA" );
	// TaskFactory::registerTaskRunner<SquareCuts>( "SquareCuts" );


	TaskEngine engine( argc, argv );
	engine.setDefaultTask( "MVA" );
	return 0;
}
