# PhageForge - Phase 1 Specification

**Date**: June 28, 2026  
**Version**: 0.1.0

---

## Project Overview

**PhageForge** is a scientific gamification platform where players design bacteriophages to combat antibiotic-resistant bacteria. Players manipulate phage genomes at the codon level, and the game simulates real biophysical interactions (electrostatics, quorum sensing, molecular binding) to determine if the phage can successfully infect and lyse the target bacteria.

**Phase 1** establishes the foundational biological data layer: DNA codon translation, amino acid properties, genome manipulation, and serialization.

---

## Game Concept

### The Problem
Antimicrobial Resistance (AMR) is a global crisis. By 2050, 10 million people annually will die from drug-resistant infections. Phage therapy (using viruses to kill specific bacteria) is a promising solution, but designing effective phages requires extensive laboratory work.

### The Solution
PhageForge turns phage design into a game. Players:

- Engineer phage genomes at the DNA level (codon by codon)
- Simulate binding to bacterial surface receptors using real physics (electrostatics)
- Receive real-time feedback on binding affinity
- Export successful designs for laboratory synthesis

The game leverages human intuition for pattern recognition and 3D spatial reasoning, potentially discovering novel phage designs that classical algorithms miss.

---

## Phase 1 Goals

### Core Objectives
- ✅ **DNA Codon System** – Represent and translate codons to amino acids
- ✅ **Amino Acid Database** – Physical/chemical properties of all 20 amino acids
- ✅ **Genome Representation** – Phage genome with mutation and crossover
- ✅ **Serialization** – Save/load phage designs (JSON format)
- ✅ **Build System** – CMake with C++20
- ✅ **Testing Framework** – Unit and integration test stubs

### Technical Requirements
- **Language**: C++20
- **Build System**: CMake 3.20+
- **Dependencies**: None for Phase 1 (Boost optional for Phase 2)
- **Platform**: Cross-platform (Linux, macOS, Windows via WSL)

---

## Architecture Overview

```
PhageForge/
│
├── config/
│   └── amino_acids.toml
│
├── data/
│   ├── bacteria/
│   │   ├── e_coli_o157.json
│   │   └── pseudomonas_aeruginosa.json
│   └── genomes/
│       ├── default_phage.json
│       └── lab_strain_phage.json
│
├── docs/
│   └── PHASE1_SPECIFICATION.md
│
├── src/
│   ├── biology/
│   │   ├── AminoAcid.cpp
│   │   ├── AminoAcid.hpp
│   │   ├── Bacteria.cpp
│   │   ├── Bacteria.hpp
│   │   ├── Codon.cpp
│   │   ├── Codon.hpp
│   │   ├── Genome.cpp
│   │   ├── Genome.hpp
│   │   ├── QuorumSensing.cpp
│   │   ├── QuorumSensing.hpp
│   │   ├── Receptor.cpp
│   │   └── Receptor.hpp
│   ├── core/
│   │   ├── Constants.hpp
│   │   ├── Exceptions.hpp
│   │   ├── Types.hpp
│   │   ├── Utilities.cpp
│   │   └── Utilities.hpp
│   ├── physics/
│   │   ├── BindingAssay.cpp
│   │   ├── BindingAssay.hpp
│   │   ├── Electrostatics.cpp
│   │   ├── Electrostatics.hpp
│   │   ├── VectorMath.cpp
│   │   └── VectorMath.hpp
│   ├── serialization/
│   │   ├── BinarySerializer.cpp
│   │   ├── BinarySerializer.hpp
│   │   ├── JSONSerializer.cpp
│   │   └── JSONSerializer.hpp
│   ├── CMakeLists.txt
│   └── main.cpp
│
├── tests/
│   ├── integration/
│   │   └── test_binding_pathway.cpp
│   ├── unit/
│   │   ├── test_codon.cpp
│   │   ├── test_electrostatics.cpp
│   │   ├── test_genome.cpp
│   │   └── test_quorum.cpp
│   └── CMakeLists.txt
│
├── .clang-format
├── .clang-tidy
├── .gitignore
├── CMakeLists.txt
└── build_and_run.sh
```

---

## Component Specifications

### 1. DNA Codon System (`Codon.hpp/cpp`)

**Purpose**: Translate DNA codons (3-base sequences) to amino acids using the standard genetic code.

**Key Features**:
- `Codon` struct with 3 DNA bases
- `fromString()` – Parse codon from "ATG" format
- `toString()` – Convert codon to string
- `translate()` – Look up amino acid in genetic code table
- `isValid()` – Validate codon bases

**Example**:
```cpp
auto codon = Codon::fromString("ATG");
AminoAcidCode aa = codon->translate();  // Returns MET
```

---

### 2. Amino Acid Properties (`AminoAcid.hpp/cpp`)

**Purpose**: Store and retrieve physical/chemical properties of amino acids.

**Properties Tracked**:
- `net_charge_at_ph7` – Charge in elementary charge units
- `hydrophobicity` – Kyte-Doolittle scale
- `van_der_waals_radius` – Size in nanometers
- `molecular_weight` – In Daltons (g/mol)

**Phase 1**: Properties are hardcoded (no external dependencies)

