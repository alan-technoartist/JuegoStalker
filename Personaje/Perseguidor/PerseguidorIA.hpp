#include "../Personaje.hpp"

class PerseguidorIA : public Personaje {
public:
	PerseguidorIA();
	~PerseguidorIA();

	void mover() override;
};