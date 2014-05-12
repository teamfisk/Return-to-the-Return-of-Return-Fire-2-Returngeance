#ifndef Components_Model_h__
#define Components_Model_h__

#include <string>

#include "Component.h"
#include "Color.h"

namespace Components
{

struct Model : Component
{
	Model() : Visible(true), ShadowCaster(true) { }
	std::string ModelFile;
	Color Color;
	bool Visible;
	bool ShadowCaster;

	virtual Model* Clone() const override { return new Model(*this); }
};

}
#endif // !Components_Model_h__