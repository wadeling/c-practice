#pragma once

#include <string>
#include <vector>
#include <map>

using namespace std;

template <class Base> class FactoryRegistry {

public:
    static std::string allFactory() {
        std::vector<std::string> ret;
        ret.reserve(factories().size());
        for (const auto& factory : factories()) {
            ret.push_back(factory.first);
        }
//        std::sort(ret.begin(), ret.end());

        return std::string("");
    }

    static std::map<std::string, Base*>& factories() {
        static auto* factories = new std::map<std::string, Base*>;
        return *factories;
    }

    static void registerFactory(Base& factory, std::string name) {
        auto result = factories().emplace(std::make_pair(name, &factory));
        if (!result.second) {
            printf("double register %s\r\n",name.c_str());
        }
    }
};
