/*
 *
 * Copyright 2021-2023 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#pragma once

#include "srsran/rlc/rlc_config.h"
#include "fmt/format.h"

namespace srsran {

struct rlc_tm_tx_metrics {
  uint32_t num_small_allocs; ///< Number of allocations that are too small to TX PDU
};

struct rlc_um_tx_metrics {
  uint32_t num_pdus_with_segmentation;      ///< Number of transmitted PDUs with segmentation
  uint32_t num_pdu_bytes_with_segmentation; ///< Number of transmitted PDU bytes with segmentation
};

struct rlc_am_tx_metrics {
  uint32_t num_pdus_with_segmentation;      ///< Number of transmitted PDUs with segmentation
  uint32_t num_pdu_bytes_with_segmentation; ///< Number of transmitted PDU bytes with segmentation
  uint32_t num_retx_pdus;                   ///< Number of RETX'ed PDUs
  uint32_t num_retx_pdu_bytes;              ///< Number of RETX'ed PDUs bytes
  uint32_t num_ctrl_pdus;                   ///< Number of control PDUs
  uint32_t num_ctrl_pdu_bytes;              ///< Number of control PDUs bytes
};

struct rlc_tx_metrics {
  // SDU metrics
  uint32_t num_sdus;             ///< Number of SDUs
  size_t   num_sdu_bytes;        ///< Number of SDU bytes
  uint32_t num_dropped_sdus;     ///< Number of dropped SDUs (due to full queue)
  uint32_t num_discarded_sdus;   ///< Number of discarded SDUs (instructed from higher layer)
  uint32_t num_discard_failures; ///< Number of failed SDU discards (instructed from higher layer)

  // PDU metrics
  uint32_t num_pdus_no_segmentation;      ///< Number of transmitted PDUs without segmentation
  size_t   num_pdu_bytes_no_segmentation; ///< Number of transmitted PDU bytes without segmentation

  /// RLC mode of the entity
  rlc_mode mode;

  /// Mode-specific metrics
  ///
  /// The associated union member is indicated by \c mode.
  /// Contents of the other fields are undefined.
  union {
    rlc_tm_tx_metrics tm;
    rlc_um_tx_metrics um;
    rlc_am_tx_metrics am;
  } mode_specific;
};

class rlc_tx_metrics_interface
{
public:
  rlc_tx_metrics_interface()                                            = default;
  virtual ~rlc_tx_metrics_interface()                                   = default;
  rlc_tx_metrics_interface(const rlc_tx_metrics_interface&)             = delete;
  rlc_tx_metrics_interface& operator=(const rlc_tx_metrics_interface&)  = delete;
  rlc_tx_metrics_interface(const rlc_tx_metrics_interface&&)            = delete;
  rlc_tx_metrics_interface& operator=(const rlc_tx_metrics_interface&&) = delete;

  virtual rlc_tx_metrics get_metrics()           = 0;
  virtual rlc_tx_metrics get_and_reset_metrics() = 0;
  virtual void           reset_metrics()         = 0;
};
} // namespace srsran

namespace fmt {

// RLC TX metrics formatter
// TODO print mode-specific metrics
template <>
struct formatter<srsran::rlc_tx_metrics> {
  template <typename ParseContext>
  auto parse(ParseContext& ctx) -> decltype(ctx.begin())
  {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(srsran::rlc_tx_metrics m, FormatContext& ctx) -> decltype(std::declval<FormatContext>().out())
  {
    return format_to(ctx.out(),
                     "num_sdus={} num_sdu_bytes={} num_dropped_sdus={} num_discarded_sdus={} "
                     "num_discard_failures={} num_pdus_no_segm={} num_pdu_bytes_no_segm={}",
                     m.num_sdus,
                     m.num_sdu_bytes,
                     m.num_dropped_sdus,
                     m.num_discarded_sdus,
                     m.num_discard_failures,
                     m.num_pdus_no_segmentation,
                     m.num_pdu_bytes_no_segmentation);
  }
};
} // namespace fmt
