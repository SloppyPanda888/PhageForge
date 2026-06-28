# PhageForge - Phase 1 Specification

## Project Overview

**PhageForge** is a scientific gamification platform where players design bacteriophages to combat antibiotic-resistant bacteria. Players manipulate phage genomes at the codon level, and the game simulates real biophysical interactions (electrostatics, quorum sensing, molecular binding) to determine if the phage can successfully infect and lyse the target bacteria.

**Phase 1** establishes the foundational biological data layer: DNA codon translation, amino acid properties, genome manipulation, and serialization.

---

## Game Concept

### The Problem
Antimicrobial Resistance (AMR) is a global crisis. By 2050, 10 million people annually will die from drug-resistant infections. Phage therapy (using viruses to kill specific bacteria) is a promising solution, but designing effective phages requires extensive laboratory work.

### The Solution
PhageForge turns phage design into a game. Players:
1. **Engineer phage genomes** at the DNA level (codon by codon)
2. **Simulate binding** to bacterial surface receptors using real physics (electrostatics)
3. **Receive real-time feedback** on binding affinity
4. **Export successful designs** for laboratory synthesis

The game leverages human intuition for pattern recognition and 3D spatial reasoning, potentially discovering novel phage designs that classical algorithms miss.

---

## Phase 1 Goals

### Core Objectives
1. ✅ **DNA Codon System** – Represent and translate codons to amino acids
2. ✅ **Amino Acid Database** – Physical/chemical properties of all 20 amino acids
3. ✅ **Genome Representation** – Phage genome with mutation and crossover
4. ✅ **Serialization** – Save/load phage designs (JSON format)
5. ✅ **Build System** – CMake with C++20
6. ✅ **Testing Framework** – Unit and integration test stubs

### Technical Requirements
- **Language**: C++20
- **Build System**: CMake 3.20+
- **Dependencies**: None for Phase 1 (Boost optional for Phase 2)
- **Platform**: Cross-platform (Linux, macOS, Windows via WSL)

---

## Architecture Overview
