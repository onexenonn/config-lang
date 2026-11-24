#pragma once

#include "ast.h"

#include <ostream>
#include <string>

class TomlWriter {
public:
    explicit TomlWriter(std::ostream& out);

    void write(const ValuePtr& root);

private:
    void writeTable(const EntryList& entries,
                    const std::string& path,
                    bool root);
    void writeValue(const ValuePtr& value);
    std::string formatNumber(const std::string& raw) const;
    std::string escapeString(const std::string& value) const;

    std::ostream& out_;
};
