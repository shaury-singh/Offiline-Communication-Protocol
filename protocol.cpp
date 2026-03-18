#include <iostream>
#include "generatingFunctions.h"
#include "merchant.h"
#include "payer.h"
#include "packet.h"
#include <vector>
#include <fstream>
#include <cstdint>
#include <cmath>

using namespace std;

std::vector<uint8_t> serializePacket(const Packet& p) {
    std::vector<uint8_t> data;
    auto appendInt = [&](int x) {
        for (int i = 0; i < 4; i++)
            data.push_back((x >> (i * 8)) & 0xFF);
    };
    appendInt(p.header.seqNum);
    appendInt(p.header.ackNum);
    data.push_back(p.header.SYN);
    data.push_back(p.header.ACK);
    appendInt(p.payload.stringData.size());
    for (char c : p.payload.stringData)
        data.push_back((uint8_t)c);
    return data;
}

std::vector<int> bytesToBits(const std::vector<uint8_t>& bytes) {
    std::vector<int> bits;
    for (uint8_t b : bytes) {
        for (int i = 7; i >= 0; i--)
            bits.push_back((b >> i) & 1);
    }
    return bits;
}

std::vector<double> fskModulate(const std::vector<int>& bits) {
    double f0 = 1200;
    double f1 = 2200;
    double sampleRate = 44100;
    double bitDuration = 0.01;

    int samplesPerBit = sampleRate * bitDuration;
    std::vector<double> signal;

    for (int bit : bits) {
        double freq = bit ? f1 : f0;
        for (int i = 0; i < samplesPerBit; i++) {
            double t = i / sampleRate;
            signal.push_back(sin(2 * M_PI * freq * t));
        }
    }
    return signal;
}

void writeWav(const std::string& filename,const std::vector<double>& signal,int sampleRate = 44100){
    std::ofstream file(filename, std::ios::binary);
    int16_t numChannels = 1;
    int16_t bitsPerSample = 16;
    int32_t byteRate = sampleRate * numChannels * bitsPerSample / 8;
    int16_t blockAlign = numChannels * bitsPerSample / 8;
    int32_t dataSize = signal.size() * sizeof(int16_t);
    int32_t chunkSize = 36 + dataSize;
    // RIFF Header
    file.write("RIFF", 4);
    file.write(reinterpret_cast<const char*>(&chunkSize), 4);
    file.write("WAVE", 4);
    // fmt subchunk
    file.write("fmt ", 4);
    int32_t subchunk1Size = 16;
    int16_t audioFormat = 1; // PCM
    file.write(reinterpret_cast<const char*>(&subchunk1Size), 4);
    file.write(reinterpret_cast<const char*>(&audioFormat), 2);
    file.write(reinterpret_cast<const char*>(&numChannels), 2);
    file.write(reinterpret_cast<const char*>(&sampleRate), 4);
    file.write(reinterpret_cast<const char*>(&byteRate), 4);
    file.write(reinterpret_cast<const char*>(&blockAlign), 2);
    file.write(reinterpret_cast<const char*>(&bitsPerSample), 2);
    // data subchunk
    file.write("data", 4);
    file.write(reinterpret_cast<const char*>(&dataSize), 4);
    // write samples
    for (double s : signal) {
        int16_t sample = static_cast<int16_t>(s * 32767);
        file.write(reinterpret_cast<const char*>(&sample), sizeof(sample));
    }
    file.close();
}

Merchant merchant(65425);
Payer payer("16073");
Packet p{};

void logHeader(const std::string& sender, const Header& pkt) {
    std::ofstream logFile("handshake_log.txt", std::ios::app);
    if (logFile.is_open()) {
        logFile << sender << " : "
                << "[" << pkt.seqNum << "]"
                << "[" << pkt.ackNum << "]"
                << "[" << pkt.SYN << "]"
                << "[" << pkt.ACK << "]"
                << std::endl;
    }
    logFile.close();
}

