﻿#ifndef SRSGNB_FAPI_MESSAGES_H
#define SRSGNB_FAPI_MESSAGES_H

#include <array>
#include <cstdint>

namespace srsgnb {

namespace fapi {

/// Message type IDs.
enum class message_type_id : uint16_t {
  param_request      = 0x00,
  param_response     = 0x01,
  config_request     = 0x02,
  config_response    = 0x03,
  start_request      = 0x04,
  stop_request       = 0x05,
  stop_indication    = 0x06,
  error_indication   = 0x07,
  dl_tti_request     = 0x80,
  dl_tti_response    = 0x8A,
  ul_tti_request     = 0x81,
  slot_indication    = 0x82,
  ul_dci_request     = 0x83,
  tx_data_request    = 0x84,
  rx_data_indication = 0x85,
  crc_indication     = 0x86,
  uci_indication     = 0x87,
  srs_indication     = 0x88,
  rach_indication    = 0x89
};

/// Main PHY API message header.
struct message_header {
  uint8_t num_messages;
  uint8_t handle;
  uint8_t padding[2];
};

/// Common PHY API message structure.
struct base_message {
  message_type_id message_type;
  uint8_t         padding[2];
  uint32_t        length;
};

/// Slot indication message.
struct slot_indication_message : public base_message {
  uint16_t sfn;
  uint16_t slot;
};

/// Maximum number of supported DCIs per slot.
// :TODO: fapi speaks about max dci per slots, but maybe this is better named max dcis per PDU?
static constexpr unsigned MAX_DCI_PER_SLOT = 1;

/// PDCCH PDU maintenance information added in FAPIv3.
struct dl_pdcch_pdu_maintenance_v3 {
  uint16_t pdcch_pdu_index;
  uint16_t obsolete;

  /// DCI-specific parameters.
  struct maintenance_info {
    uint16_t dci_index;
    uint8_t  collocated_AL16_candidate;
    int16_t  pdcch_dmrs_power_offset_profile_sss;
    int16_t  pdcch_data_power_offset_profile_sss;
  };

  std::array<maintenance_info, MAX_DCI_PER_SLOT> info;
};

/// PDCCH PDU parameters added in FAPIv4.
struct dl_pdcch_pdu_parameters_v4 {
  /// DCI-specific parameters.
  struct dci_params {
    uint16_t nid_pdcch_dmrs;
  };
  std::array<dci_params, MAX_DCI_PER_SLOT> params;

  //: TODO: spatial_stream_indices_present
  //: TODO: mu_mimo;
};

/// Cyclic prefix type IDs.
enum class cyclic_prefix_type : uint8_t { normal, extended };

/// Downlink DCI PDU configuration.
struct dl_dci_pdu {
  /// Maximum number of bytes supported by the DCI payload.
  static constexpr unsigned MAX_DCI_PAYLOAD_BYTES = 32;

  uint16_t rnti;
  uint16_t nid_pdcch_data;
  uint16_t nrnti_pdcch_data;
  uint8_t  cce_index;
  uint8_t  aggregation_level;
  //: TODO: beamforming info
  uint8_t                                    beta_pdcch_1_0;
  int8_t                                     power_control_offset_ss_profile_nr;
  uint16_t                                   payload_size_bits;
  std::array<uint8_t, MAX_DCI_PAYLOAD_BYTES> payload;
};

/// Downlink PDCCH PDU information.
struct dl_pdcch_pdu {
  uint16_t                                 coreset_bwp_size;
  uint16_t                                 coreset_bwp_start;
  uint8_t                                  subcarrier_spacing;
  cyclic_prefix_type                       cyclic_prefix;
  uint8_t                                  start_symbol_index;
  uint8_t                                  duration_symbols;
  std::array<uint8_t, 6>                   freq_domain_resource;
  uint8_t                                  cce_reg_mapping_type;
  uint8_t                                  reg_bundle_size;
  uint8_t                                  interleaver_size;
  uint8_t                                  core_set_type;
  uint16_t                                 shift_index;
  uint8_t                                  precoder_granularity;
  uint16_t                                 num_dl_dci;
  std::array<dl_dci_pdu, MAX_DCI_PER_SLOT> dl_dci;
  dl_pdcch_pdu_maintenance_v3              maintenance_v3;
  dl_pdcch_pdu_parameters_v4               parameters_v4;
};

/// \note For this release num_coreset_rm_patterns = 0.
/// \note For this release num_prb_sym_rm_patts_by_value = 0.
struct dl_pdsch_maintenance_parameters_v3 {
  uint8_t                 pdsch_trans_type;
  uint16_t                coreset_start_point;
  uint16_t                initial_dl_bwp_size;
  uint8_t                 ldpc_base_graph;
  uint32_t                tb_size_lbrm_bytes;
  uint8_t                 tb_crc_required;
  std::array<uint16_t, 8> ssb_pdus_for_rate_matching;
  uint16_t                ssb_config_for_rate_matching;
  uint8_t                 prb_sym_rm_patt_bmp_size_byref;
  //: TODO: determine max size of this array
  std::array<uint8_t, 16> prb_sym_rm_patt_bmp_byref;
  uint8_t                 num_prb_sym_rm_patts_by_value;
  uint8_t                 num_coreset_rm_patterns;
  uint16_t                pdcch_pdu_index;
  uint16_t                dci_index;
  uint8_t                 lte_crs_rm_pattern_bmp_size;
  //: TODO: determine max size of this array
  std::array<uint8_t, 16> lte_crs_rm_pattern;
  uint8_t                 num_csi_rs_for_rm;
  //: TODO: determine max size of this array
  std::array<uint16_t, 16> csi_for_rm;
  int16_t                  pdsch_dmrs_power_offset_profile_sss;
  int16_t                  pdsch_data_power_offset_profile_sss;
  uint8_t                  max_num_cbg_per_tb;
  //: TODO: determine max size of this array.
  std::array<uint8_t, 16> cbg_tx_information;
};

/// PDSCH PTRS maintenance parameters added in FAPIv3.
struct dl_pdsch_ptrs_maintenance_v3 {
  int16_t pdsch_ptrs_power_offset_profile_sss;
};

struct dl_pdsch_parameters_v4 {
  uint8_t coreset_rm_pattern_bmp_size_by_ref;
  //: TODO: determine max size of this array
  std::array<uint8_t, 16> coreset_rm_pattern_bmp_by_ref;
  uint8_t                 lte_crs_mbsfn_derivation_method;
  //: TODO: determine max size of this array
  std::array<uint8_t, 16> lte_crs_mbsfn_pattern;
  //: TODO: MU-MIMO fields
};

/// Codeword information.
struct dl_pdsch_codeword {
  uint16_t target_code_rate;
  uint8_t  qam_mod_order;
  uint8_t  mcs_index;
  uint8_t  mcs_table;
  uint8_t  rv_index;
  uint32_t tb_size;
};

/// Downlink PDSCH PDU information.
struct dl_pdsch_pdu {
  /// Maximum number of codewords per PDU.
  static constexpr unsigned MAX_NUM_CW_PER_PDU = 2;

  uint16_t                                          pdu_bitmap;
  uint16_t                                          rnti;
  uint16_t                                          pdu_index;
  uint16_t                                          bwp_size;
  uint16_t                                          bwp_start;
  uint8_t                                           subcarrier_spacing;
  cyclic_prefix_type                                cyclic_prefix;
  uint8_t                                           num_codewords;
  std::array<dl_pdsch_codeword, MAX_NUM_CW_PER_PDU> cws;
  uint16_t                                          nid_pdsch;
  uint8_t                                           num_layers;
  uint8_t                                           transmission_scheme;
  uint8_t                                           ref_point;
  uint16_t                                          dl_dmrs_symb_pos;
  uint8_t                                           dmrs_config_type;
  uint16_t                                          pdsch_dmrs_scrambling_id;
  uint16_t                                          pdsch_dmrs_scrambling_id_compl;
  uint8_t                                           low_papr_dmrs;
  uint8_t                                           nscid;
  uint8_t                                           num_dmrs_cdm_grps_no_data;
  uint16_t                                          dmrs_ports;
  uint8_t                                           resource_alloc;
  std::array<uint8_t, 36>                           rb_bitmap;
  uint16_t                                          rb_start;
  uint16_t                                          rb_size;
  uint8_t                                           vrb_to_prb_mapping;
  uint8_t                                           start_symbol_index;
  uint8_t                                           nr_of_symbols;
  //: TODO: PTRS
  //: TODO: beamforming
  uint8_t                            power_control_offset_profile_nr;
  uint8_t                            power_control_offset_ss_profile_nr;
  uint8_t                            is_last_cb_present;
  uint8_t                            is_inline_tb_crc;
  std::array<uint32_t, 2>            dl_tb_crc_cw;
  dl_pdsch_maintenance_parameters_v3 pdsch_maintenance_v3;
  dl_pdsch_ptrs_maintenance_v3       ptrs_maintenance_v3;
  //: TODO: Rel16 PDSCH params v3
  dl_pdsch_parameters_v4 pdsch_parameters_v4;
};

/// CSI-RS maintenance parameters added in FAPIv3.
struct dl_csi_rs_maintenance_v3 {
  uint16_t csi_rs_pdu_index;
  int16_t  csi_rs_power_offset_profile_sss;
};

/// Downlink CSI-RS PDU information.
struct dl_csi_rs_pdu {
  uint8_t            subcarrier_spacing;
  cyclic_prefix_type cyclic_prefix;
  uint16_t           start_rb;
  uint16_t           num_rbs;
  uint8_t            csi_type;
  uint8_t            row;
  uint16_t           freq_domain;
  uint8_t            symb_L0;
  uint8_t            symb_L1;
  uint8_t            cdm_type;
  uint8_t            freq_density;
  uint16_t           scramb_id;
  uint8_t            power_control_offset_profile_nr;
  uint8_t            power_control_offset_ss_profile_nr;
  //: TODO: beamforming struct
  dl_csi_rs_maintenance_v3 csi_rs_maintenance_v3;
  //: TODO: csi params v4
};

/// PHY generated MIB PDU information.
struct dl_ssb_phy_mib_pdu {
  uint8_t dmrs_typeA_position;
  uint8_t pdcch_config_sib1;
  uint8_t cell_barred;
  uint8_t intrafreq_reselection;
};

/// BCH payload information.
struct dl_ssb_bch_payload {
  union {
    uint32_t           bch_payload;
    dl_ssb_phy_mib_pdu phy_mib_pdu;
  };
};

/// Encodes the first symbol mapping for candidate SSB locations [3GPP TS 38.213 [4], sec 4.1].
enum class ssb_case_type : uint8_t { case_a, case_b, case_c, case_d, case_e };

/// SSB/PBCH maintenance parameters added in FAPIv3.
struct dl_ssb_maintenance_v3 {
  uint8_t       ssb_pdu_index;
  ssb_case_type case_type;
  uint8_t       subcarrier_spacing;
  uint8_t       lmax;
  int16_t       ss_pbch_block_power_scaling;
  int16_t       beta_pss_profile_sss;
};

enum class beta_pss_profile_type : uint8_t { dB_0 = 0, dB_3 = 1, beta_pss_profile_sss = 255 };

enum class bch_payload_type : uint8_t { mac_full, phy_timing, phy_full };

/// Downlink SSB PDU information.
struct dl_ssb_pdu {
  uint16_t              phys_cell_id;
  beta_pss_profile_type beta_pss_profile_nr;
  uint8_t               ssb_block_index;
  uint8_t               ssb_subcarrier_offset;
  uint16_t              ssb_offset_pointA;
  bch_payload_type      bch_payload_flag;
  dl_ssb_bch_payload    bch_payload;
  //: TODO: beamforming
  dl_ssb_maintenance_v3 ssb_maintenance_v3;
  //: TODO: params v4 - MU-MIMO
};

/// Downlink PDU type ID.
enum class dl_pdu_type : uint16_t { PDCCH, PDSCH, CSI_RS, SSB };

/// Common downlink PDU information.
struct dl_tti_request_pdu {
  dl_pdu_type pdu_type;
  uint16_t    pdu_size;
  union {
    dl_pdcch_pdu  pdcch_pdu;
    dl_pdsch_pdu  pdsch_pdu;
    dl_csi_rs_pdu csi_rs_pdu;
    dl_ssb_pdu    ssb_pdu;
  };
};

/// Downlink TTI request message.
struct dl_tti_request_message : public base_message {
  /// Maximum number of supported DL PDU types or DCIs supported in this release.
  static constexpr unsigned MAX_NUM_DL_TYPES = 5;
  /// Maximum number of supported PDUs in this message.
  static constexpr unsigned MAX_NUM_PDUS = 64;
  /// Maximum number of supported PDU groups.
  //: TODO: this value seems to be a capability in the phy see tags 9a,9b. we should keep this in sync
  static constexpr unsigned MAX_PDU_GROUPS = 16;

