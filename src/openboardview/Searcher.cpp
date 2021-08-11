#include "platform.h"
#include "Searcher.h"

void Searcher::setNets(SharedVector<Net> nets) {
	this->m_nets = nets;
}

void Searcher::setParts(SharedVector<Component> components) {
	this->m_parts = components;
}

bool Searcher::isMode(SearchMode sm) {
	return m_searchMode == sm;
}

void Searcher::setMode(SearchMode sm) {
	m_searchMode = sm;
}

bool Searcher::strstrModeSearch(const std::string &strhaystack, const std::string &strneedle) {
	size_t nl = strneedle.size();
	size_t hl = strhaystack.size();
	const char *needle = strneedle.c_str();
	const char *haystack = strhaystack.c_str();
	const char *sr;

	sr = strcasestr(haystack, needle);
	if (sr) {
		if ((m_searchMode == SearchMode::Sub) || ((m_searchMode == SearchMode::Prefix) && (sr == haystack)) ||
		    ((m_searchMode == SearchMode::Whole) && (sr == haystack) && (nl == hl))) {
			return true;
		}
	}

	return false;
}

template<class T, class TFieldCollection> SharedVector<T> Searcher::searchFor(const std::string& search, const SharedVector<T>& v, int limit, const TFieldCollection& fields_to_look) {
	SharedVector<T> results;
	std::vector<std::string T::*> fields;
	for(auto& [field, enabled] : fields_to_look)
	{
		if (enabled)
		{
			fields.push_back(field);
		}
	}

	if (search.empty() || fields.empty()) return results;

	for (auto &p : v) {
		bool matched = false;
		for (auto& field : fields)
		{
			matched |= strstrModeSearch((*p).*field, search);
		}
		if (matched) {
			results.push_back(p);
			limit--;
		}
		if (limit == 0) return results;
	}
	return results;
}

SharedVector<Component> Searcher::parts(const std::string& search, int limit) {
	return searchFor(search, m_parts, limit, part_fields_enabled);
}

SharedVector<Component> Searcher::parts(const std::string& search) {
	return parts(search, -1);
}

SharedVector<Net> Searcher::nets(const std::string& search, int limit) {
	return searchFor(search, m_nets, limit, net_fields_enabled);
}

SharedVector<Net> Searcher::nets(const std::string& search) {
	return nets(search, -1);
}
