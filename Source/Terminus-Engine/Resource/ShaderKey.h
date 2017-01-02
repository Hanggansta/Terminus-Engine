#ifndef ShaderKey_h
#define ShaderKey_h

#include "AssetCommon.h"

namespace Terminus { namespace Resource {
  
    struct ShaderKey
    {
        // |            |        |
        // | 3-bits     | 1-bit  | 1-bit
        // | Renderable | Albedo |
        
        uint64_t _key;
        
        ShaderKey()
        {
            _key = 0;
        }
        
        ShaderKey(RenderableType type, bool albedo, bool normal, bool metalness, bool roughness, bool parallax)
        {
            EncodeMeshType(type);
            EncodeAlbedo(albedo);
            EncodeNormal(normal);
            EncodeMetalness(metalness);
            EncodeRoughness(roughness);
            EncodeParallaxOcclusion(parallax);
        }
        
        inline void EncodeParallaxOcclusion(bool option)
        {
            uint64 temp = option;
            _key |= temp << 56;
        }
        
        inline void EncodeMetalness(bool option)
        {
            uint64 temp = option;
            _key |= temp << 57;
        }
        
        inline void EncodeRoughness(bool option)
        {
            uint64 temp = option;
            _key |= temp << 58;
        }
        
        inline void EncodeNormal(bool option)
        {
            uint64 temp = option;
            _key |= temp << 59;
        }
        
        inline void EncodeAlbedo(bool option)
        {
            uint64 temp = option;
            _key |= temp << 60;
        }
        
        inline void EncodeMeshType(RenderableType type)
        {
            uint64 temp = type;
            _key |= temp << 61;
        }
        
        inline int	DecodeMeshType()
        {
            return (_key >> 61);
        }
        
        inline int	DecodeAlbedo()
        {
            return (_key >> 60) & 1;
        }
        
        inline int	DecodeNormal()
        {
            return (_key >> 59) & 1;
        }
        
        inline int	DecodeRoughness()
        {
            return (_key >> 58) & 1;
        }
        
        inline int	DecodeMetalness()
        {
            return (_key >> 57) & 1;
        }
        
        inline int	DecodeParallaxOcclusion()
        {
            return (_key >> 56) & 1;
        }
        
    };
    
} }

#endif