  uint16_t                                     sfn;
  uint16_t                                     slot;
  uint16_t                                     num_pdus;
  uint8_t                                      num_dl_types;
  std::array<uint16_t, MAX_NUM_DL_TYPES>       num_pdus_of_each_type;
  uint16_t                                     num_groups;
  std::array<dl_tti_request_pdu, MAX_NUM_PDUS> pdus;
  //: TODO: groups array
  //: TODO: top level rate match patterns
};

/// Downlink TTI response pdu information.
struct dl_tti_response_pdu {
  uint16_t pdu_index;
  uint8_t  cw;
  uint32_t tb_crc;
};

/// Downlink TTI response message.
struct dl_tti_response_message : public base_message {
  /// Maximum number of supported PDUs in this message.
  // :TODO: keep in sync with dl_tti_request_message
  static constexpr unsigned MAX_NUM_PDUS = 64;

  uint16_t                                      sfn;
  uint16_t                                      slot;
  uint16_t                                      num_pdus;
  std::array<dl_tti_response_pdu, MAX_NUM_PDUS> pdus;
};

/// Uplink PDU type ID.
enum class ul_pdu_type : uint16_t { PRACH, PUSCH, PUCCH, SRS, msg_a_PUSCH };

/// PRACH maintenance parameters added in FAPIv3.
struct ul_prach_maintenance_v3 {
  uint32_t handle;
  uint8_t  prach_config_scope;
  uint16_t prach_res_config_index;
  uint8_t  num_fd_ra;
  uint8_t  start_preamble_index;
  uint8_t  num_preamble_indices;
};

/// Uplink PRACH PDU information.
struct ul_prach_pdu {
  uint16_t phys_cell_id;
  uint8_t  num_prach_ocas;
  uint8_t  prach_format;
  uint8_t  index_fd_ra;
  uint8_t  prach_start_symbol;
  uint16_t num_cs;
  uint8_t  is_msg_a_prach;
  uint8_t  has_msg_a_pusch_beamforming;
  //: TODO: beamforming struct
  ul_prach_maintenance_v3 maintenance_v3;
  //: TODO: uplink spatial assignment struct
  //: TODO: msgA signalling in v4
  //: TODO: msgA pusch beamforming
};

/// Uplink PUSCH data information.
struct ul_pusch_data {
  uint8_t  rv_index;
  uint8_t  harq_process_id;
  uint8_t  new_data;
  uint32_t tb_size;
  uint16_t num_cb;
  //: TODO: determine size of this array
  std::array<uint8_t, 2> cb_present_and_position;
};

/// Uplink PUSCH UCI information.
struct ul_pusch_uci {
  uint16_t harq_ack_bit_length;
  uint16_t csi_part1_bit_length;
  uint16_t flags_csi_part2;
  uint8_t  alpha_scaling;
  uint8_t  beta_offset_harq_ack;
  uint8_t  beta_offset_csi1;
  uint8_t  beta_offset_csi2;
};

/// Uplink PUSCH PTRS information.
struct ul_pusch_ptrs {
  /// Per-port specific information.
  struct ptrs_port_info {
    uint16_t ptrs_port_index;
    uint8_t  ptrs_dmrs_port;
    uint8_t  ptrs_re_offset;
  };

  uint8_t                       num_ptrs_ports;
  std::array<ptrs_port_info, 2> port_info;
  uint8_t                       ptrs_time_density;
  uint8_t                       ptrs_freq_density;
  uint8_t                       ul_ptrs_power;
};

/// Uplink PUSCH DFTs-OFDM information.
struct ul_pusch_dfts_ofdm {
  uint8_t  low_papr_group_number;
  uint16_t low_papr_sequence_number;
  uint8_t  ul_ptrs_sample_density;
  uint8_t  ul_ptrs_time_density_transform_precoding;
};

/// PUSCH PDU maintenance information added in FAPIv3.
struct ul_pusch_maintenance_v3 {
  uint8_t  pusch_trans_type;
  uint16_t delta_bwp0_start_from_active_bwp;
  uint16_t initial_ul_bwp_size;
  uint8_t  group_or_sequence_hopping;
  uint16_t pusch_second_hop_prb;
  uint8_t  ldpc_base_graph;
  uint32_t tb_size_lbrm_bytes;
};

/// PUSCH PDU parameters added in FAPIv4.
struct ul_pusch_params_v4 {
  /// Maximum number of spatial streams.
  static constexpr unsigned MAX_NUM_SPATIAL_STREAMS = 64;

  uint8_t                                      cb_crc_status_request;
  uint32_t                                     srs_tx_ports;
  uint8_t                                      ul_tpmi_index;
  uint8_t                                      num_ul_spatial_streams_ports;
  std::array<uint8_t, MAX_NUM_SPATIAL_STREAMS> ul_spatial_stream_ports;
};

/// UCI information for determining UCI Part1 to Part2 correspondence.
struct uci_part1_to_part2_correspondence_v3 {
  /// Maximum number of part2 info.
  static constexpr unsigned MAX_NUM_PART2_INFO = 100;

  struct part2_info {
    uint16_t                priority;
    uint8_t                 num_part1_params;
    std::array<uint16_t, 4> param_offsets;
    std::array<uint8_t, 4>  param_sizes;
    uint16_t                part2_size_map_index;
    uint8_t                 part2_size_map_scope;
  };

  uint16_t                                   num_parts2s;
  std::array<part2_info, MAX_NUM_PART2_INFO> part2;
};

/// Uplink PUSCH PDU information.
struct ul_pusch_pdu {
  uint16_t                pdu_bitmap;
  uint16_t                rnti;
  uint32_t                handle;
  uint16_t                bwp_size;
  uint16_t                bwp_start;
  uint8_t                 subcarrier_spacing;
  cyclic_prefix_type      cyclic_prefix;
  uint16_t                target_code_rate;
  uint8_t                 qam_mod_order;
  uint8_t                 mcs_index;
  uint8_t                 mcs_table;
  uint8_t                 transform_precoding;
  uint16_t                nid_pusch;
  uint8_t                 num_layers;
  uint16_t                ul_dmrs_symb_pos;
  uint8_t                 dmrs_config_type;
  uint16_t                pusch_dmrs_scrambling_id;
  uint16_t                pusch_dmrs_scramblig_id_complement;
  uint8_t                 low_papr_dmrs;
  uint16_t                pusch_dmrs_identity;
  uint8_t                 nscid;
  uint8_t                 num_dmrs_cdm_grps_no_data;
  uint16_t                dmrs_ports;
  uint8_t                 resource_alloc;
  std::array<uint8_t, 36> rb_bitmap;
  uint16_t                rb_start;
  uint16_t                rb_size;
  uint8_t                 vrb_to_prb_mapping;
  uint8_t                 intra_slot_frequency_hopping;
  uint16_t                tx_direct_current_location;
  uint8_t                 uplink_frequency_shift_7p5kHz;
  uint8_t                 start_symbol_index;
  uint8_t                 nr_of_symbols;
  ul_pusch_data           pusch_data;
  ul_pusch_uci            pusch_uci;
  ul_pusch_ptrs           pusch_ptrs;
  ul_pusch_dfts_ofdm      pusch_ofdm;
  //: TODO: beamforming struct
  ul_pusch_maintenance_v3              pusch_maintenance_v3;
  ul_pusch_params_v4                   pusch_params_v4;
  uci_part1_to_part2_correspondence_v3 uci_correspondence;
};

/// PUCCH PDU maintenance information added in FAPIv3.
struct ul_pucch_maintenance_v3 {
  uint8_t max_code_rate;
  uint8_t ul_bwp_id;
};

/// Encodes PUCCH pdu.
struct ul_pucch_pdu {
  uint16_t           rnti;
  uint32_t           handle;
  uint16_t           bwp_size;
  uint16_t           bwp_start;
  uint8_t            subcarrier_spacing;
  cyclic_prefix_type cyclic_prefix;
  uint8_t            format_type;
  uint8_t            multi_slot_tx_indicator;
  uint8_t            pi2_bpsk;
  uint16_t           prb_start;
  uint16_t           prb_size;
  uint8_t            start_symbol_index;
  uint8_t            nr_of_symbols;
  uint8_t            intra_slot_frequency_hopping;
  uint16_t           second_hop_prb;
  uint8_t            pucch_group_hopping;
  uint8_t            reserved;
  uint16_t           nid_pucch_hopping;
  uint16_t           initial_cyclic_shift;
  uint16_t           nid_pucch_scrambling;
  uint8_t            time_domain_occ_index;
  uint8_t            pre_dft_occ_idx;
  uint8_t            pre_dft_occ_len;
  uint8_t            add_dmrs_flag;
  uint16_t           nid0_pucch_dmrs_scrambling;
  uint8_t            m0_pucch_dmrs_cyclic_shift;
  uint8_t            sr_bit_len;
  uint16_t           bit_len_harq;
  uint16_t           csi_part1_bit_length;
  //: TODO: beamforming struct
  ul_pucch_maintenance_v3              pucch_maintenance_v3;
  uci_part1_to_part2_correspondence_v3 uci_correspondence;
};

/// MsgA-PUSCH PDU.
struct ul_msg_a_pusch_pdu {
  uint16_t                pdu_bitmap;
  uint8_t                 prach_to_pru_map_type;
  uint32_t                prach_to_pru_map_indicator;
  uint16_t                msg_a_prach_sfn;
  uint16_t                msg_a_prach_slot;
  uint32_t                handle;
  uint16_t                bwp_size;
  uint16_t                bwp_start;
  uint8_t                 subcarrier_spacing;
  cyclic_prefix_type      cyclic_prefix;
  uint8_t                 msg_a_mcs;
  uint8_t                 transform_precoding;
  uint8_t                 n_id_msg_a_pusch;
  std::array<uint16_t, 2> pusch_dmrs_scrambling_ids;
  uint16_t                ul_dmrs_symb_pos;
  uint8_t                 dmrs_mapping_type;
  uint16_t                start_rb_pusch_ocas;
  uint8_t                 num_rb_pusch_ocas;
  uint8_t                 guard_band;
  uint8_t                 intra_slot_frequency_hopping;
  uint8_t                 intra_slot_frequency_hopping_bits;
  uint16_t                tx_direct_current_location;
  uint8_t                 uplink_frequency_shift_7p5kHz;
  uint8_t                 start_symb_id_pusch_ocas;
  uint8_t                 duration_pusch_ocas;
  uint8_t                 guard_period;
  uint32_t                tb_size;
};

/// SRS parameters in FAPIv4.
struct ul_srs_params_v4 {
  /// Maximum number of UL spatial stream ports.
  static constexpr unsigned MAX_NUM_UL_SPATIAL_STREAM_PORTS = 64;

