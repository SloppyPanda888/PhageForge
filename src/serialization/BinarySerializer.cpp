#include "BinarySerializer.hpp"
#include "../core/Exceptions.hpp"
#include <fstream>
#include <vector>

namespace phageforge::serialization {

std::vector<uint8_t> BinarySerializer::serialize(const biology::Genome& /*genome*/) {
    // Phase 1 stub - Phase 4+ will implement binary serialization
    return std::vector<uint8_t>();
}

biology::Genome BinarySerializer::deserialize(const std::vector<uint8_t>& /*data*/) {
    // Phase 1 stub
    return biology::Genome();
}

void BinarySerializer::saveToFile(const biology::Genome& genome, const std::string& filepath) {
    auto data = serialize(genome);
    std::ofstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        throw core::SerializationException("Failed to open file for binary writing: " + filepath);
    }
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

biology::Genome BinarySerializer::loadFromFile(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        throw core::SerializationException("Failed to open file for binary reading: " + filepath);
    }
    
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::vector<uint8_t> data(size);
    file.read(reinterpret_cast<char*>(data.data()), size);
    
    return deserialize(data);
}

} // namespace phageforge::serialization
