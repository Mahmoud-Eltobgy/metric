/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.

Copyright (c) 2019 Panda Team
*/

#include <vector>
#include <random>
#include <cmath>
//#include "distance/details/k-related/entropy.hpp"
#include "../metric_distance.hpp"
#include <iostream>
#include <fstream>
//#include "distance/details/k-related/chebyshev.hpp"

#include <random> // for random vector generation



template <typename T>
void save_ds(std::vector<std::vector<T>> ds, std::string filename)
{
    std::ofstream f;
    typedef std::numeric_limits<T> dbl;
    f.precision(dbl::max_digits10);

    f.open(filename);
    for (size_t i = 0; i < ds.size(); i++)
    {
        for (size_t j = 0; j < ds[i].size() - 1; j++)
            f << ds[i][j] << ";";
        f << ds[i][ds[i].size()-1] << std::endl;
    }
    f.close();
}


template <typename T>
std::vector<std::vector<T>> load_ds(std::string filename)
{
    std::vector<std::vector<T>> res;
    std::ifstream f;
    f.open(filename);
    std::string line;
    while (f.good())
    {
        std::getline(f, line, '\n');
        std::istringstream ln_str(line);
        std::string data;
        std::vector<T> ln_num;
        while (getline(ln_str, data, ';'))
        {
            ln_num.push_back(atof(data.c_str()));
        }
        res.push_back(ln_num);
    }
    res.pop_back();
    f.close();
    return res;
}




