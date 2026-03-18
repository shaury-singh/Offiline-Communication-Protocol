#ifndef FSK_H
#define FSK_H

#include <vector>
#include <cstdint>
#include "packet.h"

std::vector<uint8_t> serializePacket(const Packet& p);
std::vector<int> bytesToBits(const std::vector<uint8_t>& bytes);
std::vector<double> fskModulate(const std::vector<int>& bits);
void writeWav(const std::string& filename,const std::vector<double>& signal,int sampleRate = 44100);

#endif