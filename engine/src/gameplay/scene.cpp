#include <gameplay/scene.h>
#include <core/frame_packet.h>
#include <core/context.h>

TERMINUS_BEGIN_NAMESPACE

Scene::Scene()
{
	for (int& version : m_versions)
	{
		version = 0;
	}
}

Scene::~Scene()
{

}

void Scene::initialize()
{
	m_camera_system.initialize(this);
	m_transform_system.initialize(this);
	m_script_system.initialize(this);
	m_physics_system.initialize(this);
	m_render_system.initialize(this);
}

void Scene::shutdown()
{
	m_script_system.shutdown();
	m_camera_system.shutdown();
	m_transform_system.shutdown();
	m_physics_system.shutdown();
	m_render_system.shutdown();

	if (m_physics_scene.active())
		context::get_physics_engine().destroy_scene(m_physics_scene);
}

void Scene::simulate(FramePacket* pkt, double dt)
{
	update_deferred_removal_list();

	if (m_physics_scene.active())
		m_physics_scene.simulate(dt);

	m_script_system.simulate(dt);
	m_transform_system.simulate(dt);
	m_camera_system.simulate(dt);
	m_physics_system.simulate(dt);
	

	m_render_system.simulate(pkt, dt);
}

void Scene::update_deferred_removal_list()
{
	uint32_t front = m_removal_queue.front();
	uint32_t back = m_removal_queue.back();

	for (uint32_t _front = front; _front < back; _front++)
	{
		RemovedEntity& removed_entity = m_removal_queue.entities[_front & DEFERRED_QUEUE_MASK];
		removed_entity.counter++;

		if (removed_entity.counter >= 2)
		{
			m_mesh_pool.remove(removed_entity.entity);
			m_removal_queue.pop_front();
		}
		else
			break;
	}
}

PhysicsScene& Scene::physics_scene()
{
	return m_physics_scene;
}

Entity* Scene::get_entity_array()
{
	return &m_entities._objects[0];
}

uint32_t Scene::get_num_entities()
{
	return m_entities._num_objects;
}

// attach methods

TransformComponent& Scene::attach_transform_component(Entity& entity)
{
	return m_transform_pool.create(entity);
}

MeshComponent& Scene::attach_mesh_component(Entity& entity)
{
	return m_mesh_pool.create(entity);
}

ColliderComponent& Scene::attach_collider_component(Entity& entity)
{
	return m_collider_pool.create(entity);
}

CameraComponent& Scene::attach_camera_component(Entity& entity)
{
	return m_camera_pool.create(entity);
}

LuaScriptComponent& Scene::attach_lua_script_component(Entity& entity)
{
	return m_lua_script_pool.create(entity);
}

CppScriptComponent& Scene::attach_cpp_script_component(Entity& entity)
{
	return m_cpp_script_pool.create(entity);
}

RigidBodyComponent& Scene::attach_rigid_body_component(Entity& entity)
{
	return m_rigid_body_pool.create(entity);
}

PointLightComponent& Scene::attach_point_light_component(Entity& entity)
{
	return m_point_light_pool.create(entity);
}

SpotLightComponent& Scene::attach_spot_light_component(Entity& entity)
{
	return m_spot_light_pool.create(entity);
}

DirectionalLightComponent& Scene::attach_directional_light_component(Entity& entity)
{
	return m_directional_light_pool.create(entity);
}

SkyComponent& Scene::attach_sky_component(Entity& entity)
{
	return m_sky_pool.create(entity);
}

// get id methods

ID Scene::get_transform_id(Entity& entity)
{
	return m_transform_pool.get_id(entity);
}

ID Scene::get_rigid_body_id(Entity& entity)
{
	return m_rigid_body_pool.get_id(entity);
}

ID Scene::get_collision_shape_id(Entity& entity)
{
	return m_collider_pool.get_id(entity);
}

// get methods

TransformComponent& Scene::get_transform_component(Entity& entity)
{
	return m_transform_pool.lookup(entity);
}

MeshComponent& Scene::get_mesh_component(Entity& entity)
{
	return m_mesh_pool.lookup(entity);
}

