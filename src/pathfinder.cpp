//
// Created by Vinzenz Weist on 17.02.25.
//

#include <fstream>

#include "sense/pathfinder.h"

namespace sense {
    Pathfinder::Pathfinder() = default;
    Pathfinder::~Pathfinder() = default;

    std::string Pathfinder::get_value(const std::string& path) {
        auto value = std::string();
        stream_.open(path, std::fstream::in);
        stream_ >> value; stream_.close(); return value;
    }

    template <typename T>
    void Pathfinder::set_value(const std::string& path, const T& value) {
        stream_.open(path, std::fstream::out);
        stream_ << value; stream_.close();
    }

    template void Pathfinder::set_value<std::string>(const std::string&, const std::string&);
    template void Pathfinder::set_value<uint8_t>(const std::string&, const uint8_t&);
} // namespace sense