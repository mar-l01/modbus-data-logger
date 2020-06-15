#include "domain/framework/includes/JsonFileReader.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

namespace Framework {

JsonFileReader::JsonFileReader() {}

void JsonFileReader::readConfigurationFile(const std::string& path)
{
    std::ifstream jsonFs(path);
    if (!jsonFs) {
        std::cerr << "[JsonFileReader] Failed to open file: " << path << '\n';
        return;
    }

    try {
        nlohmann::json j;
        jsonFs >> j;

        std::cout << "[JsonFileReader]\n" << j.dump(4) << '\n';
    } catch (std::exception& ex) {
        std::cerr << "[JsonFileReader] Error: " << ex.what() << '\n';
    }
}

Entity::ModbusConfiguration JsonFileReader::getModbusConfiguration() const
{
    return Entity::ModbusConfiguration();
}

}