int main() {

    typedef std::numeric_limits<long double> ldbl;
    std::cout.precision(ldbl::max_digits10);

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(0, 4);
    std::uniform_real_distribution<long double> uni_real(-1, 1);     // interval [a, b)

    std::vector<long double> cases1{1,3,2,4,2};
    std::vector<long double> cases2{4,3,2,1,4};




    //* // entropies for fixed vector

    std::cout << "entropies for {{5,5}, {2,2}, {3,3}, {5,1}}:\n";

    std::vector<std::vector<double>> v = {{5,5}, {2,2}, {3,3}, {5,1}};
//    std::vector<std::vector<double>> v = {{1,1}, {2,2}, {3,3}, {5}};
//    std::vector<std::vector<double>> v = {{1}, {2}, {3}, {5}};
//    std::vector<std::vector<double>> v = {{1}, {2}, {3}, {2}};

    //auto e = entropy<double,metric::distance::Euclidian<double>>(v);
    auto e = entropy(v, 3, 2.0, metric::distance::Chebyshev<double>());
    std::cout << "Chebyshev: " << e << std::endl;

    e = entropy(v, 3, 2.0, metric::distance::P_norm<double>(3));
    std::cout << "General Minkowsky, 3: " << e << std::endl;

    e = entropy(v, 3, 2.0, metric::distance::P_norm<double>(2));
    std::cout << "General Minkowsky, 2: " << e << std::endl;

    e = entropy(v, 3, 2.0, metric::distance::Euclidian<double>());
    std::cout << "Euclidean: " << e << std::endl;

    e = entropy(v, 3, 2.0, metric::distance::P_norm<double>(1));
    std::cout << "General Minkowsky, 1: " << e << std::endl;

    e = entropy(v, 3, 2.0, metric::distance::Manhatten<double>());
    std::cout << "Manhatten: " << e << std::endl;

    e = metric::distance::entropy(v);
    std::cout << "Default: " << e << std::endl;

    std::vector<std::vector<long double>> v_ld = {{5,5}, {2,2}, {3,3}, {5,1}};
    for (size_t i = 0; i < 10; i++)
    {
        e = entropy(v_ld, 3, 2.0L, metric::distance::P_norm<long double>(3));
        std::cout << "General Minkowsky, 3: " << e << std::endl;
    }

    //*/





    /* // entropy on fixed random integer arrays


    std::cout << "\n";

    std::cout << "random array entropy test 5000\n";

    std::vector<std::vector<long double>> rnd21;
    for (size_t i = 0; i < 5000; i++)
        rnd21.push_back({cases1[uni(rng)], cases2[uni(rng)]});


    for (size_t i = 0; i < 10; i++)
        std::cout << metric::distance::entropy(rnd21, 3,  2.0L, metric::distance::Chebyshev<long double>()) << std::endl;
        //std::cout << metric::distance::entropy(rnd21) << std::endl; // eqclidean by default

    save_ds(rnd21, "rnd_i5000.csv");

    //*/




    int ds_len = 20;

    /* // entropy on fixed random real arrays


    std::cout << "\n";



    std::cout << "random array entropy test " << ds_len << "\n";

    std::vector<std::vector<long double>> rnd22;
    for (size_t i = 0; i < ds_len; i++)
        rnd22.push_back({uni_real(rng), uni_real(rng)});


    for (size_t i = 0; i < 10; i++)
        //std::cout << metric::distance::entropy(rnd22) << std::endl;
        std::cout << metric::distance::entropy(rnd22, 3, 2.0L, metric::distance::Chebyshev<long double>()) << std::endl;

    save_ds(rnd22, "rnd_r.csv");

    //*/



    /* // entropy on fixed random dataset read from .csv

    std::cout << "\n";
    std::cout << "dataset from .csv\n";

    auto f_1_csv = load_ds<long double>("rnd_r.csv");
    //auto f_1_csv = load_ds<long double>("rnd_i5000.csv");

    for (size_t i = 0; i < 10; i++)
        std::cout << "rnd_f_1_scv, value " << i << ": " << f_1_csv[i][0] << " " << f_1_csv[i][1] << "\n";


    for (size_t i = 0; i < 10; i++)
        //std::cout << metric::distance::entropy(f_1_csv) << std::endl;
        std::cout << metric::distance::entropy(f_1_csv, 3, 2.0L, metric::distance::Chebyshev<long double>()) << std::endl;

    //*/









    /* // pluginEstimator for integers

    std::vector<std::vector<int>> v_int = {{5,5}, {2,2}, {3,3}, {5,5}};
    auto pe = metric::distance::pluginEstimator(v_int);
    std::cout << "\n\npluginEstimator results for {{5,5}, {2,2}, {3,3}, {5,5}}:\n";
    for(auto & d : pe.first) {
        std::cout << d << ", ";
    }
    std::cout << std::endl;

    //*/





    std::vector<std::vector<double>> v1 = {{5,5}, {2,2}, {3,3}, {5,5}};
    std::vector<std::vector<double>> v2 = {{5,5}, {2,2}, {3,3}, {1,1}};


    //* // MI for short fixed vectors

    std::cout << "\n";

//    std::vector<std::vector<double>> v1 = {{5,5}, {2,2}, {3,3}, {5}};
//    std::vector<std::vector<double>> v2 = {{5,5}, {2,2}, {3,3}};

//    std::vector<std::vector<double>> v1 = {{5}, {2}, {3}, {5}};
//    std::vector<std::vector<double>> v2 = {{5}, {2}, {3}, {5}};
    std::cout << "MI for v1 = {{5,5}, {2,2}, {3,3}, {5,5}}, v2 = {{5,5}, {2,2}, {3,3}, {1,1}}:\n";
    std::cout << "v2 MI for v1, v1: " << metric::distance::mutualInformation(v1, v1) << std::endl;
    std::cout << "v1 MI for v1, v1: " << metric::distance::mutualInformation<double, metric::distance::Chebyshev<double>>(v1, v1, 3, metric::distance::Chebyshev<double>(), 1) << std::endl;
    std::cout << "v2 MI for v2, v2: " << metric::distance::mutualInformation(v2, v2) << std::endl;
    std::cout << "v1 MI for v2, v2: " << metric::distance::mutualInformation<double, metric::distance::Chebyshev<double>>(v2, v2, 3, metric::distance::Chebyshev<double>(), 1) << std::endl;
    std::cout << "v2 MI for v1, v2: " << metric::distance::mutualInformation(v1, v2) << std::endl;
    std::cout << "v1 MI for v1, v2: " << metric::distance::mutualInformation<double, metric::distance::Chebyshev<double>>(v1, v2, 3, metric::distance::Chebyshev<double>(), 1) << std::endl;
    std::cout << "v2 MI for v2, v1: " << metric::distance::mutualInformation(v2, v1) << std::endl;
    std::cout << "v1 MI for v2, v1: " << metric::distance::mutualInformation<double, metric::distance::Chebyshev<double>>(v2, v1, 3, metric::distance::Chebyshev<double>(), 1) << std::endl;


    //*/





    /* //

    std::cout << "\n";

    std::cout << "MI repetitive test:\n";
    for (size_t i = 0; i<30; i++)
        std::cout << "v2 MI for v1, v2: " << metric::distance::mutualInformation(v1, v2) << std::endl;

    std::vector<std::vector<long double>> v_rt = {
        {2.007415917194179447, 4.01552913751076210507},
        {2.01867557968358818333, 2.99022722115945258558},
        {2.0126611855337706422, 3.99494123454067399004},
        {2.00761037750748919345, 4.00121675026966979426},
        {1.00484312155070253504, 4.0057914046244738099}
    };
    std::vector<std::vector<long double>> v_rt_2 = {
        {2.01494079076453558763, 0.997948417393070972962},
        {0.990392453844131542199, 2.00376926963403764531},
        {1.97447010238119510708, 2.00653618788061233235},
        {2.99757850501772919998, 3.01514391302774881383},
        {2.00045758502432601098, 3.99812816215996605796}
    };

    std::cout << std::endl;

    for (size_t i = 0; i<30; i++)
        std::cout << "v2 MI for v_rt, v_rt_2: " << metric::distance::mutualInformation(v_rt, v_rt_2) << std::endl;

    //*/







    /* // random array entropy test

    std::cout << "\n";

    //std::vector<std::vector<int>> i1 = {{5,5}, {2,2}, {3,3}, {5,5}};
    //std::vector<std::vector<int>> i2 = {{5,5}, {2,2}, {3,3}, {1,1}};
    //std::cout << metric::distance::mutualInformation<int>(i1, i2) << std::endl; // now this overload only raises the logic_error exception with "Not implemented" message


    std::cout << "random array entropy test\n";

    std::vector<std::vector<long double>> rnd;
    for (size_t i = 0; i < 5000; i++)
        rnd.push_back({cases1[uni(rng)], cases2[uni(rng)]});


    for (size_t i = 0; i < 10; i++)
        std::cout << metric::distance::entropy(rnd, 3, 2.0L, metric::distance::Chebyshev<long double>()) << std::endl;
    //std::cout << metric::distance::entropy(rnd, 3, 2.0L, metric::distance::Chebyshev<double>()) << std::endl;

    //*/





    /* // random array of int values - MI test 50000

    std::cout << "\n";

    std::cout << "random array mutualInformation test 50000\n";

    std::vector<std::vector<long double>> rnd5; // smaller vector with same distribution
    for (size_t i = 0; i < 50000; i++)
        rnd5.push_back({cases1[uni(rng)], cases2[uni(rng)]});
    std::vector<std::vector<long double>> rnd6; // smaller vector with same distribution
    for (size_t i = 0; i < 50000; i++)
        rnd6.push_back({cases1[uni(rng)], cases2[uni(rng)]});

    for (size_t i = 0; i < 10; i++)
        std::cout << metric::distance::mutualInformation(rnd5, rnd6) << std::endl;

   //*/




    /* // // random array of int values - MI test 5000

    std::cout << "\n";

    std::cout << "random array mutualInformation test 5000\n";

    std::vector<std::vector<long double>> rnd1;
    for (size_t i = 0; i < 5000; i++)
        rnd1.push_back({cases1[uni(rng)], cases2[uni(rng)]});

    std::vector<std::vector<long double>> rnd2; // second independent vector with same distribution
    for (size_t i = 0; i < 5000; i++)
        rnd2.push_back({cases1[uni(rng)], cases2[uni(rng)]});

    for (size_t i = 0; i < 10; i++)
        std::cout << metric::distance::mutualInformation(rnd1, rnd2) << std::endl;

    //save_ds(rnd1, "rnd_i5000_1.csv");
    //save_ds(rnd2, "rnd_i5000_2.csv");

    //*/





    /* // // random array of int values - MI test 50, 5

    std::cout << "\n";

    std::cout << "random array of int values - mutualInformation test 500\n";

    std::vector<std::vector<long double>> rnd3; // smaller vector with same distribution
    for (size_t i = 0; i < 500; i++)
        rnd3.push_back({cases1[uni(rng)], cases2[uni(rng)]});
    std::vector<std::vector<long double>> rnd4; // smaller vector with same distribution
    for (size_t i = 0; i < 500; i++)
        rnd4.push_back({cases1[uni(rng)], cases2[uni(rng)]});

    for (size_t i = 0; i < 10; i++)
        std::cout << metric::distance::mutualInformation(rnd3, rnd4) << std::endl;



    std::cout << "\n";

    std::cout << "random array of int values -  mutualInformation test 50\n";

    std::vector<std::vector<long double>> rnd7; // smaller vector with same distribution
    for (size_t i = 0; i < 50; i++)
        rnd7.push_back({cases1[uni(rng)], cases2[uni(rng)]});
    std::vector<std::vector<long double>> rnd8; // smaller vector with same distribution
    for (size_t i = 0; i < 50; i++)
        rnd8.push_back({cases1[uni(rng)], cases2[uni(rng)]});

    for (size_t i = 0; i < 10; i++)
        std::cout << metric::distance::mutualInformation(rnd7, rnd8) << std::endl;

    //*/







    /* //

    std::cout << "\n";

    std::cout << "random real uniform array mutualInformation test 500\n";

    std::vector<std::vector<long double>> rnd_r500_1; // smaller vector with same distribution
    for (size_t i = 0; i < 500; i++)
        rnd_r500_1.push_back({uni_real(rng), uni_real(rng)});
    std::vector<std::vector<long double>> rnd_r500_2; // smaller vector with same distribution
    for (size_t i = 0; i < 500; i++)
        rnd_r500_2.push_back({uni_real(rng), uni_real(rng)});

//    for (size_t i = 0; i < 10; i++)
//        std::cout << "rnd_r500_1, value " << i << ": " << rnd_r500_1[i][0] << " " << rnd_r500_1[i][1] << "\n";

    for (size_t i = 0; i < 10; i++)
        std::cout << metric::distance::mutualInformation(rnd_r500_1, rnd_r500_2) << std::endl;

    //save_ds(rnd_r500_1, "rnd_r500_1.csv");
    //save_ds(rnd_r500_2, "rnd_r500_2.csv");

    //*/





    /* //

    std::cout << "\n";

    std::cout << "random real uniform array mutualInformation test 5000\n";

    //std::uniform_real_distribution<long double> uni_real(-1, 1);     // interval [a, b)

    std::vector<std::vector<long double>> rnd_r5000_1; // smaller vector with same distribution
    for (size_t i = 0; i < 5000; i++)
        rnd_r5000_1.push_back({uni_real(rng), uni_real(rng)});
    std::vector<std::vector<long double>> rnd_r5000_2; // smaller vector with same distribution
    for (size_t i = 0; i < 5000; i++)
        rnd_r5000_2.push_back({uni_real(rng), uni_real(rng)});

    for (size_t i = 0; i < 10; i++)
        std::cout << "rnd_r5000_1, value " << i << ": " << rnd_r5000_1[i][0] << " " << rnd_r5000_1[i][1] << "\n";

    for (size_t i = 0; i < 10; i++)
        std::cout << metric::distance::mutualInformation(rnd_r5000_1, rnd_r5000_2) << std::endl;

    //save_ds(rnd_r5000_1, "rnd_r5000_1.csv");
    //save_ds(rnd_r5000_2, "rnd_r5000_2.csv");

    //*/



    /* //

    std::cout << "\n";

    std::cout << "random real uniform array mutualInformation test 50000\n";

    //std::uniform_real_distribution<long double> uni_real(-1, 1);     // interval [a, b)

    std::vector<std::vector<long double>> rnd_r50000_1; // smaller vector with same distribution
    for (size_t i = 0; i < 50000; i++)
        rnd_r50000_1.push_back({uni_real(rng), uni_real(rng)});
    std::vector<std::vector<long double>> rnd_r50000_2; // smaller vector with same distribution
    for (size_t i = 0; i < 50000; i++)
        rnd_r50000_2.push_back({uni_real(rng), uni_real(rng)});

    for (size_t i = 0; i < 10; i++)
        std::cout << "rnd_r50000_1, value " << i << ": " << rnd_r50000_1[i][0] << " " << rnd_r50000_1[i][1] << "\n";

    for (size_t i = 0; i < 10; i++)
        std::cout << metric::distance::mutualInformation(rnd_r50000_1, rnd_r50000_2) << std::endl;

    //save_ds(rnd_r50000_1, "rnd_r50000_1.csv");
    //save_ds(rnd_r50000_2, "rnd_r50000_2.csv");

    //*/





    /* // correlated datasets

    long double pearson_r = 0.99;  // set the desired value of Pearson coeff here

    std::normal_distribution<long double> dis(0, 1);

    // for simplicity of adjusting or making arbitrary changes in the code we first generate independent vectors, then mix
    std::vector<std::vector<long double>> rnd_corr_r5000_1;
    for (size_t i = 0; i < 5000; i++)
        rnd_corr_r5000_1.push_back({dis(rng), dis(rng)});
    std::vector<std::vector<long double>> rnd_normal2;
    for (size_t i = 0; i < 5000; i++)
        rnd_normal2.push_back({dis(rng), dis(rng)});

    // mixing
    long double mixing_pair = sqrt(1 - pearson_r*pearson_r);
    std::vector<std::vector<long double>> rnd_corr_r5000_2;
    for (size_t i = 0; i < rnd_corr_r5000_1.size(); i++)
    {
        rnd_corr_r5000_2.push_back({mixing_pair*rnd_normal2[i][0] + pearson_r*rnd_corr_r5000_1[i][0],
                                    mixing_pair*rnd_normal2[i][1] + pearson_r*rnd_corr_r5000_1[i][1]});
    }

    save_ds(rnd_corr_r5000_1, "rnd_corr_r5000_1.csv");
    save_ds(rnd_corr_r5000_2, "rnd_corr_r5000_2.csv");

    //*/





    /* // MI on fixed random dataset read from .csv

    std::cout << "\n";
    std::cout << "dataset from .csv\n";

    auto f_1_csv = load_ds<long double>("rnd_corr_r5000_1.csv");
    auto f_2_csv = load_ds<long double>("rnd_corr_r5000_2.csv");

    for (size_t i = 0; i < 10; i++)
        std::cout << "rnd_f_1_scv, value " << i << ": " << f_1_csv[i][0] << " " << f_1_csv[i][1] << "\n";


    for (size_t i = 0; i < 10; i++)
        std::cout << metric::distance::mutualInformation(f_1_csv, f_2_csv) << std::endl;

    //*/



    //* // VOI, normalized VOI

	std::cout << std::endl;
	std::cout << "negative entropy" << std::endl;
	std::cout << std::endl;

    std::vector<std::vector<float>> v31 = { {0}, {0}, {0}, {0} };
    std::vector<std::vector<float>> v32 = { {0}, {0}, {0}, {0} };

    auto eX = entropy(v31, 3, 2.0f, metric::distance::Chebyshev<long double>());
    auto eY = entropy(v32, 3, 2.0f, metric::distance::Chebyshev<long double>());

    auto mi = metric::distance::mutualInformation(v31, v32);

	auto voi = eX + eY - 2 * mi;

	std::cout << "eX = " << eX << std::endl;
	std::cout << "eY = " << eY << std::endl;
    std::cout << "mi = " << mi << std::endl;
    std::cout << "voi = " << voi << std::endl;
    for (size_t i = 0; i<5; i++)
        std::cout << "voi = " << metric::distance::variationOfInformation(v31, v32) << std::endl;
    for (size_t i = 0; i<5; i++)
        std::cout << "voi_normalized = " << metric::distance::variationOfInformation_normalized(v31, v32) << std::endl;

    metric::distance::VOI<double> f_voi = metric::distance::VOI(3, 2.0);
    for (size_t i = 0; i<5; i++)
        std::cout << "voi_functor = " << f_voi(v31, v32) << std::endl;

    auto f_voi_norm = metric::distance::VOI_normalized(3, 2.0);
    for (size_t i = 0; i<5; i++)
        std::cout << "voi_norm_functor = " << f_voi_norm(v31, v32) << std::endl;


    //*/


    /* // some distance tests, TODO remove

    std::vector<long double> el1 = {5, 2, 3, 5};
    std::vector<long double> el2 = {5, 2, 3, 1};
    auto cheb = metric::distance::Chebyshev(); // default return type is double
    //metric::distance::Chebyshev<float> cheb;   // non-default
    auto cheb_val = cheb(el1, el2);
    std::cout << "\n\ncheb_test: " << cheb_val << std::endl;

    //*/


    return 0;
}
