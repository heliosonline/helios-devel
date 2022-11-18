#pragma once

#include <string>
#include <unordered_map>

// TODO: Tempoary .. remove assert.h
#include <cassert>
// TODO: Tempoary .. remove iostream
#include <iostream>


#if 0
template<class CLASS>
void luaRegisterClass(lua_State* L, const char* cname, const char* ctor)
{
	// create class table
	lua_newtable(L);
	lua_pushvalue(L, lua_gettop(L));
	lua_setglobal(L, cname);

	// create constructor function
	std::cout << typeid(CLASS).name() << std::endl;
	lua_pushcfunction(L, [](lua_State* L) -> int
	{
		// let lua allocate memory for the object
		void* ptr = lua_newuserdata(L, sizeof(CLASS));
		assert(ptr != nullptr);

		// create class object in memory and let c++ call the constructor (if any)
		new (ptr) CLASS();

		// setup object's metatable
		std::string tname("MetaTable_");
		tname += typeid(CLASS).name();// cname;
		luaL_getmetatable(L, tname.c_str());
		assert(lua_istable(L, -1));
		lua_setmetatable(L, -2);

		// setup table for user values (non native properties)
		lua_newtable(L);
		lua_setuservalue(L, 1);

		// num of return values on the lua-stack
		return 1;
	});
	lua_setfield(L, -2, ctor);
}


template<class CLASS>
void luaRegisterMethod(lua_State* L, const char* mname, lua_CFunction pfn)
{
	/* setup wrapper function */
	lua_pushcclosure(L, [](lua_State* L) -> int
		{
			/* Lua stack: -1 userdata */
			assert(lua_isuserdata(L, -1));
			/* get the object */
			CLASS* obj = (CLASS*)lua_touserdata(L, -1);
			/* call the method */
			return obj->pfn(L);
		}, 0);
	/* add the method */
	lua_setfield(L, -2, mname);
}
#endif


/*! \brief Enables the class to be handled by luaMacros
* @param <class C> Name of the class in C++
*/
#define luaMacros_ClassEnable(C)                                                  \
	static std::unordered_map<std::string, lua_CFunction> _luaMacros_PropGetters; \
	static std::unordered_map<std::string, lua_CFunction> _luaMacros_PropSetters;


/*! \brief Implementation of the enabling code
* @param <class C> Name of the class in C++
*/
#define luaMacros_ClassImpl(C)                                                 \
	std::unordered_map<std::string, lua_CFunction> C::_luaMacros_PropGetters;  \
	std::unordered_map<std::string, lua_CFunction> C::_luaMacros_PropSetters;


