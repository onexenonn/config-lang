#include "driver.h"
#include "toml_writer.h"

#include <iostream>

extern int yyparse();
extern Driver* g_driver;

int main() {
    Driver driver;
    g_driver = &driver;

    int result = yyparse();

    if (result != 0 || driver.hasErrors()) {
        driver.printErrors(std::cerr);
        return 1;
    }

    if (!driver.root()) {
        std::cerr << "Error: root table not found\n";
        return 1;
    }

    TomlWriter writer(std::cout);
    writer.write(driver.root());

    return 0;
}