  struct symbol_info {
    uint16_t srs_bandwidth_start;
    uint8_t  sequence_group;
    uint8_t  sequence_number;
  };

  uint16_t                                             srs_bandwidth_size;
  std::array<symbol_info, 4>                           sym_info;
  uint32_t                                             usage;
  std::array<uint8_t, 32>                              report_type;
  uint8_t                                              singular_value_representation;
  uint8_t                                              iq_representation;
  uint16_t                                             prg_size;
  uint8_t                                              num_total_ue_antennas;
  uint32_t                                             ue_antennas_in_this_srs_resource_set;
  uint32_t                                             sampled_ue_antennas;
  uint8_t                                              report_scope;
  uint8_t                                              num_ul_spatial_streams_ports;
  std::array<uint8_t, MAX_NUM_UL_SPATIAL_STREAM_PORTS> ul_spatial_stream_ports;
};

/// SRS PDU.
struct ul_srs_pdu {
  uint16_t           rnti;
  uint32_t           handle;
  uint16_t           bwp_size;
  uint16_t           bwp_start;
  uint8_t            subcarrier_spacing;
  cyclic_prefix_type cyclic_prefix;
  uint8_t            num_ant_ports;
  uint8_t            num_symbols;
  uint8_t            num_repetitions;
  uint8_t            time_start_position;
  uint8_t            config_index;
  uint16_t           sequence_id;
  uint8_t            bandwidth_index;
  uint8_t            comb_size;
  uint8_t            comb_offset;
  uint8_t            cyclic_shift;
  uint8_t            frequency_position;
  uint16_t           frequency_shift;
  uint8_t            frequency_hopping;
  uint8_t            group_or_sequence_hopping;
  uint8_t            resource_type;
  uint16_t           t_srs;
  uint16_t           t_offset;
  // :TODO: beamforming.
  ul_srs_params_v4 srs_params_v4;
};

/// Common uplink PDU information.
struct ul_tti_request_pdu {
  ul_pdu_type pdu_type;
  uint16_t    pdu_size;
  union {
    ul_prach_pdu       prach_pdu;
    ul_pusch_pdu       pusch_pdu;
    ul_pucch_pdu       pucch_pdu;
    ul_msg_a_pusch_pdu msg_a_pusch_pdu;
    ul_srs_pdu         srs_pdu;
  };
};

/// Uplink TTI request message.
struct ul_tti_request_message : public base_message {
  /// Maximum number of supported UL PDU types in this release.
  static constexpr unsigned MAX_NUM_UL_TYPES = 6;
  /// Maximum number of supported UL PDUs in this message.
  static constexpr unsigned MAX_NUM_UL_PDUS = 128;

  uint16_t                                        sfn;
  uint16_t                                        slot;
  uint16_t                                        num_pdus;
  uint8_t                                         num_ul_types;
  std::array<uint16_t, MAX_NUM_UL_TYPES>          num_pdus_of_each_type;
  uint16_t                                        num_groups;
  std::array<ul_tti_request_pdu, MAX_NUM_UL_PDUS> pdus;
  //: TODO: groups array
};

/// Uplink DCI PDU information.
struct ul_dci_pdu {
  uint16_t     pdu_type;
  uint16_t     pdu_size;
  dl_pdcch_pdu pdu;
};

/// Uplink DCI request message.
struct ul_dci_request_message : public base_message {
  /// Maximum number of supported UL PDU types in this release.
  static constexpr unsigned MAX_NUM_DL_TYPES = 2;
  /// Maximum number of supported UCI PDUs in this message.
  static constexpr unsigned MAX_NUM_UCI_PDUS = 128;

  uint16_t                                 sfn;
  uint16_t                                 slot;
  uint16_t                                 num_pdus;
  uint8_t                                  num_dl_types;
  std::array<uint16_t, MAX_NUM_DL_TYPES>   num_pdus_of_each_type;
  std::array<ul_dci_pdu, MAX_NUM_UCI_PDUS> pdus;
};

/// Encodes the generic information of a TLV.
/// \note Value parameter type is 32 bits or a multiple of number of bits indicated by pdschMacPduBitsAlignment
/// capability.
struct tlv_info {
  uint16_t tag;
  uint32_t length;
  uint32_t value;
};

/// Transmission data request PDU information.
struct tx_data_req_pdu {
  /// Maximum number of TLVs.
  static constexpr unsigned MAX_NUM_TLVS = 1024;

  uint32_t                           pdu_length;
  uint16_t                           pdu_index;
  uint8_t                            cw_index;
  uint32_t                           num_tlvs;
  std::array<tlv_info, MAX_NUM_TLVS> tlvs;
};

/// Transmission request message.
struct tx_data_request_message : public base_message {
  /// Maximum number of supported UCI PDUs in this message.
  static constexpr unsigned MAX_NUM_DL_PDUS_PER_SLOT = 128;

  uint16_t                                              sfn;
  uint16_t                                              slot;
  uint16_t                                              control_length;
  uint16_t                                              num_pdus;
  std::array<tx_data_req_pdu, MAX_NUM_DL_PDUS_PER_SLOT> pdus;
};

/// Reception data indication PDU information.
struct rx_data_indication_pdu {
  uint32_t handle;
  uint16_t rnti;
  uint8_t  RAPID;
  uint8_t  harq_id;
  uint32_t pdu_length;
  uint8_t  pdu_tag;
  //: TODO: non-conformant, revise
  void* data;
};

/// Reception data indication message.
struct rx_data_indication_message : public base_message {
  /// Maximum number of supported UCI PDUs in this message.
  static constexpr unsigned MAX_NUM_ULSCH_PDUS_PER_SLOT = 64;

  uint16_t                                                        sfn;
  uint16_t                                                        slot;
  uint16_t                                                        control_length;
  uint16_t                                                        number_of_pdus;
  std::array<rx_data_indication_pdu, MAX_NUM_ULSCH_PDUS_PER_SLOT> pdus;
};

/// Reception data indication PDU information.
struct crc_ind_pdu {
  /// Maximum number of supported CBs in this message.
  static constexpr unsigned MAX_NUM_CB_PER_TTI_BYTES = 192;

  uint32_t                                      handle;
  uint16_t                                      rnti;
  uint8_t                                       RAPID;
  uint8_t                                       harq_id;
  uint8_t                                       tb_crc_status;
  uint16_t                                      num_cb;
  std::array<uint8_t, MAX_NUM_CB_PER_TTI_BYTES> cb_crc_status;
  int16_t                                       ul_sinr_metric;
  uint16_t                                      timing_advance_offset;
  int16_t                                       timing_advance_offset_ns;
  uint16_t                                      rssi;
  uint16_t                                      rsrp;
};

/// CRC indication message.
struct crc_indication_message : public base_message {
  /// Maximum number of supported CRCs in this message.
  static constexpr unsigned MAX_NUM_CRCS_PER_SLOT = 32;

  uint16_t                                       sfn;
  uint16_t                                       slot;
  uint16_t                                       num_crcs;
  std::array<crc_ind_pdu, MAX_NUM_CRCS_PER_SLOT> pdus;
};

/// UCI CSI part1 information.
struct uci_csi_part1 {
  /// Maximum number of supported CSI part 1 bytes in this message.
  static constexpr unsigned MAX_CSI_PART1_LEN = 214;

  uint8_t                                csi_part1_detection_status;
  uint16_t                               expected_csi_part1_bitlen;
  std::array<uint8_t, MAX_CSI_PART1_LEN> csi_part1_payload;
};

/// UCI CSI part2 information.
struct uci_csi_part2 {
  /// Maximum number of supported CSI part 2 bytes in this message.
  static constexpr unsigned MAX_CSI_PART2_LEN = 214;

  uint8_t                                csi_part2_detection_status;
  uint16_t                               expected_csi_part2_bitlen;
  std::array<uint8_t, MAX_CSI_PART2_LEN> csi_part2_payload;
};

/// UCI HARQ PDU information.
struct uci_harq_pdu {
  /// Maximum number of supported bytes in this message.
  static constexpr unsigned MAX_UCI_HARQ_LEN = 214;

