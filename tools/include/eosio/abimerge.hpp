#pragma once

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#pragma GCC diagnostic ignored "-Wcovered-switch-default"
#include <jsoncons/json.hpp>
#include "abi.hpp"

#include <string>
#include <vector>

using jsoncons::json;
using jsoncons::ojson;

class ABIMerger {
   public:
      ABIMerger(ojson a) : abi(a) {}
      void set_abi(ojson a) {
         abi = a;
      }
      std::string get_abi_string()const {
         std::stringstream ss;
         ss << pretty_print(abi);
         return ss.str();
      }
      ojson merge(ojson other) {
         ojson ret;
         ret["____comment"] = abi["____comment"]; 
         ret["version"]  = merge_version(other);
         ret["types"]    = merge_types(other);
         ret["structs"]  = merge_structs(other);
         ret["actions"]  = merge_actions(other);
         ret["events"]   = merge_events(other);
         ret["tables"]   = merge_tables(other);
         ret["variants"] = merge_variants(other);
         return ret;
      }
   private:
      std::string merge_version(ojson b) {
         std::string ver_a = abi["version"].as<std::string>();
         std::string ver_b = b["version"].as<std::string>();
         return std::stod(ver_a.substr(ver_a.size()-3))*10 < std::stod(ver_b.substr(ver_b.size()-3))*10 ?
            ver_b : ver_a;
      }

      static bool struct_is_same(ojson a, ojson b) {
         bool same_fields = a["fields"].size() == b["fields"].size();
         for (auto a_field : a["fields"].array_range()) {
            bool found_field = false;
            for (auto b_field : b["fields"].array_range()) {
               if (a_field["name"] == b_field["name"] &&
                   a_field["type"] == b_field["type"])
                  found_field = true;
            }
            if (!found_field)
               return false;
         }
         return a["name"] == b["name"] &&
                a["base"] == b["base"] && same_fields;
      }

      static bool type_is_same(ojson a, ojson b) {
         return a["new_type_name"] == b["new_type_name"] &&
                a["type"] == b["type"];
      }

      static bool action_is_same(ojson a, ojson b) {
         return a["name"] == b["name"] &&
                a["type"] == b["type"];
      }

      template <typename T>
      static bool action_is_almost_same(ojson a, ojson b, T& rc) {
         return a["name"] == b["name"] &&
                a["type"] == b["type"];
      }

      static bool event_is_same(ojson a, ojson b) {
         return a["name"] == b["name"] &&
                a["type"] == b["type"];
      }

      static bool variant_is_same(ojson a, ojson b) {
         for (auto tya : a["types"].array_range()) {
            bool found_ty = false;
            for (auto tyb : b["types"].array_range()) {
               if (tyb == tya)
                  found_ty = true;
            }
            if (!found_ty)
               return false;
         }
         return a["name"] == b["name"];
      }

      static inline bool optional_is_same(ojson a, ojson b, const ojson::string_view_type& name) {
         return (!a.has_key(name)) ? (!b.has_key(name)) : (b.has_key(name) && a[name] == b[name]);
      }

      static bool orders_are_same(ojson a_orders, ojson b_orders) {
         if (a_orders.size() != b_orders.size())
            return false;
         for (auto a : a_orders.array_range()) {
            bool found = false;
            for (auto b : b_orders.array_range()) {
               if (a["field"] == b["field"] &&
                   a["order"] == b["order"]) {
                  found = true;
                  break;
               }
            }
            if (!found) return false;
         }
         return true;
      }

      static bool indexes_are_same(ojson a_indexes, ojson b_indexes) {
         if (a_indexes.size() != b_indexes.size())
            return false;
         for (auto a : a_indexes.array_range()) {
            bool found = false;
            for (auto b : b_indexes.array_range()) {
               if (a["name"] == b["name"] &&
                   a["unique"] == b["unique"] &&
                   orders_are_same(a["orders"], b["orders"])) {
                  found = true;
                  break;
               }
            }
            if (!found) return false;
         }
         return true;
      }

      static bool table_is_same(ojson a, ojson b) {
         if (!indexes_are_same(a["indexes"], b["indexes"]))
            return false;
         return a["name"] == b["name"] &&
                a["type"] == b["type"] &&
                optional_is_same(a, b, "scope_type");
      }
      
      template <typename F>
      void add_object(ojson& ret, ojson a, ojson b, std::string type, std::string id, F&& is_same_func) {
         for (auto obj_a : a[type].array_range()) {
            ret.push_back(obj_a);
         }
         for (auto obj_b : b[type].array_range()) {
            bool should_skip = false;
            for (auto obj_a : a[type].array_range()) {
               if (obj_a[id] == obj_b[id]) {
                  if (!is_same_func(obj_a, obj_b)) {
                     throw std::runtime_error(std::string("Error, ABI structs malformed : ")+obj_a[id].as<std::string>()+" already defined");
                  }
                  else
                     should_skip = true;
               }
            }
            if (!should_skip)
               ret.push_back(obj_b);
         }
      }
      
      ojson merge_structs(ojson b) {
         ojson structs = ojson::array();
         add_object(structs, abi, b, "structs", "name", struct_is_same);
         return structs;
      }

      ojson merge_types(ojson b) {
         ojson types = ojson::array();
         add_object(types, abi, b, "types", "new_type_name", type_is_same);
         return types;
      }

      ojson merge_variants(ojson b) {
         ojson vars = ojson::array();
         add_object(vars, abi, b, "variants", "name", variant_is_same);
         return vars;
      }

      ojson merge_actions(ojson b) {
         ojson acts = ojson::array();
         add_object(acts, abi, b, "actions", "name", action_is_same);
         return acts;
      }

      ojson merge_events(ojson b) {
         ojson acts = ojson::array();
         add_object(acts, abi, b, "events", "name", event_is_same);
         return acts;
      }

      ojson merge_tables(ojson b) {
         ojson tabs = ojson::array();
         add_object(tabs, abi, b, "tables", "name", table_is_same);
         return tabs;
      }

      ojson abi;
};
#pragma GCC diagnostic pop
