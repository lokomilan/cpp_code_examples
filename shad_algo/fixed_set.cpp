#include <iostream>
#include <vector>
#include <random>

using std::vector;

vector<int> Input() {

	vector<int> numbers;

	int numbers_size;
	std::cin >> numbers_size;
	for (int i = 0; i < numbers_size; ++i) {
		int current_number;
		std::cin >> current_number;
		numbers.push_back(current_number);
	}

	return numbers;
}


struct Hash {
	long long first_coefficient, second_coefficient;
	int modulo;
};

Hash GetRandomHash(std::default_random_engine& generator) {
	Hash hash;
	hash.modulo = 2000000011;
	std::uniform_int_distribution<int> distribution(0, hash.modulo - 1);
	hash.first_coefficient = distribution(generator);
	hash.second_coefficient = distribution(generator);
	return hash;
}

long long GetHash(const int number, const Hash hash, const int range) {
	long long fc = hash.first_coefficient;
	long long sc = hash.second_coefficient;
	int mod = hash.modulo;
	return (((fc * number + sc) % mod + mod) % mod) % range;
}

class Bucket {

private:
	int range_;
	vector<int> table_;
	Hash hash_;

public:

	int size;
	vector<int> elements;

	explicit Bucket(std::default_random_engine& generator) {
		size = 0;
		elements = vector<int>(0);
		table_ = vector<int>(0);
		range_ = 2000000001;
		hash_ = GetRandomHash(generator);
	}

	void FillTable(std::default_random_engine& generator) {
		bool is_filled = false;
		while (!is_filled) {
			is_filled = true;
			table_ = vector<int>(size * size, range_);
			hash_ = GetRandomHash(generator);
			for (auto element : elements) {
				int position = GetHash(element, hash_, size * size);
				if (table_[position] < range_) {
					is_filled = false;
					break;
				}
				table_[position] = element;
			}
		}
	}

	bool Contains(int number) {
		if (size == 0) {
			return false;
		}
		int position = GetHash(number, hash_, size * size);
		return table_[position] == number;
	}
};


class FixedSet {

private:
	int size_;
	int range_;
	Hash hash_;
	long long squares_sum_;
	vector<Bucket> buckets_;
	std::default_random_engine generator_;

public:

	explicit FixedSet() {
		generator_ = std::default_random_engine(generator_);
		size_ = 0;
		range_ = 2000000001;
		buckets_ = vector<Bucket>(0, Bucket(generator_));
		hash_ = GetRandomHash(generator_);
		squares_sum_ = -1;
	}

	void Initialize(const vector<int>& numbers) {
		size_ = numbers.size();
		while (squares_sum_ < 0 || squares_sum_ > 2 * size_) {
			buckets_ = vector<Bucket>(size_, Bucket(generator_));
			hash_ = GetRandomHash(generator_);
			FillBuckets(numbers);
			SetSquaresSum();
		}

		for (int i = 0; i < buckets_.size(); ++i) {
			buckets_[i].FillTable(generator_);
		}
	}

	void FillBuckets(const vector<int>& numbers) {
		for (auto number : numbers) {
			int position = GetHash(number, hash_, size_);
			buckets_[position].elements.push_back(number);
			buckets_[position].size++;
		}
	}

	void SetSquaresSum() {
		long long current_sum = 0;
		for (auto bucket : buckets_) {
			current_sum += bucket.size * bucket.size;
		}
		squares_sum_ = current_sum;
	}

	bool Contains(int number) {
		int position = GetHash(number, hash_, size_);
		return buckets_[position].Contains(number);
	}
};


int main() {

	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	vector<int> numbers = Input();

	FixedSet set = FixedSet();
	set.Initialize(numbers);

	int queries_number;
	std::cin >> queries_number;

	for (int i = 0; i < queries_number; ++i) {
		int number;
		std::cin >> number;
		std::cout << (set.Contains(number) ? "Yes" : "No") << '\n';
	}
}
