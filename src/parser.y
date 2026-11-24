%{
#include "driver.h"
#include "ast.h"

#include <string>

extern Driver* g_driver;
int yylex();
void yyerror(const char* message);
%}

%code requires {
#include "ast.h"
#include <string>
}

%union {
    std::string* text;
    Value* value;
    EntryList* entries;
    Entry* entry;
}

%token <text> NUMBER STRING IDENT CONST_REF
%token TABLE CONST_DECL

%type <value> value table root_table
%type <entries> entries entry_list
%type <entry> entry

%start program

%%

program:
    statements root_table opt_semi
    {
        g_driver->setRoot(ValuePtr($2));
    }
;

statements:
    %empty
  | statements const_decl
;

const_decl:
    IDENT CONST_DECL value ';'
    {
        g_driver->addConstant(*$1, ValuePtr($3));
        delete $1;
    }
;

root_table:
    TABLE '(' '[' entries ']' ')'
    {
        $$ = new Value(*Value::makeTable(*$4));
        delete $4;
    }
;

value:
    NUMBER
    {
        $$ = new Value(*Value::makeNumber(*$1));
        delete $1;
    }
  | STRING
    {
        $$ = new Value(*Value::makeString(*$1));
        delete $1;
    }
  | table
    {
        $$ = $1;
    }
  | CONST_REF
    {
        $$ = new Value(*g_driver->getConstant(*$1));
        delete $1;
    }
;

table:
    TABLE '(' '[' entries ']' ')'
    {
        $$ = new Value(*Value::makeTable(*$4));
        delete $4;
    }
;

entries:
    %empty
    {
        $$ = new EntryList();
    }
  | entry_list
    {
        $$ = $1;
    }
;

entry_list:
    entry
    {
        $$ = new EntryList();
        $$->push_back(*$1);
        delete $1;
    }
  | entry_list ',' entry
    {
        $1->push_back(*$3);
        delete $3;
        $$ = $1;
    }
  | entry_list ','
    {
        $$ = $1;
    }
;

entry:
    IDENT '=' value
    {
        $$ = new Entry(*$1, ValuePtr($3));
        delete $1;
    }
;

opt_semi:
    %empty
  | ';'
;

%%

void yyerror(const char* message) {
    g_driver->error(message);
}
