
# Stock Matching Engine

**Status: Work in progress** — a C++ educational project exploring how electronic order books process and match orders. The current version is a small, single-instrument simulation, not a production trading system.

## Overview

The engine maintains buy and sell limit orders and matches compatible orders according to **price–time priority**: better prices take precedence, and orders at the same price are processed in arrival order.

The project is a way to explore exchange mechanics, data structures, and algorithm design through an evolving implementation.

## Implemented Features

- **Limit orders:** Submit buy and sell orders with a price and quantity.
- **Price–time priority:** Price levels are ordered using `std::map`, with `std::deque` preserving FIFO order within each level.
- **Order matching:** Incoming orders execute against compatible resting orders, including partial fills.
- **Cancellation:** Orders can be marked as cancelled and are skipped when encountered during matching; cancelled quantities are excluded from the displayed book.
- **Order-book display:** Print aggregated quantities at each active price level and execution messages to the console.
- **Demonstration:** The `main()` function runs a fixed sequence of example orders and cancellations.

## Build and Run

Requires a C++17-compatible compiler, such as GCC or Clang.

```bash
g++ -std=c++17 -O2 -Wall -Wextra -pedantic Stock_Matching_Engine.cpp -o matching_engine
./matching_engine
```

On Windows, run the resulting `matching_engine.exe` executable.

## Planned Development

This repository is **under active development**. Potential next steps include:

- Add **market orders** and more complete order validation and lifecycle handling.
- Add automated tests for matching priority, partial fills, and cancellation edge cases.
- Improve cancellation handling and investigate alternative data structures; introduce benchmarks before making any low-latency claims.
- Extend the simulation to support **multiple instruments** and richer market data.
- Explore **graph-based arbitrage detection** as a separate module. One approach is to represent exchange rates as weighted graph edges and detect profitable cycles using Bellman–Ford or Yen's improvements to that algorithm.
- Investigate Yen's **k-shortest-path algorithm** for alternative trading route discovery, where its assumptions hold. This algorithm does not directly detect arbitrage.
- Consider transaction costs, bid–ask spreads, and liquidity when evaluating potential arbitrage opportunities.

These are development ideas, **not features currently implemented**.

## Current Limitations

- Only priced limit orders are supported.
- The program uses a hard-coded demonstration rather than live market feeds or an interactive interface.
- Cancellation is handled lazily.
- Prices use floating-point values.
- The implementation has not been benchmarked or tested for production use.

## Purpose

This project is a personal learning exercise in market microstructure and algorithmic trading systems.

The aim is to develop it incrementally while keeping the implementation and documentation aligned.
