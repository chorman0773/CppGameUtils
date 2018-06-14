/*
    C++ Wrapper Library for Interfacing with Lua.
    Alot of Interfaces would be used similarly to Lua.
*/
#ifndef __LuaCpp_hpp_2018_06_12_14_40
#define __LuaCpp_hpp_2018_06_12_14_40
#include <lua/lobject.h>
#include <string>
#include <vector>
#include <initializer_list>
#include <type_traits>

using std::declval;

using std::initializer_list;

using std::string;
using std::vector;

class LuaValue;

/*
    Wrapper for having multiple values.
    Varargs is an immutable type that stores multiple lua values.
    You can obtain a new instance of Varargs, you can construct one by Simply placing comma 
        separated LuaValues (or convertible values) or Varargs (or both).
    For example, the Expression Varargs args("hello",1,1.1,nil,)
*/
class Varargs{
private:
    const vector<LuaValue> varargs;
    size_t size;
    
    Varargs(const vector<LuaValue>&);
public:
    Varargs();
    Varargs(const LuaValue&);
    Varargs(LuaValue&&);
    Varargs(const Varargs&);
    Varargs(Varargs&&);
    template<typename... Args,
    typename= typename std::enable_if<std::conjunction<std::is_convertible<LuaValue,Args>...>::value>::type>
        Varargs(Args&&... args):varargs({LuaValue(std::forward(args)...}),size(sizeof...(Args)){}
    ~Varargs();
    Varargs operator,(const Varargs&)const;
    Varargs operator,(const LuaValue&)const;
    LuaValue&& toTable();
    LuaValue& operator[](size_t);
    const LuaValue& operator[](size_t)const;
    operator LuaValue&();
    operator const LuaValue&()const;
    operator bool()const;
    operator int()const;
    operator double()const;
    operator const string&()const;
};

/*
    Class which represents a LuaValue.
    this value can be a number, a boolean, a string,a table, or any other valid lua value.
    It can even be nil.
    LuaValue is type-erased, that is, all compile-time type information is lost by it.
    LuaValue will use Lua's type-system to fix this
*/
class LuaValue{
private:
    TValue* val;
    struct TableKeyIndex;
    struct TableArrayIndex{
    private:
        const size_t pos;
        TValue*& table;
        TableArrayIndex(const TableArrayIndex&)=delete;
        TableArrayIndex& operator=(const TableArrayIndex&)=delete;
        TableArrayIndex& operator=(TableArrayIndex&&)=delete;
    public:
        TableArrayIndex(size_t,TValue*&);
        
        TableArrayIndex& operator=(const LuaValue&)const;
        TableArrayIndex& operator=(LuaValue&&)const;
        TableArrayIndex& operator=(int);
        TableArrayIndex& operator=(double);
        TableArrayIndex& operator=(const string&);
        TableArrayIndex& operator=(bool);
        TableArrayIndex& operator=(nullptr_t);
        TableArrayIndex& operator=(TValue*);
        TableArrayIndex& operator=(initializer_list<Varargs>&&);
        TableArrayIndex& operator=(const initializer_list<Varargs>&);
        template<typename... Args,
        typename= typename std::enable_if<std::conjunction<std::is_convertible<LuaValue,Args>...>::value>::type>
             Varargs&& operator()(Args&&... args)const{
                return (*this)(Varargs((LuaValue(std::forward(args)), ...)));
        }
        Varargs&&  operator()(Varargs&& varargs)const;
        Varargs&& operator()(const Varargs& varargs)const;
        Varargs&& operator,(const LuaValue&)const;
        Varargs&& operator,(LuaValue&&)const;
        operator int()const;
        operator double()const;
        explicit operator bool()const;
        operator string()const;
        TableArrayIndex&& operator[](size_t);
        TableArrayIndex&& const operator[](size_t)const;
        TableKeyIndex&& operator[](const LuaValue&);
        TableKeyIndex&& const operator[](const LuaValue&)const;
    };
    struct TableKeyIndex{
    private:
        TValue* const target;
        TValue*& table;
        TableKeyIndex(const TableKeyIndex&)=delete;
        TableKeyIndex& operator=(const TableKeyIndex&)=delete;
        TableKeyIndex& operator=(TableKeyIndex&&)=delete;
    public:
        TableKeyIndex(size_t,TValue*&);
        
        TableKeyIndex& operator=(const LuaValue&)const;
        TableKeyIndex& operator=(LuaValue&&)const;
        TableKeyIndex& operator=(int);
        TableKeyIndex& operator=(double);
        TableKeyIndex& operator=(const string&);
        TableKeyIndex& operator=(bool);
        TableKeyIndex& operator=(nullptr_t);
        TableKeyIndex& operator=(TValue*);
        TableKeyIndex& operator=(initializer_list<Varargs>&&);
        TableKeyIndex& operator=(const initializer_list<Varargs>&);
        template<typename... Args,
        typename= typename std::enable_if<std::conjunction<std::is_convertible<LuaValue,Args>...>::value>::type>
             Varargs&& operator()(Args&&... args)const{
                return (*this)(Varargs((LuaValue(args), ...)));
             }
        Varargs&& operator()(Varargs&& varargs)const;
        Varargs&& operator()(const Varargs& varargs)const;
        Varargs&& operator,(const LuaValue&)const;
        Varargs&& operator,(LuaValue&&)const;
        operator int()const;
        operator double()const;
        explicit operator bool()const;
        operator string()const;
        TableArrayIndex operator[](size_t);
        TableArrayIndex const operator[](size_t)const;
        TableKeyIndex operator[](const LuaValue&);
        TableKeyIndex const operator[](const LuaValue&)const;
    };
public:
    LuaValue();
    LuaValue(nullptr_t);
    LuaValue(const string&);
    LuaValue(double);
    LuaValue(int);
    LuaValue(bool);
    LuaValue(TValue*);

    LuaValue(const initializer_list<LuaValue>&);
    LuaValue(initializer_list<LuaValue>&&);
    LuaValue(const initializer_list<Varargs>&);
    LuaValue(initializer_list<Varargs>&&);
    template<typename... Args,
        typename=typename std::enable_if<std::conjunction<std::is_convertible<Args,LuaValue>...>::value>::type>
             Varargs&& operator()(Args&&... args)const{
                return (*this)(Varargs((LuaValue(args), ...)));
             }
    Varargs&& operator()(Varargs&& varargs)const;
    Varargs&& operator()(const Varargs& varargs)const;
    Varargs&& operator,(const LuaValue&)const;
    Varargs&& operator,(LuaValue&&)const;
    operator int()const;
    operator double()const;
    explicit operator bool()const;
    operator string()const;
    TableArrayIndex operator[](size_t);
    const TableArrayIndex operator[](size_t)const;
    TableKeyIndex operator[](const LuaValue&);
    const TableKeyIndex operator[](const LuaValue&)const;
    template<typename... Args,typename=typename std::enable_if<std::conjunction<std::is_convertible<Args,string>...>::value>::type>
       static Varargs&& error(Args&&... args){
           return throw (string(std::forward<Args>(args)) + ...);
       }
    static Varargs&& unrecoverable();
};
const LuaValue nil;

#endif