//
// Created by Vinzenz Weist on 17.02.25.
//

#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <string>
#include <fstream>

namespace sense {
    class Pathfinder {
    public:
        /**
         * @brief create instance of `Pathfinder`
         */
        Pathfinder();

        /**
         * @brief destroy instance of `Pathfinder`
         */
        ~Pathfinder();

        /**
         * @brief get data from a fstream
         * @param path the path to read from
         * @return the value
         */
        std::string get_value(const std::string& path);

        /**
         * @brief set data to a fstream
         * @param path the path to write to
         * @param value the value to write
         */
        template <typename T>
        void set_value(const std::string& path, const T& value);

    private:
        /**
         * @brief file stream
         */
        std::fstream stream_;
    };
} // namespace sense
#endif //PATHFINDER_H