  uint8_t                               harq_detection_status;
  uint16_t                              expected_harq_bitlen;
  std::array<uint8_t, MAX_UCI_HARQ_LEN> harq_payload;
};

/// PUSCH UCI PDU information.
struct uci_pusch_pdu {
  uint8_t       pdu_bitmap;
  uint32_t      handle;
  uint16_t      rnti;
  int16_t       ul_sinr_metric;
  uint16_t      timing_advance_offset;
  int16_t       timing_advance_offset_ns;
  uint16_t      rssi;
  uint16_t      rsrp;
  uci_harq_pdu  harq;
  uci_csi_part1 csi_part1;
  uci_csi_part2 csi_part2;
};

/// SR PDU for format 0 or 1.
struct sr_pdu_format_0_1 {
  uint8_t sr_indication;
  uint8_t sr_confidence_level;
};

/// UCI HARQ PDU for format 0 or 1.
struct uci_harq_format_0_1 {
  uint8_t                num_harq;
  uint8_t                harq_confidence_level;
  std::array<uint8_t, 2> harq_values;
};

/// UCI PUCCH for format 0 or 1.
struct uci_pucch_format_0_1 {
  uint8_t             pdu_bitmap;
  uint32_t            handle;
  uint16_t            rnti;
  uint8_t             pucch_format;
  int16_t             ul_sinr_metric;
  uint16_t            timing_advance_offset;
  int16_t             timing_advance_offset_ns;
  uint16_t            rssi;
  uint16_t            rsrp;
  sr_pdu_format_0_1   sr;
  uci_harq_format_0_1 harq;
};

/// SR PDU for format 2, 3, or 4.
struct sr_pdu_format_2_3_4 {
  /// Maximum number of supported bytes in this message.
  static constexpr unsigned MAX_SR_PAYLOAD_SIZE = 1;

  uint16_t                                 sr_bitlen;
  std::array<uint8_t, MAX_SR_PAYLOAD_SIZE> sr_payload;
};

/// UCI payload for PUSCH or PUCCH.
struct uci_payload_pusch_pucch {
  /// Maximum number of supported bytes in this message.
  static constexpr unsigned MAX_UCI_PAYLOAD_LEN = 214;

  uint8_t                                  uci_detection_status;
  uint16_t                                 expected_uci_payload_size;
  std::array<uint8_t, MAX_UCI_PAYLOAD_LEN> uci_payload;
};

/// UCI PUCCH for format 2, 3, or 4.
struct uci_pucch_format_2_3_4 {
  uint8_t                 pdu_bitmap;
  uint32_t                handle;
  uint16_t                rnti;
  uint8_t                 pucch_format;
  int16_t                 ul_sinr_metric;
  uint16_t                timing_advance_offset;
  int16_t                 timing_advance_offset_ns;
  uint16_t                rssi;
  uint16_t                rsrp;
  sr_pdu_format_2_3_4     sr;
  uci_harq_pdu            harq;
  uci_csi_part1           csi_part1;
  uci_csi_part2           csi_part2;
  uci_payload_pusch_pucch uci_part1;
  uci_payload_pusch_pucch uci_part2;
};

/// Reception data indication PDU information.
struct uci_indication_pdu {
  uint16_t pdu_type;
  uint16_t pdu_size;
  union {
    uci_pusch_pdu          pusch_pdu;
    uci_pucch_format_0_1   pucch_pdu_f01;
    uci_pucch_format_2_3_4 pucch_pdu_f234;
  };
};

/// UCI indication message.
struct uci_indication_message : public base_message {
  /// Maximum number of supported UCI PDUs in this message.
  //: TODO: shared with ul_dci_request_message
  static constexpr unsigned MAX_NUM_UCI_PDUS = 128;

  uint16_t                                         sfn;
  uint16_t                                         slot;
  uint16_t                                         num_ucis;
  std::array<uci_indication_pdu, MAX_NUM_UCI_PDUS> pdus;
};

/// Encodes the PRGs.
struct srs_prg {
  uint8_t rb_snr;
};

/// Encodes the reported symbols for the beamforming report.
struct srs_reported_symbols {
  /// Maximum number of PRGs.
  static constexpr unsigned MAX_NUM_PRGS = 273;

  uint16_t                          num_prg;
  std::array<srs_prg, MAX_NUM_PRGS> prgs;
};

/// Encodes the beamforming report.
struct srs_beamforming_report {
  /// Maximum number of reported symbols.
  static constexpr unsigned MAX_NUM_REP_SYMBOLS = 4;

  uint16_t                                              prg_size;
  uint8_t                                               num_symbols;
  uint8_t                                               wideband_snr;
  uint8_t                                               num_reported_symbols;
  std::array<srs_reported_symbols, MAX_NUM_REP_SYMBOLS> rep_symbols;
};

/// Encodes the normalized IQ representation.
enum class normalized_iq_representation : uint8_t { normal, extended };

/// Encodes the normalized channel IQ matrix report.
struct srs_normalized_channel_iq_matrix {
  // :TODO: The real value is way too big to represent it ~4M.
  static constexpr unsigned MAX_MATRIX_H_SIZE = 1024;

  normalized_iq_representation           iq_representation;
  uint16_t                               num_gnb_antenna_elements;
  uint16_t                               num_ue_srs_ports;
  uint16_t                               prg_size;
  uint16_t                               num_prgs;
  std::array<uint8_t, MAX_MATRIX_H_SIZE> channel_matrix_h;
};

/// Encodes the SVD array for each PRG.
struct srs_svd_prg {
  static constexpr unsigned MAX_SIZE_LEFT_EIGENVECTOR  = 256;
  static constexpr unsigned MAX_SIZE_SINGULAR_MATRIX   = 32;
  static constexpr unsigned MAX_SIZE_RIGHT_EIGENVECTOR = 8736;

  std::array<uint8_t, MAX_SIZE_LEFT_EIGENVECTOR>  left_eigenvectors;
  std::array<uint8_t, MAX_SIZE_SINGULAR_MATRIX>   singular_matrix;
  std::array<uint8_t, MAX_SIZE_RIGHT_EIGENVECTOR> right_eigenvectors;
};

/// Encodes the normalized singular representation.
enum class normalized_singular_representation : uint8_t { normal, extended };

/// Encodes the channel svd representation.
struct srs_channel_svd_representation {
  /// Maximum number of PRGs.
  static constexpr unsigned NUM_MAX_PRG = 273;

  normalized_iq_representation         iq_representation;
  normalized_singular_representation   singular_representation;
  int8_t                               singular_value_scaling;
  uint16_t                             num_gnb_antenna_elements;
  uint8_t                              num_ue_srs_ports;
  uint16_t                             prg_size;
  uint16_t                             num_prgs;
  std::array<srs_svd_prg, NUM_MAX_PRG> svd_prg;
};

/// Encodes the usage of the srs.
enum class srs_usage_mode : uint8_t { beam_management, codebook, non_codebook, antenna_switching, reserved };

/// SRS indication pdu.
struct srs_indication_pdu {
  uint32_t       handle;
  uint16_t       rnti;
  uint16_t       timing_advance_offset;
  int16_t        timing_advance_offset_ns;
  srs_usage_mode srs_usage;
  uint8_t        report_type;
  tlv_info       report;
};

/// SRS indication message.
struct srs_indication_message : public base_message {
  /// Maximum number of supported SRS PDUs in this message.
  static constexpr unsigned MAX_NUM_SRS_PDUS = 32;

  uint16_t                                         sfn;
  uint16_t                                         slot;
  uint16_t                                         control_length;
  uint8_t                                          num_pdu;
  std::array<srs_indication_pdu, MAX_NUM_SRS_PDUS> pdus;
};

/// RACH indication pdu preamble.
struct rach_indication_pdu_preamble {
  uint8_t  preamble_index;
  uint16_t timing_advance_offset;
  uint32_t timing_advance_offset_ns;
  uint32_t preamble_pwr;
  uint8_t  preamble_snr;
};

/// RACH indication pdu.
struct rach_indication_pdu {
  /// Maximum number of supported preambles per slot.
  static constexpr unsigned MAX_NUM_PREAMBLES = 64;

  uint16_t                                                    handle;
  uint8_t                                                     symbol_index;
  uint8_t                                                     slot_index;
  uint8_t                                                     ra_index;
  uint16_t                                                    avg_rssi;
  uint16_t                                                    rsrp;
  uint8_t                                                     avg_snr;
  uint8_t                                                     num_preambles;
  std::array<rach_indication_pdu_preamble, MAX_NUM_PREAMBLES> preambles;
};

/// RACH indication message
struct rach_indication_message : public base_message {
  /// Maximum number of supported measurement PDUs in this message.
  static constexpr unsigned MAX_NUM_RACH_PDUS = 64;

  uint16_t                                           sfn;
  uint16_t                                           slot;
  uint8_t                                            num_pdu;
  std::array<rach_indication_pdu, MAX_NUM_RACH_PDUS> pdus;
};

/// Encodes the error codes.
enum class error_code_id : uint8_t {
  msg_ok                               = 0x0,
  msg_invalid_state                    = 0x1,
  msg_invalid_config                   = 0x2,
  out_of_sync                          = 0x3,
  msg_slot_err                         = 0x4,
  msg_bch_missing                      = 0x5,
  msg_invalid_sfn                      = 0x6,
  msg_ul_dci_err                       = 0x7,
  msg_tx_err                           = 0x8,
  msg_invalid_phy_id                   = 0x9,
  msg_uninstantiated_phy               = 0xa,
  msg_invalid_dfe_profile              = 0xb,
  phy_profile_incompatible_running_phy = 0xc
};

/// Encodes the optional and mandatory status for the TLVs.
enum class tlv_config_status : uint8_t {
  idle_optional,
  idle_mandatory,
  idle_and_configured_optionals,
  idle_mandatory_configured_optional,
  idle_and_configured_and_running_optionals,
  idle_mandatory_configured_and_running_optionals
};

/// Encodes the tlv configuration report.
struct config_tlv_report {
  uint16_t          tag;
  uint8_t           length;
  tlv_config_status status;
};

/// Encodes the signal power profile.
struct signal_power_profile {
  uint8_t reference_rs;
  int16_t power_offset_min;
  int16_t power_offset_max;
};

/// Encodes the cell parameters.
struct cell_param {
  /// Maximum number of config TLVs to report
  static constexpr unsigned MAX_TLVS_TO_REPORT = 1505;

