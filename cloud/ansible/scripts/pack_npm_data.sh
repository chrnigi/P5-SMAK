#!/usr/bin/env bash
set -euo pipefail

NPM_DATA_DIR="./.TEMPDUMP/nginxproxymanager/data"
OUT_ENC="./playbooks/files/npm-data.enc"

read -s -p "Archive password: " PASSWORD
echo

TMP_TAR="$(mktemp)"
tar -C "$NPM_DATA_DIR" -cf "$TMP_TAR" .

openssl enc -aes-256-cbc -salt -pbkdf2 \
  -in "$TMP_TAR" \
  -out "$OUT_ENC" \
  -pass pass:"$PASSWORD"

shred -u "$TMP_TAR"

echo "Encrypted tar created at: $OUT_ENC"