---

### 3. Genome System (`Genome.hpp/cpp`)

**Purpose**: Represent and manipulate phage genomes.

**Key Operations**:
- `translateTailFiber()` – Convert genome to amino acid sequence
- `getDNASequence()` – Get full DNA string
- `mutatePoint()` – Single base mutation
- `mutateRandom()` – Multiple random point mutations
- `crossover()` – Genetic crossover

**Serialization**:
```json
{
  "length": 20,
  "tail_fiber_dna": "AAAGAAAAAGAAA..."
}
```

---

### 4. Bacterial System (Stubs for Phase 2)

- **Bacteria** – Bacterial strain with receptors
- **Receptor** – 3D position, charge, radius
- **QuorumSensing** – LuxI/LuxR/AHL dynamics

---

### 5. Physics Engine (Stubs for Phase 3)

- **Electrostatics** – Debye-Hückel solver
- **BindingAssay** – Binding scoring
- **VectorMath** – 3D vector operations

---

### 6. Serialization System

- **JSONSerializer** – Save/load genomes (Phase 1)
- **BinarySerializer** – Compact binary format (Phase 4+)

---

### 7. Core Utilities

- **Types.hpp** – Type aliases (`u32`, `f32`, etc.)
- **Constants.hpp** – Physical constants
- **Exceptions.hpp** – Exception classes
- **Utilities.hpp/cpp** – Helper functions

---

## Build System

### CMake Configuration
- C++20 standard
- Warnings: `-Wall -Wextra -Wpedantic`
- Optimizations: `-O3` (Release)

### Build Commands
```bash
./build_and_run.sh
```

Or manually:
```bash
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
./phageforge_test
```

---

## Testing

### Test Harness (`main.cpp`)
Validates:
1. Amino Acid Loading
2. Codon Translation (ATG → MET)
3. Genome Creation
4. Mutation System
5. JSON Serialization
6. Physical Constants

### Unit Tests (`tests/unit/`)
- `test_codon.cpp` – Codon translation
- `test_genome.cpp` – Genome mutation
- `test_electrostatics.cpp` – Phase 3 stub
- `test_quorum.cpp` – Phase 2 stub

---

## Data Files

### Configuration (`config/`)
- `amino_acids.toml` – Amino acid properties

### Game Data (`data/`)
**Bacteria**:
- `e_coli_o157.json` – E. coli O157:H7
- `pseudomonas_aeruginosa.json` – P. aeruginosa

**Genomes**:
- `default_phage.json` – Template
- `lab_strain_phage.json` – Optimized

---

## Phase 1 Success Criteria

- [x] All C++ files compile without errors
- [x] Codon translation system works
- [x] Amino acid properties loaded
- [x] Genome can be mutated and serialized
- [x] Test harness runs successfully
- [x] CMake build system works
- [x] No external dependencies

---

## Next Steps (Phase 2)

Phase 2 will add **Bacterial Quorum Sensing**:
1. Boost::odeint integration
2. LuxI/LuxR/AHL dynamics
3. Bacterial growth curves
4. Receptor mutation
5. Phage-bacteria interaction

---

## File Checklist

### Root Files
- [x] CMakeLists.txt
- [x] .clang-format
- [x] .clang-tidy
- [x] .gitignore
- [x] build_and_run.sh

### src/biology/
- [x] AminoAcid.cpp / .hpp
- [x] Bacteria.cpp / .hpp
- [x] Codon.cpp / .hpp
- [x] Genome.cpp / .hpp
- [x] QuorumSensing.cpp / .hpp
- [x] Receptor.cpp / .hpp

### src/core/
- [x] Constants.hpp
- [x] Exceptions.hpp
- [x] Types.hpp
- [x] Utilities.cpp / .hpp

### src/physics/
- [x] BindingAssay.cpp / .hpp
- [x] Electrostatics.cpp / .hpp
- [x] VectorMath.cpp / .hpp

### src/serialization/
- [x] BinarySerializer.cpp / .hpp
- [x] JSONSerializer.cpp / .hpp

### tests/
- [x] CMakeLists.txt
- [x] unit/test_codon.cpp
- [x] unit/test_genome.cpp
- [x] unit/test_electrostatics.cpp
- [x] unit/test_quorum.cpp
- [x] integration/test_binding_pathway.cpp

### Data Files
- [x] config/amino_acids.toml
- [x] data/bacteria/e_coli_o157.json
- [x] data/bacteria/pseudomonas_aeruginosa.json
- [x] data/genomes/default_phage.json
- [x] data/genomes/lab_strain_phage.json

---

## References

- **Genetic Code**: NCBI, "The Genetic Codes"
- **Kyte-Doolittle Scale**: Kyte J, Doolittle RF. J Mol Biol. 1982
- **Debye-Hückel Theory**: Debye P, Hückel E. Physikalische Zeitschrift. 1923
- **Quorum Sensing**: Miller MB, Bassler BL. Annu Rev Microbiol. 2001

---

## Version

**Author**: PhageForge Team  
**Version**: 0.1.0 (Phase 1)  
**Date**: June 28, 2026  
**Status**: ✅ Phase 1 Complete
