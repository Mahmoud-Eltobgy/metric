/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.

Copyright (c) 2018 Panda Team
*/

#ifndef _METRIC_DISTANCE_K_STRUCTURED_KOHONEN_HPP
#define _METRIC_DISTANCE_K_STRUCTURED_KOHONEN_HPP

#include "../../mapping/SOM.hpp"
#include "../../utils/graph.hpp"

namespace metric {

/**
 * @class kohonen_distance
 *
 * @brief 
 * 
 * The idea of the Kohonen distance is: to train a SOM on a dataset and then compute the EMD for two records in the Kohonen space.
 * 
 * The egdes of the SOM graph (distances between nodes) are the ground distance for the EMD.
 * Then for every record, to get a vector of distances between all SOM nodes, it is weights for ground distances.
 * Having the vectors and ground distances we can compute EMD. It is a Kohonen distance.
 *
 */
template <typename D, typename Sample, typename Graph = metric::Grid4, typename Metric = metric::Euclidian<D>, 
	typename Distribution = std::uniform_real_distribution<typename Sample::value_type>> // D is a distance return type
struct kohonen_distance {
    using distance_return_type = D;

    /**
     * @brief Construct a new kohonen_distance object
     *
     * @param som_model - trained SOM model
     */
	kohonen_distance(metric::SOM<Sample, Graph, Metric, Distribution> som_model);

    /**
     * @brief Construct a new kohonen_distance object
     *
     * @param samples - samples for SOM train
     * @param nodesWidth - width of the SOM grid
     * @param nodesHeight - height of the SOM grid
     */
	kohonen_distance(std::vector<Sample>& samples, size_t nodesWidth, size_t nodesHeight);

    /**
     * @brief Construct a new kohonen_distance object
     *
     * @param samples - samples for SOM train
     * @param graph
     * @param metric
     * @param start_learn_rate
     * @param finish_learn_rate
     * @param iterations
     * @param distribution
     */
	kohonen_distance(std::vector<Sample>& samples, Graph graph, Metric metric = Metric(), double start_learn_rate = 0.8, double finish_learn_rate = 0.0, size_t iterations = 20, 
		Distribution distribution = Distribution(-1, 1));

    /**
     * @brief Compute the EMD for two records in the Kohonen space.
     *
     * @param sample_1 first sample
     * @param sample_2 second sample
     * @return distance on kohonen space
     */
    distance_return_type operator()(const Sample& sample_1, const Sample& sample_2);

private:
	
	void calculate_distance_matrix();

	metric::SOM<Sample, Graph, Metric, Distribution> som_model_;
	
	//metric::EMD<D> emd_distance_;
	std::vector<std::vector<D>> distance_matrix_;
};

}  // namespace metric

#include "kohonen_distance.cpp"

#endif  // Header Guard
