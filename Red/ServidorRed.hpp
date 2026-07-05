#pragma once

#include "RedBase.hpp"

class ServidorRed : public RedBase {
private:
	awaitable<void> loop(std::shared_ptr<ip::tcp::socket> socket);

public:
	void inicializar();
};