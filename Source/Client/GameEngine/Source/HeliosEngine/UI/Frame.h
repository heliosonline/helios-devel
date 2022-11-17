#pragma once
/*

#include "HeliosEngine/Core/Timestep.h"

#include "lua/lua.h"
#include "LuaHelper/LuaMacros.h"


namespace Helios {


	class Frame
	{
	// LuaAPI
	public:
		Frame();
		~Frame();

		int Lua_GetName(lua_State* L);
		int Lua_SetName(lua_State* L);

		int Lua_GetParent(lua_State* L);
		int Lua_SetParent(lua_State* L);

	// Engine
	public:
		void OnConstruction();
		void OnDestruction();

		void OnUpdate(Timestep ts);

	// internal
	private:
//		Frame& m_Parent;
		std::vector<Frame&> m_Childs;

	// Lua
	public:
		static void InitLuaState(lua_State* L);
		luaMacros_ClassEnable(Frame);
	};


} // namespace Helios
*/
