#ifndef STORE_HPP
#define STORE_HPP
#pragma once

#include <string>
#include <unordered_map>

template <typename Key, typename Value>
class Store {
  public:
	inline void Clear() { _store.clear(); }

	inline void Set(const Key &key, const Value &value) {
		_store[key] = value;
	}

	inline Value Get(const Key &key) {
		if (!Has(key))
			return Value();

		return _store[key];
	}

	inline bool Has(const Key &key) {
		return _store.find(key) != _store.end();
	}

	inline void Remove(const Key &key) {
		if (!Has(key))
			return;

		_store.erase(key);
	}

	inline const std::unordered_map<Key, Value> &GetStore() {
		return _store;
	}

  private:
	std::unordered_map<Key, Value> _store;
};

using StringStore = Store<std::string, std::string>;

#endif // STORE_HPP