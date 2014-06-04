#ifndef Components_Health_h__
#define Components_Health_h__

#include "Component.h"

namespace Components
{

	struct Health : Component
	{
		Health()
			: Amount(1.0f) 
			, VulnerableToSplash(true)
		{ }

		float Amount;
		bool VulnerableToSplash;

		virtual Health* Clone() const override { return new Health(*this); }
	};

}

#endif // Components_Health_h__
