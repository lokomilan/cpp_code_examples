#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <vector>

using namespace std;


class Person {
public:
	Person(const string& first_name, const string& last_name, int year) {
		birth_year = year;
		first_name_changes[year] = first_name;
		last_name_changes[year] = last_name;
	}

	void ChangeFirstName(const string& first_name, int year) {
		if (year >= birth_year) {
			first_name_changes[year] = first_name;
		}
	}

	void ChangeLastName(const string& last_name, int year) {
		if (year >= birth_year) {
			last_name_changes[year] = last_name;
		}
	}

	string GetFullName(int year) const {
		const string current_first_name = GetFirstName(year);
		const string current_last_name = GetLastName(year);
		if (current_first_name.empty() && current_last_name.empty()) {
			return "Incognito";
		}
		if (current_first_name.empty()) {
			return current_last_name + " with unknown first name";
		}
		if (current_last_name.empty()) {
			return current_first_name + " with unknown last name";
		}
		return current_first_name + " " + current_last_name;
	}

	string GetFullNameWithHistory(int year) const {
		const vector<string> first_name_history = GetFirstNameHistory(year);
		const vector<string> last_name_history = GetLastNameHistory(year);
		if (first_name_history.empty() && last_name_history.empty()) {
			return "No person";
		}
		if (first_name_history.empty()) {
			return GetHistoryString(last_name_history) + " with unknown first name";
		}
		if (last_name_history.empty()) {
			return GetHistoryString(first_name_history) + " with unknown last name";
		}
		return GetHistoryString(first_name_history) + " " + GetHistoryString(last_name_history);
	}

private:
	int birth_year;
	map<int, string> first_name_changes;
	map<int, string> last_name_changes;

	string GetFirstName(int year) const {
		string current_first_name = "";
		for (const auto& item : first_name_changes) {
			if (item.first > year) {
				return current_first_name;
			}
			current_first_name = item.second;
		}
		return current_first_name;
	}

	vector<string> GetFirstNameHistory(int year) const {
		vector<string> history;
		for (const auto& item : first_name_changes) {
			if (item.first > year) {
				return history;
			}
			if (history.empty() || item.second != history.back()) {
				history.push_back(item.second);
			}
		}
		return history;
	}

	string GetLastName(int year) const {
		string current_last_name = "";
		for (const auto& item : last_name_changes) {
			if (item.first > year) {
				return current_last_name;
			}
			current_last_name = item.second;
		}
		return current_last_name;
	}

	vector<string> GetLastNameHistory(int year) const {
		vector<string> history;
		for (const auto& item : last_name_changes) {
			if (item.first > year) {
				return history;
			}
			if (history.empty() || item.second != history.back()) {
				history.push_back(item.second);
			}
		}
		return history;
	}

	string GetHistoryString(const vector<string>& history) const {
		if (history.empty()) {
			return "";
		}
		string history_string = history.back();
		if (history.size() == 1) {
			return history_string;
		}
		history_string += " (";
		for (int i = history.size() - 2; i >= 0; --i) {
			history_string += history[i] + ", ";
		}
		history_string.resize(history_string.size() - 2);
		history_string += ")";
		return history_string;
	}
};
