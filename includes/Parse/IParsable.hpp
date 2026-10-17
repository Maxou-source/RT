#pragma once

#include "GpuObjects.hpp"

struct IParsable
{
	virtual ~IParsable() = default;
	virtual bool setProperty(const std::string& key, const std::string& val) = 0;
	virtual bool preBuild() = 0;
	virtual std::vector<std::string> requiredKeys() const = 0;
	virtual const char * typeString() const = 0;
	virtual bool packIntoGpuScene(GpuScene&) = 0;
};