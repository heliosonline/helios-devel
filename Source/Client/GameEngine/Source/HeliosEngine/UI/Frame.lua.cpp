#include "pch.h"

#include "HeliosEngine/UI/Frame.h"


namespace Helios {


	Frame::Frame()
	{
		// do the lua stuff...

		OnConstruction();
	}


	Frame::~Frame()
	{
		// do the lua stuff...

		OnDestruction();
	}


	int Frame::Lua_GetName(lua_State* L)
	{
		return 0;
	}


	int Frame::Lua_SetName(lua_State* L)
	{
		return 0;
	}


	int Frame::Lua_GetParent(lua_State* L)
	{
		return 0;
	}


	int Frame::Lua_SetParent(lua_State* L)
	{
		return 0;
	}


	// Implement luaMacros support for this class
	luaMacros_ClassImpl(Frame);


	// Setup all bindings in Lua for this class
	void Frame::InitLuaState(lua_State* L)
	{
		// class object
		luaBindClassBegin(L, Frame, "Frame", "Create");
		// class methods
		luaBindMethod(L, Frame, "GetName", Frame::Lua_GetName);
		luaBindMethod(L, Frame, "SetName", Frame::Lua_SetName);
		luaBindMethod(L, Frame, "GetParent", Frame::Lua_GetParent);
		luaBindMethod(L, Frame, "SetParent", Frame::Lua_SetParent);

	//		// class properties
	//		luaPropGetter(L, Frame, "x", [](lua_State* L) -> int
	//			{
	//				std::cout << "c++ >> __index::   propgetter(c++): x" << std::endl;
	//				Frame* obj = luaGetGetterObj(L, Frame);
	//				lua_pushnumber(L, obj->x);
	//				return 1;
	//			});
	//		luaPropSetter(L, Frame, "x", [](lua_State* L) -> int
	//			{
	//				std::cout << "c++ >> __index::   propsetter(c++): x" << std::endl;
	//				Frame* obj = luaGetSetterObj(L, Frame);
	//				// Lua stack: -1 value is a number?
	//				assert(lua_isnumber(L, -1));
	//				obj->x = (int)lua_tonumber(L, -1);
	//				return 1;
	//			});

		luaBindClassEnd(L, Frame);
	}


} // namespace Helios
