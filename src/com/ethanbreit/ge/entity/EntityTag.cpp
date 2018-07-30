#include <ge/entity/EntityTag.h>


namespace ge
{

	EntityTag::EntityTag() {};

	EntityTag::EntityTag(std::string name)
	{
		this->name = name; 
	}
}