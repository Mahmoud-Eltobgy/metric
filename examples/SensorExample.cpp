#include <variant>
#include <chrono>
#include <deque> // for Record test
#include <vector>
#include <thread>
#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <codecvt>
#include <random>
#include <math.h>  

#include <string.h>
#include <libpq-fe.h>

//#include "utils/poor_mans_quantum.hpp"

#include "modules/crossfilter.hpp"

#include "modules/mapping/details/classification/metric_classification.hpp"
//#include "../details/classification/details/correlation_weighted_accuracy.hpp"

template <typename T0, typename ... Ts>
std::ostream & operator<< (std::ostream & s,
	std::variant<T0, Ts...> const & v)
{
	std::visit([&](auto && arg) { s << arg; }, v); return s;
}

template <typename T>
void vector_print(const std::vector<T> &vec, int maxRows = -1)
{

	std::cout << "[";
	for (auto i = 0; i < vec.size(); i++)
	{
		if (i < vec.size() - 1)
		{
			std::cout << vec[i] << ", ";
		}
		else
		{
			std::cout << vec[i] << "]" << std::endl;
		}

		if (maxRows >= 0 && i >= maxRows - 1)
		{
			if (vec.size() > maxRows)
			{
				std::cout << " ... " << "]" << std::endl;
			}
			break;
		}
	}
}


template <typename T>
void matrix_print(const std::vector<std::vector<T>> &mat, int maxRows = -1, int maxCols = -1)
{

	std::cout << "[" << std::endl;
	for (auto i = 0; i < mat.size(); i++)
	{
		std::cout << "  [ ";
		for (auto j = 0; j < mat[i].size(); j++)
		{
			std::cout << mat[i][j];
			if (j < mat[i].size() - 1)
			{
				std::cout << ", ";
			}
			if (maxCols >= 0 && j >= maxCols - 1)
			{
				if (mat[i].size() > maxCols)
				{
					std::cout << " ... ";
				}
				break;
			}
		}
		std::cout << " ]" << std::endl;

		if (maxRows >= 0 && i >= maxRows - 1)
		{
			if (mat.size() > maxRows)
			{
				std::cout << "  ... ";
			}
			break;
		}
	}
	std::cout << "]" << std::endl;
}

void query_print(PGresult *res, int maxRows = -1)
{
	int ncols = PQnfields(res);
	printf("There are %d columns:", ncols);
	printf("\n");
	printf("\n");
	for (int i = 0; i < ncols; i++)
	{
		char *name = PQfname(res, i);
		printf("%s | ", name);
	}
	printf("\n");
	printf("\n");

	int nrows = PQntuples(res);
	for (auto i = 0; i < nrows; i++)
	{
		for (auto j = 0; j < ncols; j++)
		{
			char* v = PQgetvalue(res, i, j);
			printf("%s | ", v);
		}
		printf("\n");

		if (maxRows >= 0 && i >= maxRows - 1)
		{
			if (nrows > maxRows)
			{
				std::cout << "  ... ";
			}
			break;
		}
	}
	printf("\n");

	printf("Total: %d rows\n", nrows);
}

std::wstring stringToWstring(const std::string& t_str)
{
	//setup converter
	typedef std::codecvt_utf8<wchar_t> convert_type;
	std::wstring_convert<convert_type, wchar_t> converter;

	//use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
	return converter.from_bytes(t_str);
}

// Converts UTC time string to a time_t value.
std::time_t getEpochTime(const char* dateTime)
{
	std::string dateTime_str(dateTime);

	// Let's consider we are getting all the input in
	// this format: '2014-07-25 20:17:22'
	// A better approach would be to pass in the format as well.
	static const std::wstring dateTimeFormat{ L"%Y-%m-%d %H:%M:%S" };

	// Create a stream which we will use to parse the string,
	// which we provide to constructor of stream to fill the buffer.
	std::wistringstream ss{ stringToWstring(dateTime_str) };

	// Create a tm object to store the parsed date and time.
	std::tm dt;

	// Now we read from buffer using get_time manipulator
	// and formatting the input appropriately.
	ss >> std::get_time(&dt, dateTimeFormat.c_str());

	// Convert the tm structure to time_t value and return.
	return std::mktime(&dt);
}

/* Close connection to database */
void CloseConn(PGconn *conn)
{
	PQfinish(conn);
	printf("Connection closed\n");
	//getchar();
	//exit(1);
}

