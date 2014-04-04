#ifndef ComponentFactory_h__
#define ComponentFactory_h__

#include <string>
#include <memory>
#include <functional>
#include <map>

template <typename T>
class Factory
{
public:
	void Register(std::string name, std::function<T(void)> factoryFunction)
	{
		m_FactoryFunctions[name] = factoryFunction;
	}

	T Create(std::string name)
	{
		auto it = m_FactoryFunctions.find(name);
		if (it != m_FactoryFunctions.end()) {
			return it->second();
		} else {
			return nullptr;
		}
	}

private:
	std::map<std::string, std::function<T(void)>> m_FactoryFunctions;
};

#endif // ComponentFactory_h__