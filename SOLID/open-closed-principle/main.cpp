#include <iostream>
#include <ostream>
#include <vector>

/* Open-Closed Principle
 * The open-closed principle states that the system should blue
 * Open to extension and closed for modification
 * */

enum class Color {red, green , blue};
enum class Size { small, medium, large};

struct Product {
	std::string name;
	Color color;
	Size size;
};

struct ProductFilter {
	std::vector<Product*> byColor(std::vector<Product*> items, Color color) {
		std::vector<Product*> result;
		for (auto& i: items)
			if (i->color == color)
				result.push_back(i);
		return result;
	}
};

/* Achieving open-closeness via the especification pattern */
template <typename T> struct Specification { /* Defining a generic interface */
	virtual bool isSatisfied(T* item) = 0;
};

template<typename T> struct Filter {
	virtual std::vector<T*> filter(std::vector<T*>items, Specification<T>& spec) = 0;
};

struct BetterFilter: Filter<Product> {
	std::vector<Product*> filter(std::vector<Product*> items, Specification<Product> &spec) override {
		std::vector<Product*> result;
		for (auto& item : items)
			if (spec.isSatisfied(item))
				result.push_back(item);
		return result;
	}
};

struct ColorSpecification : Specification<Product> {
	Color color;

	ColorSpecification(Color color) : color(color) {}

	bool isSatisfied(Product *item) override {
		return item->color == color;
	}
};

struct SizeSpecification : Specification<Product> {
	Size size;

	explicit SizeSpecification(const Size size)
		: size{size} {}

	bool isSatisfied(Product *item) override {
		return item->size == size;
	}
};

/* Generic specification */
template<typename T> struct AndSpecification : Specification<T> {
	Specification<T>& first;
	Specification<T>& second;

	AndSpecification(Specification<T> &first, Specification<T> &second) : first(first), second(second){}

	bool isSatisfied(T *item) override {
		return first.isSatisfied(item) && second.isSatisfied(item);
	}
};

int main() {
	Product apple{"Apple", Color::green, Size::small};
	Product house{"House", Color::red, Size::large};
	Product tree{"Tree", Color::green, Size::large};

	ProductFilter pf;
	std::vector<Product*> items { &apple, &tree, &house };
	auto greenThings = pf.byColor(items, Color::green); 
	for (auto& item : greenThings)
		std::cout << item->name << " is green" << std::endl;

	BetterFilter bf;
	ColorSpecification green(Color::green);

	for(auto& item: bf.filter(items, green))
		std::cout << item->name << " is green" << std::endl;

	SizeSpecification large(Size::large);
	AndSpecification<Product> greenAndLarge(green, large);

	for(auto& item: bf.filter(items, greenAndLarge))
		std::cout << item->name << " is green and large" << std::endl;


	return 0;
}
