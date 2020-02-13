#pragma once

#include "../../contracts/eosio/action.hpp"
#include "../../core/eosio/name.hpp"
#include "../../core/eosio/serialize.hpp"
#include "../../core/eosio/fixed_bytes.hpp"

#include <vector>
#include <tuple>
#include <boost/hana.hpp>
#include <functional>
#include <utility>
#include <type_traits>
#include <iterator>
#include <limits>
#include <algorithm>
#include <optional>
#include <memory>

#define chaindb_assert(_EXPR, ...) eosio::check(_EXPR, __VA_ARGS__)

#ifndef CHAINDB_ANOTHER_CONTRACT_PROTECT
#  define CHAINDB_ANOTHER_CONTRACT_PROTECT(_CHECK, _MSG) \
     chaindb_assert(_CHECK, _MSG)
#endif // CHAINDB_ANOTHER_CONTRACT_PROTECT


namespace eosio {
using account_name_t = eosio::name::raw;
using payer_name_t   = eosio::name::raw;
using scope_t        = uint64_t;
using table_name_t   = eosio::name::raw;
using index_name_t   = eosio::name::raw;
using cursor_t       = int32_t;

typedef uint64_t primary_key_t;
static constexpr primary_key_t end_primary_key = static_cast<primary_key_t>(-1);
static constexpr primary_key_t unset_primary_key = static_cast<primary_key_t>(-2);

namespace internal_use_do_not_use {
   extern "C" {
      __attribute__((eosio_wasm_import))
      cursor_t chaindb_begin(account_name_t, scope_t, table_name_t, index_name_t);
      __attribute__((eosio_wasm_import))
      cursor_t chaindb_end(account_name_t, scope_t, table_name_t, index_name_t);
      __attribute__((eosio_wasm_import))
      cursor_t chaindb_lower_bound(account_name_t, scope_t, table_name_t, index_name_t, void* key, int32_t);
      __attribute__((eosio_wasm_import))
      cursor_t chaindb_lower_bound_pk(account_name_t, scope_t, table_name_t, primary_key_t);
      __attribute__((eosio_wasm_import))
      cursor_t chaindb_upper_bound(account_name_t, scope_t, table_name_t, index_name_t, void* key, int32_t);
      __attribute__((eosio_wasm_import))
      cursor_t chaindb_upper_bound_pk(account_name_t, scope_t, table_name_t, primary_key_t);
      __attribute__((eosio_wasm_import))
      cursor_t chaindb_locate_to(account_name_t, scope_t, table_name_t, index_name_t, primary_key_t, void* key, int32_t);
      __attribute__((eosio_wasm_import))
      cursor_t chaindb_clone(account_name_t, cursor_t);

      __attribute__((eosio_wasm_import))
      void chaindb_close(account_name_t, cursor_t);

      __attribute__((eosio_wasm_import))
      primary_key_t chaindb_current(account_name_t, cursor_t);
      __attribute__((eosio_wasm_import))
      primary_key_t chaindb_next(account_name_t, cursor_t);
      __attribute__((eosio_wasm_import))
      primary_key_t chaindb_prev(account_name_t, cursor_t);

      __attribute__((eosio_wasm_import))
      int32_t chaindb_datasize(account_name_t, cursor_t);
      __attribute__((eosio_wasm_import))
      primary_key_t chaindb_data(account_name_t, cursor_t, void* data, int32_t size);
      __attribute__((eosio_wasm_import))
      int32_t chaindb_service(account_name_t, cursor_t, void* data, int32_t size);

      __attribute__((eosio_wasm_import))
      primary_key_t chaindb_available_primary_key(account_name_t, scope_t, table_name_t);

      __attribute__((eosio_wasm_import))
      int32_t chaindb_insert(account_name_t, scope_t, table_name_t, payer_name_t, primary_key_t, void* data, int32_t);
      __attribute__((eosio_wasm_import))
      int32_t chaindb_update(account_name_t, scope_t, table_name_t, payer_name_t, primary_key_t, void* data, int32_t);
      __attribute__((eosio_wasm_import))
      int32_t chaindb_delete(account_name_t, scope_t, table_name_t, payer_name_t, primary_key_t);

      __attribute__((eosio_wasm_import))
      void chaindb_ram_state(account_name_t, scope_t, table_name_t, primary_key_t, int32_t);
   }
}

constexpr static inline name same_payer{};

template<class Class,typename Type,Type (Class::*PtrToMemberFunction)()const>
struct const_mem_fun {
  typedef typename std::remove_reference<Type>::type result_type;

  template<typename ChainedPtr>

  auto operator()(const ChainedPtr& x)const -> std::enable_if_t<!std::is_convertible<const ChainedPtr&, const Class&>::value, Type> {
    return operator()(*x);
  }

  Type operator()(const Class& x)const {
    return (x.*PtrToMemberFunction)();
  }

  Type operator()(const std::reference_wrapper<const Class>& x)const {
    return operator()(x.get());
  }

  Type operator()(const std::reference_wrapper<Class>& x)const {
    return operator()(x.get());
  }
};

template<typename A>
struct get_result_type {
    using type = typename A::result_type;
}; // struct get_result_type

template<typename Value, typename... Fields>
struct composite_key {
    using result_type = bool;
    using key_type = std::tuple<typename get_result_type<Fields>::type...>;

    auto operator()(const Value& v) {
        return std::make_tuple(Fields()(v)...);
    }
}; // struct composite_key

namespace _detail {
template<class Class,typename Type,Type Class::*PtrToMember>
struct const_member_base {
    typedef Type result_type;

    template<typename ChainedPtr>
    auto operator()(const ChainedPtr& x) const
    -> std::enable_if_t<!std::is_convertible<const ChainedPtr&, const Class&>::type, Type&> {
        return operator()(*x);
    }

    Type& operator()(const Class& x) const {
        return x.*PtrToMember;
    }
    Type& operator()(const std::reference_wrapper<const Class>& x) const {
        return operator()(x.get());
    }
    Type& operator()(const std::reference_wrapper<Class>& x) const {
        return operator()(x.get());
    }
}; // struct const_member_base

template<class Class,typename Type,Type Class::*PtrToMember>
struct non_const_member_base {
    typedef Type result_type;

    template<typename ChainedPtr>
    auto operator()(const ChainedPtr& x) const
    -> std::enable_if_t<!std::is_convertible<const ChainedPtr&, const Class&>::type, Type&> {
        return operator()(*x);
    }

