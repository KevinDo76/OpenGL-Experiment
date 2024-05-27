#include "light.h"
#include <glm/glm.hpp>
lightPoint::lightPoint()
	:activated(false), lightPower(100), position(0), color(1)//, ambient(10), diffuse(100), specular(50)
{

}

lighDir::lighDir()
	:activated(false), lightPower(100), position(0), color(1), direction(0), cutOffIn(17.5f), cutOffOut(2455)
{
}
