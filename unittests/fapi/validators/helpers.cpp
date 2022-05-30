/*
 *
 * Copyright 2013-2022 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#include "helpers.h"
#include "srsgnb/fapi/message_builders.h"
#include <random>

static std::mt19937 gen(0);
using namespace srsgnb;
using namespace fapi;
using namespace unittest;

static unsigned generate_sfn()
{
  std::uniform_int_distribution<unsigned> dist(0, 1023);
  return dist(gen);
}

static unsigned generate_slot()
{
  std::uniform_int_distribution<unsigned> dist(0, 159);

  return dist(gen);
}

static rnti_t generate_rnti()
{
  std::uniform_int_distribution<unsigned> dist(1, 65535);

  return to_rnti(dist(gen));
}

static unsigned generate_rapid()
{
  std::uniform_int_distribution<unsigned> dist(0, 128);
  unsigned                                value = dist(gen);

  return (value < 64U) ? value : 255U;
}

static unsigned generate_harq()
{
  std::uniform_int_distribution<unsigned> dist(0, 15);

  return dist(gen);
}

static unsigned generate_handle()
{
  std::uniform_int_distribution<unsigned> dist(0, std::numeric_limits<uint32_t>::max());

  return dist(gen);
}

/// Returns random values for the maintenance v3 basic parameters. Every parameter is within the range defined in
/// SCF-222 v4.0 Section 3.4.2.4.
static std::tuple<ssb_pattern_case, subcarrier_spacing, unsigned> generate_random_maintenance_v3_basic_params()
{
  std::uniform_int_distribution<unsigned> enum_dist(0, 4);

  static constexpr std::array<unsigned, 3> lmax{4U, 8U, 64U};
  std::uniform_int_distribution<unsigned>  lmax_dist(0, 2);

  return {static_cast<ssb_pattern_case>(enum_dist(gen)),
          static_cast<subcarrier_spacing>(enum_dist(gen)),
          lmax[lmax_dist(gen)]};
}

static pci_t generate_pci()
{
  std::uniform_int_distribution<unsigned> dist(0, 1007);

  return dist(gen);
}

dl_ssb_pdu unittest::build_valid_dl_ssb_pdu()
{
  dl_ssb_pdu                              pdu;
  dl_ssb_pdu_builder                      builder(pdu);
  std::uniform_int_distribution<unsigned> pci_dist(0, 1007);
  std::uniform_int_distribution<unsigned> binary_dist(0, 1);
  std::uniform_int_distribution<unsigned> block_index_dist(0, 63);
  std::uniform_int_distribution<unsigned> subcarrier_offset_dist(0, 31);
  std::uniform_int_distribution<unsigned> offset_pointA_dist(0, 2199);
  std::uniform_int_distribution<unsigned> sib1_dist(0, 255);
  std::uniform_real_distribution<>        power_dist(-30.8, 69.5);
  auto                                    pss_profile = static_cast<beta_pss_profile_type>(binary_dist(gen));
  builder.set_basic_parameters(
      pci_dist(gen), pss_profile, block_index_dist(gen), subcarrier_offset_dist(gen), offset_pointA_dist(gen));
  builder.set_bch_payload_phy_full(binary_dist(gen), sib1_dist(gen), binary_dist(gen), binary_dist(gen));
  const auto& v3 = generate_random_maintenance_v3_basic_params();
  builder.set_maintenance_v3_basic_parameters(std::get<0>(v3), std::get<1>(v3), std::get<2>(v3));
  // When the PSS to SSS NR profile is configured to use the field beta PSS Profile SSS then generate a random number,
  // otherwise leave it unset, as per SCF-222 Section 3.4.2.4.
  optional<float> beta_pss;
  if (pss_profile == beta_pss_profile_type::beta_pss_profile_sss) {
    beta_pss = power_dist(gen);
  }
  builder.set_maintenance_v3_tx_power_info(power_dist(gen), beta_pss);

  return pdu;
}

dl_pdcch_pdu unittest::build_valid_dl_pdcch_pdu()
{
  dl_pdcch_pdu         pdu;
  dl_pdcch_pdu_builder builder(pdu);

  // Random generators.
  std::uniform_int_distribution<unsigned> sfn_dist(0, 1023);
  std::uniform_int_distribution<unsigned> slot_dist(0, 159);
  std::uniform_int_distribution<unsigned> bwp_size_dist(1, 275);
  std::uniform_int_distribution<unsigned> bwp_start_dist(0, 274);
  std::uniform_int_distribution<unsigned> start_symbol_index_dist(0, 13);
  std::uniform_int_distribution<unsigned> duration_symbol_dist(1, 3);
  std::uniform_int_distribution<unsigned> shift_index_dist(0, 275);
  std::uniform_int_distribution<unsigned> n_rnti_dist(0, 65535);
  std::uniform_int_distribution<unsigned> cce_dist(0, 135);
  std::uniform_int_distribution<unsigned> aggregation_dist(0, 4);
  std::uniform_int_distribution<unsigned> nid_dmrs_dist(0, 65535);
  std::uniform_int_distribution<unsigned> nid_data_dist(0, 65535);
  std::uniform_int_distribution<unsigned> binary_dist(0, 1);
  std::uniform_int_distribution<int>      power_dist(-8, 8);
  std::uniform_int_distribution<unsigned> custom_dist(2, 3);

  // :TODO: generate  better frequency domain resource bitmap.
  static_vector<uint8_t, 6> freq_domain = {3, 2, 1, 4, 5, 1};

  // Always work with the biggest numerology.
  builder.set_bwp_parameters(bwp_size_dist(gen),
                             bwp_start_dist(gen),
                             subcarrier_spacing::kHz240,
                             static_cast<cyclic_prefix_type>(binary_dist(gen)));

  builder.set_coreset_parameters(start_symbol_index_dist(gen),
                                 duration_symbol_dist(gen),
                                 {freq_domain},
                                 static_cast<cce_to_reg_mapping_type>(binary_dist(gen)),
                                 custom_dist(gen),
                                 custom_dist(gen),
                                 static_cast<pdcch_coreset_type>(binary_dist(gen)),
                                 shift_index_dist(gen),
                                 static_cast<precoder_granularity_type>(binary_dist(gen)));

  // Add DCI.
  auto builder_dci = builder.add_dl_dci();

  builder_dci.set_basic_parameters(to_rnti(n_rnti_dist(gen) + 1),
                                   nid_data_dist(gen),
                                   n_rnti_dist(gen),
                                   cce_dist(gen),
                                   std::pow(2, aggregation_dist(gen)));
  optional<float> profile_nr;
  profile_nr.emplace(power_dist(gen));
  builder_dci.set_tx_power_info_parameter(profile_nr);

  // Payload.
  static_vector<uint8_t, 128> payload(32, 1);

  builder_dci.set_payload({payload});

  optional<float> profile_data;
  optional<float> profile_dmrs;
  builder_dci.set_maintenance_v3_dci_parameters(true, profile_dmrs, profile_data);
  builder_dci.set_parameters_v4_dci(nid_dmrs_dist(gen));

  return pdu;
}

dl_pdsch_pdu unittest::build_valid_dl_pdsch_pdu()
{
  static constexpr unsigned iterations = 10000;
  std::vector<unsigned>     results;
  results.reserve(iterations);

  // Random generators.
  std::uniform_int_distribution<unsigned> sfn_dist(0, 1023);
  std::uniform_int_distribution<unsigned> slot_dist(0, 159);
  std::uniform_int_distribution<unsigned> rnti_dist(1, 65535);
  std::uniform_int_distribution<unsigned> bwp_size_dist(1, 275);
  std::uniform_int_distribution<unsigned> bwp_start_dist(0, 274);
  std::uniform_int_distribution<unsigned> nid_pdsch_dist(0, 1023);
  std::uniform_int_distribution<unsigned> nof_layers_dist(1, 8);
  std::uniform_int_distribution<unsigned> dmrs_scrambling_dist(0, 65535);
  std::uniform_int_distribution<unsigned> binary_dist(0, 1);
  std::uniform_int_distribution<unsigned> dmrs_cdm_grps_no_data_dist(1, 3);
  std::uniform_int_distribution<unsigned> nr_of_symbols_dist(1, 14);
  std::uniform_int_distribution<unsigned> start_symbol_index_dist(0, 13);
  std::uniform_real_distribution<float>   power_dist(-32, 32.0);

  unsigned                       pdu_bitmap          = 0;
  cyclic_prefix_type             cyclic_p            = cyclic_prefix_type::normal;
  pdsch_ref_point_type           ref_point           = pdsch_ref_point_type::point_a;
  dmrs_config_type               config_type         = dmrs_config_type::type_1;
  pdsch_low_papr_dmrs_type       low_papr            = pdsch_low_papr_dmrs_type::independent_cdm_group;
  pdsch_resource_allocation_type resource_alloc      = pdsch_resource_allocation_type::type_0;
  pdsch_vrb_to_prb_mapping_type  vrb_prb_mapping     = pdsch_vrb_to_prb_mapping_type::non_interleaved;
  nzp_csi_rs_epre_to_ssb         power_ss_profile_nr = nzp_csi_rs_epre_to_ssb::dB_minus_3;
  int                            power_nr            = -7;
  int                            power               = -30;
  pdsch_trans_type               trasn_type          = pdsch_trans_type::non_interleaved_other;
  ldpc_base_graph_type           ldpc_graph          = ldpc_base_graph_type::bg_1;

  rnti_t   rnti                     = to_rnti(rnti_dist(gen));
  unsigned bwp_size                 = bwp_size_dist(gen);
  unsigned bwp_start                = bwp_start_dist(gen);
  unsigned nid_pdsch                = nid_pdsch_dist(gen);
  unsigned scrambling_id            = dmrs_scrambling_dist(gen);
  unsigned scrambling_id_complement = dmrs_scrambling_dist(gen);
  bool     n_scid                   = binary_dist(gen);
  unsigned dmrs_cdm_grps_no_data    = dmrs_cdm_grps_no_data_dist(gen);
  unsigned start_symbol_index       = start_symbol_index_dist(gen);
  unsigned nr_of_symbols            = nr_of_symbols_dist(gen);
  unsigned coreset_start            = bwp_size_dist(gen);
  unsigned initial_bwp_size         = bwp_size_dist(gen);
  unsigned tb_size_lbrm_bytes       = bwp_size_dist(gen);
  unsigned dl_dmrs_symbol           = rnti_dist(gen);
  unsigned rb_size                  = nr_of_symbols_dist(gen);
  unsigned rb_start                 = start_symbol_index_dist(gen);
  float    profile_sss              = power_dist(gen);

  std::array<uint8_t, 36> rb_bitmap = {};

  dl_pdsch_pdu         pdu;
  dl_pdsch_pdu_builder builder_pdsch(pdu);

  builder_pdsch.set_basic_parameters(pdu_bitmap & 1, (pdu_bitmap >> 1) & 1, rnti);

  // Always work with the biggest numerology.
  builder_pdsch.set_bwp_parameters(bwp_size, bwp_start, subcarrier_spacing::kHz240, cyclic_p);
  builder_pdsch.set_codeword_information_parameters(nid_pdsch, 1, 0, ref_point);
  builder_pdsch.set_dmrs_parameters(
      dl_dmrs_symbol, config_type, scrambling_id, scrambling_id_complement, low_papr, n_scid, dmrs_cdm_grps_no_data, 0);

  auto builder_cw = builder_pdsch.add_codeword();

  unsigned target_code = 2;
  unsigned qam_mod     = 2;
  unsigned mcs         = 20;
  unsigned mcs_table   = 1;
  unsigned rv_index    = 0;
  unsigned tb_size     = 42;

  builder_cw.set_basic_parameters(target_code, qam_mod, mcs, mcs_table, rv_index, tb_size);

  if (resource_alloc == pdsch_resource_allocation_type::type_0) {
    builder_pdsch.set_pdsch_allocation_in_frequency_type_0({rb_bitmap}, vrb_prb_mapping);
  } else {
    builder_pdsch.set_pdsch_allocation_in_frequency_type_1(rb_start, rb_size, vrb_prb_mapping);
  }

  builder_pdsch.set_pdsch_allocation_in_time_parameters(start_symbol_index, nr_of_symbols);

  optional<int>   profile_nr;
  optional<float> dmrs_profile;
  if (power_nr != -9) {
    profile_nr.emplace(power_nr);
  } else {
    dmrs_profile.emplace(profile_sss);
  }

  builder_pdsch.set_tx_power_info_parameters(profile_nr, power_ss_profile_nr);

  // :TODO: not filling CBG to retx control parameters.

  builder_pdsch.set_maintenance_v3_bwp_parameters(trasn_type, coreset_start, initial_bwp_size);
  builder_pdsch.set_maintenance_v3_codeword_parameters(
      ldpc_graph, tb_size_lbrm_bytes, pdu_bitmap & 1U, (pdu_bitmap >> 1) & 1U);

  optional<float> data_profile;
  if (power != -33) {
    data_profile.emplace(power);
  }

  builder_pdsch.set_maintenance_v3_tx_power_info_parameters(dmrs_profile, data_profile);

  std::vector<uint8_t> rm_vector;

  builder_pdsch.set_maintenance_v4_basic_parameters({rm_vector}, binary_dist(gen), {rm_vector});

  return pdu;
}

dl_csi_rs_pdu unittest::build_valid_dl_csi_pdu()
{
  dl_csi_rs_pdu         pdu;
  dl_csi_rs_pdu_builder builder(pdu);

  std::uniform_int_distribution<unsigned> nof_rb_dist(24, 276);
  std::uniform_int_distribution<unsigned> start_rb_dist(0, 274);
  std::uniform_int_distribution<unsigned> csi_type_dist(0, 2);
  std::uniform_int_distribution<unsigned> row_dist(1, 18);
  std::uniform_int_distribution<unsigned> symb_l0_dist(0, 13);
  std::uniform_int_distribution<unsigned> symb_l1_dist(2, 12);
  std::uniform_int_distribution<unsigned> cdm_type_dist(0, 3);
  std::uniform_int_distribution<unsigned> freq_density_dist(0, 3);
  std::uniform_int_distribution<unsigned> scram_id_dist(0, 1023);
  std::uniform_int_distribution<unsigned> binary_dist(0, 1);
  std::uniform_int_distribution<int>      power_dist(-8, 8);
  std::uniform_int_distribution<unsigned> ss_power_dist(0, 10);

  builder.set_basic_parameters(start_rb_dist(gen),
                               nof_rb_dist(gen),
                               static_cast<csi_type>(csi_type_dist(gen)),
                               row_dist(gen),
                               freq_density_dist(gen),
                               symb_l0_dist(gen),
                               symb_l1_dist(gen),
                               static_cast<csi_cdm_type>(cdm_type_dist(gen)),
                               static_cast<csi_freq_density_type>(freq_density_dist(gen)),
                               scram_id_dist(gen));

  // Always work with the biggest numerology.
  builder.set_bwp_parameters(subcarrier_spacing::kHz240, static_cast<cyclic_prefix_type>(binary_dist(gen)));

  optional<float> profile_nr;
  profile_nr.emplace(power_dist(gen));
  unsigned ss_power = ss_power_dist(gen);
  ss_power          = (ss_power > 3) ? 255 : ss_power;
  builder.set_tx_power_info_parameters(profile_nr, static_cast<nzp_csi_rs_epre_to_ssb>(ss_power));

  optional<float> profile_sss;
  builder.set_maintenance_v3_tx_power_info_parameters(profile_sss);

  return pdu;
}

dl_tti_request_message unittest::build_valid_dl_tti_request()
{
  dl_tti_request_message         msg;
  dl_tti_request_message_builder builder(msg);

  std::uniform_int_distribution<unsigned> sfn_dist(0, 1023);
  std::uniform_int_distribution<unsigned> slot_dist(0, 159);

  // :TODO: at this moment code does not support groups, the number of groups is set to 0.
  builder.set_basic_parameters(sfn_dist(gen), slot_dist(gen), 0);

  // Manually add the SSB PDU to reuse the functions above.
  msg.pdus.emplace_back();
  msg.pdus.back().pdu_type = dl_pdu_type::SSB;
  msg.pdus.back().ssb_pdu  = build_valid_dl_ssb_pdu();

  // Manually add the PDCCH PDU to reuse the functions above.
  msg.pdus.emplace_back();
  msg.pdus.back().pdu_type  = dl_pdu_type::PDCCH;
  msg.pdus.back().pdcch_pdu = build_valid_dl_pdcch_pdu();

  // Manually add the PDSCH PDU to reuse the functions above.
  msg.pdus.emplace_back();
  msg.pdus.back().pdu_type  = dl_pdu_type::PDSCH;
  msg.pdus.back().pdsch_pdu = build_valid_dl_pdsch_pdu();

  // Manually add the CSI PDU to reuse the functions above.
  msg.pdus.emplace_back();
  msg.pdus.back().pdu_type   = dl_pdu_type::CSI_RS;
  msg.pdus.back().csi_rs_pdu = build_valid_dl_csi_pdu();

  return msg;
}

ul_dci_request_message unittest::build_valid_ul_dci_request()
{
  ul_dci_request_message         msg;
  ul_dci_request_message_builder builder(msg);

  builder.set_basic_parameters(generate_sfn(), generate_slot());

  // Manually add the PDCCH PDU to reuse the functions above.ul
  msg.pdus.emplace_back();
  msg.pdus.back().pdu_type = ul_dci_pdu_type::PDCCH;
  msg.pdus.back().pdu      = build_valid_dl_pdcch_pdu();

  return msg;
}

slot_indication_message unittest::build_valid_slot_indication()
{
  slot_indication_message msg;
  msg.slot = generate_slot();
  msg.sfn  = generate_sfn();

  return msg;
}

error_indication_message unittest::build_valid_error_indication()
{
  error_indication_message msg;

  msg.slot          = generate_slot();
  msg.sfn           = generate_sfn();
  msg.message_id    = message_type_id::tx_data_request;
  msg.error_code    = error_code_id::msg_invalid_config;
  msg.expected_sfn  = std::numeric_limits<uint16_t>::max();
  msg.expected_slot = std::numeric_limits<uint16_t>::max();

  return msg;
}

error_indication_message unittest::build_valid_out_of_sync_error_indication()
{
  error_indication_message msg;

  msg.slot          = generate_slot();
  msg.sfn           = generate_sfn();
  msg.message_id    = message_type_id::tx_data_request;
  msg.error_code    = error_code_id::out_of_sync;
  msg.expected_sfn  = generate_sfn();
  msg.expected_slot = generate_slot();

  return msg;
}

rx_data_indication_message unittest::build_valid_rx_data_indication()
{
  rx_data_indication_message msg;

  msg.sfn            = generate_sfn();
  msg.slot           = generate_slot();
  msg.control_length = 0;

  msg.pdus.emplace_back();
  auto& pdu   = msg.pdus.back();
  pdu.handle  = generate_handle();
  pdu.rnti    = generate_rnti();
  pdu.rapid   = generate_rapid();
  pdu.harq_id = generate_harq();

  return msg;
}

static uint8_t generate_num_prach_ocas()
{
  std::uniform_int_distribution<unsigned> dist(1, 7);

  return dist(gen);
}

static uint8_t generate_index_fd_ra()
{
  std::uniform_int_distribution<unsigned> dist(0, 7);

  return dist(gen);
}

static uint8_t generate_prach_start_symbol()
{
  std::uniform_int_distribution<unsigned> dist(0, 13);

  return dist(gen);
}

static uint8_t generate_num_cs()
{
  std::uniform_int_distribution<unsigned> dist(0, 419);

  return dist(gen);
}

static uint8_t generate_is_msga_prach()
{
  std::uniform_int_distribution<unsigned> dist(0, 2);

  return dist(gen);
}

static prach_format_type generate_prach_format()
{
  std::uniform_int_distribution<unsigned> dist(0, 13);

  return static_cast<prach_format_type>(dist(gen));
}

static prach_config_scope_type generate_prach_config_scope()
{
  std::uniform_int_distribution<unsigned> dist(0, 1);

  return static_cast<prach_config_scope_type>(dist(gen));
}

static uint8_t generate_start_preamble()
{
  unsigned value = generate_num_cs();

  return (value < 64U) ? value : 255U;
}

static uint8_t generate_num_preambles_indices()
{
  std::uniform_int_distribution<unsigned> dist(1, 64);

  return dist(gen);
}

ul_prach_pdu unittest::build_valid_ul_prach_pdu()
{
  ul_prach_pdu pdu;

  pdu.phys_cell_id                = generate_pci();
  pdu.num_prach_ocas              = generate_num_prach_ocas();
  pdu.prach_format                = generate_prach_format();
  pdu.index_fd_ra                 = generate_index_fd_ra();
  pdu.prach_start_symbol          = generate_prach_start_symbol();
  pdu.num_cs                      = generate_num_cs();
  pdu.is_msg_a_prach              = generate_is_msga_prach();
  pdu.has_msg_a_pusch_beamforming = generate_is_msga_prach();

  // Maintenance v3 parameters.
  auto& v3                = pdu.maintenance_v3;
  v3.prach_config_scope   = generate_prach_config_scope();
  v3.num_fd_ra            = generate_index_fd_ra() + 1U;
  v3.start_preamble_index = generate_start_preamble();
  v3.num_preamble_indices = generate_num_preambles_indices();

  return pdu;
}