  uint16_t                                          release_capability;
  uint16_t                                          phy_state;
  uint8_t                                           skip_blank_dl_config;
  uint8_t                                           skip_blank_ul_config;
  uint16_t                                          num_config_tlvs_to_report;
  std::array<config_tlv_report, MAX_TLVS_TO_REPORT> config_tlv;
  uint8_t                                           power_profiles_supported;
  std::array<signal_power_profile, 8>               signal_profile;
  uint8_t                                           ss_pbch_block_power_scaling_unit_choice;
  uint16_t                                          max_num_pdu_dl_tti;
  uint16_t                                          max_num_pdu_ul_tti;
  uint16_t                                          max_num_pdu_ul_dci;
};

/// Encodes the carrier parameters.
struct carrier_param {
  uint8_t  cyclic_prefix;
  uint8_t  supported_subcarrier_spacings_dl;
  uint8_t  supported_subcarrier_spacings_ssb;
  uint16_t supported_bandwidth_dl;
  uint8_t  supported_subcarrier_spacings_ul;
  uint16_t supported_bandwidth_ul;
  uint8_t  ss_pbch_multiple_carriers_in_a_band;
  uint8_t  multiple_cells_ss_pbch_in_a_carrier;
  uint8_t  meaning_of_carrier_frequency;
};

/// Encodes the pdcch parameters.
struct pdcch_param {
  uint8_t cce_mapping_type;
  uint8_t coreset_outside_first_3_ofdm_syms_of_slot;
  uint8_t precoder_granularity_coreset;
  uint8_t pdcch_mu_mimo;
  uint8_t pdcch_precoder_cycling;
  uint8_t max_pdcchs_per_slot;
};

/// Encodes the modes for the signaling mechanism of PUCCH group and Sequency hopping.
enum class pucch_signaling_modes : uint8_t { dynamic, semi_static, both };

/// Encodes the pucch parameters.
struct pucch_param {
  uint8_t               pucch_formats;
  uint8_t               max_pucchs_per_slot;
  pucch_signaling_modes pucch_group_and_sequence_hopping;
  uint8_t               max_num_ul_bwp_ids;
  uint8_t               pucch_aggregation;
};

/// Encodes the modulation order di.
enum class modulation_order_dl : uint8_t { QPSK, QAM_16, QAM_64, QAM_256 };

/// Encodes the ssb rate match support.
enum class ssb_rate_match_type : uint8_t { not_supported, SSB_configuration, SSB_pdu_index, both };

/// Encodes the PDSCH parameters.
/// \note not adding numOfRateMAtchPatternLTECrsPerSlot, numOfRateMatchPatternLTEInPhy,
/// lteCrsRateMatchMbsfnDerivation, supportedLTECrsRateMatchMethod as they refer to LTE.
struct pdsch_param {
  uint8_t                 pdsch_mapping_type;
  uint8_t                 pdsch_allocation_types;
  uint8_t                 pdsch_vrb_to_prb_mapping;
  uint8_t                 pdsch_cbg;
  uint8_t                 pdsch_dmrs_config_types;
  uint8_t                 pdsch_dmrs_max_length;
  uint8_t                 pdsch_dmrs_additional_pos;
  uint8_t                 max_pdschs_tbs_per_slot;
  uint8_t                 max_num_mimo_layers_pdsch;
  modulation_order_dl     supported_max_modulation_order_dl;
  uint8_t                 max_mu_mimo_users_dl;
  uint8_t                 pdsch_data_in_dmrs_symbols;
  uint8_t                 preemption_support;
  uint8_t                 pdsch_non_slot_support;
  ssb_rate_match_type     ssb_rate_match;
  uint16_t                supported_rate_match_pattern_type_and_method;
  uint8_t                 pdcch_rate_match;
  uint8_t                 csi_rs_rate_match;
  uint8_t                 pdsch_trans_type_support;
  uint8_t                 pdsch_mac_pdu_bit_alignment;
  std::array<uint16_t, 5> max_number_prb_sym_bitmap_per_slot;
  std::array<uint16_t, 5> max_number_csi_rs_rate_match_per_slot;
  uint8_t                 max_num_ssb_per_pdsch_slot;
  uint8_t                 universal_rate_match;
  uint8_t                 require_pdsch_signaling_associations;
};

/// Encodes the modulation order ul.
enum class modulation_order_ul : uint8_t { QPSK, QAM_16, QAM_64, QAM_256 };

/// Encodes the PUSCH parameters.
struct pusch_param {
  uint8_t             uci_mux_ulsch_in_pusch;
  uint8_t             uci_only_pusch;
  uint8_t             pusch_frequency_hopping;
  uint8_t             pusch_dmrs_config_types;
  uint8_t             pusch_dmrs_max_len;
  uint8_t             pusch_dmrs_additional_pos;
  uint8_t             pusch_cbg;
  uint8_t             pusch_mapping_type;
  uint8_t             pusch_allocation_types;
  uint8_t             pusch_vrb_to_prb_mapping;
  uint8_t             pusch_max_ptrs_ports;
  uint8_t             pusch_max_puschs_tbs_per_slot;
  uint8_t             max_num_mimo_layers_non_cb_pusch;
  uint8_t             max_num_mimo_layers_cb_pusch;
  modulation_order_ul supported_max_modulation_order_ul;
  uint8_t             max_mu_mimo_users_ul;
  uint8_t             dfts_ofdm_support;
  uint8_t             pusch_aggregation_factor;
  uint8_t             pusch_lbrm_support;
  uint8_t             pusch_trans_type_support;
  uint8_t             pusch_mac_pdu_bit_alignment;
};

/// Encodes the MsgA-PUSCH parameters.
struct msg_a_pusch_param {
  uint16_t msg_a_num_pusch_res_config;
  uint8_t  msg_a_pusch_tran_precoding;
  uint16_t msg_a_intra_slot_pusch_freq_hop_h;
  uint8_t  msg_a_max_pusch_fd_occasions;
  uint8_t  msg_a_guard_band;
  uint8_t  msg_a_guard_period;
  uint8_t  msg_a_pusch_mapping_type;
  uint8_t  msg_a_pusch_dmrs_max_len;
  uint8_t  msg_a_pusch_dmrs_cdm_group;
  uint8_t  msg_a_pusch_dmrs_additional_pos;
  uint8_t  msg_a_supported_max_modulation_order_ul;
  uint8_t  msg_a_max_pusch_per_pru;
  uint8_t  msg_a_max_num_cdm_group;
  uint8_t  msg_a_max_num_dmrs_port;
  uint8_t  msg_a_prach_to_pusch_mapping_scheme;
};

/// Encodes the PRACH parameters.
struct prach_param {
  /// Maximum number RO format types.
  static constexpr unsigned MAX_NUM_RO_FORMATS = 16;

  uint8_t                                  prach_long_formats;
  uint16_t                                 prach_short_formats;
  uint8_t                                  prach_restricted_sets;
  uint8_t                                  max_prach_fd_occasions_in_a_slot;
  uint16_t                                 max_num_prach_configurations;
  uint8_t                                  prach_multiple_carriers_in_a_band;
  uint8_t                                  prach_frequency_offset_meaning;
  std::array<uint16_t, 5>                  max_num_td_fd_prach_occasions_per_slot;
  std::array<uint16_t, MAX_NUM_RO_FORMATS> prach_root_processing_rate;
  std::array<uint32_t, MAX_NUM_RO_FORMATS> prach_occasion_queue_size;
};

/// Encodes the MsgA-PRACH parameters.
struct msg_a_prach_param {
  uint16_t msg_a_max_num_prach_confs;
  uint8_t  msg_a_prach_long_formats;
  uint16_t msg_a_prach_short_formats;
  uint8_t  msg_a_prach_restricted_sets;
  uint8_t  msg_a_max_prach_fd_occasions_in_slot;
  uint8_t  msg_a_prach_multiple_carriers_in_a_band;
};

/// Encodes the Measurement reporting parameters.
struct measurement_param {
  uint8_t rssi_measurement_support;
  uint8_t rsrp_measurement_support;
};

/// Encodes the UCI parameters.
struct uci_param {
  uint16_t max_num_uci_maps;
  uint8_t  uci_report_format_pucch_format234;
};

/// Encodes the PHY parameters.
struct phy_param {
  /// Maximum number of downlink port ranges.
  static constexpr unsigned MAX_NUM_DL_PORT_RANGES = 64;
  /// Maximum number of uplink port ranges.
  static constexpr unsigned MAX_NUM_UL_PORT_RANGES = 64;

  // :TODO: Review the parameter DL BB Ports. It is not codified right now.
  uint8_t                                      num_dl_port_ranges;
  std::array<uint16_t, MAX_NUM_DL_PORT_RANGES> dl_port_range_start;
  std::array<uint16_t, MAX_NUM_DL_PORT_RANGES> dl_port_range_length;
  uint8_t                                      num_ul_port_ranges;
  std::array<uint16_t, MAX_NUM_UL_PORT_RANGES> ul_port_range_start;
  std::array<uint16_t, MAX_NUM_UL_PORT_RANGES> ul_port_range_length;
};

/// Encodes the PHY support profiles parameters.
struct phy_profiles_param {
  /// Maximum number of PHY IDs supported by the PHY profile.
  static constexpr unsigned MAX_NUM_PHY_IDS = 64;

  uint8_t                                max_num_phys;
  std::array<phy_param, MAX_NUM_PHY_IDS> phy;
};

/// Encodes the supported phy profiles. Only supported by PHY ID 0.
struct phy_profiles_supported {
  /// Maximum number of PHY profiles supported.
  static constexpr unsigned MAX_NUM_PHY_PROFILES = 64;

  uint16_t                                             num_phy_profiles;
  uint16_t                                             num_dl_bb_ports;
  uint16_t                                             num_ul_bb_ports;
  std::array<phy_profiles_param, MAX_NUM_PHY_PROFILES> phy_profiles;
};

/// Encodes the FAPI PHY protocol version.
struct phy_fapi_protocol {
  uint8_t phy_fapi_protocol_version;
  uint8_t phy_fapi_negotiated_protocol_version;
};

/// Encodes the PHY Support parameters.
struct phy_support_param {
  phy_profiles_supported phy_profiles;
  uint8_t                time_management;
  phy_fapi_protocol      fapi_version;
  std::array<uint8_t, 6> more_than_one_indication_per_slot;
  std::array<uint8_t, 4> more_than_one_request_per_slot;
};

/// Encodes the PHY/DFE profile validity parameters.
struct phy_dfe_profile_param {
  /// Maximum size for the Validity map in Bytes.
  static constexpr unsigned MAX_VALIDITY_MAP_SIZE_IN_BYTES = 8;

