#!/bin/bash

echo "=========================================="
echo "BENCHMARK: Comparação 1 Thread vs 3 Threads"
echo "=========================================="
echo ""

echo "Executando versão com 1 THREAD (5 vezes)..."
echo "------------------------------------------"
for i in {1..5}; do
    echo "Execução $i:"
    ./single_thread
    echo ""
done

echo ""
echo "=========================================="
echo ""

echo "Executando versão com 3 THREADS (5 vezes)..."
echo "------------------------------------------"
for i in {1..5}; do
    echo "Execução $i:"
    ./multi_thread
    echo ""
done
