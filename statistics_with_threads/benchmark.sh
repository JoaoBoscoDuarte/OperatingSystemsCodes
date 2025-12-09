#!/bin/bash

OUTPUT_FILE="benchmark/resultados.csv"

echo "single_thread,multi_thread,single_process,multi_process" > "$OUTPUT_FILE"

for i in {1..20}; do
    # Executa single_thread e extrai tempo
    t1=$(./bin/single_thread | grep -oP 'Tempo de execução: \K[0-9.]+' | head -1)
    
    # Executa multi_thread e extrai tempo total
    t2=$(./bin/multi_thread | grep -oP 'Tempo total.*: \K[0-9.]+' | head -1)
    
    # Executa single_process e extrai tempo
    t3=$(./bin/single_process | grep -oP 'Tempo de execução: \K[0-9.]+' | head -1)
    
    # Executa multi_process e extrai tempo total
    t4=$(./bin/multi_process | grep -oP 'Tempo total.*: \K[0-9.]+' | head -1)
    
    echo "$t1,$t2,$t3,$t4" >> "$OUTPUT_FILE"
    echo "Execução $i/20 concluída"
done

echo ""
echo "Benchmark concluído! Resultados salvos em: $OUTPUT_FILE"