    const Type& operator()(const Class& x) const {
        return x.*PtrToMember;
    }
    Type& operator()(Class& x) const {
        return x.*PtrToMember;
    }
    const Type& operator()(const std::reference_wrapper<const Class>& x) const {
        return operator()(x.get());
    }
    Type& operator()(const std::reference_wrapper<Class>& x) const {
        return operator()(x.get());
    }
}; // struct non_const_member_base
}

template<class Class,typename Type,Type Class::*PtrToMember>
struct member: std::conditional<
    std::is_const<Type>::value,
    _detail::const_member_base<Class,Type,PtrToMember>,
    _detail::non_const_member_base<Class,Type,PtrToMember>
  >::type
{
};

//
//  intrusive_ptr
//
//  A smart pointer that uses intrusive reference counting.
//
//  Relies on unqualified calls to
//
//      void intrusive_ptr_add_ref(T * p);
//      void intrusive_ptr_release(T * p);
//
//          (p != 0)
//
//  The object is responsible for destroying itself.
//
template<class T> class intrusive_ptr {
private:
    typedef intrusive_ptr this_type;

public:
    typedef T element_type;

    constexpr intrusive_ptr() = default;

    constexpr intrusive_ptr(T * p): px(p) {
        if (px != nullptr) {
            intrusive_ptr_add_ref(px);
        }
    }

    constexpr intrusive_ptr(const intrusive_ptr& rhs): px(rhs.px) {
        if (px != nullptr) {
            intrusive_ptr_add_ref( px );
        }
    }

    constexpr intrusive_ptr(intrusive_ptr&& rhs): px(rhs.detach()) {
    }

    ~intrusive_ptr() {
        if (px != nullptr) {
            intrusive_ptr_release( px );
        }
    }

    constexpr intrusive_ptr& operator=(const intrusive_ptr& rhs) {
        if (this == &rhs) {
            return *this;
        }
        this_type(rhs).swap(*this);
        return *this;
    }

    constexpr intrusive_ptr& operator=(intrusive_ptr&& rhs) {
        if (this == &rhs) {
            return *this;
        }
        px = rhs.detach();
        return *this;
    }

    constexpr intrusive_ptr & operator=(T* rhs) {
        this_type(rhs).swap(*this);
        return *this;
    }

    void reset() {
        this_type().swap(*this);
    }

    void reset(T * rhs) {
        this_type(rhs).swap(*this);
    }

    constexpr T* get() const {
        return px;
    }

    constexpr T* detach() {
        T* ret = px;
        px = nullptr;
        return ret;
    }

    constexpr operator bool () const {
        return px != nullptr;
    }

    constexpr T & operator*() const {
        chaindb_assert(px != nullptr, "px != nullptr" );
        return *px;
    }

    constexpr T* operator->() const {
        chaindb_assert(px != nullptr, "px != nullptr");
        return px;
    }

    void swap(intrusive_ptr & rhs) {
        T* tmp = px;
        px = rhs.px;
        rhs.px = tmp;
    }

private:
    T * px = nullptr;
}; // class intrusive_ptr


namespace _detail {
    template <typename T> constexpr T& min(T& a, T& b) {
        return a > b ? b : a;
    }

    template<int I, int Max> struct comparator_helper {
        template<typename L, typename V>
        bool operator()(const L& left, const V& value) const {
            return std::get<I>(left) == std::get<I>(value) &&
                comparator_helper<I + 1, Max>()(left, value);
        }
    }; // struct comparator_helper

    template<int I> struct comparator_helper<I, I> {
        template<typename... L> bool operator()(L&&...) const { return true; }
    }; // struct comparator_helper

    template<int I, int Max> struct converter_helper {
        template<typename L, typename V> void operator()(L& left, const V& value) const {
            std::get<I>(left) = std::get<I>(value);
            converter_helper<I + 1, Max>()(left, value);
        }
    }; // struct converter_helper

    template<int I> struct converter_helper<I, I> {
        template<typename... L> void operator()(L&&...) const { }
    }; // struct converter_helper
} // namespace _detail

template<typename Key>
struct key_comparator {
    constexpr static bool compare_eq(const Key& left, const Key& right) {
        return left == right;
    }
}; // struct key_comparator

template<typename... Indices>
struct key_comparator<std::tuple<Indices...>> {
    using key_type = std::tuple<Indices...>;

    template<typename Value> constexpr static bool compare_eq(const key_type& left, const Value& value) {
        return std::get<0>(left) == value;
    }

    template<typename... Values> constexpr static bool compare_eq(const key_type& left, const std::tuple<Values...>& value) {
        using value_type = std::tuple<Values...>;
        using namespace _detail;

        return comparator_helper<0, min(std::tuple_size<value_type>::value, std::tuple_size<key_type>::value)>()(left, value);
    }
}; // struct key_comparator

template<typename Key>
struct key_converter {
    constexpr static Key convert(const Key& key) {
        return key;
    }
}; // struct key_converter

template<typename... Indices>
struct key_converter<std::tuple<Indices...>> {
    using key_type = std::tuple<Indices...>;

    template<typename Value> constexpr static key_type convert(const Value& value) {
        key_type index;
        std::get<0>(index) = value;
        return index;
    }

    template<typename... Values> constexpr static key_type convert(const std::tuple<Values...>& value) {
        using namespace _detail;
        using value_type = std::tuple<Values...>;

        key_type index;
        converter_helper<0, min(std::tuple_size<value_type>::value, std::tuple_size<key_type>::value)>()(index, value);
        return index;
    }
}; // struct key_converter

struct service_info {
    eosio::name payer;
    int  size   = 0;
    bool in_ram = false;
}; // struct service_info

template<typename T, typename MultiIndex>
struct multi_index_item: public T {
    template<typename Constructor>
    multi_index_item(const MultiIndex& midx, Constructor&& constructor)
    : code_(midx.code()),
      scope_(midx.scope()) {
        constructor(*this);
    }

    const account_name_t code_;
    const scope_t scope_ = 0;
    service_info  service_;

    bool deleted_ = false;
    int ref_cnt_ = 0;
}; // struct multi_index_item

template <typename T, typename MultiIndex>
inline void intrusive_ptr_add_ref(eosio::multi_index_item<T, MultiIndex>* obj) {
    ++obj->ref_cnt_;
}

template <typename T, typename MultiIndex>
inline void intrusive_ptr_release(eosio::multi_index_item<T, MultiIndex>* obj) {
    --obj->ref_cnt_;
    if (!obj->ref_cnt_) delete obj;
}

template<index_name_t IndexName, typename Extractor>
struct indexed_by {
    enum constants { index_name = static_cast<uint64_t>(IndexName) };
    typedef Extractor extractor_type;
};

struct primary_key_extractor {
    template<typename T> constexpr primary_key_t operator()(const T& o) const {
        return o.primary_key();
    }
}; // struct primary_key_extractor

template<typename O> void pack_object(const O& o, char* data, const size_t size) {
    datastream<char*> ds(data, size);
    ds << o;
}

template<typename O> void unpack_object(O& o, const char* data, const size_t size) {
    datastream<const char*> ds(data, size);
    ds >> o;
}

template<typename Size, typename Lambda>
void safe_allocate(const Size size, const char* error_msg, Lambda&& callback) {
    chaindb_assert(size > 0, error_msg);

    constexpr static size_t max_stack_data_size = 512;

    struct allocator {
        char* data = nullptr;
        const bool use_malloc;
        const size_t size;
        allocator(const size_t s)
            : use_malloc(s > max_stack_data_size), size(s) {
            if (use_malloc) data = static_cast<char*>(malloc(s));
        }

        ~allocator() {
            if (use_malloc) free(data);
        }
    } alloc(static_cast<size_t>(size));

    if (!alloc.use_malloc) alloc.data = static_cast<char*>(alloca(alloc.size));
    chaindb_assert(alloc.data != nullptr, "unable to allocate memory");

    callback(alloc.data, alloc.size);
}

template<eosio::name::raw TableName, eosio::name::raw IndexName> struct lower_bound final {
    template<typename Key>
    cursor_t operator()(account_name_t code, scope_t scope, const Key& key) const {
        cursor_t cursor;
        safe_allocate(pack_size(key), "Invalid size of key on lower_bound", [&](auto& data, auto& size) {
            pack_object(key, data, size);
            cursor = internal_use_do_not_use::chaindb_lower_bound(code, scope, TableName, IndexName, data, size);
        });
        return cursor;
    }
}; // struct lower_bound

template<eosio::name::raw TableName> struct lower_bound<TableName, "primary"_n> final {
    cursor_t operator()(account_name_t code, scope_t scope, const primary_key_t pk) const {
        return internal_use_do_not_use::chaindb_lower_bound_pk(code, scope, TableName, pk);
    }
}; // struct lower_bound

template<eosio::name::raw TableName, eosio::name::raw IndexName> struct upper_bound final {
    template<typename Key>
    cursor_t operator()(account_name_t code, scope_t scope, const Key& key) const {
        cursor_t cursor;
        safe_allocate(pack_size(key), "Invalid size of key on upper_bound", [&](auto& data, auto& size) {
            pack_object(key, data, size);
            cursor = internal_use_do_not_use::chaindb_upper_bound(code, scope, TableName, IndexName, data, size);
        });
        return cursor;
    }
}; // struct upper_bound

template<eosio::name::raw TableName> struct upper_bound<TableName, "primary"_n> final {
    cursor_t operator()(account_name_t code, scope_t scope, const primary_key_t pk) const {
        return internal_use_do_not_use::chaindb_upper_bound_pk(code, scope, TableName, pk);
    }
}; // struct upper_bound

/**
 * @defgroup multiindex Multi Index Table
 * @ingroup contracts
 * @brief Defines CyberWay Multi Index Table
 *
 * @details CyberWay Multi-Index table, like EOSIO, requires exactly a uint64_t primary key. For the table to be able to retrieve the primary key, the object stored inside the table is required to have a const member function called primary_key() that returns uint64_t.
 * Multi-Index table also supports up to 16 secondary indices.
 */
template<eosio::name::raw TableName, typename T, typename... Indices>
class multi_index {
private:
    static_assert(sizeof...(Indices) <= 16, "multi_index only supports a maximum of 16 secondary indices");