/* Establish connection to database */
PGconn *ConnectDB()
{
	std::string m_dbhost = "95.216.78.62";
	int         m_dbport = 5432;
	std::string m_dbname = "DAA";
	std::string m_dbuser = "stepan";
	std::string m_dbpass = "Ghokle73";

	PGconn *conn = NULL;

	// Make a connection to the database
	std::string m_connectionString = "user=" + m_dbuser + " password=" + m_dbpass + " dbname=" + m_dbname + " hostaddr=" + m_dbhost + " port=" + std::to_string(m_dbport);
	conn = PQconnectdb(m_connectionString.c_str());

	// Check to see that the backend connection was successfully made 
	if (PQstatus(conn) != CONNECTION_OK)
	{
		printf("Connection to database failed");
		CloseConn(conn);
	}

	printf("Connection to database - OK\n");

	return conn;
}

//typedef std::vector< std::variant<bool, int, long int, double, std::string> > Record;
typedef std::vector< double > Record;

struct Feature
{
	int index;
	std::string id;
	std::string bezeichnung;
};

int lookupFeatureIndex(std::string id, const std::vector<Feature> &features)
{
	for (auto j = 0; j < features.size(); j++)
	{
		if ((id.compare(features[j].id)) == 0)
		{
			return features[j].index;
		}
	}

	return -1;
}

int countFeatures(PGconn *conn)
{
	// Execute with sql statement
	PGresult *res = NULL;

	res = PQexec(conn, "SELECT COUNT(*) FROM public.sensormetadata");
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
		CloseConn(conn);

	int numFeatures = atoi(PQgetvalue(res, 0, 0));

	// Clear result
	PQclear(res);

	return numFeatures;
}

