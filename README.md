# Assignment 01

This repository contains the implementation of **Assignment 01** for the **CS509 Laboratory** course.

The assignment includes the implementation of:

- General Matrix Multiplication (GEMM)
- Compressed Sparse Row (CSR) representation for graphs

The project follows a modular structure with separate directories for source code, driver program, test cases, and output files.

---

# Directory Structure

```
Assignment_01
│
├── driver
│   └── main.cpp
│
├── src
│   ├── matrix.cpp
│   ├── matrix.h
│   ├── csr.cpp
│   └── csr.h
│
├── tests
│   ├── gemm_test_01.txt
│   └── graph_test_01.txt
│
├── output
│   └── result.txt
│
└── README.md
```

---

# Implemented Tasks

- General Matrix Multiplication (GEMM)
- Compressed Sparse Row (CSR) Graph Representation

---

# Project Organization

```
Assignment_01/
│
├── driver/        # Driver program
├── src/           # Source and Header files
├── tests/         # Input test files
├── output/        # Generated output files
└── README.md
```

---

# Build Instructions

Navigate to the Assignment_01 directory.

Compile the project using:

```bash
g++ driver/main.cpp src/matrix.cpp src/csr.cpp -o assignment
```

Run the executable:

```bash
./assignment
```

---

# Input Files

The project uses the following test files:

- `gemm_test_01.txt`
- `graph_test_01.txt`

---

# Output

The generated results are stored inside the **output/** directory.

Example output:

```
===== SIMPLE GEMM =====
58 64
139 154
Execution Time : 0.0028 ms

===== BLOCKING GEMM =====
58 64
139 154
Execution Time : 0.0049 ms
```

---

# GEMM Results

## 9.1 GEMM Results Table

| Test File | Input Type / Size | Expected Output | Actual Output | Simple Time | Blocking Time | Block Size | Status |
|-----------|-------------------|-----------------|---------------|------------:|--------------:|-----------:|:------:|
| gemm_test_01.txt | Matrix A (2 × 2), Matrix B (2 × 2) | `[[58, 64], [139, 154]]` | `[[58, 64], [139, 154]]` | **0.0028 ms** | **0.0049 ms** | 2 | ✅ Pass |

### Performance Analysis

- Both **Simple GEMM** and **Blocking GEMM** produced identical output matrices.
- The computed result matched the expected matrix multiplication output.
- For this small matrix size (2 × 2), the **Simple GEMM** implementation executed faster (**0.0028 ms**) than the **Blocking GEMM** implementation (**0.0049 ms**).
- This behavior is expected because blocking introduces additional loop overhead, which only becomes beneficial for larger matrices where cache optimization significantly improves performance.

---

# Technologies Used

- Programming Language: **C++**
- Compiler: **GCC (g++)**
- Platform: **Linux**

---

# Assignment Objectives

This assignment focuses on:

- Matrix Computations
- Cache-Optimized Matrix Multiplication
- Sparse Data Structures
- Graph Representation using Compressed Sparse Row (CSR)

---

# Academic Information

**Student:** Avivardhan Kumar Singh

**Course:** CS509 Laboratory

**Programme:** M.Tech in Artificial Intelligence

**Institute:** Indian Institute of Technology (IIT) Ropar

---

# Note

This assignment has been developed for academic and educational purposes as part of the **CS509 Laboratory** course.
