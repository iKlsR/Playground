#ifndef PL_ENTITY_HPP
#define PL_ENTITY_HPP

#include <vector>

#include "Model.hpp"
#include "Shader.hpp"

class Entity
{
public:
    Entity();
    void addEntity(Model *mesh);
    void update(const QSharedPointer<Shader> &shader);
    std::vector<Model*> getEntities() {
        return entities;
    }

private:
    std::vector<Model*> entities;
};

#endif // PL_ENTITY_HPP
