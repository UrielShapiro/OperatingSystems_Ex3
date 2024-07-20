#include <vector>
#include <cstdlib>
#include <random>
#include <chrono>
#include <iostream>

#include "../graph_library/kosaraju.hpp"
#include "kosaraju_deque.hpp"
#include "../graph_library/Graph.hpp"
#include "../graph_library/AdjacencyGraph.hpp"

#define TIMES_TO_RUN 10
#define MAX_VERTICES 200

using std::vector, std::pair;

/**
 * @brief Generate a random adjacency matrix
 * @param num_vertices The number of vertices in the graph
 * @param density The density of the graph
 * @return A random adjacency matrix
 */
vector<vector<bool>> generate_adjacency_matrix(int num_vertices, double density)
{
    // Initialize the adjacency matrix with all false, meaning no edges
    vector<vector<bool>> adj_matrix(num_vertices, vector<bool>(num_vertices, false));

    static std::random_device rd;             // random number generator
    static std::mt19937 gen(rd());            // seeder
    std::bernoulli_distribution dis(density); // Bernoulli distribution on the density

    for (int i = 0; i < num_vertices; ++i)
    {
        for (int j = 0; j < num_vertices; ++j)
        {
            // dis(gen) returns true with bernoulli probability of the density
            if (i != j && dis(gen))
            {
                adj_matrix[i][j] = true;
            }
        }
    }
    return adj_matrix;
}

int main()
{
    vector<size_t> amount_of_vertices = {5, 10, 20, 50, 100, 150, 170, 200};    // The amount of vertices to test
    std::cout << "All times in microseconds averaged over " << TIMES_TO_RUN << " runs using the same graph for both implementations" << std::endl;
    std::cout << "Graph(n,p)\tDeque\tList\n";

    // Test for different densities
    for (double density = 0.1; density < 1; density += 0.2)
    {
        for (size_t num_vertices : amount_of_vertices)
        {
            // Using int64_t because chrono::duration::count() returns a int64_t
            int64_t deque_sum = 0;
            int64_t list_sum = 0;

            // Run the test TIMES_TO_RUN times
            for (size_t i = 0; i < TIMES_TO_RUN; i++)
            {
                vector<vector<bool>> adj_matrix = generate_adjacency_matrix(num_vertices, density); // Generate a random adjacency matrix

                vector<pair<vertex, vertex>> edges; 

                // Convert the adjacency matrix to a list of edges
                for (size_t i = 0; i < num_vertices; i++)
                {
                    for (size_t j = 0; j < num_vertices; j++)
                    {
                        if (adj_matrix[i][j])
                        {
                            edges.push_back(std::make_pair(i + 1, j + 1));
                        }
                    }
                }

                AdjacencyGraph adj_graph(num_vertices, edges);

                size_t dequeu_imp, list_imp;    // The amount of strongly connected components for each implementation

                // Open parenthesis to use the same variables again
                {
                    auto start = std::chrono::high_resolution_clock::now();
                    list_imp = kosaraju(adj_graph).size();
                    auto end = std::chrono::high_resolution_clock::now();

                    list_sum += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                }
                {
                    auto start = std::chrono::high_resolution_clock::now();
                    dequeu_imp = kosaraju_deque(adj_graph).size();
                    auto end = std::chrono::high_resolution_clock::now();

                    deque_sum += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                }
                if (dequeu_imp != list_imp)
                {
                    throw std::invalid_argument("Error");
                }
            }
            // Print the average time for each implementation for each graph
            std::cout << "G(" << num_vertices << ", " << density << "): \t" << deque_sum / TIMES_TO_RUN << "\t" << list_sum / TIMES_TO_RUN << std::endl;
        }
    }
}
