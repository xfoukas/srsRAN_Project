/*
 *
 * Copyright 2013-2022 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#pragma once

#include "gtpu_pdu.h"
#include "srsgnb/adt/byte_buffer.h"
#include "srsgnb/gtpu/gtpu_config.h"
#include "srsgnb/gtpu/gtpu_tunnel_tx.h"
#include <cstdint>

namespace srsgnb {

/// Class used for transmitting GTP-U bearers.
class gtpu_tunnel_tx : public gtpu_tunnel_tx_lower_layer_interface
{
public:
  gtpu_tunnel_tx(gtpu_config::gtpu_tx_config cfg_, gtpu_tunnel_tx_upper_layer_notifier& upper_dn_) :
    logger(srslog::fetch_basic_logger("GTPU")), cfg(cfg_), upper_dn(upper_dn_)
  {
    // Validate configuration
    logger.info("GTPU TX entity configured. Configuration={}", cfg);
  }

  /*
   * SDU/PDU handlers
   */
  void handle_sdu(byte_buffer buf) final
  {
    gtpu_header hdr         = {};
    hdr.flags.version       = GTPU_FLAGS_VERSION_V1;
    hdr.flags.protocol_type = GTPU_FLAGS_GTP_PROTOCOL;
    hdr.message_type        = GTPU_MSG_DATA_PDU;
    hdr.length              = buf.length();
    hdr.teid                = cfg.peer_teid;
    bool write_ok           = gtpu_write_header(buf, hdr, logger);
    if (!write_ok) {
      logger.error("Error writing GTP-U header, discarding.");
      return;
    }
    logger.debug(buf.begin(), buf.end(), "TX GTP-U SDU");
    upper_dn.on_new_pdu(std::move(buf), cfg.peer_addr);
  }

private:
  srslog::basic_logger&                logger;
  const gtpu_config::gtpu_tx_config    cfg;
  gtpu_tunnel_tx_upper_layer_notifier& upper_dn;
};
} // namespace srsgnb