  uint16_t                                            num_phy_profiles;
  uint16_t                                            num_dfe_profiles;
  std::array<uint8_t, MAX_VALIDITY_MAP_SIZE_IN_BYTES> profile_validity;
};

/// Encodes the Delay management parameters.
struct delay_management_param {
  uint32_t dl_tti_timing_offset;
  uint32_t ul_tti_timing_offset;
  uint32_t ul_dci_timing_offset;
  uint32_t tx_data_timing_offset;
  uint16_t timing_window;
  uint8_t  timing_info_period;
};

/// Encodes user plane parameters.
struct user_plane_param {
  uint32_t user_plane_encapsulation_control_plane_separation;
  uint8_t  message_encoding_support;
};

/// Encodes the SRS parameters.
struct srs_param {
  uint32_t                 supported_srs_usage_and_pairing;
  std::array<uint16_t, 10> rb_subsampling_resolution_for_srs_derived_report;
  std::array<uint16_t, 10> max_srs_channel_reports_per_slot;
  uint16_t                 max_num_srs_pdus_per_slot;
  uint32_t                 iq_sample_formats;
  uint32_t                 singular_value_formats;
  std::array<uint8_t, 4>   max_srs_tx_ant_ports_per_ue;
  uint8_t                  max_num_consecutive_srs_slots;
  uint16_t                 max_duty_cycle_in_percentage;
  uint8_t                  max_srs_comb_size;
  uint8_t                  max_num_srs_cyclic_shifts;
  std::array<uint16_t, 2>  srs_symbols_bitmap;
  uint8_t                  max_num_srs_symbols_per_slot;
};

/// Encodes MIMO parameters.
struct mimo_param {
  std::array<uint16_t, 5> max_num_carriers_bw_layers_product_dl;
  std::array<uint16_t, 5> max_num_carriers_bw_layers_product_ul;
  std::array<uint16_t, 5> max_num_carriers_bw_antennas_product_dl;
  std::array<uint16_t, 5> max_num_carriers_bw_antennas_product_ul;
  uint32_t                mu_mimo_dl;
  std::array<uint32_t, 3> supported_spatial_mux_per_dl_channel_type;
  std::array<uint8_t, 3>  max_num_mu_mimo_layers_per_dl_channel_type;
  uint16_t                cross_channel_spatial_multiplexing_for_dl;
  uint32_t                mu_mimo_ul;
  std::array<uint32_t, 2> supported_spatial_mux_per_ul_channel_type;
  std::array<uint8_t, 2>  max_num_mu_mimo_layers_per_ul_channel_type;
  uint16_t                cross_channel_spatial_multiplexing_for_ul;
  std::array<uint16_t, 3> max_mu_mimo_groups_in_fd_per_dl_channel;
  std::array<uint8_t, 3>  max_mu_mimo_groups_in_td_per_dl_channel;
  uint16_t                max_mu_mimo_new_precoders_per_slot;
  uint8_t                 all_prbs_groups_prgs;
  std::array<uint16_t, 2> max_mu_mimo_groups_in_fd_per_ul_channel;
  std::array<uint8_t, 2>  max_mu_mimo_groups_in_td_per_ul_channel;
  uint16_t                min_rb_resolution_for_srs_derived_reports;
  uint16_t                max_number_dl_spatial_streams;
  uint32_t                dl_spatial_stream_channel_priority;
  uint16_t                max_number_ul_spatial_streams;
  uint8_t                 ul_tpmi_capability;
};

/// Encodes the validity for a capability.
struct capability_validity {
  uint16_t capability_tag;
  uint8_t  validity_scope;
};

/// Encodes the capability parameters.
struct capability_param {
  /// Maximum number of tag capabilities supported.
  static constexpr unsigned MAX_NUM_SUPPORTED_TAGS = 2048;

  uint16_t                                                num_capabilities;
  std::array<capability_validity, MAX_NUM_SUPPORTED_TAGS> capabilities;
};

/// Param request message.
struct param_request : public base_message {
  uint8_t protocol_version;
};

/// Encodes the TLV data structures used for the request.
struct tlv_request_data {
  /// Maximum number of tags supported.
  static constexpr unsigned MAX_NUM_SUPPORTED_TAGS = 2048;

  std::array<uint16_t, MAX_NUM_SUPPORTED_TAGS> tags;

  /// TLVs intermediate data structures.
  cell_param             cell;
  carrier_param          carrier;
  pdcch_param            pdcch;
  pucch_param            pucch;
  pdsch_param            pdsch;
  pusch_param            pusch;
  msg_a_pusch_param      msg_a_pusch;
  prach_param            prach;
  msg_a_prach_param      msg_a_prach;
  measurement_param      measurement;
  uci_param              uci;
  capability_param       capabilities;
  phy_support_param      phy;
  phy_dfe_profile_param  phy_dfe_profile;
  delay_management_param delay;
  user_plane_param       user_plane;
  srs_param              srs;
  mimo_param             mimo;
};

/// Param response message.
struct param_response : public base_message {
  error_code_id    errorcode;
  uint8_t          num_tlv;
  tlv_request_data tlvs;
};

/// Encodes the PHY configuration.
struct phy_config {
  uint16_t               profile_id;
  std::array<uint8_t, 6> indication_instances_per_slot;
  std::array<uint8_t, 4> request_instances_per_slot;
  uint16_t               sfn;
  uint8_t                slot;
  uint8_t                subcarrier_spacing;
  cyclic_prefix_type     cyclic_prefix;
};

/// Encodes the carrier configuration.
struct carrier_config {
  uint16_t                dl_bandwidth;
  uint32_t                dl_freq;
  std::array<uint16_t, 5> dl_k0;
  std::array<uint16_t, 5> dl_grid_size;
  uint16_t                num_tx_ant;
  uint16_t                ul_bandwidth;
  uint32_t                ul_freq;
  std::array<uint16_t, 5> ul_k0;
  std::array<uint16_t, 5> ul_grid_size;
  uint16_t                num_rx_ant;
  uint8_t                 freq_shift_7p5kHz;
  uint8_t                 power_profile;
  uint8_t                 power_offset_rs_index;
};

/// Encodes the cell configuration.
struct cell_config {
  uint16_t phy_cell_id;
  uint8_t  frame_duplex_type;
  uint8_t  pdsch_trans_type_validity;
  uint8_t  pusch_trans_type_validity;
};

/// Encodes the SSB/PBCH configuration.
struct ssb_pbch_config {
  uint32_t ss_pbch_power;
  int16_t  ss_pbch_block_power_scaling;
  uint8_t  bch_payload_flag;
};

/// Encodes the PRACH Fd occasion configuration.
struct prach_fd_occasion_config {
  /// Maximum number of unused root sequences allowed.
  static constexpr unsigned MAX_NUM_UNUSED_ROOT_SEQ = 63;

  uint16_t                                      prach_root_sequence_index;
  uint8_t                                       num_root_sequences;
  int16_t                                       prach_freq_offset;
  uint16_t                                      prach_guardband_offset;
  uint8_t                                       prach_zero_corr_conf;
  uint16_t                                      num_unused_root_sequences;
  std::array<uint16_t, MAX_NUM_UNUSED_ROOT_SEQ> unused_root_sequences;
};

/// Encodes the PRACH configuration.
struct prach_config {
  /// Maximum number of PRACH Fd occasions allowed.
  static constexpr unsigned MAX_NUM_PRACH_FD_OCCASIONS = 64;

  uint16_t                                                         prach_res_config_index;
  uint8_t                                                          prach_sequence_length;
  uint8_t                                                          prach_subcarrier_spacing;
  uint8_t                                                          prach_ul_bwp_pusch_scs;
  uint8_t                                                          restricted_set_config;
  uint8_t                                                          num_prach_fd_occasions;
  uint8_t                                                          prach_config_index;
  uint8_t                                                          prach_format;
  uint8_t                                                          num_prach_td_occasions;
  uint8_t                                                          num_preambles;
  uint8_t                                                          start_preamble_index;
  std::array<prach_fd_occasion_config, MAX_NUM_PRACH_FD_OCCASIONS> fd_occasions;
  uint8_t                                                          ssb_per_rach;
};

/// Encodes the Multi-PRACH configuration.
struct multi_prach_config {
  /// Maximum number of PRACH configurations supported.
  static constexpr unsigned MAX_NUM_PRACH_CONFIGURATIONS = 128;

  uint16_t                                               num_prachs;
  std::array<prach_config, MAX_NUM_PRACH_CONFIGURATIONS> prachs;
};

/// PRACH preamble group structure.
struct prach_preamble_group {
  /// Maximum number of associated dmrs.
  static constexpr unsigned MAX_NUM_ASSOC_DMRS = 16;

  uint16_t                                 pru_preamble_index_for_start_preamble_index;
  uint8_t                                  num_pusch_assoc_dmrs_occasions_for_this_prach_ro;
  std::array<uint16_t, MAX_NUM_ASSOC_DMRS> msg_a_pusch_assoc_dmrs_occasion;
};

/// Encodes the MsgA-PRACH-to-(PRU & DMRS) map multiple PRACH configurations for the carrier.
struct msg_a_prach_to_pru_dmrs_map {
  /// Maximum number of supported preambles.
  static constexpr unsigned MAX_NUM_PREAMBLES = 8;

  uint8_t                                             valid_ro;
  std::array<uint8_t, MAX_NUM_PREAMBLES>              cat_a_or_b_bitmap;
  std::array<prach_preamble_group, MAX_NUM_PREAMBLES> preamble_group;
};

/// Encodes MsgA-PRACH-to-(PRU & DMRS) configurations for the carrier.
struct msg_a_prach_to_pru_dmrs_config {
  /// Maximum number of PRACH FD indices.
  static constexpr unsigned MAX_NUM_PRACH_FD_INDICES = 64;
  /// Maximum number of PRACH TD indices.
  static constexpr unsigned MAX_NUM_PRACH_TD_INDICES = 64;

  std::array<std::array<msg_a_prach_to_pru_dmrs_map, MAX_NUM_PRACH_TD_INDICES>, MAX_NUM_PRACH_FD_INDICES> map;
};

/// Encodes PRACH slot configuration.
struct prach_slot_config {
  /// Maximum number of PRACH slot indices modulus.
  static constexpr unsigned MAX_NUM_PRACH_SLOT = 64;

  uint16_t                                 num_prach_slots_for_prach_to_pru_and_dmrs_map;
  std::array<uint16_t, MAX_NUM_PRACH_SLOT> slot_indices_modulus;
  msg_a_prach_to_pru_dmrs_config           mapping;
};

/// Encodes the MsgA-PUSCH configuration.
struct msg_a_pusch_config {
  /// Maximum number of PRACH slots.
  static constexpr unsigned MAX_NUM_PRACH_SLOTS = 64;

