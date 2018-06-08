#ifndef __LuaCpp_hpp_2018_06_07_22_39
#define __LuaCpp_hpp_2018_06_07_22_39
#include <lua/lobject.h>
#include <string>
#include <vector>
#include <map>
#include <functional>

#ifdef __MSC_VER
#if _MSC_VER < 1800
#define __REF_QUALIFIER
#else
#define __REF_QUALIFIER &
#define __USE_RVALUE_QUALIFIER
#endif
#endif

using std::string;
using std::vector;
using std::map;
using std::function
class LuaValue{
  TValue* val;
public:
  LuaValue();
  LuaValue(nullptr_t);
  LuaValue(const vector<LuaValue>&);
  LuaValue(const map<LuaValue,LuaValue>&);
  LuaValue(const vector<LuaValue>&,const map<LuaValue,LuaValue>&);
  LuaValue(const string&);
  LuaValue(function<int(LuaState*)>);
  LuaValue(int);
  LuaValue(double);
  LuaValue(TValue*);
  LuaValue(const LuaValue&);
  LuaValue(LuaValue&&);
  ~LuaValue();
  LuaValue& operator=(const LuaValue&);
  LuaValue& operator=(LuaValue&&);
  LuaValue& operator[](int)__REF_QUALIFIER;
  const LuaValue& operator[](int)__REF_QUALIFIER const;
  LuaValue& operator[](const LuaValue&)__REF_QUALIFIER;
  LuaValue& operator[](LuaValue&&)__REF_QUALIFIER;
  const LuaValue& operator[](const LuaValue&)__REF_QUALIFIER const;
  const LuaValue& operator[](LuaValue&&)__REF_QUALIFIER const;
#ifdef __USE_RVALUE_QUALIFIER
  LuaValue&& operator[](const LuaValue&)&&;
  LuaValue&& operator[](LuaValue&&)&&;
  LuaValue&& operator[](int)&&;
  operator string&&()&&;
#endif
  operator string&()__REF_QUALFIER;
  operator const string&()__REF_QUALFIER const;
  operator int()const;
  operator double()const;
  int operator()(LuaState*)const;
};

#endif
