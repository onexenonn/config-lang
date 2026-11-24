#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

struct Value;

using ValuePtr = std::shared_ptr<Value>;
using Entry = std::pair<std::string, ValuePtr>;
using EntryList = std::vector<Entry>;

struct Value {
    enum class Kind {
        Number,
        String,
        Table
    };

    Kind kind = Kind::String;
    std::string number;
    std::string text;
    EntryList entries;

    static ValuePtr makeNumber(const std::string& value) {
        auto result = std::make_shared<Value>();
        result->kind = Kind::Number;
        result->number = value;
        return result;
    }

    static ValuePtr makeString(const std::string& value) {
        auto result = std::make_shared<Value>();
        result->kind = Kind::String;
        result->text = value;
        return result;
    }

    static ValuePtr makeTable(const EntryList& value) {
        auto result = std::make_shared<Value>();
        result->kind = Kind::Table;
        result->entries = value;
        return result;
    }
};
