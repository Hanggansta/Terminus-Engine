#include <Resource/material_factory.h>
#include <Core/context.h>

namespace terminus
{
	MaterialFactory::MaterialFactory()
	{

	}

	MaterialFactory::~MaterialFactory()
	{

	}

	Material* MaterialFactory::Create(AssetCommon::TextLoadData* data)
	{
        TextureCache& cache = context::get_texture_cache();
		JsonDocument doc;
		doc.Parse(data->buffer);

		Material* material = new Material();

		if (doc.HasMember("diffuse_map"))
		{
			String key = std::string(doc["diffuse_map"].GetString());
            material->texture_maps[DIFFUSE] = (Texture2D*)cache.Load(key);
		}
		else
		{
			assert(doc.HasMember("diffuse_value"));
			rapidjson::Value& val = doc["diffuse_value"];

			Vector4 color;
			color.r = val["r"].GetFloat();
			color.g = val["g"].GetFloat();
			color.b = val["b"].GetFloat();
			color.a = val["a"].GetFloat();

			material->diffuse_value = color;
		}

		if (doc.HasMember("normal_map"))
		{
			String key = std::string(doc["normal_map"].GetString());
			material->texture_maps[NORMAL] = (Texture2D*)cache.Load(key);
		}

		if (doc.HasMember("roughness_map"))
		{
			String key = std::string(doc["roughness_map"].GetString());
			material->texture_maps[ROUGHNESS] = (Texture2D*)cache.Load(key);
		}
		else
		{
			assert(doc.HasMember("roughness_value"));
			material->roughness_value = doc["roughness_value"].GetFloat();
		}

		if (doc.HasMember("metalness_map"))
		{
			String key = std::string(doc["metalness_map"].GetString());
			material->texture_maps[METALNESS] = (Texture2D*)cache.Load(key);
		}
		else
		{
			assert(doc.HasMember("metalness_value"));
			material->metalness_value = doc["metalness_value"].GetFloat();
		}

		if (doc.HasMember("displacement_map"))
		{
			String key = std::string(doc["displacement_map"].GetString());
			material->texture_maps[DISPLACEMENT] = (Texture2D*)cache.Load(key);
		}

		return material;
	}
}