ColliderComponent& Scene::get_collider_component(Entity& entity)
{
	return m_collider_pool.lookup(entity);
}

CameraComponent& Scene::get_camera_component(Entity& entity)
{
	return m_camera_pool.lookup(entity);
}

LuaScriptComponent& Scene::get_lua_script_component(Entity& entity)
{
	return m_lua_script_pool.lookup(entity);
}

CppScriptComponent& Scene::get_cpp_script_component(Entity& entity)
{
	return m_cpp_script_pool.lookup(entity);
}

RigidBodyComponent& Scene::get_rigid_body_component(Entity& entity)
{
	return m_rigid_body_pool.lookup(entity);
}

PointLightComponent& Scene::get_point_light_component(Entity& entity)
{
	return m_point_light_pool.lookup(entity);
}

SpotLightComponent& Scene::get_spot_light_component(Entity& entity)
{
	return m_spot_light_pool.lookup(entity);
}

DirectionalLightComponent& Scene::get_directional_light_component(Entity& entity)
{
	return m_directional_light_pool.lookup(entity);
}

SkyComponent& Scene::get_sky_component(Entity& entity)
{
	return m_sky_pool.lookup(entity);
}

// has methods

bool Scene::has_transform_component(Entity& entity)
{
	return m_transform_pool.has(entity);
}

bool Scene::has_mesh_component(Entity& entity)
{
	return m_mesh_pool.has(entity);
}

bool Scene::has_camera_component(Entity& entity)
{
	return m_camera_pool.has(entity);
}

bool Scene::has_lua_script_component(Entity& entity)
{
	return m_lua_script_pool.has(entity);
}

bool Scene::has_cpp_script_component(Entity& entity)
{
	return m_cpp_script_pool.has(entity);
}

bool Scene::has_collider_component(Entity& entity)
{
	return m_collider_pool.has(entity);
}

bool Scene::has_rigid_body_component(Entity& entity)
{
	return m_rigid_body_pool.has(entity);
}

bool Scene::has_point_light_component(Entity& entity)
{
	return m_point_light_pool.has(entity);
}

bool Scene::has_spot_light_component(Entity& entity)
{
	return m_spot_light_pool.has(entity);
}

bool Scene::has_directional_light_component(Entity& entity)
{
	return m_directional_light_pool.has(entity);
}

bool Scene::has_sky_component(Entity& entity)
{
	return m_sky_pool.has(entity);
}

Entity& Scene::create_entity(std::string name = "")
{
	assert(m_entities._num_objects != MAX_ENTITIES);

	ID id = m_entities.add();
	Entity& entity = m_entities.lookup(id);
	entity._name = name;
	entity._id = id;
	entity._version = m_versions[INDEX_FROM_ID(id)]++;

	return entity;
}

Entity& Scene::create_entity_from_prefab(std::string prefab)
{
	Entity& entity = create_entity();
	return entity;
}

void Scene::destroy_entity(Entity& entity)
{
	if (entity._id != INVALID_ID && is_alive(entity))
	{
		m_removal_queue.push_back(entity);

		m_script_system.on_entity_destroyed(entity);
		m_camera_system.on_entity_destroyed(entity);
		m_transform_system.on_entity_destroyed(entity);
		m_physics_system.on_entity_destroyed(entity);
		m_render_system.on_entity_destroyed(entity);

		// remove all components belonging to entity
		m_transform_pool.remove(entity);
		m_cpp_script_pool.remove(entity);
		m_lua_script_pool.remove(entity);
		m_rigid_body_pool.remove(entity);
		m_collider_pool.remove(entity);
		m_point_light_pool.remove(entity);
		m_spot_light_pool.remove(entity);
		m_directional_light_pool.remove(entity);
		m_sky_pool.remove(entity);

		m_versions[INDEX_FROM_ID(entity._id)]++;
		m_entities.remove(entity._id);
	}
}

bool Scene::is_alive(Entity& entity)
{
	return (entity._version == m_versions[INDEX_FROM_ID(entity._id)] - 1);
}

void Scene::set_name(const char* name)
{
	m_name = name;
}

TERMINUS_END_NAMESPACE