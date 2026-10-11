#pragma once

// Only allowed headers as per problem statement
#include <new>
#include <utility>

namespace sjtu {

/**
 * @brief A simple unique_ptr implementation matching the given interface.
 *
 * The implementation stores only a raw pointer, guaranteeing that the size of
 * `unique_ptr<T>` does not exceed the size of a pointer (as required by the
 * static_assert in the problem description).
 */
template <typename _Tp>
class unique_ptr {
 private:
  _Tp* _ptr = nullptr;  // owned pointer

 public:
  // Default constructor – creates a null pointer.
  constexpr unique_ptr() noexcept = default;

  // Delete copy operations – ownership cannot be shared.
  unique_ptr(const unique_ptr&) = delete;
  unique_ptr& operator=(const unique_ptr&) = delete;

  // Move constructor – transfer ownership and null the source.
  constexpr unique_ptr(unique_ptr&& other) noexcept : _ptr(other._ptr) {
    other._ptr = nullptr;
  }

  // Destructor – delete the owned object (delete nullptr is safe).
  ~unique_ptr() noexcept { delete _ptr; }

  // Move assignment – delete current object, then take ownership.
  unique_ptr& operator=(unique_ptr&& other) noexcept {
    if (this != &other) {
      delete _ptr;
      _ptr = other._ptr;
      other._ptr = nullptr;
    }
    return *this;
  }

  // Construct from a raw pointer – takes ownership.
  explicit constexpr unique_ptr(_Tp* p) noexcept : _ptr(p) {}

  // Reset to nullptr (deleting any owned object).
  void reset() noexcept {
    delete _ptr;
    _ptr = nullptr;
  }

  // Release ownership and return the raw pointer.
  _Tp* release() noexcept {
    _Tp* temp = _ptr;
    _ptr = nullptr;
    return temp;
  }

  // Get the stored pointer without releasing ownership.
  constexpr _Tp* get() const noexcept { return _ptr; }

  // Dereference operators.
  constexpr _Tp& operator*() const noexcept { return *_ptr; }
  constexpr _Tp* operator->() const noexcept { return _ptr; }
};

/**
 * @brief Create a unique_ptr by copying a value.
 * This matches the required signature `make_unique(const _Tp&)`.
 */
template <typename _Tp>
constexpr unique_ptr<_Tp> make_unique(const _Tp& value) {
  return unique_ptr<_Tp>(new _Tp(value));
}

/**
 * @brief Perfect‑forwarding overload for constructing an object in place.
 * This is the optional bonus implementation.
 */
template <typename _Tp, typename... Args>
constexpr unique_ptr<_Tp> make_unique(Args&&... args) {
  return unique_ptr<_Tp>(new _Tp(std::forward<Args>(args)...));
}

} // namespace sjtu

