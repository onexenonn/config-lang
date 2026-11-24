#pragma once

#include "ast.h"

#include <iosfwd>
#include <map>
#include <string>
#include <vector>

class Driver {
public:
    void addConstant(const std::string& name, ValuePtr value);
    ValuePtr getConstant(const std::string& name);
    void setRoot(ValuePtr root);
    ValuePtr root() const;

    void error(const std::string& message);
    bool hasErrors() const;
    void printErrors(std::ostream& out) const;

private:
    std::map<std::string, ValuePtr> constants_;
    ValuePtr root_;
    std::vector<std::string> errors_;
};
