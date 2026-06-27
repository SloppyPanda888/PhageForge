#pragma once

#include "../biology/Genome.hpp"
#include <string>
#include <vector>
#include <cstdint>

namespace phageforge::serialization {

class BinarySerializer {
public:
    static std::vector<uint8_t> serialize(const biology::Genome& genome);
    static biology::Genome deserialize(const std::vector<uint8_t>& data);
    
    static void saveToFile(const biology::Genome& genome, const std::string& filepath);
    static biology::Genome loadFromFile(const std::string& filepath);
};

} // namespace phageforge::serialization
