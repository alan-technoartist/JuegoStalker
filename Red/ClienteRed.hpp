#pragma once

#include "RedBase.hpp"

class ClienteRed : public RedBase {
private:
	awaitable<void> loop(std::shared_ptr<ip::tcp::socket> socket) override;

public:
	ClienteRed(std::shared_ptr<UI> ui);

	void inicializar(size_t tamanoMensaje) override;
};