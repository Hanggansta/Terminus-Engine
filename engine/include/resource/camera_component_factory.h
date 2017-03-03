#pragma once

#include <types.h>
#include <ECS/entity.h>
#include <ECS/component_types.h>
#include <Core/context.h>
#include <editor/editor_scene.h>
#include <editor/editor_entity.h>

namespace terminus
{
	namespace camera_component_factory
	{
        void create_internal(CameraComponent& component, JsonValue& json)
        {
            rapidjson::Value& transform = json["transform"];
            
            {
                rapidjson::Value& position = transform["position"];
                
                component.transform._position.x = position["x"].GetFloat();
                component.transform._position.y = position["y"].GetFloat();
                component.transform._position.z = position["z"].GetFloat();
                
                rapidjson::Value& rotation = transform["rotation"];
                
                component.transform._euler_angles = Vector3(rotation["x"].GetFloat(),
                                                            rotation["y"].GetFloat(),
                                                            rotation["z"].GetFloat());
                
                component.transform._rotation = glm::quat(Vector3(glm::radians(component.transform._euler_angles.x),
                                                                  glm::radians(component.transform._euler_angles.y),
                                                                  glm::radians(component.transform._euler_angles.z)));
            }
            
            rapidjson::Value& projection_info = json["projection_info"];
            
            {
                component.far_plane = projection_info["far_plane"].GetFloat();
                component.near_plane = projection_info["near_plane"].GetFloat();
                
                String proj_type = String(projection_info["projection_type"].GetString());
                
                if(proj_type == "PERSPECTIVE")
                {
                    component.projection_type = ProjectionType::PERSPECTIVE;
                    
                    component.persp_info.fov = projection_info["field_of_view"].GetFloat();
                    
                    float aspect_x = projection_info["aspect_ratio_x"].GetFloat();
                    float aspect_y = projection_info["aspect_ratio_y"].GetFloat();
                    
                    if(aspect_x == 0 && aspect_y == 0)
                    {
                        aspect_x = context::get_platform().get_width();
                        aspect_y = context::get_platform().get_height();
                    }
                    
                    component.persp_info.aspect_ratio = aspect_x / aspect_y;
                }
                else
                {
                    component.projection_type = ProjectionType::ORTHOGRAPHIC;
                }
            }
            
            component.is_active = json["is_active"].GetBool();
            component.is_offscreen = json["is_offscreen"].GetBool();
            
            rapidjson::Value& viewport_rect = json["viewport_rect"];
            
            {
                component.screen_rect.x = viewport_rect["x"].GetFloat();
                component.screen_rect.y = viewport_rect["y"].GetFloat();
                component.screen_rect.z = viewport_rect["w"].GetFloat();
                component.screen_rect.w = viewport_rect["h"].GetFloat();
            }
            
            RenderingPathCache& rp_cache = context::get_rendering_path_cache();
            component.rendering_path = rp_cache.load(String(json["rendering_path"].GetString()));
        }
        
		void create(JsonValue& json, Entity& entity, Scene* scene)
		{
            CameraComponent& component = scene->attach_camera_component(entity);
            create_internal(component, json);
        }
        
#if defined(TERMINUS_WITH_EDITOR)
        void create_from_desc(CameraDesc* desc, Entity& entity, Scene* scene)
        {
            CameraComponent& component = scene->attach_camera_component(entity);
            component = desc->cmp;
        }
        
        void create(JsonValue& json, EditorEntity& entity)
        {
            CameraDesc* desc = new CameraDesc();
            create_internal(desc->cmp, json);
            entity.add_component_desc(EditorComponentType::CAMERA, desc);
        }
#endif
	}
}