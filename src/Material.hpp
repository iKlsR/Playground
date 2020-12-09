#ifndef PL_MATERIAL_HPP
#define PL_MATERIAL_HPP

#include <QString>
#include <QMap>

enum class TextureSlot
{
    ALBEDO,
    ROUGHNESS,
    METALLIC,
    NORMAL,
    AO,
    NONE
};

class Material 
{
public:
    explicit Material();
    ~Material();

    QMap<TextureSlot, QString> textures;
private:
    TextureSlot texture_slot;
    QString texturePath;
};

#endif // PL_MATERIAL_HPP