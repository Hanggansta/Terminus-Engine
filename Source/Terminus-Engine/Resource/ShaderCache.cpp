#include "../Graphics/Config.h"
#include "ShaderCache.h"
#include "../IO/FileSystem.h"
#include <iostream>

namespace Terminus { namespace Resource {

		ShaderCache::ShaderCache()
		{

		}

		ShaderCache::~ShaderCache()
		{

		}

		void ShaderCache::Initialize(Graphics::RenderDevice* device)
		{
			m_device = device;
			m_Factory.Initialize(device);
		}

		Graphics::ShaderProgram* ShaderCache::Load(const char* _vertexID,
												   const char* _pixelID,
												   const char* _geometryID,
												   const char* _tessevalID,
												   const char* _tesscontrolID)
		{
			Graphics::Shader* vertex;
			Graphics::Shader* pixel; 
			Graphics::Shader* geometry;
			Graphics::Shader* tess_eval;
			Graphics::Shader* tess_control;

#if defined(TERMINUS_OPENGL)
			std::string extension = ".glsl";
#else defined(TERMINUS_DIRECT3D11)
			std::string extension = ".hlsl";
#endif

			// load vertex shader
			{
				if (_vertexID != nullptr)
				{
					std::string filename = std::string(_vertexID);
					std::string id = filename + extension;

					if (m_ShaderMap.find(id) == m_ShaderMap.end())
					{
						std::string extension = FileSystem::get_file_extention(id);

						if (m_LoaderMap.find(extension) == m_LoaderMap.end())
						{
							return nullptr;
						}
						else
						{
							AssetCommon::TextLoadData* data = static_cast<AssetCommon::TextLoadData*>(m_LoaderMap[extension]->Load(id));
							data->shader_type = ShaderType::VERTEX;

							vertex = m_Factory.Create(data);
						}
					}
					else
						vertex = m_ShaderMap[id];
				}
				else
					return nullptr;
			}

			// load pixel shader
			{
				if (_vertexID != nullptr)
				{
					std::string filename = std::string(_pixelID);
					std::string id = filename + extension;

					if (m_ShaderMap.find(id) == m_ShaderMap.end())
					{
						std::string extension = FileSystem::get_file_extention(id);

						if (m_LoaderMap.find(extension) == m_LoaderMap.end())
						{
							return nullptr;
						}
						else
						{
							AssetCommon::TextLoadData* data = static_cast<AssetCommon::TextLoadData*>(m_LoaderMap[extension]->Load(id));
							data->shader_type = ShaderType::PIXEL;

							pixel = m_Factory.Create(data);
						}
					}
					else
						pixel = m_ShaderMap[id];
				}
				else
					return nullptr;
			}

			// load geometry shader
			{
				if (_geometryID != nullptr)
				{
					std::string filename = std::string(_geometryID);
					std::string id = filename + extension;

					if (m_ShaderMap.find(id) == m_ShaderMap.end())
					{
						std::string extension = FileSystem::get_file_extention(id);

						if (m_LoaderMap.find(extension) == m_LoaderMap.end())
						{
							return nullptr;
						}
						else
						{
							AssetCommon::TextLoadData* data = static_cast<AssetCommon::TextLoadData*>(m_LoaderMap[extension]->Load(id));
							data->shader_type = ShaderType::GEOMETRY;

							geometry = m_Factory.Create(data);
						}
					}
					else
						geometry = m_ShaderMap[id];
				}
				else
					geometry = nullptr;
			}

			// load tessellation evaluation shader
			{
				if (_tessevalID != nullptr)
				{
					std::string filename = std::string(_tessevalID);
					std::string id = filename + extension;

					if (m_ShaderMap.find(id) == m_ShaderMap.end())
					{
						std::string extension = FileSystem::get_file_extention(id);

						if (m_LoaderMap.find(extension) == m_LoaderMap.end())
						{
							return nullptr;
						}
						else
						{
							AssetCommon::TextLoadData* data = static_cast<AssetCommon::TextLoadData*>(m_LoaderMap[extension]->Load(id));
							data->shader_type = ShaderType::TESSELLATION_EVALUATION;

							tess_eval = m_Factory.Create(data);
						}
					}
					else
						tess_eval = m_ShaderMap[id];
				}
				else
					tess_eval = nullptr;
			}

			// load tessellation control shader
			{
				if (_tesscontrolID != nullptr)
				{
					std::string filename = std::string(_tesscontrolID);
					std::string id = filename + extension;

					if (m_ShaderMap.find(id) == m_ShaderMap.end())
					{
						std::string extension = FileSystem::get_file_extention(id);

						if (m_LoaderMap.find(extension) == m_LoaderMap.end())
						{
							return nullptr;
						}
						else
						{
							AssetCommon::TextLoadData* data = static_cast<AssetCommon::TextLoadData*>(m_LoaderMap[extension]->Load(id));
							data->shader_type = ShaderType::TESSELLATION_CONTROL;

							tess_control = m_Factory.Create(data);
						}
					}
					else
						tess_control = m_ShaderMap[id];
				}
				else
					tess_control = nullptr;
			}
			
			Graphics::ShaderProgram* program = m_device->CreateShaderProgram(vertex, pixel, geometry, tess_control, tess_eval);

			if (program)
			{
				m_ShaderProgramMap[vertex] = program;
				m_ShaderProgramMap[pixel] = program;

				if (geometry)
					m_ShaderProgramMap[geometry] = program;
				if (tess_control)
					m_ShaderProgramMap[tess_control] = program;
				if (tess_eval)
					m_ShaderProgramMap[tess_eval] = program;

				return program;
			}
			else
				return nullptr;
		}

		Graphics::ShaderProgram* ShaderCache::Load(ShaderKey key)
		{
			return nullptr;
		}

		void ShaderCache::Unload(Graphics::ShaderProgram* program)
		{
			// TODO : erase from map
			m_device->DestoryShaderProgram(program);
		}

} }