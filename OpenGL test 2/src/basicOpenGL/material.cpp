#include "material.h"

material::material(glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular, float _shininess)
	:ambient(_ambient), diffuse(_diffuse), specular(_specular), shininess(_shininess)
{
}

material::material()
	: ambient(1), diffuse(1), specular(1), shininess(32)
{
}
