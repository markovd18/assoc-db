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

- parsing will be done with **regex**
- when parsing, **white spaces need to be ignored!!**
