#include "../../modules/utils/dnn.hpp"
#include "../../modules/utils/dnn/Utils/Random.h"
#include "../../modules/utils/datasets.hpp"
#include "../../modules/mapping/autoencoder.hpp"

#include <iostream>
#include <chrono>


using namespace std;
using namespace MiniDNN;


template <typename Scalar>
blaze::DynamicMatrix<Scalar> getRandomMatrix(const size_t rows, const size_t columns)
{
	blaze::DynamicMatrix<Scalar> m(rows, columns);

	for (size_t i = 0UL; i < rows; i++) {
		for (size_t j = 0UL; j < columns; j++) {
			m(i, j) = blaze::rand<Scalar>(-1, 1);
		}
	}

	return m;
}

template<typename T>
void printVector(const T& vector)
{
	for (auto e: vector) {
		cout << +e << " ";
	}
	cout << endl;
}

template<typename T>
void printMatrix(const T& vector, size_t rows, size_t columns)
{
	cout << setw(3);
	for (auto i = 0; i < rows; ++i) {
		for (auto j = 0; j < columns; ++j) {
			cout << setw(3) << +vector[i * columns + j] << " ";
		}
		cout << endl;
	}
}

template<typename T>
int vectorDiff(const vector<T>& vector1, const vector<T>& vector2)
{
	if (vector1.size() != vector2.size()) {
		cout << "v1.size() != v2.size()" << endl;
		return -1;
	}

	int diff = 0;
	for (size_t i = 0; i < vector1.size(); ++i) {
		diff += (int(vector1[i]) - int(vector2[i]));
	}

	return diff;
}

int main()
{
	/* Load data */
	Datasets datasets;
	auto [labels, shape, features] = datasets.getMnist("data.cereal");

	if (shape.empty()) {
		cout << "Data file is empty. Exiting." << endl;
		return EXIT_FAILURE;
	}


	Autoencoder<uint8_t, double> autoencoder(features, shape[1] * shape[2], 255);

	cout << "Train" << endl;
	autoencoder.train(1, 256);

	cout << "Sample:" << endl;
	vector<uint8_t> sample(features.begin(), features.begin() + shape[1] * shape[2]);
	printMatrix(sample, shape[1], shape[2]);

	cout << "prediction" << endl;
	auto prediction = autoencoder.predict(sample);
	printMatrix(prediction, shape[1], shape[2]);

	cout << "latent vector" << endl;
	vector<double> latentVector = autoencoder.encode(sample);
	printVector(latentVector);

	float t = vectorDiff(prediction, autoencoder.decode(latentVector));
	cout << "test:" << t << endl;

	return EXIT_SUCCESS;
}
