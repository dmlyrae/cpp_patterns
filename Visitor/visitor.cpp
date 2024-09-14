/*
* https://en.wikipedia.org/wiki/Visitor_pattern
*/

#include <iostream>
#include <string>

/**
 * Предварительное объявление классов для компилятора.
 */
class Zoo;
class Cinema;
class Circus;

/**
 * @brief Интерфейс "посетителя"
 * Виртуальный интерфейс для конкретной реализации далее.
 */
class IVisitor {
public:
	virtual void visit(Zoo& zoo) = 0;
	virtual void visit(Cinema& cinema) = 0;
	virtual void visit(Circus& circus) = 0;
};

/**
 * @brief Абстрактный класс места.
 * Должен реализовывать метод для принятия "посетителя".
 * В данном случае accept.
 */
class Place {
public:
	virtual void accept(IVisitor& v) = 0;
	// Виртуальный деструктор для корректного разрушения производных классов
	virtual ~Place() = default; 
};

/**
 * @brief Класс Zoo, наследующий от Place
 */
class Zoo : public Place {
public:
	// Реализация accept
	void accept(IVisitor& v) override {
 		// Передаем текущий объект в метод visit
		v.visit(*this);
	}
};

/**
 * @brief Класс Cinema, наследующий от Place
 */
class Cinema : public Place {
public:
	void accept(IVisitor& v) override {
		v.visit(*this);
	}
};

/**
 * @brief Класс Circus, наследующий от Place
 */
class Circus : public Place {
public:
	void accept(IVisitor& v) override {
		v.visit(*this);
	}
};

/**
 * @brief Класс HolidayMaker, реализующий интерфейс IVisitor
 */
class HolidayMaker : public IVisitor {
public:
 	// Переменная для хранения информации о месте
	std::string value;

	void visit(Zoo& zoo) override {
		value = "zoo";
	}

	void visit(Cinema& cinema) override {
		value = "cinema";
	}

	void visit(Circus& circus) override {
		value = "circus";
	}
};

int main() {

	Zoo zoo;
	Cinema cinema;
	Circus circus;

	Place* places[] = { &zoo, &cinema, &circus };

	// Проходим по всем местам и применяем "посетителя".
	for (auto place : places) {
		HolidayMaker visitor;
		place->accept(visitor);
		std::cout << visitor.value << std::endl;
	}

	return 0;
}
