#include <fstream>
#include <iostream>
#include <vector>
#include <boost/lexical_cast.hpp>

/* Single Responsability Principle 
 * is the idea that a class must have a single reason to change its state
 * this means that we should correctly separate the concerns of each class
 * */

struct Journal {
	std::string title;
	std::vector<std::string> entries;

	Journal(const std::string &title) : title(title) {}

	void addEntry(const std::string& entry) {
		static int count = 1;
		entries.push_back(boost::lexical_cast<std::string>(count++) + ": " + entry);
	}
	
	/*
	void save(const std::string& filename) {
		std::ofstream ofs(filename);
		for (auto& e: entries)
			ofs << e << std::endl;
	}
	*/
};

struct PersistenceManager {
	void save(const Journal& j, const std::string& filename) {
		std::ofstream ofs(filename);
		for (auto& e: j.entries)
			ofs << e << std::endl;
	}
};

int main() {
	Journal journal{"Captain's log. Stardate 4513.3"};
	journal.addEntry("After having been taken over by an android, the Enterprise has been under way at warp seven for four days.");
	journal.addEntry("We can only assume the Klingons have attacked the station. We're going in armed for battle.");
	/* instead of journal.save("captainslog.txt") we can use: */
	PersistenceManager pm;
	pm.save(journal, "capitainslog.txt");

	return 0;
}