    constexpr static bool validate_table_name(table_name_t n) {
        // Limit table names to 12 characters so that
        // the last character (4 bits) can be used to distinguish between the secondary indices.
        return (static_cast<uint64_t>(n) & 0x000000000000000FULL) == 0;
    }

    static_assert(
        validate_table_name(TableName),
        "multi_index does not support table names with a length greater than 12");

    using item = multi_index_item<T, multi_index>;
    using item_ptr = intrusive_ptr<item>;
    using primary_key_extractor_type = primary_key_extractor;

    template<index_name_t IndexName, typename Extractor>
    struct iterator_extractor_impl {
        template<typename Iterator>
        auto operator()(const Iterator& itr) const {
            return Extractor()(*itr);
        }
    }; // iterator_extractor_impl

    template<typename E>
    struct iterator_extractor_impl<"primary"_n, E> {
        template<typename Iterator>
        const primary_key_t& operator()(const Iterator& itr) const {
            return itr.primary_key_;
        }
    }; // iterator_extractor_impl

    template<index_name_t IndexName, typename Extractor> struct index;

    const account_name_t code_;
    const scope_t scope_;

    mutable primary_key_t next_primary_key_ = end_primary_key;

    struct cache_map_t_ {
        std::vector<item_ptr> vector;
        std::map<primary_key_t, item_ptr> map;

        item_ptr find(const primary_key_t pk) {
            auto mtr = map.find(pk);
            if (map.end() != mtr) {
                return mtr->second;
            }

            auto vtr = std::find_if(vector.rbegin(), vector.rend(), [&pk](const auto& itm) {
                return primary_key_extractor_type()(*itm) == pk;
            });
            if (vector.rend() != vtr) {
                return (*vtr);
            }
            return item_ptr();
        }

        void insert(item_ptr ptr) {
            if (vector.size() >= 8) {
                map.emplace(primary_key_extractor_type()(*ptr), ptr);
            } else {
                vector.push_back(ptr);
            }
        }

        void remove(primary_key_t pk) {
            auto mtr = map.find(pk);
            if (map.end() != mtr) {
                mtr->second->deleted_ = true;
                map.erase(mtr);
                return;
            }

            auto vtr = std::find_if(vector.rbegin(), vector.rend(), [&pk](const auto& itm) {
                return primary_key_extractor_type()(*itm) == pk;
            });
            if (vector.rend() != vtr) {
                (*vtr)->deleted_ = true;
                vector.erase(--(vtr.base()));
            }
        }

        void clear() {
            for (auto& itm_ptr: vector) {
                itm_ptr->deleted_ = true;
            }
            vector.clear();

            for (auto& itr: map) {
                itr->second->deleted_ = true;
            }
            map.clear();
        }
    }; // struct cache_map_t_

    using cache_vector_t_ = std::vector<item_ptr>;
    struct cache_item_t_ {
        const account_name_t code;
        const scope_t scope;
        cache_map_t_ items_map;

        cache_item_t_(const account_name_t code, const scope_t scope)
        : code(code), scope(scope) {
        }
    };

    static cache_map_t_& get_items_map(const account_name_t code, const scope_t scope) {
        static std::deque<cache_item_t_> cache_map;
        for (auto& itm: cache_map) {
            if (itm.code == code && itm.scope == scope) return itm.items_map;
        }
        cache_map.push_back({code, scope});
        return cache_map.back().items_map;
    }

    cache_map_t_& items_map_;

    template<index_name_t IndexName>
    struct const_iterator_impl: public std::iterator<std::bidirectional_iterator_tag, const T> {
    public:
        using pointer   = const T*;
        using reference = const T&;

    public:
        constexpr friend bool operator == (const const_iterator_impl& a, const const_iterator_impl& b) {
            if (a.cursor_ != uninitialized_find_by_pk && a.cursor_ == b.cursor_) return true;
            a.lazy_open();
            b.lazy_open();
            return a.primary_key_ == b.primary_key_;
        }
        constexpr friend bool operator != (const const_iterator_impl& a, const const_iterator_impl& b) {
            return !(operator == (a, b));
        }

        constexpr static table_name_t table_name()       { return TableName;         }
        constexpr static index_name_t index_name()       { return IndexName;         }
        constexpr account_name_t      code()       const { return multidx().code();  }
        constexpr scope_t             scope()      const { return multidx().scope(); }

        constexpr reference operator*() const {
            return *this->operator->();
        }
        constexpr pointer operator->() const {
            lazy_load_object();
            return static_cast<pointer>(item_.get());
        }
        constexpr primary_key_t pk() const {
            lazy_open();
            return primary_key_;
        }
        constexpr int size() const {
            lazy_load_object();
            return item_.get()->service_.size;
        }
        constexpr const eosio::name& payer() const {
            lazy_load_object();
            return item_.get()->service_.payer;
        }
        constexpr bool in_ram() const {
            lazy_load_object();
            return item_.get()->service_.in_ram;
        }

        const_iterator_impl operator++(int) {
            const_iterator_impl result(*this);
            this->operator++();
            return result;
        }
        const_iterator_impl& operator++() {
            lazy_open();
            chaindb_assert(primary_key_ != end_primary_key, "cannot increment end iterator");
            primary_key_ = internal_use_do_not_use::chaindb_next(code(), cursor_);
            item_.reset();
            return *this;
        }

