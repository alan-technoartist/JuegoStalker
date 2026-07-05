#pragma once

#include "RedBase.hpp"

class ClienteRed : public RedBase {
private:
	awaitable<void> loop(std::shared_ptr<ip::tcp::socket> socket);

public:
	void inicializar();
};