std::vector<std::string> getTimestamps(PGconn *conn, const int limit = 100)
{
	// Execute with sql statement
	PGresult *res = NULL;

	std::string queryString = "SELECT DISTINCT date FROM public.sensordata ORDER BY date ASC LIMIT " + std::to_string(limit);

	res = PQexec(conn, queryString.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
		CloseConn(conn);

	std::vector<std::string> stamps;

	int nrows = PQntuples(res);
	for (int i = 0; i < nrows; i++)
	{
		stamps.push_back(std::string(PQgetvalue(res, i, 0)));
	}

	// Clear result
	PQclear(res);

	return stamps;
}

std::vector<Feature> getFeatures(PGconn *conn, const int limit = 0)
{
	// Execute with sql statement
	PGresult *res = NULL;

	std::string queryString = "SELECT id, bezeichnung FROM public.sensormetadata";
	if (limit > 0)
	{
		queryString += " LIMIT " + std::to_string(limit);
	} 

	res = PQexec(conn, queryString.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
		CloseConn(conn);

	std::vector<Feature> features;

	int nrows = PQntuples(res);
	for (int i = 0; i < nrows; i++)
	{
		features.push_back( { i, std::string(PQgetvalue(res, i, 0)), std::string(PQgetvalue(res, i, 1)) } );
	}

	// Clear result
	PQclear(res);

	return features;
}

Record getSensorRecord(PGconn *conn, const std::string date, const std::vector<Feature> &features)
{
	// Execute with sql statement
	PGresult *res = NULL;

	std::string queryString = "SELECT * FROM public.sensordata WHERE date = '" + date + "'";

	res = PQexec(conn, queryString.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
		CloseConn(conn);

	int value_fnum = PQfnumber(res, "value");
	int feature_fnum = PQfnumber(res, "metaid");

	int hasFeatures = PQntuples(res);

	Record row(features.size(), -1);

	double value;
	std::string feature;
	int featureIndex;

	for (int i = 0; i < hasFeatures; ++i)
	{
		value = atof(PQgetvalue(res, i, value_fnum));
		feature = std::string(PQgetvalue(res, i, feature_fnum));
		featureIndex = lookupFeatureIndex(feature, features);
		if (featureIndex >= 0)
		{
			row[featureIndex] = value;
		}
	}

	return row;
}

struct CrossRecord
{
	std::string date;
	int quantity;
	int total;
	int tip;
	std::string type;
	std::vector<std::string> productIDS;
};

void printCrossRecord(const std::vector<CrossRecord> &vec)
{
	for (int i = 0; i < vec.size(); ++i)
	{
		std::cout << "\"" << vec[i].date << "\"" << " " << vec[i].quantity << " " << vec[i].total << " " << vec[i].tip << " " << vec[i].type << " "
			<< "{\"";
		for (auto j = 0; j < vec[i].productIDS.size() - 1; ++j)
		{
			std::cout << vec[i].productIDS[j] << "\", \"";
		}
		std::cout << vec[i].productIDS[vec[i].productIDS.size() - 1] << "\"}" << std::endl;
	}
}

void printRecords(const std::vector<Record> &mat, const std::vector<std::string> &timestamps, const std::vector<Feature> &features, int maxRows = -1, int maxCols = -1, int maxStringSize = 10)
{
	std::string str;
	maxStringSize = maxStringSize - 3;
	int startStrSize = ceil(maxStringSize/2) + 1;
	int endStrSize = floor(maxStringSize / 2);
	for (auto i = 0; i < features.size(); ++i)
	{
		str = features[i].bezeichnung;
		std::cout << str.replace(str.begin() + startStrSize, str.end() - endStrSize, "...");
		if (i < features.size() - 1)
		{
			std::cout << " | ";
		}

		if (maxCols >= 0 && i >= maxCols - 1)
		{
			if (features.size() > maxCols)
			{
				std::cout << " ... ";
			}
			break;
		}
	}
	std::cout << std::endl;

	for (auto i = 0; i < mat.size(); ++i)
	{
		std::cout << timestamps[i] << " -> { ";
		for (auto j = 0; j < mat[i].size(); j++)
		{
			std::cout << mat[i][j];
			if (j < mat[i].size() - 1)
			{
				std::cout << " | ";
			}
			if (maxCols >= 0 && j >= maxCols - 1)
			{
				if (mat[i].size() > maxCols)
				{
					std::cout << " ... ";
				}
				break;
			}
		}
		std::cout << " }" << std::endl;

		if (maxRows >= 0 && i >= maxRows - 1)
		{
			if (mat.size() > maxRows)
			{
				std::cout << " ... " << std::endl;
			}
			break;
		}
	}
	std::cout << " Total rows: " << mat.size() << ", total features: " << features.size() << std::endl;
}

void printFeatures(const std::vector<Feature> &vec, int maxRows = -1)
{
	for (auto i = 0; i < vec.size(); ++i)
	{
		std::cout << "{ id: " << vec[i].id << ", bezeichnung: " << vec[i].bezeichnung << " }" << std::endl;

		if (maxRows >= 0 && i >= maxRows - 1)
		{
			if (vec.size() > maxRows)
			{
				std::cout << " ... " << std::endl;
			}
			break;
		}
	}
}

int main()
{
	std::cout << "we have started" << std::endl;
	std::cout << '\n';

	PGconn     *conn = NULL;

	conn = ConnectDB();

	auto numFeatures = countFeatures(conn);

	std::cout << "num features: " << numFeatures << std::endl;

	auto timestamps = getTimestamps(conn, 100);

	vector_print(timestamps, 5);

	auto features = getFeatures(conn);

	printFeatures(features, 5);

	//

	std::vector<Record> records;

	const auto t1 = std::chrono::steady_clock::now();
	for (int i = 0; i < timestamps.size(); ++i)
	{
		records.push_back(getSensorRecord(conn, timestamps[i], features));
	}
	const auto t2 = std::chrono::steady_clock::now();

	std::cout << '\n';
	std::cout << " (Time = " << double(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()) / 1000000 << " s)" << std::endl;
	std::cout << '\n';

	printRecords(records, timestamps, features, 10, 10, 15);
	
	//auto dataset_0 = getSensorData(conn, "SELECT * FROM public.sensordata WHERE date = '2018-12-10 23:43:15' LIMIT 100", features);
	//auto dataset_0 = getSensorData(conn, "SELECT * FROM public.sensordata WHERE metaid @> '{1,7,8}'::int[] LIMIT 10000");
	//auto dataset_1 = getSensorData(conn, "SELECT * FROM public.sensordata WHERE metaid @> '{1,7,8}'::int[] AND value = '1' LIMIT 100");
	CloseConn(conn);


	std::cout << '\n';


	cross::filter<Record> recordsFilter(records);
	std::vector<Record> filtered_results;
	static const int featureGutproduktionIndex = lookupFeatureIndex("{1,7,8}", features);
	std::cout << "feature Gutproduktion index: " << featureGutproduktionIndex << std::endl;
	auto feature_Gutproduktion = recordsFilter.dimension([](Record r) { return r[featureGutproduktionIndex]; });
	feature_Gutproduktion.filter(0);
	//feature_Gutproduktion.filter([](auto d) { return d == 0; });
	auto dataset_0 = recordsFilter.all_filtered();
	std::cout << '\n';
	std::cout << "Gutproduktion == 0\n";
	printRecords(dataset_0, timestamps, features, 10, 10, 15);

	feature_Gutproduktion.filter();
	feature_Gutproduktion.filter(1);
	auto dataset_1 = recordsFilter.all_filtered();
	std::cout << "Gutproduktion == 1\n";
	printRecords(dataset_1, timestamps, features, 10, 10, 15);


	//printRecord(dataset_0, 10);
	//matrix_print(dataset_0, 10);
	//matrix_print(dataset_1, 10);

	//PMQ set0(dataset_0);
	//PMQ set1(dataset_1);
	//float SignificantDifferent = (set1 != set0); // values between 0...1

	return 0;

}