        const_iterator_impl operator--(int) {
            const_iterator_impl result(*this);
            this->operator++();
            return result;
        }
        const_iterator_impl& operator--() {
            lazy_open();
            primary_key_ = internal_use_do_not_use::chaindb_prev(code(), cursor_);
            item_.reset();
            chaindb_assert(primary_key_ != end_primary_key, "out of range on decrement of iterator");
            return *this;
        }

        constexpr const_iterator_impl() = default;

        constexpr const_iterator_impl(const_iterator_impl&& src) {
            this->operator=(std::move(src));
        }
        constexpr const_iterator_impl& operator=(const_iterator_impl&& src) {
            if (this == &src) {
                return *this;
            }

            multidx_ = src.multidx_;
            cursor_ = src.cursor_;
            primary_key_ = src.primary_key_;
            item_ = src.item_;

            src.cursor_ = uninitialized_state;
            src.primary_key_ = end_primary_key;
            src.item_.reset();

            return *this;
        }

        constexpr const_iterator_impl(const const_iterator_impl& src) {
            this->operator=(src);
        }
        constexpr const_iterator_impl& operator=(const const_iterator_impl& src) {
            if (this == &src) {
                return *this;
            }

            multidx_ = src.multidx_;

            cursor_  = src.cursor_;
            if (src.is_cursor_initialized()) {
                src.cursor_ = internal_use_do_not_use::chaindb_clone(code(), cursor_);
            }

            primary_key_ = src.primary_key_;
            item_ = src.item_;

            return *this;
        }

        ~const_iterator_impl() {
            if (is_cursor_initialized()) {
                internal_use_do_not_use::chaindb_close(code(), cursor_);
            }
        }

    private:
        friend multi_index;
        template<index_name_t, typename> friend struct iterator_extractor_impl;

        constexpr const_iterator_impl(const multi_index* midx, const cursor_t cursor)
        : multidx_(midx), cursor_(cursor) {
            if (is_cursor_initialized()) {
                primary_key_ = internal_use_do_not_use::chaindb_current(code(), cursor_);
            }
        }

        constexpr const_iterator_impl(const multi_index* midx, const cursor_t cursor, const primary_key_t pk, item_ptr item)
        : multidx_(midx), cursor_(cursor), primary_key_(pk), item_(std::move(item)) {
        }

        const multi_index* multidx_ = nullptr;
        mutable cursor_t cursor_ = uninitialized_state;
        mutable primary_key_t primary_key_ = end_primary_key;
        mutable item_ptr item_;

        void lazy_load_object() const {
            if (item_ && !item_->deleted_) {
                return;
            }

            lazy_open();
            chaindb_assert(primary_key_ != end_primary_key, "cannot load object from end iterator");
            item_ = multidx_->load_object(cursor_, primary_key_);
        }

        void lazy_open() const {
            if (is_cursor_initialized()) {
                return;
            }

            switch (cursor_) {
                case uninitialized_begin:
                    lazy_open_begin();
                    break;

                case uninitialized_end:
                    lazy_open_end();
                    break;

                case uninitialized_find_by_pk:
                    lazy_open_find_by_pk();
                    break;

                default:
                    break;
            }
            chaindb_assert(is_cursor_initialized(), "unable to open cursor");
        }

        constexpr bool is_cursor_initialized() const {
            return cursor_ > uninitialized_state;
        }

        void lazy_open_begin() const {
            cursor_ = internal_use_do_not_use::chaindb_begin(code(), scope(), table_name(), index_name());
            chaindb_assert(is_cursor_initialized(), "unable to open begin iterator");
            primary_key_ = internal_use_do_not_use::chaindb_current(code(), cursor_);
        }

        void lazy_open_end() const {
            cursor_ = internal_use_do_not_use::chaindb_end(code(), scope(), table_name(), index_name());
            chaindb_assert(is_cursor_initialized(), "unable to open end iterator");
        }

        void lazy_open_find_by_pk() const {
            cursor_ = internal_use_do_not_use::chaindb_lower_bound_pk(code(), scope(), table_name(), primary_key_);
            chaindb_assert(is_cursor_initialized(), "unable to open find_by_pk iterator");

            auto pk = internal_use_do_not_use::chaindb_current(code(), cursor_);
            chaindb_assert(primary_key_ == pk, "primary key from cursor does not equal expected");
        }

        constexpr const multi_index& multidx() const {
            chaindb_assert(multidx_ != nullptr, "iterator is not intitialized");
            return *multidx_;
        }

    private:
        static constexpr cursor_t uninitialized_state = 0;
        static constexpr cursor_t uninitialized_end = -1;
        static constexpr cursor_t uninitialized_begin = -2;
        static constexpr cursor_t uninitialized_find_by_pk = -3;
    }; /// struct multi_index::const_iterator_impl

    template<index_name_t IndexName>
    struct const_reverse_iterator_impl
        : public std::iterator<
            typename std::iterator_traits<const_iterator_impl<IndexName>>::iterator_category,
            typename std::iterator_traits<const_iterator_impl<IndexName>>::value_type,
            typename std::iterator_traits<const_iterator_impl<IndexName>>::difference_type,
            typename std::iterator_traits<const_iterator_impl<IndexName>>::pointer,
            typename std::iterator_traits<const_iterator_impl<IndexName>>::reference>
    {
    public:
        using const_iterator_type = const_iterator_impl<IndexName>;
        using iterator_type = const_iterator_type;

    private:
        using traits_type_ = std::iterator_traits<iterator_type>;

    public:
        using difference_type = typename traits_type_::difference_type;
        using pointer         = typename traits_type_::pointer;
        using reference       = typename traits_type_::reference;

    public:
        constexpr friend bool operator == (const const_reverse_iterator_impl& a, const const_reverse_iterator_impl& b) {
            return a.pos_ == b.pos_;
        }
        constexpr friend bool operator != (const const_reverse_iterator_impl& a, const const_reverse_iterator_impl& b) {
            return !(operator == (a, b));
        }

        constexpr static table_name_t   table_name()       { return TableName;    }
        constexpr static index_name_t   index_name()       { return IndexName;    }
        constexpr        account_name_t code()       const { return pos_.code();  }
        constexpr        scope_t        scope()      const { return pos_.scope(); }

        constexpr const_reverse_iterator_impl() noexcept = default;

        constexpr explicit const_reverse_iterator_impl(iterator_type src)
        : pos_(src) {
        }

        constexpr const_reverse_iterator_impl(const const_reverse_iterator_impl& src) {
            this->operator=(src);
        }
        const const_reverse_iterator_impl& operator=(const const_reverse_iterator_impl& src) {
            if (this != &src) {
                pos_   = src.pos_;
                value_ = src.value_;
            }
            return *this;
        }

        constexpr const_reverse_iterator_impl(const_reverse_iterator_impl&& src) {
            this->operator=(std::move(src));
        }
        constexpr const const_reverse_iterator_impl& operator=(const_reverse_iterator_impl&& src) {
            if (this != &src) {
                pos_   = std::move(src.pos_);
                value_ = std::move(src.value_);
            }
            return *this;
        }

        constexpr const const_iterator_type& base() const {
            return pos_;
        }

        constexpr reference operator*() const {
            lazy_init_value();
            return (*value_).operator*();
        }

        constexpr pointer operator->() const {
            lazy_init_value();
            return (*value_).operator->();
        }

        const_reverse_iterator_impl& operator++() {
            --pos_;
            if (value_.has_value()) {
                --(*value_);
            }
            return *this;
        }

        const_reverse_iterator_impl operator++(int) {
            auto tmp = *this;
            this->operator++();
            return tmp;
        }

        const_reverse_iterator_impl& operator--() {
            ++pos_;
            if (value_.has_value()) {
                ++(*value_);
            }
            return *this;
        }

        const_reverse_iterator_impl operator--(int) {
            auto tmp = *this;
            this->operator--();
            return tmp;
        }

    private:
        const_iterator_type pos_;
        mutable std::optional<const_iterator_type> value_;

    private:
        void lazy_init_value() const {
            if (!value_.has_value()) {
                value_.emplace(pos_);
                --(*value_);
            }
        }
    }; // struct multi_index::const_reverse_iterator_impl

