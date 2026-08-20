#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

struct AdjacencyList {
    int V = 0;
    long long E = 0; 
    std::vector<std::vector<std::pair<int, long long>>> rows;
};


struct CSRGraph {
    int V = 0;
    long long E = 0;
    std::vector<long long> row_ptr;  
    std::vector<int> col_idx;       
    std::vector<long long> values;   
};


struct InputFormatError : public std::runtime_error {
    explicit InputFormatError(const std::string& msg) : std::runtime_error(msg) {}
};


AdjacencyList load_adjacency_list_from_file(const std::string& path);

CSRGraph build_csr_from_adjacency(const AdjacencyList& g);

 
