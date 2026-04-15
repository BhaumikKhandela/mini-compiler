#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"
mkdir -p outputs
for f in tests/*.toy; do
  name="$(basename "$f" .toy)"
  out="outputs/$name"
  mkdir -p "$out"
  ./compiler "$f" "$out"
  echo "Compiled $f -> $out"
done