    template<index_name_t IndexName, typename Extractor>
    struct index {
    public:
        using pointer   = const T*;
        using reference = const T&;
        using extractor_type = Extractor;
        using iterator_extractor_type = iterator_extractor_impl<IndexName, Extractor>;
        using key_type = typename std::decay<decltype(Extractor()(static_cast<const T&>(*(const T*)nullptr)))>::type;
        using const_iterator = const_iterator_impl<IndexName>;
        using const_reverse_iterator = const_reverse_iterator_impl<IndexName>;

        constexpr static bool validate_index_name(index_name_t n) {
           return static_cast<uint64_t>(n) != 0;
        }

        static_assert(validate_index_name(IndexName), "invalid index name used in multi_index");

        /**
        *  The @ref table_name method is used to get the name of a table.
        *  "multi_index does not support table names with a length greater than 12"
        *  @ingroup multiindex
        *
        *  @return A table name.
        *  @note multi_index does not support table names with a length greater than 12.
        */
        constexpr static table_name_t   table_name()       { return TableName;         }

        /**
        *  The index_name method is used to obtain the name of index when a certain table field is selected (similar to getting a column name in a table).
        *  @ingroup multiindex
        *
        *  @return An index name.
        */
        constexpr static index_name_t   index_name()       { return IndexName;         }

        /**
        *  The get_code method is used to get the account name that owns a primare table.
        *  @ingroup multiindex
        *
        *  @return Account name of the code that owns the primary table.
        */
        constexpr        account_name_t code()       const { return multidx_->code();  }

        /**
        *  The get_scope method is used to get `scope` member property.
        *  @ingroup multiindex
        *
        *  @return Identifier of the scope within a code of the current receiver under which the desired primary table instance can be found.
        */
        constexpr        scope_t        scope()      const { return multidx_->scope(); }

        /**
        *  The @ref cbegin method is used to set an iterator to the beginning of the Multi-Index table. If the table is empty, the returned iterator will be equal to @ref cend().
        *  @ingroup multiindex
        *
        *  @return An iterator pointing to the first object with the lowest primary key value in the Multi-Index table.
        *
        *  The following figure shows location of the items in a Table.
        *  @image html std-cbegin.png
        */
        constexpr const const_iterator& cbegin() const {
            if (!cbegin_.has_value()) {
                cbegin_.emplace(const_iterator(multidx_, const_iterator::uninitialized_begin));
            }
            return *cbegin_;
        }

        /**
        *  The @ref begin method is used to set an iterator to the beginning of the container. If the container is empty, the returned iterator will be equal to @ref end().
        *  @ingroup multiindex
        *
        *  @return An iterator pointing to the first object with the lowest primary key value in the Multi-Index table.
        */
        constexpr const const_iterator& begin() const {
            return cbegin();
        }

        /**
        *  The @ref cend method is used to set an iterator to the object following the last object of the container.
        *  @ingroup multiindex
        *
        *  @return An iterator pointing to the first object with the highest primary key value in the Multi-Index table.
        */
        constexpr const const_iterator& cend() const {
            if (!cend_.has_value()) {
                cend_.emplace(const_iterator(multidx_, const_iterator::uninitialized_end));
            }
            return *cend_;
        }

        /**
        *  The @ref cend method is used to set an iterator to the object following the last object of the container.
        *  @ingroup multiindex
        *
        *  @return An iterator pointing to the first object with the highest primary key value in the Multi-Index table.
        */
        constexpr const const_iterator& end() const{
            return cend();
        }

        /**
        *  The @ref crbegin method is used to set reverse iterator to the beginning of the reversed container. The reversed container objects are arranged in reverse order. It corresponds to the last object of the non-reversed container. If the container is empty, the returned iterator is equal to @ref crend().
        *  @ingroup multiindex
        *
        *  @return An iterator pointing to the first object with the lowest primary key value in the Multi-Index table.
        */
        constexpr const const_reverse_iterator& crbegin() const {
            if (!crbegin_.has_value()) {
                crbegin_.emplace(const_iterator(multidx_, const_iterator::uninitialized_end));
            }
            return *crbegin_;
        }

        /**
        *  The @ref rbegin method is used to set reverse iterator to the beginning of the reversed container. The reversed container objects are arranged in reverse order. It corresponds to the last object of the non-reversed container. If the container is empty, the returned iterator is equal to rend().
        *  @ingroup multiindex
        *
        *  @return An iterator pointing to the first object with the lowest primary key value in the Multi-Index table.
        */
        constexpr const const_reverse_iterator& rbegin() const {
            return crbegin();
        }

        /**
        *  Returns an iterator pointing to the first object with the lowest primary key value in the Multi-Index table.
        *  @ingroup multiindex
        *
        *  @return An iterator pointing to the first object with the lowest primary key value in the Multi-Index table.
        *
        *  @details Both @ref rend and @ref crend returns a reverse iterator to the object following the last object of the reversed container. It corresponds to the object preceding the first object of the non-reversed container. This object acts as a placeholder, attempting to access it results in undefined behavior. Reverse iterator stores an iterator to the next object than the one it actually reverse to.
        *
        *  The following figure shows location of the last items in containers with forward and reverse sequences.
        *  @image html std-crend.png
        */
        constexpr const const_reverse_iterator& crend() const {
            if (!crend_.has_value()) {
                crend_.emplace(const_iterator(multidx_, const_iterator::uninitialized_begin));
            }
            return *crend_;
        }

        /**
        *  Returns an iterator pointing to the first object with the lowest primary key value in the Multi-Index table.
        *  @ingroup multiindex
        *
        *  @return An iterator pointing to the first object with the lowest primary key value in the Multi-Index table.
        */
        constexpr const const_reverse_iterator& rend() const {
            return crend();
        }

        /**
        *  The @ref find method is used to search for an existing object (in a table) equal to value specified in the parameter.
        *  @ingroup multiindex
        *
        *  @param value - the value to compare the objects to
        *
        *  @return An iterator to the found object whose value is equal to that specified in the parameter OR past-the-end iterator of the referenced table if an object is not found.
        */
        template<typename Value> const_iterator find(const Value& value) const {
            auto key = key_converter<key_type>::convert(value);
            auto itr = lower_bound(key);
            if (itr == cend()) {
                return cend();
            }
            if (!key_comparator<key_type>::compare_eq(iterator_extractor_type()(itr), value)) {
                return cend();
            }
            return itr;
        }

