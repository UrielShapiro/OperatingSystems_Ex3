#include <unistd.h>
#include <vector>
#include <stdexcept>
#include "HandleClients.hpp"

GraphInput initialize_graph(int userfd)
{
    GraphInput output;
    std::vector<std::pair<vertex, vertex>> edges;
    char *vertices = "Enter number of vertices: ";
    char *edges_str = "Enter number of edges: ";
    char *pairs = "Enter edge pairs: ";

    if (write(userfd, vertices, sizeof(*vertices)) < 0)
        throw std::runtime_error("Error sending a messege to the client");
    if (read(userfd, (void *)output.vertex_count, sizeof(output.vertex_count) < 0))
    {
        throw std::runtime_error("Error reading from the client");
    }
    if (write(userfd, edges_str, sizeof(*vertices)) < 0)
        throw std::runtime_error("Error sending a messege to the client");

    if (read(userfd, (void *)output.edges_count, sizeof(output.edges_count) < 0))
        throw std::runtime_error("An error accoured while trying to receive the amount of edges");

    if (write(userfd, pairs, sizeof(*vertices)) < 0)
        throw std::runtime_error("Error while sending a messege to the client");

    for (size_t i = 0; i < output.edges_count; ++i)
    {
        vertex src, dst;
        if (read(userfd, (void *)src, sizeof(vertex)) < 0)
            throw std::runtime_error("Error while trying to receive the src edge");
        if (read(userfd, (void *)dst, sizeof(vertex)) < 0)
            throw std::runtime_error("Error while trying to receive the dst edge");
        output.edges.push_back(std::make_pair(src, dst));
    }
}

char *handle_client_input(int userfd)
{
    char buffer[MAX_SEGMENT_SIZE];
    ssize_t bytes_read = read(userfd, buffer, MAX_SEGMENT_SIZE);
    if (bytes_read < 0)
        throw std::runtime_error("Something went wrong when trying to read from the socket");

    buffer[bytes_read] = '\0'; // Null-terminate the buffer if it's a string
    return buffer;
}
