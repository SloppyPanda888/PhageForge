#include "JSONSerializer.hpp"
#include "../core/Exceptions.hpp"
#include <fstream>
#include <sstream>

namespace phageforge::serialization {

void saveGenomeToFile(const biology::Genome& genome, const std::string& filepath) {
    std::string json = genome.toJSON();
    std::ofstream file(filepath);
    if (!file.is_open()) {
        throw core::SerializationException("Failed to open file for writing: " + filepath);
    }
    file << json;
}

biology::Genome loadGenomeFromFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw core::SerializationException("Failed to open file for reading: " + filepath);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return biology::Genome::fromJSON(buffer.str());
}

std::string genomeToJSONString(const biology::Genome& genome) {
    return genome.toJSON();
}

biology::Genome genomeFromJSONString(const std::string& json_str) {
    return biology::Genome::fromJSON(json_str);
}

} // namespace phageforge::serialization
