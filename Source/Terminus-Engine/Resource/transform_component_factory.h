#pragma once

#include <types.h>
#include <ECS/scene.h>

namespace terminus
{
	namespace transform_component_factory
	{
		void create(JsonValue& json, Entity& entity, Scene* scene)
		{
            TransformComponent& component = scene->attach_transform_component(entity);
            
            rapidjson::Value& position = value["position"];
            
            component._position.x = position["x"].GetFloat();
            component._position.y = position["y"].GetFloat();
            component._position.z = position["z"].GetFloat();
            
            rapidjson::Value& rotation = value["rotation"];
            
            component._rotation.x = rotation["x"].GetFloat();
            component._rotation.y = rotation["y"].GetFloat();
            component._rotation.z = rotation["z"].GetFloat();
            
            rapidjson::Value& scale = value["scale"];
            
            component._scale.x = scale["x"].GetFloat();
            component._scale.y = scale["y"].GetFloat();
            component._scale.z = scale["z"].GetFloat();
            
            if(!value["parent_entity"].IsNull())
            {
                component._parent_entity_name = std::string(value["parent_entity"].GetString());
            }

		}
	}
}