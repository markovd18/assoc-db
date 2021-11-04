# Associative Storage

- as a value in the DB, there will be `std::variant<int, double, std::string>` used
- for average function there will be the `std::visit` pattern used
- predefined pseudo-API:

```c++
class CMemory_Database {
  public:
    enum class Db_Operation
    {
      Equals,
      Greater_Than,
      Less_Than
    };

    // probably variadic template here
    result_type Insert(key_type key, ... ...);

    // probably variadic template here
    result_type Delete(key_type key, ... ...);

    result_type Search_Key(key_type key, db_operation op);

    // functor_type will be functor/function/lambda, which fulfills the concept (returns bool, whether the result conforms or not)
    result_type Find_Value(functor_type func);
};
```

- parsing will be done from begin to first `(`
  - if `(` not found - error, has to be there
  - parsed substring has to be one of `INSERT`, `DELETE`, `KEY_EQUALS`, `AVERAGE`, `KEY_GREATER`, `KEY_LESS`, `FIND_VALUE`, `MAX`, `MIN`
    - stored i a `std::map` of strings to enumeration types?
    - if none found - error
- then parsing until `)` is found
  - if not found - error, has to be there
  - then parsing by `,`
- when parsing, **white spaces need to be ignored!!**
