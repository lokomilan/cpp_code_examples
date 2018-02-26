#include <iostream>
#include <fstream>
#include <map>
#include <string>

using namespace std;

void ChangeCapital(map<string, string>& capitals,
	const string& country, const string& new_capital) {

	if (!capitals.count(country)) {
		capitals[country] = new_capital;
		cout << "Introduce new country " << country <<
			" with capital " << new_capital;
	}
	else if (capitals[country] == new_capital) {
		cout << "Country " << country << " hasn't changed its capital";
	}
	else {
		const auto old_capital = capitals[country];
		capitals[country] = new_capital;
		cout << "Country " << country <<
			" has changed its capital from " << old_capital << " to " << new_capital;
	}
}

void Rename(map<string, string>& capitals,
	const string& old_country_name, const string& new_country_name) {

	if (old_country_name == new_country_name ||
		!capitals.count(old_country_name) || capitals.count(new_country_name)) {
		cout << "Incorrect rename, skip";
	}
	else {
		const auto capital = capitals[old_country_name];
		capitals.erase(old_country_name);
		capitals[new_country_name] = capital;
		cout << "Country " << old_country_name << " with capital " <<
			capital << " has been renamed to " << new_country_name;
	}
}

void About(map<string, string>& capitals, const string& country) {
	if (!capitals.count(country)) {
		cout << "Country " << country << " doesn't exist";
	}
	else {
		cout << "Country " << country << " has capital " << capitals[country];
	}
}

void Dump(const map<string, string>& capitals) {
	if (capitals.empty()) {
		cout << "There are no countries in the world";
	}
	else {
		for (const auto& item : capitals) {
			cout << item.first << "/" << item.second << " ";
		}
	}
}


int main() {

	int queries_count;
	cin >> queries_count;

	map<string, string> capitals;

	for (int i = 0; i < queries_count; ++i) {
		string code;
		cin >> code;
		if (code == "CHANGE_CAPITAL") {
			string country, new_capital;
			cin >> country >> new_capital;
			ChangeCapital(capitals, country, new_capital);
		}
		else if (code == "RENAME") {
			string old_country_name, new_country_name;
			cin >> old_country_name >> new_country_name;
			Rename(capitals, old_country_name, new_country_name);
		}
		else if (code == "ABOUT") {
			string country;
			cin >> country;
			About(capitals, country);
		}
		else {
			Dump(capitals);
		}
		cout << endl;
	}
	return 0;
}

