/*
* https://en.wikipedia.org/wiki/Lazy_initialization
*/ 

#include <cassert>
#include <functional>
#include <optional>
#include <string>
#include <mutex>

using namespace std;

template <typename T>
class LazyValue {
public:
    explicit LazyValue(function<T()> init);

    bool HasValue() const;
    const T& Get() const;

private:
    mutable optional<T> value_;
    mutable mutex mtx_; // Мьютекс для синхронизации доступа
    function<T()> init_;
};

template <typename T>
LazyValue<T>::LazyValue(function<T()> init) : init_(init) {
}

template <typename T>
bool LazyValue<T>::HasValue() const {
    lock_guard<mutex> lock(mtx_); // Защита от одновременного доступа
    return value_.has_value();
}

template <typename T>
const T& LazyValue<T>::Get() const {
    lock_guard<mutex> lock(mtx_); // Защита от одновременного доступа

    if (!value_) {
        value_ = init_(); // Инициализация значения
    }
    return *value_; // Возвращаем ссылку на инициализированное значение
}

void UseExample() {
    const string big_string = "Giant amounts of memory"s;

    LazyValue<string> lazy_string([&big_string] {
        return big_string;
    });

    assert(!lazy_string.HasValue());
    assert(lazy_string.Get() == big_string);
    assert(lazy_string.Get() == big_string);
}

void TestInitializerIsntCalled() {
    bool called = false;

    {
        LazyValue<int> lazy_int([&called] {
            called = true;
            return 0;
        });
    }
    assert(!called);
}

int main() {
    UseExample();
    TestInitializerIsntCalled();
}