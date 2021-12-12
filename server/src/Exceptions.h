#pragma once

#include <exception>
#include <string>

class NotImplemented : public std::exception {
private:
    std::string m_error;

public:
    NotImplemented(std::string error = "Function not yet implemented")
            : m_error(error) {}

    const char *what() const noexcept override { return m_error.c_str(); }
};

class InvalidInputs : public std::exception {
private:
    std::string m_error;

public:
    InvalidInputs(std::string error = "Invalid inputs")
            : m_error(std::move(error)) {
    }

    const char *what() const noexcept override { return m_error.c_str(); }
};