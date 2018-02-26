#include <iostream>
#include <vector>

using std::vector;
typedef long long ll;

struct Player {

	ll strength;
	int position;

	bool operator< (const Player& other) {
		return strength < other.strength;
	}
};

struct Team {

	ll total_strength;
	vector<int> player_positions;
	int weakest_player_number;
	int strongest_player_number;
};


void Input(vector<Player>* players) {

	int elements_number;
	std::cin >> elements_number;

	for (int position = 0; position < elements_number; ++position) {
		ll strength;
		std::cin >> strength;
		players->push_back({ strength, position + 1 });
	}
}

void Output(const Team& optimal_team) {

	std::cout << optimal_team.total_strength << '\n';

	for (auto player_position : optimal_team.player_positions) {
		std::cout << player_position << ' ';
	}
}

template <typename T>
vector<T> Merge(vector<T> first_sequence, vector<T> second_sequence) {

	vector<T> merged_sequence;
	int first_iterator = 0, second_iterator = 0;

	while (first_iterator < first_sequence.size() && second_iterator < second_sequence.size()) {

		if (first_sequence[first_iterator] < second_sequence[second_iterator]) {
			merged_sequence.push_back(first_sequence[first_iterator++]);
		}
		else {
			merged_sequence.push_back(second_sequence[second_iterator++]);
		}
	}

	while (first_iterator < first_sequence.size()) {
		merged_sequence.push_back(first_sequence[first_iterator++]);
	}

	while (second_iterator < second_sequence.size()) {
		merged_sequence.push_back(second_sequence[second_iterator++]);
	}

	return merged_sequence;
}

template <typename T>
vector<vector<T>> GetInitialSortingState(vector<T> sequence) {

	vector< vector<T> > initial_state;

	for (int i = 0; i < sequence.size(); ++i) {
		vector<T> current_subsequence;
		current_subsequence.push_back(sequence[i]);
		initial_state.push_back(current_subsequence);
	}

	return initial_state;
}

template <typename T>
vector<vector<T>> GetNextSortingState(const vector<vector<T>> current_state) {

	vector<vector<T>> next_state;

	for (int i = 1; i < current_state.size(); i += 2) {
		next_state.push_back(Merge(current_state[i - 1], current_state[i]));
	}

	if (current_state.size() % 2)
		next_state.push_back(current_state.back());

	return next_state;
}

template <typename T>
vector<T> Sort(const vector<T> sequence) {

	vector<vector<T>> current_state = GetInitialSortingState(sequence);

	while (current_state.size() > 1) {
		current_state = GetNextSortingState(current_state);
	}

	return current_state[0];
}

vector<int> GetSortedPlayerPositions(const vector<Player> &sorted_players,
	const int left_player_number, const int right_player_number) {

	vector<int> player_numbers;

	for (int i = left_player_number; i <= right_player_number; ++i) {
		player_numbers.push_back(sorted_players[i].position);
	}

	return Sort(player_numbers);
}

bool isCorrectTeam(const Team answer, const vector<Player> &sorted_players) {

	bool team_is_trivial = answer.strongest_player_number - answer.weakest_player_number < 2;

	const ll weakest_player_strength = sorted_players[answer.weakest_player_number].strength;
	const ll second_player_strength = sorted_players[answer.weakest_player_number + 1].strength;
	const ll strongest_player_strength = sorted_players[answer.strongest_player_number].strength;

	bool team_is_friendly = strongest_player_strength <=
		weakest_player_strength + second_player_strength;

	return team_is_trivial || team_is_friendly;
}

Team GetTrivialTeam(const vector<Player>& sorted_players) {

	Team trivial_team;
	trivial_team.total_strength = sorted_players[0].strength;
	trivial_team.player_positions = { 1 };

	return trivial_team;
}

Team GetOptimalTeam(const vector<Player>& sorted_players) {

	if (sorted_players.size() == 1) {
		return GetTrivialTeam(sorted_players);
	}

	vector<int> player_positions;
	const ll initial_total_strength = sorted_players[0].strength + sorted_players[1].strength;
	Team current_team = { initial_total_strength, player_positions, 0, 1 };
	Team optimal_team = current_team;

	while (current_team.strongest_player_number < sorted_players.size()) {

		current_team.strongest_player_number++;
		if (current_team.strongest_player_number == sorted_players.size())
			continue;
		current_team.total_strength +=
			sorted_players[current_team.strongest_player_number].strength;
		while (!isCorrectTeam(current_team, sorted_players)) {
			current_team.total_strength -=
				sorted_players[current_team.weakest_player_number].strength;
			current_team.weakest_player_number++;
		}

		if (current_team.total_strength > optimal_team.total_strength) {
			optimal_team = current_team;
		}
	}

	const int final_weakest_player_number = optimal_team.weakest_player_number;
	const int final_strongest_player_number = optimal_team.strongest_player_number;
	optimal_team.player_positions = GetSortedPlayerPositions
	(sorted_players, final_weakest_player_number, final_strongest_player_number);
	return optimal_team;
}


int main() {

	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	vector <Player> players;
	Input(&players);
	vector <Player> sorted_players = Sort(players);
	Team optimal_team = GetOptimalTeam(sorted_players);
	Output(optimal_team);

	return 0;
}
