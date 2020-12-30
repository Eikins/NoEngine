#include "Core/Assets/Shader.h"

#include <vector>
#include <fstream>

namespace Core
{
    std::string Shader::GetEntryPoint() const
    {
        return _entryPoint;
    }

    std::vector<char> Shader::GetCompiledCode() const
    {
        return std::vector<char>(_code);
    }

	Shader Shader::LoadFromFile(std::string name, std::string path, std::string entrypoint)
	{
        Shader shader(name);
        shader._entryPoint = entrypoint;

        std::ifstream file(path, std::ios::ate | std::ios::binary);

        if (file.is_open() == false)
        {
            throw std::runtime_error("Failed to open file");
        }

        size_t fileSize = (size_t)file.tellg();
        shader._codeSize = fileSize;
        shader._code.resize(fileSize);

        file.seekg(0);
        file.read(shader._code.data(), fileSize);
        file.close();

        return shader;
	}
}