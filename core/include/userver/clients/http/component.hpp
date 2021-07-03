#pragma once

#ifdef USERVER_TVM2_HTTP_CLIENT
#error Use components::Http from clients/http.hpp instead
#endif

/// @file clients/http/component.hpp
/// @brief @copybrief components::HttpClient

#include <clients/http/client.hpp>
#include <components/loggable_component_base.hpp>
#include <components/statistics_storage.hpp>
#include <concurrent/async_event_channel.hpp>
#include <taxi_config/snapshot.hpp>

namespace components {

class TaxiConfig;

// clang-format off

/// @ingroup userver_components
///
/// @brief Component that manages clients::http::Client.
///
/// Returned references to clients::http::Client live for a lifetime of the
/// component and are safe for concurrent use.
///
/// ## Dynamic options:
/// * @ref HTTP_CLIENT_CONNECT_THROTTLE
/// * @ref HTTP_CLIENT_CONNECTION_POOL_SIZE
/// * @ref HTTP_CLIENT_ENFORCE_TASK_DEADLINE
/// * @ref USERVER_HTTP_PROXY
///
/// ## Static options:
/// Name | Description | Default value
/// ---- | ----------- | -------------
/// pool-statistics-disable | set to true to disable statistics for connection pool | false
/// thread-name-prefix | set OS thread name to this value | ''
/// threads | number of threads to process low level HTTP related IO system calls | 8
/// fs-task-processor | task processor to run blocking HTTP related calls, like DNS resolving or hosts reading | -
/// destination-metrics-auto-max-size | set max number of automatically created destination metrics | 100
/// user-agent | User-Agent HTTP header to show on all requests, result of utils::GetUserverIdentifier() if empty | empty
/// bootstrap-http-proxy | HTTP proxy to use at service start. Will be overridden by @ref USERVER_HTTP_PROXY at runtime config update | ""
/// testsuite-enabled | enable testsuite testing support | false
/// testsuite-timeout | if set, force the request timeout regardless of the value passed in code | -
/// testsuite-allowed-url-prefixes | if set, checks that all URLs start with any of the passed prefixes, asserts if not. Set for testing purposes only. | ''
///
/// ## Static configuration example:
///
/// @snippet components/common_component_list_test.cpp  Sample http client component config

// clang-format on
class HttpClient final : public LoggableComponentBase {
 public:
  static constexpr auto kName = "http-client";

  HttpClient(const ComponentConfig&, const ComponentContext&);

  clients::http::Client& GetHttpClient();

 private:
  void OnConfigUpdate(
      const std::shared_ptr<const taxi_config::Snapshot>& config);

  formats::json::Value ExtendStatistics();

 private:
  const bool disable_pool_stats_;
  clients::http::Client http_client_;
  concurrent::AsyncEventSubscriberScope subscriber_scope_;
  utils::statistics::Entry statistics_holder_;
};

}  // namespace components