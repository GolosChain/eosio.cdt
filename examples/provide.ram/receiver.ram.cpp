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

    [[eosio::action]] void writeself() {
        require_auth(_self);
        eosio::print("Write values to the user memory: ", _self);

        values.emplace(_self, [&] (auto& value) {
            value.id = values.available_primary_key();
            value.string = "Some string written to the contract memory";
        });

    }

    [[eosio::action]] void writeuser(eosio::name user) {
        require_auth(user);
        const auto ram_provider = get_ram_provider(user);
        eosio::print("Write values to ram provider: ", ram_provider);

        values.emplace(ram_provider, [&] (auto& value) {
            value.id = values.available_primary_key();
            value.string = "Some string written to auto-defined provider";
        });

    }


    [[eosio::action]] void read() {
        eosio::print("Read values\n");
        for (const auto& value : values) {
            eosio::print("Id: ", value.id, " string: ", value.string,"\n");
        }
    }


    valtable values;
};

EOSIO_DISPATCH( receiver, (writeuser)(writeself)(read))

