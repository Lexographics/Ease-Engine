#ifndef SW_SHADER_HPP_
#define SW_SHADER_HPP_

#include <string>
#include <cstdint>

namespace sowa {
namespace gfx {
class IShader {
    public:
        virtual void New(const std::string &vsource, const std::string& fsource) = 0;
        virtual void Delete() = 0;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        virtual void UniformTexture(const std::string& name, uint32_t textureID, int slot) = 0;
};
} // namespace gfx
} // namespace sowa

#endif // SW_SHADER_HPP_