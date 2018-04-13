// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "JsonArray.hpp"
#include "JsonObject.hpp"
#include "JsonVariant.hpp"
#include "Memory/DynamicJsonBuffer.hpp"

namespace ArduinoJson {

class DynamicJsonDocument : public JsonVariant {
  DynamicJsonBuffer _buffer;

 public:
  DynamicJsonDocument() : JsonVariant() {}
  DynamicJsonDocument(size_t capacity) : JsonVariant(), _buffer(capacity) {}

  template <typename T>
  DynamicJsonDocument& operator=(const T& value) {
    _buffer.clear();
    JsonVariant::operator=(value);
    return *this;
  }

  template <typename T>
  DynamicJsonDocument& operator=(const T* value) {
    _buffer.clear();
    JsonVariant::operator=(value);
    return *this;
  }

  // JsonObject& to<JsonObject>()
  template <typename T>
  typename Internals::EnableIf<Internals::IsSame<T, JsonObject>::value,
                               JsonObject&>::type
  to() {
    clear();
    JsonObject* object = new (&_buffer) JsonObject(&_buffer);
    if (!object) return JsonObject::invalid();
    JsonVariant::operator=(object);
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
    JsonVariant::operator=(array);
    return *array;
  }

  // JsonVariant& to<JsonVariant>()
  template <typename T>
  typename Internals::EnableIf<Internals::IsSame<T, JsonVariant>::value,
                               T&>::type
  to() {
    clear();
    return *this;
  }

  DynamicJsonBuffer& buffer() {
    return _buffer;
  }

  void clear() {
    _buffer.clear();
    JsonVariant::operator=(JsonVariant());
  }

  size_t memoryUsage() const {
    return _buffer.size();
  }
};
}  // namespace ArduinoJson
