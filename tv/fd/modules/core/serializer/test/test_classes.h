#ifndef TEST_CLASSES_H_
#define TEST_CLASSES_H_

#include "serializable.h"

struct Vector3Df : Serializable
{
    Vector3Df() = default;
    float x;
    float y;
    float z;

    uint64_t serializedSize() const override
    {
        return sizeof(Vector3Df);
    }

    void serialize(uint8_t* serialized_data) const override
    {
        Serializer serializer(serialized_data);
        serializer.serializeStaticType(*this);
    }

    void deserialize(const uint8_t* serialized_data) override
    {
        Deserializer deserializer(serialized_data);
        deserializer.deserializeToStaticType(*this);
    }

};

/*class GpsData : public Serializable
{
private:
    
    std::vector<float> timestamps;
    std::string name;
    std::vector<std::string> names;
    Vector3Df current_pos;
    std::vector<Vector3Df> previous_positions;
    size_t num_values;
    float* values;
    Vector3Df* vec;

public:

    GpsData()
    {
        values = nullptr;
        vec = nullptr;
    }

    size_t serializedSize() const override
    {
        size_t string_vec_size = 0U;
        for(size_t k = 0; k < names.size(); k++)
        {
            string_vec_size += names[k].length() + 1;
        }

        return string_vec_size + sizeof(float) * timestamps.size() + name.length() + 1 + current_pos.serializedSize() + 
        current_pos.serializedSize() * previous_positions.size() + num_values * sizeof(float) + num_values * current_pos.serializedSize();
    }

    ConstWeakByteArray serialize() const override
    {
        Serializer serializer(serializedSize());
        serializer.serializePointer(timestamps.data(), timestamps.size());
        serializer.serializeString(name);

        for(size_t k = 0; k < names.size(); k++)
        {
            serializer.serializeString(names[k]);
        }

        serializer.serializeStaticType(current_pos);

        serializer.serializePointer(previous_positions.data(), previous_positions.size());
        serializer.serializePointer(values, num_values);
        serializer.serializePointer(vec, num_values);

        // TODO

        return serializer.getConstWeakByteArray();
    }

    void deserialize(const ConstWeakByteArray& serialized_data) override
    {

    }
};*/


#endif
