#pragma once

#include "RedBase.hpp"

class ServidorRed : public RedBase {
private:
	awaitable<void> loop(std::shared_ptr<ip::tcp::socket> socket) override;

public:
	ServidorRed(std::shared_ptr<UI> ui);

	void inicializar(size_t tamanoMensaje) override;
};