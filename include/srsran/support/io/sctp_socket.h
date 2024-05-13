/*
 *
 * Copyright 2021-2024 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#pragma once

#include "srsran/adt/expected.h"
#include "srsran/adt/optional.h"
#include "srsran/support/io/unique_fd.h"
#include <chrono>
#include <cstdint>
#include <sys/socket.h>

namespace srsran {

struct sctp_socket_params {
  int                  ai_family;
  int                  ai_socktype;
  int                  ai_protocol;
  bool                 reuse_addr = false;
  std::chrono::seconds rx_timeout{0};
  optional<int32_t>    rto_initial;
  optional<int32_t>    rto_min;
  optional<int32_t>    rto_max;
  optional<int32_t>    init_max_attempts;
  optional<int32_t>    max_init_timeo;
  optional<bool>       nodelay;
};

/// SCTP socket instance.
class sctp_socket
{
public:
  static expected<sctp_socket> create(const sctp_socket_params& params);

  sctp_socket();
  sctp_socket(sctp_socket&& other) noexcept = default;
  ~sctp_socket();
  sctp_socket& operator=(sctp_socket&& other) noexcept;

  bool close();

  SRSRAN_NODISCARD bool is_open() const { return sock_fd.is_open(); }
  const unique_fd&      fd() const { return sock_fd; }

  SRSRAN_NODISCARD bool bind(struct sockaddr& ai_addr, const socklen_t& ai_addrlen, const std::string& bind_interface);
  SRSRAN_NODISCARD bool connect(struct sockaddr& ai_addr, const socklen_t& ai_addrlen);
  /// \brief Start listening on socket.
  SRSRAN_NODISCARD bool listen();
  SRSRAN_NODISCARD bool set_non_blocking();

  optional<uint16_t> get_listen_port() const;

private:
  bool set_sockopts(const sctp_socket_params& params);

  srslog::basic_logger& logger;
  unique_fd             sock_fd;
};

} // namespace srsran