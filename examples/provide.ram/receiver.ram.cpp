#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/bandwith.hpp>


struct sharedval{
    uint64_t id;
    std::string string;

    uint64_t primary_key() const {return id;}
};

using valtable = eosio::multi_index<"sharedval"_n, sharedval>;

class [[eosio::contract]] receiver : public eosio::contract {
public:
    receiver( eosio::name receiver, eosio::name code, eosio::datastream<const char*> ds) : eosio::contract(receiver, code, ds), values(_self, _self.value) {}

    [[eosio::action]] void write() {
        eosio::print("Write values to ram provider: ", get_ram_provider(get_self()));

        const auto ram_provider = get_ram_provider(get_self());

        values.emplace(ram_provider, [&] (auto& value) {
            value.id = values.available_primary_key();
            value.string = "Some string";
        });

    }

    [[eosio::action]] void read() {
        eosio::print("Read values\n");
        for (const auto& value : values) {
            eosio::print("Id \n", value.id, " string ", value.string);
        }
    }


    valtable values;
};

EOSIO_DISPATCH( receiver, (write)(read))

