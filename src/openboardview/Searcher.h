#include "BRDBoard.h"

enum class SearchMode {
	Sub,
	Prefix,
	Whole,
};

class Searcher {
	SearchMode m_searchMode = SearchMode::Sub;

	SharedVector<Net> m_nets;
	SharedVector<Component> m_parts;

	template<class T, class TFieldCollection> SharedVector<T> searchFor(const std::string& search, const SharedVector<T>& v, int limit, const TFieldCollection& fields_to_look);
	bool strstrModeSearch(const std::string &strhaystack, const std::string &strneedle);
public:
	//fields to use while searching Component. Logically its a map, but pointer-to-member can't be used as key.
	std::tuple<std::string Component::*, bool> part_fields_enabled[2] =
		{{&Component::name, true}, {&Component::mfgcode, false}};
	std::tuple<std::string Net::*, bool> net_fields_enabled[1] =
		{{&Net::name, true}};
	//returns writeable pointer to inner field, that can be used to configure which part fields are searched
	template <class Collection, class Field> static bool* ptrToFieldEnablement(Collection& enablements, Field field_to_configure)
	{
		for(auto& [field, enabled] : enablements)
		{
			if (field == field_to_configure)
			{
				return &enabled;
			}
		}
		return nullptr;
	}
	void setNets(SharedVector<Net> nets);
	void setParts(SharedVector<Component> components);

	bool isMode(SearchMode sm);
	void setMode(SearchMode sm);
	SharedVector<Component> parts(const std::string& search, int limit);
	SharedVector<Component> parts(const std::string& search);
	SharedVector<Net> nets(const std::string& search, int limit);
	SharedVector<Net> nets(const std::string& search);
};