/*! \brief Bind a C++ class to a new Lua object
* @param <lua_State* L> The Lua state
* @param <class C> Name of the class in C++
* @param <char* N> Name of the class in lua
* @param <char* CTOR> Name of the constructor in lua
*/
#define luaBindClassBegin(L, C, N, CTOR)                                       \
	/* setup object in lua with given name */                                  \
	lua_newtable(L);                                                           \
	lua_pushvalue(L, lua_gettop(L));                                           \
	lua_setglobal(L, N);                                                       \
	/* setup a constructor function */                                         \
	lua_pushcclosure(L, [](lua_State* L) -> int                                \
		{                                                                      \
			/* let lua allocate memory for the object */                       \
			void* ptr = lua_newuserdata(L, sizeof(C));                         \
			/* let c++ call the constructor (if any) */                        \
			new (ptr) C();                                                     \
			/* setup objects metatable */                                      \
			luaL_getmetatable(L, "MetaTable_"#C);                              \
			assert(lua_istable(L, -1));                                        \
			lua_setmetatable(L, -2);                                           \
			/* setup table for user values(non native properties) */           \
			lua_newtable(L);                                                   \
			lua_setuservalue(L, 1);                                            \
			/* num of return values on the lua - stack */                      \
			return 1;                                                          \
		}, 0);                                                                 \
	/* add the constructor */                                                  \
	lua_setfield(L, -2, CTOR);


/*! \brief Bind a C++ class's method to a Lua object
* @param <lua_State* L> The Lua state
* @param <class C> Name of the class in C++
* @param <char* N> Name of the method in lua
* @param <class::func F> Name of the method in C++
*/
#define luaBindMethod(L, C, N, F)                                              \
	luaBindMethodUV(L, C, N, F, 0)


/*! \brief Bind a C++ class's method to a Lua object
* @param <lua_State* L> The Lua state
* @param <class C> Name of the class in C++
* @param <char* N> Name of the method in lua
* @param <class::func F> Name of the method in C++
* @param <int U> Number of upvalues
*/
#define luaBindMethodUV(L, C, N, F, U)                                         \
	/* setup wrapper function */                                               \
	lua_pushcclosure(L, [](lua_State* L) -> int                                \
		{                                                                      \
			/* Lua stack: -1 userdata */                                       \
			assert(lua_isuserdata(L, -1));                                     \
			/* get the object */                                               \
			C* obj = (C*)lua_touserdata(L, -1);                                \
			/* call the method */                                              \
			return obj->F(L);                                                  \
		}, U);                                                                 \
	/* add the method */                                                       \
	lua_setfield(L, -2, N);


/*! \brief Add a getter Function to a class property
* @param <lua_State* L> The Lua state
* @param <class C> Name of the class in C++
* @param <char* P> Name of the class's property in C++
* @param <lua_CFunction F> Function which returns the property to lua
*/
#define luaPropGetter(L, C, P, F)                                              \
	C::_luaMacros_PropGetters.insert(std::pair<std::string, lua_CFunction>(P, F))


/*! \brief Retrieve the class object
* @param <lua_State* L> The Lua state
* @param <class C> Name of the class in C++
* @return Class object
*/
#define luaGetGetterObj(L, C)                                                  \
	(C*)lua_touserdata(L, -2);


/*! \brief Add a setter Function to a class property
* @param <lua_State* L> The Lua state
* @param <class C> Name of the class in C++
* @param <char* P> Name of the class's property in C++
* @param <lua_CFunction F> Function which sets the property
*/
#define luaPropSetter(L, C, P, F)                                              \
	C::_luaMacros_PropSetters.insert(std::pair<std::string, lua_CFunction>(P, F))


/*! \brief Retrieve the class object
* @param <lua_State* L> The Lua state
* @param <class C> Name of the class in C++
* @return Class object
*/
#define luaGetSetterObj(L, C)                                                  \
	(C*)lua_touserdata(L, -3);


/*! \brief Finish the the binding block
* @param <lua_State* L> The Lua state
* @param <class C> Name of the class in C++
*/
#define luaBindClassEnd(L, C)                                                  \
	/* init metatable for object */                                            \
	luaL_newmetatable(L, "MetaTable_"#C);                                      \
	                                                                           \
	/* meta - method(destructor) ********************************************/ \
	lua_pushstring(L, "__gc");                                                 \
	lua_pushcclosure(L, [](lua_State* L) -> int                                \
		{                                                                      \
			/* Lua stack : -1 userdata */                                      \
			assert(lua_isuserdata(L, -1));                                     \
			/* get the object */                                               \
			C* obj = (C*)lua_touserdata(L, -1);                                \
			/* let c++ call the destructor (if any) */                         \
			obj->~C();                                                         \
			/* num of return values on the lua - stack */                      \
			return 0;                                                          \
		}, 0);                                                                 \
	lua_settable(L, -3);                                                       \
	                                                                           \
	/* meta - method(accessing methods / read properties) *******************/ \
	lua_pushstring(L, "__index");                                              \
	lua_pushcfunction(L, [](lua_State* L) -> int                               \
		{                                                                      \
			/* Lua stack : -2 userdata */                                      \
			assert(lua_isuserdata(L, -2));                                     \
			/* Lua stack : -1 string of accessed index(method / property) */   \
			assert(lua_isstring(L, -1));                                       \
			/* get the object */                                               \
			C* obj = luaGetGetterObj(L, C);                                    \
			/* get the index string */                                         \
			const char* index = lua_tostring(L, -1);                           \
			/* access property ? */                                            \
			auto it = C::_luaMacros_PropGetters.find(index);                   \
			if (it != C::_luaMacros_PropGetters.end())                         \
			{                                                                  \
				std::cout << "c++ >> __index::   getprop(c++):" << index << std::endl; \
				return it->second(L);                                          \
			}                                                                  \
			/* access user value / methods */                                  \
			else                                                               \
			{                                                                  \
				/* try to access a user value */                               \
				lua_getuservalue(L, 1);                                        \
				lua_pushvalue(L, 2);                                           \
				lua_gettable(L, -2);                                           \
				/* no user value->access native method */                      \
				if (lua_isnil(L, -1))                                          \
				{                                                              \
					std::cout << "c++ >> __index::   getmeth(c++):" << index << std::endl; \
					lua_getglobal(L, #C);                                      \
					lua_pushstring(L, index);                                  \
					auto ret = lua_rawget(L, -2);                              \
					/* TODO: if (lua_isnil(L, -1)) */                          \
				}                                                              \
				else                                                           \
				{                                                              \
					std::cout << "c++ >> __index::   getusrval(lua):" << index << std::endl; \
				}                                                              \
			}                                                                  \
			/* num of return values on the lua - stack */                      \
			return 1;                                                          \
		});                                                                    \
	lua_settable(L, -3);                                                       \
	                                                                           \
	/* meta - method(write properties) **************************************/ \
	lua_pushstring(L, "__newindex");                                           \
	lua_pushcfunction(L, [](lua_State* L) -> int                               \
		{                                                                      \
			/* Lua stack: -3 userdata */                                       \
			assert(lua_isuserdata(L, -3));                                     \
			/* Lua stack : -2 string of accessed index(property) */            \
			assert(lua_isstring(L, -2));                                       \
			/* Lua stack : -1 value to set */                                  \
			/* get the object */                                               \
			C* obj = luaGetSetterObj(L, C);                                    \
			/* get the index string */                                         \
			const char* index = lua_tostring(L, -2);                           \
			/* write property ? */                                             \
			auto it = C::_luaMacros_PropSetters.find(index);                   \
			if (it != C::_luaMacros_PropSetters.end())                         \
			{                                                                  \
				std::cout << "c++ >> __newindex::setprop(c++):" << index << std::endl; \
				it->second(L);                                                 \
			}                                                                  \
			/* set a user value */                                             \
			else                                                               \
			{                                                                  \
				std::cout << "c++ >> __newindex::setusrval(lua):" << index << std::endl; \
				/* 1 --table */                                                \
				lua_getuservalue(L, 1);                                        \
				/* 2 --index */                                                \
				lua_pushvalue(L, 2);                                           \
				/* 3 --value */                                                \
				lua_pushvalue(L, 3);                                           \
				/* 1[2] = 3 */                                                 \
				lua_settable(L, -3);                                           \
			}                                                                  \
			/* num of return values on the lua - stack */                      \
			return 0;                                                          \
		});                                                                    \
	lua_settable(L, -3);
