#include "entity.h"
#include "framework.h"
#include "utils.h"
#include "camera.h"
#include "shader.h"

std::vector<Entity*> Entity::toDestroy;

Entity::Entity()
{
	parent = NULL;
}

Entity::~Entity()
{
}

void Entity::render(Camera* camera, Shader* shader)
{
	for (int i = 0; i < children.size(); i++) {
		children[i]->render(camera, shader);
	}

}

void Entity::update(float elapsed_time)
{
}

std::string Entity::getName() {
	return name;
}

void Entity::setName(std::string name) {
	this->name = name;
}
Vector3 Entity::getPosition()
{
	return Vector3();
}

Matrix44 Entity::getGlobalMatrix()
{
	if (parent)
		return model*parent->getGlobalMatrix();
	return model;
}

void Entity::addChild(Entity* ent)
{
	ent->parent = this;
	children.push_back(ent);
}

void Entity::removeChild(Entity* ent)
{
	auto it = std::find(children.begin(), children.end(), ent);
	if (it == children.end())
		return;
	children.erase(it);

	ent->parent = NULL;

	//para actualizar la posicion de la entity cuando se quita como hijo
	ent->model = ent->model * getGlobalMatrix();

}

void Entity::deleteEntity()
{
	Entity* tod;
	for (int i = 0; i < toDestroy.size(); i++) {
		tod = toDestroy[i];
	}
	if (tod->parent != NULL) {
		tod->parent->removeChild(tod);
	}
	delete(tod);

}