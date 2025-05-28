#ifndef SHADER_MENU_H
#define SHADER_MENU_H

#include <string>
#include <glad/glad.h>

class ShaderMenu {
public:
    unsigned int ID;

    ShaderMenu(const char* vertexPath, const char* fragmentPath);
    void use();

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
};

#endif
