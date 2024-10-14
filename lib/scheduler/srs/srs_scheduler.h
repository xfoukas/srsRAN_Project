/*
 *
 * Copyright 2021-2024 Software Radio Systems Limited
 *
 * This file is part of srsRAN.
 *
 * srsRAN is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * srsRAN is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * A copy of the GNU Affero General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */

#pragma once

namespace srsran {

struct cell_resource_allocator;
class ue_cell_configuration;

/// SRS scheduling interface, which handles the scheduling of SRS opportunities.
class srs_scheduler
{
public:
  virtual ~srs_scheduler() = default;

  /// Schedules the SRS occasions.
  /// \param[out,in] res_alloc struct with scheduling results.
  virtual void run_slot(cell_resource_allocator& res_alloc) = 0;

  /// Adds a UE to the internal list of UEs to be scheduled.
  /// \param[in] ue_cfg dedicated configuration of the UE to be added.
  virtual void add_ue(const ue_cell_configuration& ue_cfg) = 0;

  /// Removes the UE from the internal list of UEs to be scheduled.
  /// \param[in] ue_cfg UE dedicated configuration of the UE to be removed.
  virtual void rem_ue(const ue_cell_configuration& ue_cfg) = 0;

  /// Updates the SRS configuration of this UE, if there are any changes w.r.t. the previous configuration.
  /// \param[in] ue_cfg New UE dedicated configuration of the UE to be reconfigured.
  /// \param[in] ue_cfg Old UE dedicated configuration of the UE to be reconfigured.
  virtual void reconf_ue(const ue_cell_configuration& new_ue_cfg, const ue_cell_configuration& old_ue_cfg) = 0;
};

} // namespace srsran