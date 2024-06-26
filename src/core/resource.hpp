#ifndef RESOURCE_HPP
#define RESOURCE_HPP
#pragma once

#include "sowa.hpp"
#include <string>

#include "serialize/document.hpp"

class Resource {
  public:
	Resource();
	Resource(RID rid);
	virtual ~Resource();

	inline RID GetRID() const { return _rid; }
	inline std::size_t ResourceType() { return _resourceType; }
	inline const std::string &Filepath() { return _filepath; }

	virtual void LoadResource(const Document &doc) {}
	virtual void SaveResource(Document &doc) {}

  private:
	friend class ResourceRegistry;
	RID _rid = 0;

  protected:
	std::size_t _resourceType = 0;
	std::string _filepath = "";
};

#endif // RESOURCE_HPP