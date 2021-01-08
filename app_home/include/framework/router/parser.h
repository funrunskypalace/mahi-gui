#pragma once

namespace framework
{

GreatWallConfigPtr parse_config_json(const std::string& jsonFilePathName,
                                     std::string& errorMsgs);
GreatWallConfigPtr parse_config_json_text(const std::string& jsonContent,
                                          std::string& errorMsgs);

} // namespace framework