// https://en.wikipedia.org/wiki/Smart_pointer

#include <cassert>
#include <cstddef>
#include <utility>

using namespace std;

template <typename T>
class UniquePtr {
private:
    T* raw_pointer_;
public:

    // Конструктор по умолчанию, создающий пустой умный указатель.
    UniquePtr()
        : raw_pointer_(nullptr)
    {}

    // Конструктор, принимающий T* и захватывающий владение этой динамической памятью.
    explicit UniquePtr(T* ptr) : raw_pointer_(ptr) {}

    UniquePtr(const UniquePtr&) = delete;

    // Конструктор перемещения, получающий на вход rvalue-ссылку на другой UniquePtr и отбирающий у него владение ресурсом.
    UniquePtr(UniquePtr&& other) {
        if (this == &other) {
            return;
        }
        raw_pointer_ = other.raw_pointer_;
        other.raw_pointer_ = nullptr;
     }

    UniquePtr& operator=(const UniquePtr&) = delete;

    /* 
    * Оператор присваивания, получающий на вход nullptr (тип - nullptr_t, определенный в заголовочном файле cstddef). 
    * В результате умный указатель должен стать пустым.
    */
    UniquePtr& operator=(nullptr_t) {
        delete raw_pointer_;
        raw_pointer_ = nullptr;
        return *this;
    }

    // Move-оператор присваивания, получающий на вход rvalue-ссылку на другой UniquePtr.
    UniquePtr& operator=(UniquePtr&& other) {
        if (this != &other) {
            Reset(other.ptr_);
            other.ptr_ = nullptr;
        }
        return *this;
    }

    /*
    * Оператор -> должен вернуть указатель на обычную структуру, 
    * к которой можно применить оператор -> обычного указателя.
    */ 
    T* operator->() const {
        return raw_pointer_;
    }

    T& operator*() const {
        return *raw_pointer_;
    }

    // Отменяет владение объектом и возвращает хранящийся внутри указатель.
    T* Release() {
        T* temporal_ptr = raw_pointer_;
        raw_pointer_ = nullptr;
        return temporal_ptr;
    }

    // После выполнения умный указатель должен захватить ptr.
    void Reset(T* ptr) {
        delete raw_pointer_;
        raw_pointer_ = ptr;
    }

    // Обменивает содержимое с другим умным указателем.
    void Swap(UniquePtr& other) {
        swap(other.raw_pointer_, raw_pointer_);
        // T* temporal_ptr = other.raw_pointer_;
        // other.raw_pointer_ = raw_pointer_;
        // raw_pointer_ = temporal_ptr;
    }

    // Возвращает указатель.
    T* Get() const {
        return raw_pointer_;
    }

    ~UniquePtr() {
        delete raw_pointer_;
        raw_pointer_ = nullptr;
    }
};

struct Item {
    static int counter;
    int value;
    Item(int v = 0)
        : value(v)
    {
        ++counter;
    }
    Item(const Item& other)
        : value(other.value)
    {
        ++counter;
    }
    ~Item() {
        --counter;
    }
};

int Item::counter = 0;

void TestLifetime() {
    Item::counter = 0;
    {
        UniquePtr<Item> ptr(new Item);
        assert(Item::counter == 1);

        ptr.Reset(new Item);
        assert(Item::counter == 1);
    }
    assert(Item::counter == 0);

    {
        UniquePtr<Item> ptr(new Item);
        assert(Item::counter == 1);

        auto rawPtr = ptr.Release();
        assert(Item::counter == 1);

        delete rawPtr;
        assert(Item::counter == 0);
    }
    assert(Item::counter == 0);
}

void TestGetters() {
    UniquePtr<Item> ptr(new Item(42));
    assert(ptr.Get()->value == 42);
    assert((*ptr).value == 42);
    assert(ptr->value == 42);
}

int main() {
    TestLifetime();
    TestGetters();
}