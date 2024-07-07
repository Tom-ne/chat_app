#include <iostream>
#include "Server.h"
#include "Logger.h"

int main() {
	Logger::initialize();
	Server server(8080, 0);
	Logger::finalize();

	return 0;
}