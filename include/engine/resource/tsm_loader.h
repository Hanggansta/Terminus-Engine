#pragma once

#include <Resource/asset_common.h>
#include <types.h>

namespace terminus
{
	namespace tsm_loader
    {
		extern asset_common::MeshLoadData* load(String filename);
	};
}