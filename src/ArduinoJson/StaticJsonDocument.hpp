// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "JsonVariant.hpp"
#include "Memory/StaticJsonBuffer.hpp"

namespace ArduinoJson {

template <size_t CAPACITY = sizeof(JsonVariant)>
class StaticJsonDocument {
  StaticJsonBuffer<CAPACITY> _buffer;
  JsonVariant _root;

 public:
  template <typename T>
  StaticJsonDocument& operator=(const T& value) {
    _buffer.clear();
    _root = value;
    return *this;
  }

  template <typename T>
  StaticJsonDocument& operator=(const T* value) {
    _buffer.clear();
    _root = value;
    return *this;
  }

  Internals::StaticJsonBufferBase& buffer() {
    return _buffer;
  }

  template <typename T>
  bool is() const {
    return _root.is<T>();
  }

  template <typename T>
  typename Internals::JsonVariantAs<T>::type as() const {
    return _root.as<T>();
  }

  // JsonObject& to<JsonObject>()
  template <typename T>
  typename Internals::EnableIf<Internals::IsSame<T, JsonObject>::value,
                               JsonObject&>::type
  to() {
    clear();
    JsonObject* object = new (&_buffer) JsonObject(&_buffer);
    if (!object) return JsonObject::invalid();
    _root = object;
    return *object;
  }

  // JsonArray& to<JsonArray>()
  template <typename T>
  typename Internals::EnableIf<Internals::IsSame<T, JsonArray>::value,
                               JsonArray&>::type
  to() {
    clear();
    JsonArray* array = new (&_buffer) JsonArray(&_buffer);
    if (!array) return JsonArray::invalid();
    _root = array;
    return *array;
  }

  // JsonVariant to<JsonVariant>()
  template <typename T>
  typename Internals::EnableIf<Internals::IsSame<T, JsonVariant>::value,
                               T&>::type
  to() {
    clear();
    return _root;
  }

  void clear() {
    _buffer.clear();
    _root = JsonVariant();
  }

  size_t memoryUsage() const {
    return _buffer.size();
  }
};

}  // namespace ArduinoJson
