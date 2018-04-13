// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

static void eraseString(std::string &str) {
  char *p = const_cast<char *>(str.c_str());
  while (*p) *p++ = '*';
}

TEST_CASE("std::string") {
  DynamicJsonBuffer jb;

  SECTION("JsonArray") {
    DynamicJsonArray array;

    SECTION("deserializeJson") {
      std::string json("[\"hello\"]");

      JsonError err = deserializeJson(array, json);
      eraseString(json);

      REQUIRE(err == JsonError::Ok);
      REQUIRE(std::string("hello") == array[0]);
    }

    SECTION("add()") {
      std::string value("hello");
      array.add(value);
      eraseString(value);
      REQUIRE(std::string("hello") == array[0]);
    }

    SECTION("set()") {
      std::string value("world");
      array.add("hello");
      array.set(0, value);
      eraseString(value);
      REQUIRE(std::string("world") == array[0]);
    }

    SECTION("operator[]") {
      std::string value("world");
      array.add("hello");
      array[0] = value;
      eraseString(value);
      REQUIRE(std::string("world") == array[0]);
    }

    SECTION("serializeJson()") {
      array.add(4);
      array.add(2);
      std::string json;
      serializeJson(array, json);
      REQUIRE(std::string("[4,2]") == json);
    }

    SECTION("serializeJsonPretty()") {
      array.add(4);
      array.add(2);
      std::string json;
      serializeJsonPretty(array, json);
      REQUIRE(std::string("[\r\n  4,\r\n  2\r\n]") == json);
    }
  }

  SECTION("JsonObject") {
    DynamicJsonDocument doc;

    SECTION("deserializeJson()") {
      std::string json("{\"hello\":\"world\"}");

      JsonError err = deserializeJson(doc, json);
      eraseString(json);

      REQUIRE(err == JsonError::Ok);
      REQUIRE(std::string("world") == doc["hello"]);
    }

    SECTION("operator[]") {
      char json[] = "{\"key\":\"value\"}";

      deserializeJson(doc, json);

      REQUIRE(std::string("value") == doc[std::string("key")]);
    }

    SECTION("operator[] const") {
      char json[] = "{\"key\":\"value\"}";

      deserializeJson(doc, json);
      const JsonObject &obj = doc;

      REQUIRE(std::string("value") == obj[std::string("key")]);
    }

    SECTION("set(key)") {
      std::string key("hello");
      doc.set(key, "world");
      eraseString(key);
      REQUIRE(std::string("world") == doc["hello"]);
    }

    SECTION("set(value)") {
      std::string value("world");
      doc.set("hello", value);
      eraseString(value);
      REQUIRE(std::string("world") == doc["hello"]);
    }

    SECTION("set(key,value)") {
      std::string key("hello");
      std::string value("world");
      doc.set(key, value);
      eraseString(key);
      eraseString(value);
      REQUIRE(std::string("world") == doc["hello"]);
    }

    SECTION("set(JsonArraySubscript)") {
      DynamicJsonArray arr;
      arr.add("world");

      doc.set(std::string("hello"), arr[0]);

      REQUIRE(std::string("world") == doc["hello"]);
    }

    SECTION("set(JsonObjectSubscript)") {
      DynamicJsonDocument doc2;
      JsonObject &obj = doc2.becomeObject();
      obj.set("x", "world");

      doc.set(std::string("hello"), obj["x"]);

      REQUIRE(std::string("world") == doc["hello"]);
    }

    SECTION("get<T>()") {
      char json[] = "{\"key\":\"value\"}";
      deserializeJson(doc, json);

      REQUIRE(std::string("value") ==
              doc.get<const char *>(std::string("key")));
    }

    SECTION("is<T>()") {
      char json[] = "{\"key\":\"value\"}";
      deserializeJson(doc, json);
      REQUIRE(true == doc.is<const char *>(std::string("key")));
    }

    SECTION("createNestedObject()") {
      std::string key = "key";
      char json[64];
      doc.createNestedObject(key);
      eraseString(key);
      serializeJson(doc, json, sizeof(json));
      REQUIRE(std::string("{\"key\":{}}") == json);
    }

    SECTION("createNestedArray()") {
      std::string key = "key";
      char json[64];
      doc.createNestedArray(key);
      eraseString(key);
      serializeJson(doc, json, sizeof(json));
      REQUIRE(std::string("{\"key\":[]}") == json);
    }

    SECTION("containsKey()") {
      char json[] = "{\"key\":\"value\"}";
      deserializeJson(doc, json);
      REQUIRE(true == doc.containsKey(std::string("key")));
    }

    SECTION("remove()") {
      char json[] = "{\"key\":\"value\"}";
      deserializeJson(doc, json);
      REQUIRE(1 == doc.size());
      doc.remove(std::string("key"));
      REQUIRE(0 == doc.size());
    }

    SECTION("operator[], set key") {
      std::string key("hello");
      doc[key] = "world";
      eraseString(key);
      REQUIRE(std::string("world") == doc["hello"]);
    }

    SECTION("operator[], set value") {
      std::string value("world");
      doc["hello"] = value;
      eraseString(value);
      REQUIRE(std::string("world") == doc["hello"]);
    }

    SECTION("serializeJson()") {
      doc["key"] = "value";
      std::string json;
      serializeJson(doc, json);
      REQUIRE(std::string("{\"key\":\"value\"}") == json);
    }

    SECTION("serializeJsonPretty()") {
      doc["key"] = "value";
      std::string json;
      serializeJsonPretty(doc, json);
      REQUIRE(std::string("{\r\n  \"key\": \"value\"\r\n}") == json);
    }

    SECTION("memoryUsage() increases when adding a new key") {
      std::string key1("hello"), key2("world");

      doc[key1] = 1;
      size_t sizeBefore = doc.memoryUsage();
      doc[key2] = 2;
      size_t sizeAfter = doc.memoryUsage();

      REQUIRE(sizeAfter - sizeBefore >= key2.size());
    }

    SECTION("memoryUsage() remains when adding the same key") {
      std::string key("hello");

      doc[key] = 1;
      size_t sizeBefore = doc.memoryUsage();
      doc[key] = 2;
      size_t sizeAfter = doc.memoryUsage();

      REQUIRE(sizeBefore == sizeAfter);
    }
  }
}
