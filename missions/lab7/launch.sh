#!/bin/bash

pAntler shoreside.moos &
sleep 1

pAntler alpha.moos &
pAntler bravo.moos &

echo "Misja wielopojazdowa uruchomiona"
uMAC shoreside.moos