        /**
        *  The @ref find method is used to search for an existing object in a table using its primary key.
        *  @ingroup multiindex
        *
        *  @param key - primary key value of the object
        *
        *  @return An iterator to the found object with key equivalent to key specified in the parameter OR past-the-end iterator of the referenced table if an object is not found.
        */
        const_iterator find(const key_type& key) const {
           auto itr = lower_bound(key);
           if (itr == cend()) {
               return cend();
           }
           if (key != iterator_extractor_type()(itr)) {
               return cend();
           }
           return itr;
        }

        /**
        *  The @ref require_find method is used to search for an existing object in a table using its primary key. This method is similar to @ref find, but unlike the latter, it issues an error message if an object is not found.
        *  @ingroup multiindex
        *
        *  @param key - primary key value of the object
        *  @param error_msg - error message if an object with primary key value is not found
        *
        *  @return An iterator to the found object which has a primary key equal to specified one OR throws an exception with the error message if an object is not found.
        */
        const_iterator require_find(const key_type& key, const char* error_msg = "unable to find key") const {
            auto itr = lower_bound(key);
            chaindb_assert(itr != cend(), error_msg);
            chaindb_assert(key == iterator_extractor_type()(itr), error_msg);
            return itr;
        }

        /**
        *  The get method is used to retrieve an existing object from a table using its primary key. This method is similar to @ref require_find, but unlike the latter, it returns not an iterator for the found object, but the value of the found object.
        *  @ingroup multiindex
        *
        *  @param key - primary key value of the object
        *  @param error_msg - error message if an object with primary key value is not found
        *
        *  @return A constant reference to the object containing the specified primary key OR throws an exception with the error message if an object is not found.
        */
        reference get(const key_type& key, const char* error_msg = "unable to find key") const {
            auto itr = find(key);
            chaindb_assert(itr != cend(), error_msg);
            return *itr;
        }

        /**
        *  The @ref lower_bound method is used to set an iterator to the object whose value is not less than specified by a key.
        *  @ingroup multiindex
        *
        *  @param key - key value to compare the objects to
        *
        *  @return An iterator pointing to the first object that has the lowest primary key that is greater than or equal to @a key. If no such object is found, a past-the-end iterator is returned.
        */
        const_iterator lower_bound(const key_type& key) const {
            eosio::lower_bound<TableName, IndexName> finder;
            auto cursor = finder(code(), scope(), key);
            return const_iterator(multidx_, cursor);
        }

        /**
        *  The @ref lower_bound method is used to set an iterator to the object whose value is not less than specified by a parameter.
        *  @ingroup multiindex
        *
        *  @param value - the value to compare the objects to
        *
        *  @return An iterator pointing to the first object that compares greater or equal to the parameter @a value. If no such object is found, a past-the-end iterator is returned.
        */
        template<typename Value> const_iterator lower_bound(const Value& value) const {
            return lower_bound(key_converter<key_type>::convert(value));
        }

        /**
        *  The @ref upper_bound method is used to set an iterator to the object whose value is greater than specified by a key.
        *  @ingroup multiindex
        *
        *  @param key - key value to compare the objects to
        *  @return An iterator pointing to the first object that is greater than specified by the key value. If no such object is found, a past-the-end iterator is returned.
        */
        const_iterator upper_bound(const key_type& key) const {
            eosio::upper_bound<TableName, IndexName> finder;
            auto cursor = finder(code(), scope(), key);
            return const_iterator(multidx_, cursor);
        }

        /**
        *  The @ref iterator_to method is used to find a location of an object in the Multi-Index table by the specified value.
        *  @ingroup multiindex
        *
        *  @param obj - a reference to the desired object
        *
        *  @return An iterator to the given object in the Multi-Index table OR throws an exception with error message if an object is not found.
        */
        const_iterator iterator_to(reference obj) const {
            const auto& itm = static_cast<const item&>(obj);
            chaindb_assert(multidx_->is_same_multidx(itm), "object passed to iterator_to is not in multi_index");

            auto key = extractor_type()(itm);
            auto pk = primary_key_extractor_type()(itm);
            cursor_t cursor;
            safe_allocate(pack_size(key), "invalid size of key", [&](auto& data, auto& size) {
                pack_object(key, data, size);
                cursor = internal_use_do_not_use::chaindb_locate_to(
                    code(), scope(), table_name(), index_name(), pk, data, size);
            });

            return const_iterator(multidx_, cursor, pk, item_ptr(const_cast<item*>(&itm), false));
        }

        /**
        *  The @ref modify method is used to update an object in a table (the object is searched by the given iterator).
        *  @ingroup multiindex
        *
        *  @param itr - an iterator pointing to the object to be updated
        *  @param payer - account name of the payer for the Storage usage of the updated row
        *  @param updater - lambda function that updates the target object
        *
        *  @pre Iterator points to an existing object.
        *  @pre Payer is a valid account that is authorized to execute the action and be billed for storage usage.
        *  @post The modified object is serialized, then replaces the existing object in the table.
        *  @post The primary key of the updated object is not changed.
        *  @post The payer is charged for the storage usage of the updated object.
        *  @post If payer is the same as the existing payer, payer only pays for the usage difference between existing and updated object (and is refunded if this difference is negative).
        *  @post If payer is different from the existing payer, the existing payer is refunded for the storage usage of the existing object.
        *
        *  > @b Exception  
        *  > Execution will be aborted if the method is called with an invalid precondition.
        */
        template<typename Lambda>
        void modify(const const_iterator& itr, account_name_t payer, Lambda&& updater) const {
            chaindb_assert(itr != cend(), "cannot pass end iterator to modify");
            multidx_->modify(*itr, payer, std::forward<Lambda&&>(updater));
        }


        /**
        *  The @ref erase method is used to remove an existing object from a table (the object is searched by the given iterator).
        *  @ingroup multiindex
        *
        *  @param itr - an iterator pointing to the object to be removed
        *  @param payer - account name of the payer for the storage usage
        *
        *  @pre The itr parameter points to an existing object.
        *  @pre A table, from which the object is removed, does not belong to another contract.
        *  @post The object is removed from the table and all associated storage is reclaimed.
        *
        *  @return For the signature with `const_iterator`, returns a pointer to the object following the removed object. If an object to be removed is the last one in the table, then the return value is similar to execution of @ref end().
        *
        *  > @b Exception  
        *  > The object to be removed is not in the table.
        */
        const_iterator erase(const_iterator itr, const account_name_t payer = eosio::name()) const {
            chaindb_assert(itr != cend(), "cannot pass end iterator to erase");
            const auto& obj = *itr;
            ++itr;
            multidx_->erase(obj, payer);
            return itr;
        }

        /**
        *  The extract_key method is used to obtain the primary key of an object by a given value.
        *  @ingroup multiindex
        *
        *  @param obj - a reference to the desired object
        *
        *  @return Primary key of the object corresponding to the specified value of `obj`.
        */
        static auto extract_key(reference obj) {
            return extractor_type()(obj);
        }

    private:
        friend class multi_index;

        index(const multi_index* midx)
        : multidx_(midx) { }

        const multi_index* const multidx_;

        mutable std::optional<const_iterator>         cbegin_;
        mutable std::optional<const_iterator>         cend_;
        mutable std::optional<const_reverse_iterator> crbegin_;
        mutable std::optional<const_reverse_iterator> crend_;
    }; /// struct multi_index::index

    using indices_type = decltype(boost::hana::tuple<Indices...>());

    indices_type indices_;

    index<"primary"_n, primary_key_extractor> primary_idx_;

    item_ptr find_object_in_cache(const primary_key_t pk) const {
        return items_map_.find(pk);
    }

