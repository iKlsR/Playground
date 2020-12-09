#include "Entity.hpp"

Entity::Entity()
{

}

void Entity::addEntity(Model *mesh)
{
    entities.push_back(mesh);
}

void Entity::update(const QSharedPointer<Shader> &shader)
{
    // for (auto entity : entities) {
    //     entity->draw(shader);
    // }
}
