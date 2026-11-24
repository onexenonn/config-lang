#include "driver.h"

#include <ostream>

void Driver::addConstant(const std::string& name, ValuePtr value) {
    if (constants_.count(name) != 0) {
        error("Constant '" + name + "' is already defined");
        return;
    }
    constants_[name] = value;
}

ValuePtr Driver::getConstant(const std::string& name) {
    auto it = constants_.find(name);
    if (it == constants_.end()) {
        error("Unknown constant '" + name + "'");
        return Value::makeString("");
    }
    return it->second;
}

void Driver::setRoot(ValuePtr root) {
    root_ = root;
}

ValuePtr Driver::root() const {
    return root_;
}

void Driver::error(const std::string& message) {
    errors_.push_back(message);
}

bool Driver::hasErrors() const {
    return !errors_.empty();
}

void Driver::printErrors(std::ostream& out) const {
    for (const auto& message : errors_) {
        out << "Error: " << message << "\n";
    }
}
Driver g_driver;
