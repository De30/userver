#pragma once

#include <memory>
#include <string>

#include <json/value.h>

#include <json_config/variable_map.hpp>
#include <server/request/request_config.hpp>

namespace server {
namespace net {

struct ConnectionConfig {
  std::string task_processor;
  size_t in_buffer_size = 4 * 1024;
  size_t requests_queue_size_threshold = 100;
  std::unique_ptr<request::RequestConfig> request;

  static ConnectionConfig ParseFromJson(
      const Json::Value& json, const std::string& full_path,
      const json_config::VariableMapPtr& config_vars_ptr);
};

}  // namespace net
}  // namespace server
