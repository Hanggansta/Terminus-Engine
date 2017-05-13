#pragma once

#include <core/macro.h>
#include <core/types.h>

TERMINUS_BEGIN_NAMESPACE

class Scene;
struct Entity;

class LuaScript
{
public:
    LuaFunction _initialize;
    LuaFunction _update;
    LuaFunction _shutdown;
    LuaObject	_object;
    String      _instance_name;
    String      _class_name;
	StringBuffer32 _script_name;
    
    // development only
    uint32_t    _id;
    
public:
    LuaScript();
    ~LuaScript();
    
    void initialize(Scene* scene, Entity entity);
    void update(double dt);
    void shutdown();
};

TERMINUS_END_NAMESPACE
