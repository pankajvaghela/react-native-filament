//
// Created by Marc Rousavy on 21.02.24.
//

#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <jsi/jsi.h>
#include "JSIConverter.h"

namespace margelo {

using namespace facebook;

class HybridObject: public jsi::HostObject {
public:
    struct HybridFunction {
        jsi::HostFunctionType function;
        size_t parameterCount;
    };

public:
    ~HybridObject();

    void set(facebook::jsi::Runtime &, const facebook::jsi::PropNameID &name, const facebook::jsi::Value &value) override;
    jsi::Value get(facebook::jsi::Runtime& runtime, const facebook::jsi::PropNameID& propName) override;
    std::vector<jsi::PropNameID> getPropertyNames(facebook::jsi::Runtime& runtime) override;

    /**
     * Loads all native methods of this `HybridObject` to be exposed to JavaScript.
     * Example:
     *
     * ```cpp
     * int User::getAge() {
     *   return 23;
     * }
     *
     * void User::loadHybridMethods() {
     *   registerHybridMethod("getAge", &User::getAge, this);
     * }
     * ```
     */
    virtual void loadHybridMethods() = 0;

private:
    bool _didLoadMethods = false;
    std::unordered_map<std::string, HybridFunction> _methods;
    std::unordered_map<std::string, jsi::HostFunctionType> _getters;
    std::unordered_map<std::string, jsi::HostFunctionType> _setters;
    std::unordered_map<jsi::Runtime*, std::unordered_map<std::string, std::shared_ptr<jsi::Function>>> _functionCache;

private:
    void ensureInitialized();

private:
    template<typename ClassType, typename ReturnType, typename... Args, size_t... Is>
    jsi::Value callMethod(ClassType* obj,
                          ReturnType(ClassType::*method)(Args...),
                          jsi::Runtime& runtime,
                          const jsi::Value* args,
                          std::index_sequence<Is...>) {
        if constexpr (std::is_same_v<ReturnType, void>) {
            (obj->*method)(JSIConverter<Args>::fromJSI(runtime, args[Is])...);
            return jsi::Value::undefined();
        } else {
            ReturnType result = (obj->*method)(JSIConverter<Args>::fromJSI(runtime, args[Is])...);
            return JSIConverter<ReturnType>::toJSI(runtime, result);
        }
    }

private:
    template<typename Derived, typename ReturnType, typename... Args>
    jsi::HostFunctionType createHybridMethod(ReturnType(Derived::*method)(Args...),
                                             Derived* derivedInstance) {
        // TODO(marc): Use std::shared_ptr<T> instead of T* to keep a strong reference of derivedClass.
        return [this, derivedInstance, method](jsi::Runtime &runtime,
                                               const jsi::Value &thisVal,
                                               const jsi::Value *args,
                                               size_t count) -> jsi::Value {
            return callMethod(derivedInstance,
                              method,
                              runtime,
                              args,
                              std::index_sequence_for<Args...>{});
        };
    }

protected:

    template<typename Derived, typename ReturnType, typename... Args>
    void registerHybridMethod(std::string name,
                              ReturnType(Derived::*method)(Args...),
                              Derived* derivedInstance) {
        if (_methods.count(name) > 0) {
            throw std::runtime_error("Cannot add Hybrid Method \"" + name + "\" - a method/property with that name already exists!");
        }

        _methods[name] = HybridFunction {
                .function = createHybridMethod(method, derivedInstance),
                .parameterCount = sizeof...(Args)
        };
    }

    template<typename Derived, typename ReturnType>
    void registerHybridGetter(std::string name,
                              ReturnType(Derived::*method)(),
                              Derived* derivedInstance) {
        if (_getters.count(name) > 0) {
            [[unlikely]];
            throw std::runtime_error("Cannot add Hybrid Property \"" + name + "\" - a method/property with that name already exists!");
        }

        _getters[name] = createHybridMethod(method, derivedInstance);
    }

    template<typename Derived, typename ValueType>
    void registerHybridSetter(std::string name,
                              void(Derived::*method)(ValueType),
                              Derived* derivedInstance) {
        if (_setters.count(name) > 0) {
            [[unlikely]];
            throw std::runtime_error("Cannot add Hybrid Property \"" + name + "\" - a method/property with that name already exists!");
        }

        _setters[name] = createHybridMethod(method, derivedInstance);
    }
};

} // margelo

