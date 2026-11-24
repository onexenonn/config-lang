# Config Language to TOML

## 1. Общее описание

Инструмент командной строки преобразует текст на учебном конфигурационном
языке в TOML. Входные данные читаются из стандартного ввода, результат
записывается в стандартный вывод. Синтаксические и семантические ошибки
выводятся в стандартный поток ошибок.

Для разбора используется Flex + Bison. Реализация выполнена на C++17.

Поддерживаются числа, строки `q(текст)`, словари `table([ ... ])`,
вложенные словари, константы `имя <- значение;` и подстановка `#{имя}`.

## 2. Описание функций и настроек

### Командная строка

```bash
./config < input.conf
```

Или через скрипт:

```bash
./run.sh < input.conf
```

Настроек командной строки нет: инструмент читает stdin и пишет в stdout.

### Коды возврата

- `0` — успешное преобразование;
- `1` — синтаксическая или семантическая ошибка.

### Модули

- `src/lexer.l` — лексический анализатор Flex;
- `src/parser.y` — грамматика Bison;
- `src/driver.h`, `src/driver.cpp` — таблица констант и ошибки;
- `src/ast.h` — представление значений;
- `src/toml_writer.h`, `src/toml_writer.cpp` — вывод TOML;
- `src/main.cpp` — точка входа.

### Синтаксис языка

Числа:

```text
-12
3.14
.5
5.
1e3
-2.5E-2
```

Строки:

```text
q(Это строка)
```

Словари:

```text
table([
  key = value,
  nested = table([
    inner = q(text)
  ])
])
```

Константы:

```text
MAX_CONN <- 100;
HOST <- q(example.com);
```

Подстановка:

```text
#{MAX_CONN}
#{HOST}
```

## 3. Сборка и запуск тестов

Зависимости: g++ с поддержкой C++17, flex, bison, make.

Сборка:

```bash
make
```

Запуск:

```bash
./run.sh < examples/web_server.conf
```

Тесты:

```bash
make test
```

Очистка:

```bash
make clean
```

## 4. Примеры использования

### Веб-сервер

```bash
./run.sh < examples/web_server.conf
```

Фрагмент результата:

```toml
[app]
name = "MyShop"
debug = "false"
[server]
host = "0.0.0.0"
port = 8080
```

### Игра

```bash
./run.sh < examples/game.conf
```

Фрагмент результата:

```toml
[window]
title = "Space Runner"
width = 1280
height = 720
```

### Университет

```bash
./run.sh < examples/university.conf
```

Фрагмент результата:

```toml
[university]
name = "РТУ МИРЭА"
city = "Москва"
```

## Git

Рекомендуемые сообщения коммитов:

```text
feat(lexer): add tokens for numbers, strings and constants
feat(parser): add table grammar and nested values
feat(semantic): add compile-time constants
feat(toml): add TOML writer for nested tables
test: cover all language constructs
docs: add README with build and usage instructions
chore: add Makefile, run script and gitignore
```