    void add_object_to_cache(item_ptr ptr) const {
        items_map_.insert(ptr);
    }

    void remove_object_from_cache(const primary_key_t pk) const {
        items_map_.remove(pk);
    }

    item_ptr load_object(const cursor_t cursor, const primary_key_t pk) const {
        auto ptr = find_object_in_cache(pk);
        if (ptr) {
            return std::move(ptr);
        }

        auto size = internal_use_do_not_use::chaindb_datasize(code(), cursor);

        safe_allocate(size, "object doesn't exist", [&](auto& data, auto& datasize) {
            auto dpk = internal_use_do_not_use::chaindb_data(code(), cursor, data, datasize);
            chaindb_assert(dpk == pk, "invalid packet object");
            ptr = item_ptr(new item(*this, [&](auto& itm) {
                T& obj = static_cast<T&>(itm);
                unpack_object(obj, data, datasize);
            }));
        });

        auto ptr_pk = primary_key_extractor_type()(*ptr);
        chaindb_assert(ptr_pk == pk, "invalid primary key of object");

        safe_allocate(sizeof(service_info), "object doesn't exist", [&](auto& data, auto& datasize) {
            internal_use_do_not_use::chaindb_service(code(), cursor, data, datasize);
            unpack_object(ptr->service_, data, datasize);
        });

        add_object_to_cache(ptr);
        return std::move(ptr);
    }

    constexpr bool is_same_multidx(const item& o) const {
        return (o.code_ == code() && o.scope_ == scope());
    }

public:
    using pointer   = const T*;
    using reference = const T&;
    using const_iterator = const_iterator_impl<"primary"_n>;
    using const_reverse_iterator = const_reverse_iterator_impl<"primary"_n>;

public:
    multi_index(const account_name_t code, const scope_t scope)
    : code_(code), scope_(scope), primary_idx_(this), items_map_(get_items_map(code, scope)) {
    }

    constexpr static table_name_t table_name()       { return TableName; }
    constexpr        account_name_t code()     const { return code_; }
    constexpr        scope_t        scope()    const { return scope_; }

    constexpr const const_iterator& cbegin() const { return primary_idx_.cbegin(); }
    constexpr const const_iterator&  begin() const { return cbegin();              }

    constexpr const const_iterator& cend()   const  { return primary_idx_.cend();  }
    constexpr const const_iterator&  end()   const  { return cend();               }

    constexpr const const_reverse_iterator& crbegin() const { return primary_idx_.crbegin(); }
    constexpr const const_reverse_iterator&  rbegin() const { return crbegin();              }

    constexpr const const_reverse_iterator& crend() const   { return primary_idx_.crend();   }
    constexpr const const_reverse_iterator&  rend() const   { return crend();                }

    const_iterator lower_bound(primary_key_t pk) const {
        return primary_idx_.lower_bound(pk);
    }

    const_iterator upper_bound(primary_key_t pk) const {
        return primary_idx_.upper_bound(pk);
    }

    primary_key_t available_primary_key() const {
        if (next_primary_key_ == end_primary_key) {
            next_primary_key_ = internal_use_do_not_use::chaindb_available_primary_key(code(), scope(), table_name());
            chaindb_assert(next_primary_key_ != end_primary_key, "no available primary key");
        }
        return next_primary_key_;
    }

    template<index_name_t IndexName>
    constexpr auto get_index() const {
        namespace hana = boost::hana;

        auto res = hana::find_if(indices_, [](auto&& in) {
            return std::integral_constant<
                bool, std::decay<decltype(in)>::type::index_name == static_cast<uint64_t>(IndexName)>();
        });

        static_assert(res != hana::nothing, "name provided is not the name of any secondary index within multi_index");

        return index<IndexName, typename std::decay<decltype(res.value())>::type::extractor_type>(this);
    }

    const_iterator iterator_to(reference obj) const {
        return primary_idx_.iterator_to(obj);
    }

    void flush_cache() {
        items_map_.clear();
    }

    /**
    *  The @ref emplace is used to insert a new object (i.e., row) into the table.
    *  @ingroup multiindex
    *
    *  @param payer - account name of the payer for the storage usage of the new object
    *  @param constructor - lambda function that does an in-place initialization of the object to be created in the table. A modified link is passed to this function to initialize the object.
    *
    *  @pre A multi index table has been instantiated.
    *  @post A new object is created in the Multi-Index table, with a unique primary key (as specified in the object). The object is serialized and written to the table. If the table does not exist, it is created.
    *  @post The payer is charged for the storage usage of the new object and, if the table must be created, for the overhead of the table creation.
    *
    *  @return A primary key iterator to the newly created object.
    *
    *  > @b Exception  
    *  > An object to be created in a table of another contract.
    */
    template<typename Lambda>
    const_iterator emplace(const account_name_t payer, Lambda&& constructor) const {
        // Quick fix for mutating db using multi_index that shouldn't allow mutation.
        CHAINDB_ANOTHER_CONTRACT_PROTECT(
            code() == current_receiver(),
            "cannot create objects in table of another contract");

        auto ptr = item_ptr(new item(*this, [&](auto& itm) {
            constructor(static_cast<T&>(itm));
        }));

        auto& obj = static_cast<T&>(*ptr);
        auto  pk = primary_key_extractor_type()(obj);
        chaindb_assert(pk != end_primary_key, "invalid value of primary key");

        safe_allocate(pack_size(obj), "invalid size of object", [&](auto& data, auto& size) {
            pack_object(obj, data, size);
            auto delta = internal_use_do_not_use::chaindb_insert(code(), scope(), table_name(), payer, pk, data, size);
            ptr->service_.size   = delta;
            ptr->service_.payer  = eosio::name(payer);
            ptr->service_.in_ram = true;
        });

        add_object_to_cache(ptr);

        next_primary_key_ = pk + 1;
        return const_iterator(this, const_iterator::uninitialized_find_by_pk, pk, std::move(ptr));
    }

    template<typename Lambda>
    void modify(const const_iterator& itr, const account_name_t payer, Lambda&& updater) const {
        chaindb_assert(itr != end(), "cannot pass end iterator to modify");
        modify(*itr, payer, std::forward<Lambda&&>(updater));
    }

    /**
    *  The @ref modify method is used to update an object in a table (the object is searched by its value).
    *  @ingroup multiindex
    *
    *  @param obj - a reference to the object to be updated
    *  @param payer - account name of the payer for the Storage usage of the updated row
    *  @param updater - lambda function that updates the target object
    *
    *  @pre The obj parameter refers to an existing object in the table.
    *  @pre Payer is a valid account that is authorized to execute the action and be billed for storage usage.
    *  @post The modified object is serialized, then replaces the existing object in the table.
    *  @post The primary key of the updated object is not changed.
    *  @post The payer is charged for the storage usage of the updated object.
    *  @post If payer is the same as the existing payer, payer only pays for the usage difference between existing and updated object (and is refunded if this difference is negative).
    *  @post If payer is different from the existing payer, the existing payer is refunded for the storage usage of the existing object.
    *
    *  > @b Exceptions  
    *  > The object to be modified belongs to a table of another contract.  
    *  > The object to be modified is not in the table.  
    *  > Updater changes primary key when modifying the object.
    */
    template<typename Lambda>
    void modify(reference obj, const account_name_t payer, Lambda&& updater) const {
        // Quick fix for mutating db using multi_index that shouldn't allow mutation.
        CHAINDB_ANOTHER_CONTRACT_PROTECT(
            code() == current_receiver(),
            "cannot modify objects in table of another contract");

        auto& mobj = const_cast<T&>(obj);
        auto& itm = static_cast<item&>(mobj);
        chaindb_assert(is_same_multidx(itm), "object passed to modify is not in multi_index");

        auto pk = primary_key_extractor_type()(obj);

        updater(mobj);

        auto mpk = primary_key_extractor_type()(obj);
        chaindb_assert(pk == mpk, "updater cannot change primary key when modifying an object");

        safe_allocate(pack_size(obj), "invalid size of object", [&](auto& data, auto& size) {
            pack_object(obj, data, size);
            auto delta = internal_use_do_not_use::chaindb_update(code(), scope(), table_name(), payer, pk, data, size);
            itm.service_.payer = eosio::name(payer);
            itm.service_.size += delta;
        });
    }