  uint16_t                                           msg_a_pusch_res_config_index;
  uint16_t                                           msg_a_prach_res_config_index;
  uint8_t                                            msg_a_group_a_or_b;
  uint16_t                                           nld_msg_a_pusch;
  uint16_t                                           dmrs_ports;
  uint8_t                                            available_dmrs_sequence_ids;
  std::array<uint16_t, 2>                            pusch_dmrs_scrambling_ids;
  uint8_t                                            num_pusch_occasions_fd;
  uint8_t                                            num_pusch_occasions_td;
  uint8_t                                            num_pusch_occasions_slots;
  uint8_t                                            msg_a_pusch_time_domain_offset;
  uint16_t                                           n_preambles;
  uint8_t                                            association_pattern_period;
  std::array<prach_slot_config, MAX_NUM_PRACH_SLOTS> prach_slots;
};

/// Encodes the Multi-MsgA-PUSCH configuration.
struct multi_msg_a_pusch_config {
  /// Maximum number of configurations.
  static constexpr unsigned MAX_NUM_CONFIGS = 64;

  uint16_t                                        num_configurations;
  std::array<msg_a_pusch_config, MAX_NUM_CONFIGS> configs;
};

/// Encodes the SSB resource configuration.
/// \note mask_size value is 2 or 64. When mask_size == 2 mask field of this struct is valid, otherwise (mask_size ==
/// 64) beam_id field is valid;
struct ssb_config {
  uint16_t                ssb_config_index;
  uint16_t                ssb_offset_point_a;
  uint8_t                 beta_pss_profile_nr;
  int16_t                 beta_pss_profile_sss;
  uint8_t                 ssb_period;
  uint8_t                 ssb_subcarrier_offset;
  ssb_case_types          case_kind;
  uint8_t                 subcarrier_spacing;
  uint8_t                 sub_carrier_spacing_common;
  uint8_t                 mask_size;
  std::array<uint32_t, 2> ssb_mask;
  std::array<uint8_t, 64> beam_id;
  uint8_t                 l_max;
  uint8_t                 rmsi_presence;
};

/// Encodes the Multi-SSB resource configuration.
struct multi_ssb_config {
  /// Maximum number of supported SSB TLVs.
  static constexpr unsigned MAX_NUM_SSBS = 64;

  uint8_t                              num_configs;
  std::array<ssb_config, MAX_NUM_SSBS> ssbs;
};

/// Encodes the TDD slot symbol type.
enum class tdd_slot_symbol_type : uint8_t { dl_symbol, ul_symbol, flexible_symbol };

/// Encodes the TDD configuration in the PHY.
struct tdd_phy_config {
  /// Maximum TDD periodicity.
  static constexpr unsigned MAX_TDD_PERIODICITY = 160;
  /// Maximum number of symbols per slot.
  static constexpr unsigned MAX_NUM_SYMBOL_PER_SLOT = 14;

  uint8_t                                                                                    tdd_period;
  std::array<std::array<tdd_slot_symbol_type, MAX_NUM_SYMBOL_PER_SLOT>, MAX_TDD_PERIODICITY> slot_config;
};

/// Encodes the measurement configuration.
struct measurement_config {
  uint8_t rssi_measurement;
  uint8_t rsrp_measurement;
};

/// Encodes a map which indicates how a set of UCI part1 parameters map to a length of corresponding UCI part2.
/// \note Maximum value for MAX_SIZE_MAP is 2^MAX_NUM_PART1_PARAMS = 4096.
struct uci_parts_map {
  /// Maximum number of part1 parameters.
  static constexpr unsigned MAX_NUM_PART1_PARAMS = 12;
  /// Maximum size for the map.
  static constexpr unsigned MAX_SIZE_MAP = 1024;

  uint8_t                                   num_part1_params;
  std::array<uint8_t, MAX_NUM_PART1_PARAMS> size_part1_params;
  std::array<uint16_t, MAX_SIZE_MAP>        map;
};

/// Encodes the UCI indication parameters.
struct uci_indication_config {
  /// Maximum number of maps.
  static constexpr unsigned MAX_NUM_MAPS = 128;

  uint16_t                                num_uci_to_maps;
  std::array<uci_parts_map, MAX_NUM_MAPS> maps;
};

/// Encodes the PRB-symbol rate match patterns structure.
struct prb_symbol_rm_pattern {
  uint8_t                 prb_symbol_rate_match_pattern_id;
  std::array<uint8_t, 35> freq_domain_rb;
  uint32_t                symbols_in_rb;
  uint8_t                 subcarrier_spacing;
};

/// Encodes the PRB-symbol rate match patterns bitmap configuration.
struct prb_symbol_rm_pattern_config {
  /// Maximum number of PRB-symbol rate-match patterns.
  static constexpr unsigned MAX_NUM_PATTERNS = 64;

  uint8_t                                             num_patterns;
  std::array<prb_symbol_rm_pattern, MAX_NUM_PATTERNS> patterns;
};

/// Encodes the CORESET rate-match pattern structure.
struct coreset_rm_pattern {
  uint8_t                coreset_rate_match_pattern_id;
  std::array<uint8_t, 6> freq_domain_resources;
  uint16_t               symbols_pattern;
  uint8_t                subcarrier_spacing;
};

/// Encodes the CORESET rate-match patterns configuration.
struct coreset_rm_pattern_config {
  /// Maximum number of PRB-symbol rate-match patterns.
  static constexpr unsigned MAX_NUM_PATTERNS = 64;

  uint8_t                                          num_patterns;
  std::array<coreset_rm_pattern, MAX_NUM_PATTERNS> patterns;
};

/// Encodes the UL BWP structure.
struct ul_bwp {
  uint8_t  pucch_group_hopping;
  uint16_t n_id_pucch_hopping;
};

/// Encodes the PUCCH semi-static signaling parameters.
struct pucch_semi_static_config {
  /// Maximum number of UL BWP.
  static constexpr unsigned MAX_NUM_UL_BWP = 64;

  uint8_t                            num_ul_bwp_ids;
  std::array<ul_bwp, MAX_NUM_UL_BWP> ul_bwp_id;
};

/// Encodes the PDSCH semi-static parameters.
struct pdsch_semi_static_config {
  uint8_t pdsch_cbg_scheme;
};

/// Encodes the delay management configuration.
struct delay_management_config {
  uint16_t timing_window;
  uint8_t  timing_info_mode;
  uint8_t  timing_info_period;
};

/// Encodes the TLV data structure for the configure.
struct tlv_config_data {
  /// Maximum number of allowed TLVs supported.
  static constexpr unsigned MAX_NUM_TLVS = 2048;

  std::array<uint16_t, MAX_NUM_TLVS> tlv_tags;

  /// TLVs intermediate data structures.
  phy_config                   phy;
  carrier_config               carrier;
  cell_config                  cell;
  ssb_pbch_config              ssb_pbch;
  prach_config                 prach;
  multi_prach_config           multi_prach;
  msg_a_pusch_config           msg_a_pusch;
  multi_msg_a_pusch_config     multi_msg_a_pusch;
  ssb_config                   ssb;
  multi_ssb_config             multi_ssb;
  tdd_phy_config               tdd;
  measurement_config           measurement;
  uci_indication_config        uci;
  prb_symbol_rm_pattern_config prb_symbol;
  coreset_rm_pattern_config    coreset;
  pucch_semi_static_config     pucch_semi_static;
  pdsch_semi_static_config     pdsch_semi_static;
  delay_management_config      delay;
};

/// Config request message.
struct config_request : public base_message {
  uint8_t         num_tlvs;
  tlv_config_data tlvs;
};

/// Config response message.
struct config_response : public base_message {
  /// Maximum number of invalid TLVs supported.
  static constexpr unsigned MAX_NUM_TLVS = 2048;

  error_code_id                      errorcode;
  uint8_t                            num_invalid_unsupported_tlvs;
  uint8_t                            num_invalid_tlvs_can_conf_idle;
  uint8_t                            num_invalid_tlvs_can_conf_running;
  uint8_t                            num_missing_tlvs;
  std::array<uint16_t, MAX_NUM_TLVS> unsupported_tlvs;
  std::array<uint16_t, MAX_NUM_TLVS> valid_idle_tlvs;
  std::array<uint16_t, MAX_NUM_TLVS> valid_running_tlvs;
  std::array<uint16_t, MAX_NUM_TLVS> missing_tlvs;
  tlv_config_data                    tlvs;
};

/// Start request message.
struct start_request : public base_message {};

/// Start response message.
struct start_response : public base_message {};

/// Stop request message.
struct stop_request : public base_message {};

/// Stop indication message.
struct stop_indication : public base_message {};

/// Encodes the error indication message.
struct error_indication_message : public base_message {
  uint16_t      sfn;
  uint16_t      slot;
  uint8_t       message_id;
  error_code_id error_code;
  uint16_t      expected_sfn;
  uint16_t      expected_slot;
};

/// \note not adding numOfRateMAtchPatternLTECrsPerSlot, numOfRateMatchPatternLTEInPhy,
/// lteCrsRateMatchMbsfnDerivation, supportedLTECrsRateMatchMethod as they refer to LTE.
enum class tlv_tags : uint16_t {
  /// General parameter tags.
  protocol_version = 0x0037,

  /// Cell parameter tags.
  release_capability          = 0x0001,
  phy_state                   = 0x0002,
  skip_blank_dl_config        = 0x0003,
  skip_blank_ul_config        = 0x0004,
  num_config_tlvs_to_report   = 0x0005,
  power_profiles_supported    = 0x0038,
  ss_pbch_block_power_scaling = 0x0058,
  max_num_pdu_dl_tti          = 0x0039,
  max_num_pdu_ul_tti          = 0x003a,
  max_num_pdu_ul_dci          = 0x003b,

  /// Carrier parameter tags.
  cyclic_prefix                       = 0x0006,
  supported_subcarrier_spacings_dl    = 0x0007,
  supported_subcarrier_spacings_ssb   = 0x0059,
  supported_bandwidth_dl              = 0x0008,
  supported_subcarrier_spacings_ul    = 0x0009,
  supported_bandwidth_ul              = 0x000a,
  ss_pbch_multiple_carriers_in_a_band = 0x003c,
  multiple_cells_ss_pbch_in_a_carrier = 0x003d,
  meaning_of_carrier_frequency        = 0x005a,

  /// PDCCH parameter tags.
  cce_mapping_type                          = 0x000b,
  coreset_outside_first_3_ofdm_syms_of_slot = 0x000c,
  precoder_granularity_coreset              = 0x000d,
  pdcch_mu_mimo                             = 0x000e,
  pddch_precoder_cycling                    = 0x000f,
  max_pdcchs_per_slot                       = 0x0010,

  /// PUCCH parameter tags.
  pucch_formats                    = 0x0011,
  max_pucch_per_slot               = 0x0012,
  pucch_group_and_sequence_hopping = 0x003e,
  max_num_ul_bwp_ids               = 0x003f,
  pucch_aggregation                = 0x0040,

