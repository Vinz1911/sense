/*
 * MIT License
 *
 * Copyright (c) 2025 Vinzenz Weist
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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

    void Pathfinder::set_value(const std::string& path, const std::string& value) {
        stream_.open(path, std::fstream::out);
        stream_ << value; stream_.close();
    }
} // namespace sense