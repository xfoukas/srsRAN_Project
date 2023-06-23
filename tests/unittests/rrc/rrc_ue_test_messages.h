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

#include "srsran/asn1/rrc_nr/msg_common.h"
#include "srsran/e1ap/cu_cp/e1ap_cu_cp.h"

namespace srsran {
namespace srs_cu_cp {

/// Converts a hex string (e.g. 01FA02) to a sec_as_key.
security::sec_key make_sec_key(std::string hex_str);

/// Converts a hex string (e.g. 01FA02) to a sec_128_as_key.
security::sec_128_key make_sec_128_key(std::string hex_str);

/// \brief Generates a dummy meas config
cu_cp_meas_cfg generate_dummy_meas_config();

/// \brief Constructs full RRC Reconfig request with radioBearerConfig, measConfig, masterCellGroup and NAS PDU
cu_cp_rrc_reconfiguration_procedure_request generate_rrc_reconfiguration_procedure_request();

/// \brief Generate RRC Container with invalid RRC Reestablishment Request.
byte_buffer generate_invalid_rrc_reestablishment_request_pdu(pci_t pci, rnti_t c_rnti);

/// \brief Generate RRC Container with valid RRC Reestablishment Request.
byte_buffer generate_valid_rrc_reestablishment_request_pdu(pci_t pci, rnti_t c_rnti);

/// \brief Generate RRC Container with RRC Reestablishment Complete.
byte_buffer generate_rrc_reestablishment_complete_pdu();

/// \brief Generate RRC Container with Measurement Report
byte_buffer generate_measurement_report_pdu();

} // namespace srs_cu_cp
} // namespace srsran
