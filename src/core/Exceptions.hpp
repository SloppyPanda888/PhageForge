#pragma once

#include <stdexcept>
#include <string>

namespace phageforge::core {

// Base exception for all PhageForge errors
class PhageForgeException : public std::runtime_error {
public:
    explicit PhageForgeException(const std::string& msg) : std::runtime_error(msg) {}
    explicit PhageForgeException(const char* msg) : std::runtime_error(msg) {}
};

// Specific exception types
class InvalidCodonException : public PhageForgeException {
public:
    explicit InvalidCodonException(const std::string& msg) : PhageForgeException(msg) {}
};

class ConfigurationException : public PhageForgeException {
public:
    explicit ConfigurationException(const std::string& msg) : PhageForgeException(msg) {}
};

class RuntimeException : public PhageForgeException {
public:
    explicit RuntimeException(const std::string& msg) : PhageForgeException(msg) {}
};

class InvalidArgumentException : public PhageForgeException {
public:
    explicit InvalidArgumentException(const std::string& msg) : PhageForgeException(msg) {}
};

class OutOfBoundsException : public PhageForgeException {
public:
    explicit OutOfBoundsException(const std::string& msg) : PhageForgeException(msg) {}
};

class SerializationException : public PhageForgeException {
public:
    explicit SerializationException(const std::string& msg) : PhageForgeException(msg) {}
};

} // namespace phageforge::core
