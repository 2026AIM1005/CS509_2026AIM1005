set -euo pipefail

DRIVER=./mst_driver
RUNS=5
FILES=(mst_10 mst_100 mst_10000 mst_50000 mst_100000)

printf "%-14s %8s %10s %14s %14s %10s %10s %s\n" \
  "File" "V" "E" "Kruskal(ms)" "Prim(ms)" "K_wt" "P_wt" "Equal?"

for name in "${FILES[@]}"; do
  f="tests/mst_inputs/${name}.txt"
  ksum=0; psum=0
  kwt=""; pwt=""; V=""; E=""
  for i in $(seq 1 "$RUNS"); do
    out=$($DRIVER "$f" both)
    V=$(echo "$out" | grep -oP 'V = \K[0-9]+')
    E=$(echo "$out" | grep -oP 'E = \K[0-9]+')
    kt=$(echo "$out" | awk '/Kruskal/{f=1} f&&/Execution time/{print $3; exit}')
    kw=$(echo "$out" | awk '/Kruskal/{f=1} f&&/Total MST weight/{print $4; exit}')
    pt=$(echo "$out" | awk '/Prim/{f=1} f&&/Execution time/{print $3; exit}')
    pw=$(echo "$out" | awk '/Prim/{f=1} f&&/Total MST weight/{print $4; exit}')
    ksum=$(echo "$ksum + $kt" | bc -l)
    psum=$(echo "$psum + $pt" | bc -l)
    kwt="$kw"; pwt="$pw"
  done
  kavg=$(echo "scale=4; $ksum / $RUNS" | bc -l)
  pavg=$(echo "scale=4; $psum / $RUNS" | bc -l)
  eq="No"
  [ "$kwt" == "$pwt" ] && eq="Yes"
  printf "%-14s %8s %10s %14s %14s %10s %10s %s\n" \
    "${name}.txt" "$V" "$E" "$kavg" "$pavg" "$kwt" "$pwt" "$eq"
done
