#include "toml_writer.h"

#include <cstdio>
#include <iomanip>
#include <sstream>
#include <stdexcept>

TomlWriter::TomlWriter(std::ostream& out) : out_(out) {}

void TomlWriter::write(const ValuePtr& root) {
    if (!root || root->kind != Value::Kind::Table) {
        out_ << "# error: root value is not a table\n";
        return;
    }
    writeTable(root->entries, "", true);
}

void TomlWriter::writeTable(const EntryList& entries,
                            const std::string& path,
                            bool root) {
    for (const auto& entry : entries) {
        if (!entry.second || entry.second->kind == Value::Kind::Table) {
            continue;
        }
        out_ << entry.first << " = ";
        writeValue(entry.second);
        out_ << "\n";
    }

    for (const auto& entry : entries) {
        if (!entry.second || entry.second->kind != Value::Kind::Table) {
            continue;
        }

        std::string childPath = path.empty()
            ? entry.first
            : path + "." + entry.first;

        out_ << "[" << childPath << "]\n";
        writeTable(entry.second->entries, childPath, false);
    }

    (void)root;
}

void TomlWriter::writeValue(const ValuePtr& value) {
    if (!value) {
        out_ << "\"\"";
        return;
    }

    if (value->kind == Value::Kind::Number) {
        out_ << formatNumber(value->number);
        return;
    }

    if (value->kind == Value::Kind::String) {
        out_ << "\"" << escapeString(value->text) << "\"";
        return;
    }

    out_ << "\"\"";
}

std::string TomlWriter::formatNumber(const std::string& raw) const {
    bool isFloat = raw.find('.') != std::string::npos
        || raw.find('e') != std::string::npos
        || raw.find('E') != std::string::npos;

    try {
        if (!isFloat) {
            long long value = std::stoll(raw);
            return std::to_string(value);
        }

        long double value = std::stold(raw);
        std::ostringstream stream;
        stream << std::setprecision(15)
               << std::defaultfloat
               << static_cast<double>(value);

        std::string result = stream.str();
        if (result.find('.') == std::string::npos
            && result.find('e') == std::string::npos
            && result.find('E') == std::string::npos) {
            result += ".0";
        }
        return result;
    } catch (const std::exception&) {
        return "0";
    }
}

std::string TomlWriter::escapeString(const std::string& value) const {
    std::string result;
    result.reserve(value.size());

    for (char symbol : value) {
        switch (symbol) {
            case '\\':
                result += "\\\\";
                break;
            case '"':
                result += "\\\"";
                break;
            case '\n':
                result += "\\n";
                break;
            case '\r':
                result += "\\r";
                break;
            case '\t':
                result += "\\t";
                break;
            case '\b':
                result += "\\b";
                break;
            case '\f':
                result += "\\f";
                break;
            default:
                if (static_cast<unsigned char>(symbol) < 0x20) {
                    char buffer[8];
                    std::snprintf(buffer, sizeof(buffer), "\\u%04X",
                                  static_cast<unsigned char>(symbol));
                    result += buffer;
                } else {
                    result += symbol;
                }
        }
    }

    return result;
}
