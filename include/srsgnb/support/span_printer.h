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

#include "srsgnb/adt/span.h"
#include <fstream>

namespace srsgnb {

/// \brief Writes the given span into a file in binary format.
///
/// The file is opened in truncated mode and writen in binary format.
///
/// \tparam    T        Type of the data to write.
/// \param[in] filename Output file name (possibly, with path).
/// \param[in] data     Data to write.
/// \return True on success, false otherwise.
/// \warning \c T must be standard layout.
template <typename T>
inline bool print(const char* filename, span<const T> data)
{
  static_assert(std::is_standard_layout<T>::value, "T is not standard layout");
  std::ofstream file(filename, std::ios_base::binary | std::ios_base::trunc);

  if (!file.is_open()) {
    return false;
  }

  file.write(reinterpret_cast<const char*>(data.data()), data.size() * sizeof(T));

  return true;
}

} // namespace srsgnb