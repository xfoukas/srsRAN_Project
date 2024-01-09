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
#include "srsran/phy/upper/tx_buffer.h"

namespace srsran {

/// \brief Wraps a transmit buffer instance and locks it inside a scope.
///
/// The locking mechanism ensures the buffer is locked as in the pool as long as it is being used within a scope.
class unique_tx_buffer
{
public:
  /// Public interface required for locking and unlocking the internal buffer.
  class callback : public tx_buffer
  {
  public:
    /// Locks the buffer.
    virtual void lock() = 0;

    /// Unlocks the buffer.
    virtual void unlock() = 0;
  };

  /// Default constructor - creates an invalid buffer.
  unique_tx_buffer() = default;

  /// Builds a unique transmit buffer from a reference. It locks the internal buffer.
  explicit unique_tx_buffer(callback& buffer_) : ptr(&buffer_)
  {
    if (ptr != nullptr) {
      ptr->lock();
    }
  }

  /// Destructor - it releases the buffer.
  ~unique_tx_buffer()
  {
    if (ptr != nullptr) {
      ptr->unlock();
      ptr = nullptr;
    }
  }

  /// Copy constructor is deleted to prevent the unique buffer from being shared across multiple scopes.
  unique_tx_buffer(const unique_tx_buffer& other) = delete;

  /// Move constructor moves the buffer to a different scope.
  unique_tx_buffer(unique_tx_buffer&& other) noexcept
  {
    srsran_assert(other.is_valid(), "Trying to move an invalid buffer.");
    ptr       = other.ptr;
    other.ptr = nullptr;
  };

  /// Move assignment moves the buffer to a different scope.
  unique_tx_buffer& operator=(unique_tx_buffer&& other) noexcept
  {
    // Unlocks current buffer if it is valid.
    if (ptr != nullptr) {
      ptr->unlock();
      ptr = nullptr;
    }

    // Swap pointers.
    std::swap(ptr, other.ptr);
    return *this;
  }

  /// Gets the actual transmit buffer.
  tx_buffer& get()
  {
    srsran_assert(is_valid(), "Invalid buffer.");
    return *ptr;
  }
  tx_buffer& operator*() { return get(); }
  tx_buffer& operator->() { return get(); }

  /// Returns true if the unique buffer contains a valid buffer.
  bool is_valid() const { return ptr != nullptr; }

private:
  /// Underlying pointer to the actual buffer. Set to nullptr for an invalid buffer.
  callback* ptr = nullptr;
};

} // namespace srsran