  /// PDSCH parameter tags.
  pdsch_mapping_type                           = 0x0013,
  pdsch_allocation_types                       = 0x0014,
  pdsch_vrb_to_prb_mapping                     = 0x0015,
  pdsch_cbg                                    = 0x0016,
  pdsch_dmrs_config_types                      = 0x0017,
  pdsch_dmrs_max_length                        = 0x0018,
  pdsch_dmrs_additional_pos                    = 0x0019,
  max_pdschs_tbs_per_slot                      = 0x001a,
  max_num_mimo_layers_pdsch                    = 0x001b,
  supported_max_modulation_order_dl            = 0x001c,
  max_mu_mimo_users_dl                         = 0x001d,
  pdsch_data_in_dmrs_symbols                   = 0x001e,
  preemption_support                           = 0x001f,
  pdsch_non_slot_support                       = 0x0020,
  ssb_rate_match                               = 0x0041,
  supported_rate_match_pattern_type_and_method = 0x0042,
  pdcch_rate_match                             = 0x0043,
  csi_rs_rate_match                            = 0x0046,
  pdsch_trans_type_support                     = 0x0047,
  pdsch_mac_pdu_bit_alignment                  = 0x0048,
  max_number_prb_sym_bitmap_per_slot           = 0x005d,
  max_number_csi_rs_rate_match_per_slot        = 0x005e,
  max_num_ssb_per_pdsch_slot                   = 0x005f,
  universal_rate_match                         = 0x0060,
  require_pdsch_signaling_associations         = 0x0061,

  /// PUSCH parameter tags.
  uci_mux_ulsch_in_pusch            = 0x0021,
  uci_only_pusch                    = 0x0022,
  pusch_frequency_hopping           = 0x0023,
  pusch_dmrs_config_types           = 0x0024,
  pusch_dmrs_max_len                = 0x0025,
  pusch_dmrs_additional_pos         = 0x0026,
  pusch_cbg                         = 0x0027,
  pusch_mapping_type                = 0x0028,
  pusch_allocation_types            = 0x0029,
  pusch_vrb_to_prb_mapping          = 0x002a,
  pusch_max_ptrs_ports              = 0x002b,
  pusch_max_puschs_tbs_per_slot     = 0x002c,
  max_num_mimo_layers_non_cb_pusch  = 0x002d,
  max_num_mimo_layers_cb_pusch      = 0x0049,
  supported_max_modulation_order_ul = 0x002e,
  max_mu_mimo_users_ul              = 0x002f,
  dfts_ofdm_support                 = 0x0030,
  pusch_aggregation_factor          = 0x0031,
  pusch_lbrm_support                = 0x004a,
  pusch_trans_type_support          = 0x004b,
  pusch_mac_pdu_bit_alignment       = 0x004c,

  /// MsgA-PUSCH parameter tags.
  msg_a_num_pusch_res_config              = 0x0062,
  msg_a_pusch_tran_precoding              = 0x0063,
  msg_a_intra_slot_pusch_freq_hop_h       = 0x0064,
  msg_a_max_pusch_fd_occasions            = 0x0065,
  msg_a_guard_band                        = 0x0066,
  msg_a_guard_period                      = 0x0067,
  msg_a_pusch_mapping_type                = 0x0068,
  msg_a_pusch_dmrs_max_len                = 0x0069,
  msg_a_pusch_dmrs_cdm_group              = 0x006a,
  msg_a_pusch_dmrs_additional_pos         = 0x006b,
  msg_a_supported_max_modulation_order_ul = 0x006c,
  msg_a_max_pusch_per_pru                 = 0x006d,
  msg_a_max_num_cdm_group                 = 0x006e,
  msg_a_max_num_dmrs_port                 = 0x006f,
  msg_a_prach_to_pusch_mapping_scheme     = 0x0070,

  /// PRACH parameter tags.
  prach_long_formats                     = 0x0032,
  prach_short_formats                    = 0x0033,
  prach_restricted_sets                  = 0x0034,
  max_prach_fd_occasions_in_a_slot       = 0x0035,
  max_num_prach_configurations           = 0x004d,
  prach_multiple_carriers_in_a_band      = 0x004e,
  prach_frequency_offset_meaning         = 0x0071,
  max_num_td_fd_prach_occasions_per_slot = 0x0072,
  prach_root_processing_rate             = 0x0073,
  prach_occasion_queue_size              = 0x0074,

  /// MsgA-PRACH parameter tags.
  msg_a_max_num_prach_confs               = 0x0075,
  msg_a_prach_long_formats                = 0x0076,
  msg_a_prach_short_formats               = 0x0077,
  msg_a_prach_restricted_sets             = 0x0078,
  msg_a_max_prach_fd_occasions_in_slot    = 0x0079,
  msg_a_prach_multiple_carriers_in_a_band = 0x007a,

  /// Measurement parameter tags.
  rssi_measurement_support = 0x0036,
  rsrp_measurement_support = 0x007b,

  /// UCI reporting parameter tags.
  max_num_uci_maps                  = 0x004f,
  uci_report_format_pucch_format234 = 0x007c,

  /// Capability TLVs tag.
  num_capabilities = 0x0050,

  /// PHY support parameter tags.
  phy_profiles                      = 0x0051,
  time_management                   = 0x0052,
  phy_fapi_protocol_version         = 0x0053,
  more_than_one_indication_per_slot = 0x0055,
  more_than_one_request_per_slot    = 0x0056,

  /// PHY/DFE profile validity map parameter tag.
  phy_dfe_profile_validity_map = 0x0054,

  /// Delay management parameter tags.
  dl_tti_timing_offset  = 0x0106,
  ul_tti_timing_offset  = 0x0107,
  ul_dci_timing_offset  = 0x0108,
  tx_data_timing_offset = 0x0109,
  timing_window         = 0x011e,
  timing_info_period    = 0x0120,

  /// User Plane and Encapsulation parameter tags.
  user_plane_encapsulation_control_plane_separation = 0x007d,
  message_encoding_support                          = 0x007e,

  /// SRS parameter tags.
  supported_srs_usage_and_pairing                  = 0x0080,
  rb_subsampling_resolution_for_srs_derived_report = 0x0081,
  max_srs_channel_reports_per_slot                 = 0x0082,
  max_num_srs_pdus_per_slot                        = 0x0083,
  iq_sample_formats                                = 0x0084,
  singular_value_formats                           = 0x0085,
  max_srs_tx_ant_ports_per_ue                      = 0x0086,
  max_num_consecutive_srs_slots                    = 0x0087,
  max_duty_cycle_in_percentage                     = 0x0088,
  max_srs_comb_size                                = 0x0089,
  max_num_srs_cyclic_shifts                        = 0x008a,
  srs_symbols_bitmap                               = 0x008b,
  max_num_srs_symbols_per_slot                     = 0x008c,

  /// Spatial Multiplexing and MIMO Capabilities parameter tags.
  max_num_carriers_bw_layers_product_dl      = 0x008e,
  max_num_carriers_bw_layers_product_ul      = 0x008f,
  max_num_carriers_bw_antennas_product_dl    = 0x0090,
  max_num_carriers_bw_antennas_product_ul    = 0x0091,
  mu_mimo_dl                                 = 0x0092,
  supported_spatial_mux_per_dl_channel_type  = 0x0093,
  max_num_mu_mimo_layers_per_dl_channel_type = 0x0094,
  cross_channel_spatial_multiplexing_for_dl  = 0x0095,
  mu_mimo_ul                                 = 0x0096,
  supported_spatial_mux_per_ul_channel_type  = 0x0097,
  max_num_mu_mimo_layers_per_ul_channel_type = 0x0098,
  cross_channel_spatial_multiplexing_for_ul  = 0x0099,
  max_mu_mimo_groups_in_fd_per_dl_channel    = 0x009a,
  max_mu_mimo_groups_in_td_per_dl_channel    = 0x009b,
  max_mu_mimo_new_precoders_per_slot         = 0x009c,
  all_prbs_groups_prgs                       = 0x009d,
  max_mu_mimo_groups_in_fd_per_ul_channel    = 0x009e,
  max_mu_mimo_groups_in_td_per_ul_channel    = 0x009f,
  min_rb_resolution_for_srs_derived_reports  = 0x0150,
  max_number_dl_spatial_streams              = 0x0151,
  dl_spatial_stream_channel_priority         = 0x0152,
  max_number_ul_spatial_streams              = 0x0153,
  ul_tpmi_capability                         = 0x0154,

  /// PHY configuration tags.These TLVs are only supported for PHY ID 0.
  profile_id                    = 0x102a,
  indication_instances_per_slot = 0x102b,
  request_instances_per_slot    = 0x102c,
  sfn_slot                      = 0x103d,

  /// Carrier configuration tags.
  carrier_configuration = 0x102d,

  /// Cell configuration tags.
  phy_cell_id               = 0x100c,
  frame_duplex_type         = 0x100d,
  pdsch_trans_type_validity = 0x102e,
  pusch_trans_type_validity = 0x102f,

  /// SSB/PBCH configuration tags.
  ss_pbch_power                      = 0x100e,
  ss_pbch_block_power_scaling_config = 0x1030,
  bch_payload_flag                   = 0x100f,

  /// PRACH configuration tags.
  prach_configuration = 0x1031,

  /// Multi-PRACH configuration tags.
  multi_prach_configuration = 0x1032,

  /// MsgA-PUSCH configuration tags.
  msg_a_pusch_configuration = 0x103e,

  /// Multi MsgA-PUSCH configuration tags.
  multi_msg_a_pusch_configuration = 0x103f,

  /// SSB configuration tags.
  ssb_configuration = 0x1033,

  /// Multi-SSB configuration tags.
  multi_ssb_configuration = 0x1034,

  /// TDD PHY pattern configuration tags.
  tdd_phy_pattern = 0x1035,

  /// Measurement configuration tags.
  rssi_measurement = 0x1028,
  rsrp_measurement = 0x1040,

  /// UCI indication configuration tags.
  uci_indication = 0x1036,

  /// PRB-symbol rate-match patterns configuration tags.
  prb_symbol_rate_match_patterns = 0x1037,

  /// CORESET rete-match patterns configuration tags.
  coreset_rate_match_patterns = 0x1041,

  /// PUCCH semi-static configuration tags.
  pucch_semi_static = 0x1039,

  /// PDSCH semi-static configuration tags.
  pdsch_cbg_scheme = 0x103a,

  /// Delay management configuration tags.
  /// Configuration allows to use timing_window and timing_info_period tags. The values for these tags are the same
  /// that
  /// in the parameter tags.
  timing_info_mode = 0x011f
};

} // namespace fapi
} // namespace srsgnb

#endif // SRSGNB_FAPI_MESSAGES_H
