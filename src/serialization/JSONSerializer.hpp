#pragma once

#include "../biology/Genome.hpp"
#include <string>

namespace phageforge::serialization {

void saveGenomeToFile(const biology::Genome& genome, const std::string& filepath);
biology::Genome loadGenomeFromFile(const std::string& filepath);

// Convenience functions for in-memory operations
std::string genomeToJSONString(const biology::Genome& genome);
biology::Genome genomeFromJSONString(const std::string& json_str);

} // namespace phageforge::serialization