    reference get(const primary_key_t pk, const char* error_msg = "unable to find key") const {
        auto itr = find(pk);
        chaindb_assert(itr != cend(), error_msg);
        return *itr;
    }

    const_iterator find(const primary_key_t pk) const {
        return primary_idx_.find(pk);
    }

    const_iterator require_find(const primary_key_t pk, const char* error_msg = "unable to find key") const {
        return primary_idx_.require_find(pk, error_msg);
    }


    /**
    *  Remove an existing object from a table using its primary key (the object is searched by the given iterator).
    *  @ingroup multiindex
    *
    *  @param itr - an iterator pointing to the object to be removed
    *  @param payer - account name of the payer for the storage usage 
    *
    *  @pre The itr parameter points to an existing element
    *  @post The object is removed from the table and all associated storage is reclaimed.
    *
    *  @return For the signature with `const_iterator`, returns a pointer to the object following the removed object.
    *
    *  > @b Exceptions  
    *  > The object to be removed is not in the table.  
    *  > The object to be removed belongs to a table of another contract.
    */
    const_iterator erase(const_iterator itr, const account_name_t payer = eosio::name()) const {
        chaindb_assert(itr != end(), "cannot pass end iterator to erase");

        const auto& obj = *itr;
        ++itr;
        erase(obj, payer);
        return itr;
    }


    /**
    *  The @ref erase method is used to remove an existing object from a table (the object is searched by its primary key).
    *  @ingroup multiindex
    *
    *  @param obj - object to be removed
    *  @param payer - account name of the payer for the storage usage 
    *
    *  @pre The obj parameter is an existing object in the table.
    *  @post The object is removed from the table and all associated storage is reclaimed.
    *
    *  > @b Exceptions  
    *  > The object to be removed is not in the table.  
    *  > The object to be removed belongs to a table of another contract.
    */
    void erase(reference obj, const account_name_t payer = eosio::name()) const {
        const auto& itm = static_cast<const item&>(obj);

        CHAINDB_ANOTHER_CONTRACT_PROTECT(
            code() == current_receiver(),
            "cannot delete objects from table of another contract");

        chaindb_assert(is_same_multidx(itm), "object passed to erase is not in multi_index");

        auto pk = primary_key_extractor_type()(obj);
        remove_object_from_cache(pk);
        internal_use_do_not_use::chaindb_delete(code(), scope(), table_name(), payer, pk);
    }

    /**
    *  The @ref move_to_ram method is used to move objects from the archive back to the Multi Index table (the object is searched by its value). This operation is inverse to @ref move_to_archive.
    *  @ingroup multiindex
    *
    *  @param obj - a reference to the object to be moved
    *
    *  @pre The object is placed back in the Multi-Index table.
    *  @post The object is moved from the table and cashed code contains it.
    *
    *  > @b Exceptions  
    *  > The object to be moved is not in multi_index.  
    *  > The object to be moved is already in RAM.  
    *  > The object to be moved belongs to a table of another contract.
    */
    void move_to_ram(reference obj) const {
        auto& itm = static_cast<item&>(const_cast<T&>(obj));

        CHAINDB_ANOTHER_CONTRACT_PROTECT(
            code() == current_receiver(),
            "cannot move objects from table of another contract");

        chaindb_assert(is_same_multidx(itm), "object passed to move_to_ram is not in multi_index");
        chaindb_assert(!itm.service_.in_ram, "object passed to move_to_ram is already in RAM");
        auto pk = primary_key_extractor_type()(obj);
        internal_use_do_not_use::chaindb_ram_state(code(), scope(), table_name(), pk, true);
        itm.service_.in_ram = true;
    }

    /**
    *  The @ref move_to_ram method is used to move objects from the archive back to the Multi Index table (the object is searched by the given iterator). This operation is inverse to @ref move_to_archive.
    *  @ingroup multiindex
    *
    *  @param itr - an iterator pointing to the object to be moved
    *
    *  @pre The object  is placed back in the Multi-Index table.
    *  @post The object is moved from the table and cashed code contains it.
    *
    *  > @b Exceptions  
    *  > The object to be moved is not in multi_index.  
    *  > The object to be moved is already in RAM.  
    *  > The object to be moved belongs to a table of another contract.
    */
    void move_to_ram(const const_iterator& itr) const {
        chaindb_assert(itr != end(), "cannot pass end iterator to move_to_ram");
        move_to_ram(*itr);
    }

    /**
    *  The @ref move_to_archive method is used to move rarely used objects from the table to an archive (the object is searched by its value). Moving an object is performed in order to reduce the size of cached code and therefore save storage usage.
    *  @ingroup multiindex
    *
    *  @param obj - a reference to the object to be moved
    *
    *  @pre The object is an existing object in the table.
    *  @post The object is moved from the table and cached code no longer contains it.
    *
    *  > @b Exceptions  
    *  > The object to be moved is not in the table.  
    *  > The object to be moved is already in archive.  
    *  > The object to be moved belongs to a table of another contract.
    */
    void move_to_archive(reference obj) const {
        auto& itm = static_cast<item&>(const_cast<T&>(obj));

        CHAINDB_ANOTHER_CONTRACT_PROTECT(
            code() == current_receiver(),
            "cannot move objects from table of another contract");

        chaindb_assert(is_same_multidx(itm), "object passed to move_to_archive is not in multi_index");
        chaindb_assert(itm.service_.in_ram,  "object passed to move_to_archive is already in archive");
        auto pk = primary_key_extractor_type()(obj);
        internal_use_do_not_use::chaindb_ram_state(code(), scope(), table_name(), pk, false);
        itm.service_.in_ram = false;
    }

    /**
    *  The @ref move_to_archive method is used to move rarely used objects from the table to an archive (the object is searched by the given iterator). Moving an object is performed in order to reduce the size of cached code and therefore save storage usage.
    *  @ingroup multiindex
    *
    *  @param itr - an iterator pointing to the object to be moved
    *
    *  @pre The object is an existing object in the table.
    *  @post The object is moved from the table and cached code no longer contains it.
    *
    *  > @b Exceptions  
    *  > The object to be moved is not in the table.  
    *  > The object to be moved is already in archive.  
    *  > The object to be moved belongs to a table of another contract.
    */
    void move_to_archive(const const_iterator& itr) const {
        chaindb_assert(itr != end(), "cannot pass end iterator to move_to_archive");
        move_to_archive(*itr);
    }

}; // class multi_index
}  // namespace eosio
