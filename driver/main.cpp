#include <iostream>  // Basic input output
#include <fstream>   // For file handling
#include <climits>   // C limits for INT_MAX or INT_MIN
#include <string>    // For string in text files
#include <chrono>    // Time Library
#include <iomanip>   // setprecision for printing ranks/colors

// .h files contain the functions we have to perform
#include "../src/csr.h"
#include "../src/vertexColor.h"
#include "../src/pageRank.h"

using namespace std;
using namespace chrono; // For execution time calculation

// Command line Arguments
// argc = count of arguments
// argv = actual arguments
// argv[0] --> executable name (program (linux), program.exe (Windows))
int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage:\n";
        cerr << "Vertex Coloring:\n";
        cerr << "program color " << "<input.txt> <output.txt>\n\n";

        cerr << "PageRank:\n";
        cerr << "program page " << "<input.txt> <output.txt>\n";
        return 1;
    }

    // First command line input is algorithm name
    string algorithm = argv[1];
    // Second - input file
    string inputFile = argv[2];
    // Third - output file
    string outputFile = argv[3];

    // Open output file where to write the ans
    ofstream out(outputFile);
    if (!out.is_open()) {
        // If file is not opening
        cerr << "Error: Cannot open output file: " << outputFile << endl;
        return 1;
    }

    // ---------------------------------------------------------------
    // Vertex Coloring (greedy, Welsh-Powell ordering)
    // argv[1] input is "color"
    // ---------------------------------------------------------------
    if (algorithm == "color") {

        CSRGraph graph;
        try {
            // Convert adjacency list -> CSR.
            // This is preprocessing: it happens BEFORE the timer starts,
            // per the assignment's timing rule.
            graph = convertToCSR(inputFile, /*selfLoopsAllowed=*/false);
        } catch (const InputFormatError& ex) {
            out << "Error: Invalid Vertex Coloring input - " << ex.what() << endl;
            cerr << "Error: Invalid Vertex Coloring input - " << ex.what() << endl;
            out.close();
            return 1;
        }

        if (graph.vertices == 0) {
            out << "Error reading graph.\n";
            out.close();
            return 1;
        }

        // Run Greedy Vertex Coloring, timed.
        auto start = high_resolution_clock::now();
        vector<int> colors = greedyVertexColoring(graph);
        auto stop = high_resolution_clock::now();
        double timeTaken = duration<double, milli>(stop - start).count();

        bool valid = isValidColoring(graph, colors);
        int numColors = countColorsUsed(colors);

        out << "Algorithm: Greedy Vertex Coloring" << endl;
        out << "Vertex colors:" << endl;
        for (int v = 0; v < graph.vertices; v++) {
            out << v << " " << colors[v] << endl;
        }
        out << "Colors used: " << numColors << endl;
        out << "Valid coloring: " << (valid ? "true" : "false") << endl;
        out << fixed << setprecision(4);
        out << "Execution time: " << timeTaken << " ms" << endl;
    }

    // ---------------------------------------------------------------
    // PageRank
    // argv[1] input is "page"
    // ---------------------------------------------------------------
    else if (algorithm == "page") {

        CSRGraph graph;
        try {
            // Convert adjacency list -> CSR (directed, unweighted).
            // Preprocessing happens BEFORE the timer starts.
            graph = convertToCSR(inputFile, /*selfLoopsAllowed=*/true);
        } catch (const InputFormatError& ex) {
            out << "Error: Invalid PageRank input - " << ex.what() << endl;
            cerr << "Error: Invalid PageRank input - " << ex.what() << endl;
            out.close();
            return 1;
        }

        if (graph.vertices == 0) {
            out << "Error reading graph.\n";
            out.close();
            return 1;
        }

        // The DAMPING / TOLERANCE / MAX_ITERATIONS lines follow the
        // adjacency list in the same input file, so we re-open it to
        // read just those trailing parameters (cheap, and excluded
        // from the timed section regardless).
        ifstream file(inputFile);
        if (!file.is_open()) {
            out << "Error opening input file.\n";
            out.close();
            return 1;
        }

        int V, E;
        file >> V >> E;
        for (int i = 0; i < V; i++) {
            int u, degree;
            file >> u >> degree;
            for (int j = 0; j < degree; j++) {
                int neighbour;
                file >> neighbour;
            }
        }

        string dampingWord, toleranceWord, maxIterWord;
        double damping = -1.0, tolerance = -1.0;
        int maxIterations = -1;

        file >> dampingWord >> damping;
        file >> toleranceWord >> tolerance;
        file >> maxIterWord >> maxIterations;
        file.close();

        // Validate PageRank parameters (Section 12).
        if (dampingWord != "DAMPING" || toleranceWord != "TOLERANCE" || maxIterWord != "MAX_ITERATIONS") {
            out << "Error: Invalid PageRank input - missing DAMPING/TOLERANCE/MAX_ITERATIONS section." << endl;
            out.close();
            return 1;
        }
        if (!(damping > 0.0 && damping < 1.0)) {
            out << "Error: Invalid PageRank input - damping factor must satisfy 0 < d < 1." << endl;
            out.close();
            return 1;
        }
        if (!(tolerance > 0.0)) {
            out << "Error: Invalid PageRank input - tolerance must be positive." << endl;
            out.close();
            return 1;
        }
        if (maxIterations <= 0) {
            out << "Error: Invalid PageRank input - MAX_ITERATIONS must be positive." << endl;
            out.close();
            return 1;
        }

        // Run PageRank, timed. All iterations of the update loop are
        // part of the timed algorithm section (Section 9).
        auto start = high_resolution_clock::now();
        PageRankResult result = pageRank(graph, damping, tolerance, maxIterations);
        auto stop = high_resolution_clock::now();
        double timeTaken = duration<double, milli>(stop - start).count();

        double sumRanks = 0.0;
        for (double r : result.ranks) sumRanks += r;

        out << "Algorithm: PageRank" << endl;
        out << "Damping: " << damping << endl;
        out << "Vertex ranks:" << endl;
        out << fixed << setprecision(6);
        for (int v = 0; v < graph.vertices; v++) {
            out << v << " " << result.ranks[v] << endl;
        }
        out << "Sum of ranks: " << sumRanks << endl;
        out << "Iterations: " << result.iterations << endl;
        out << "Converged: " << (result.converged ? "true" : "false") << endl;
        out << setprecision(4);
        out << "Execution time: " << timeTaken << " ms" << endl;
    }

    // Invalid Algorithm
    else {
        out << "Invalid algorithm." << endl;
        cerr << "Invalid algorithm. Use 'color' or 'page'." << endl;
        out.close();
        return 1;
    }

    out.close();
    return 0;
}