void logPacket(const std::string& sender, const Packet& pkt) {
    std::ofstream logFile("handshake_log.txt", std::ios::app);
    if (logFile.is_open()) {
        logFile << sender << " : "
                << "[" << pkt.header.seqNum << "]"
                << "[" << pkt.header.ackNum << "]"
                << "[" << pkt.header.SYN << "]"
                << "[" << pkt.header.ACK << "]"
                << "[" << pkt.header.senderID << "]"
                << "[" << pkt.payload.data << "]"
                << "[" << pkt.payload.stringData << "]"
                << std::endl;
    }
    logFile.close();
}

void controller(int pktCode, Merchant &merchant, Payer &payer){
    switch (pktCode){
        case 0:{
            Header h = merchant.sendSYN();
            logHeader("65425",h);
            break;
        }
        case 1:{
            int seqNum = merchant.getNum()[0];
            Header h = payer.receiveSYNAndSendACK(seqNum);
            logHeader("16073",h);
            break;
        }
        case 2:{
            int seqNum = payer.getNum()[0];
            int ackNum = payer.getNum()[1];
            Header h = merchant.receiveACKAndSendSYN_ACK(seqNum,ackNum);
            logHeader("65425",h);
            break;
        }
        case 3:{
            Header h = payer.sendSYN();
            logHeader("16073",h);
            break;
        }
        case 4:{
            int seqNum = payer.getNum()[0];
            Header h = merchant.receiveSYNAndSendACK(seqNum);
            logHeader("65425",h);
            break;
        }
        case 5:{
            int seqNum = merchant.getNum()[0];
            int ackNum = merchant.getNum()[1];
            Header h = payer.receiveACKAndSendSYN_ACK(seqNum,ackNum);
            logHeader("16073",h);
            break;
        }
        case 6:{
            p = payer.sendIDasPayload();
            logPacket("16073",p);
            auto bytes = serializePacket(p);
            auto bits = bytesToBits(bytes);
            auto signal = fskModulate(bits);
            writeWav("ID_as_payload.wav", signal);
            cout << "WAV written: challenge_packet.wav" << endl;
            break;
        }
        case 7:{
            std::string userID = p.payload.stringData;
            int seqNum = p.header.seqNum;
            cout << "Seq Num: " << seqNum << endl;
            p = merchant.validatePacketAndgenerateChallenge(seqNum,userID);
            logPacket("65425",p);
            auto bytes = serializePacket(p);
            auto bits = bytesToBits(bytes);
            auto signal = fskModulate(bits);
            writeWav("challenge.wav", signal);
            cout << "WAV written: challenge_packet.wav" << endl;
            break;
        }
        case 8:{
            int seqNum = p.header.seqNum;
            std::string challenge = p.payload.stringData;
            p = payer.receiveChallengeandSendDecryptedSecret(seqNum,challenge);
            logPacket("16073",p);
            auto bytes = serializePacket(p);
            auto bits = bytesToBits(bytes);
            auto signal = fskModulate(bits);
            writeWav("decryptedChallenge.wav", signal);
            cout << "WAV written: challenge_packet.wav" << endl;
            break;
        }
        case -1:{
            int seqNum = merchant.getNum()[0];
            int ackNum = merchant.getNum()[1];
            Header h = payer.receiveSYN_ACK(seqNum,ackNum);
            break;
        }
        case -2:{
            int seqNum = payer.getNum()[0];
            int ackNum = payer.getNum()[1];
            Header h = merchant.receiveSYN_ACK(seqNum,ackNum);
            logHeader("65425",h);
            break;
        }
        default:
            break;
        }
}


int main(){
    controller(3,merchant,payer);
    controller(4,merchant,payer);
    controller(5,merchant,payer);
    controller(-2,merchant,payer);
    controller(6,merchant,payer);
    controller(7,merchant,payer);
    controller(8,merchant,payer);
    return 0;
}