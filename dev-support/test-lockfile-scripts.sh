#!/usr/bin/env bash

ls /tmp > prima_lista
bash ../scripts/create-lockfile.sh "pippo"
ls /tmp > seconda_lista
bash ../scripts/delete-lockfile.sh
ls /tmp > terza_lista

echo "dopo creazione"
diff prima_lista seconda_lista

echo "dopo cancellazione"
diff prima_lista terza_lista

# pulizia
rm prima_lista seconda_lista terza_lista
