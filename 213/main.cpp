#include "CENGFlight.h"
#include "MultiGraph.h"
#include "HashTable.h"
#include <iostream>

int main(int argc, const char* argv[])
{
    CENGFlight system("\nAnkara(ESB)\nIstanbul(IST)\nIstanbul(SAW)\nMilas-Bodrum(BJW)\nIzmir(ADB)\nDalaman(DLM)\nTrabzon(TZX)\nVan(VAN)\nHatay(HTY)\nAntalya(AYT)\nAnkara(ESB) Istanbul(IST) (THY) 90 100\nAnkara(ESB) Istanbul(IST) (BoraJet) 95 80\nAnkara(ESB) Istanbul(IST) (Pegasus) 90 70\nAnkara(ESB) Istanbul(SAW) (AnadoluJet) 60 120\nAnkara(ESB) Istanbul(SAW) (BoraJet) 55 200\nAnkara(ESB) Istanbul(SAW) (Pegasus) 90 150\nAnkara(ESB) Istanbul(SAW) (OnurAir) 93 120\nAnkara(ESB) Milas-Bodrum(BJW) (Pegasus) 20 70\nAnkara(ESB) Milas-Bodrum(BJW) (AnadoluJet) 70 40\nAnkara(ESB) Izmir(ADB) (BoraJet) 50 90\nAnkara(ESB) Dalaman(DLM) (Pegasus) 60 120\nAnkara(ESB) Dalaman(DLM) (AnadoluJet) 90 50\nIstanbul(IST) Ankara(ESB) (THY) 95 110\nIstanbul(IST) Ankara(ESB) (BoraJet) 90 80\nIstanbul(IST) Ankara(ESB) (Pegasus) 100 120\nIstanbul(SAW) Ankara(ESB) (AnadoluJet) 60 220\nIstanbul(SAW) Ankara(ESB) (BoraJet) 55 60\nIstanbul(SAW) Ankara(ESB) (Pegasus) 60 150\nIstanbul(SAW) Trabzon(TZX) (Pegasus) 90 120\nIstanbul(SAW) Trabzon(TZX) (AnadoluJet) 130 80\nIstanbul(SAW) Hatay(HTY) (Pegasus) 200 100\nIstanbul(SAW) Hatay(HTY) (BoraJet) 150 120\nTrabzon(TZX) Istanbul(SAW) (Pegasus) 80 110\nTrabzon(TZX) Istanbul(SAW) (AnadoluJet) 120 90\nTrabzon(TZX) Dalaman(DLM) (Pegasus) 90 110\nTrabzon(TZX) Dalaman(DLM) (AnadoluJet) 60 90\nMilas-Bodrum(BJW) Ankara(ESB) (Pegasus) 50 60\nMilas-Bodrum(BJW) Ankara(ESB) (AnadoluJet) 40 70\nMilas-Bodrum(BJW) Dalaman(DLM) (Pegasus) 50 60\nMilas-Bodrum(BJW) Dalaman(DLM) (AnadoluJet) 40 70\nMilas-Bodrum(BJW) Van(VAN) (Pegasus) 200 100\nDalaman(DLM) Trabzon(TZX) (BoraJet) 90 110\nDalaman(DLM) Trabzon(TZX) (AnadoluJet) 50 120\nDalaman(DLM) Antalya(AYT) (BoraJet) 40 90\nIzmir(ADB) Ankara(ESB) (BoraJet) 60 20");
    //system.PrintMap();

    system.FindFlight("Ankara(ESB) ","Istanbul(IST) ",1);
    






    return 0;
}