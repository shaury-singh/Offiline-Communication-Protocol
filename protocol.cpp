#include <iostream>
#include <cstdio>
#include <vector>
#include <ctime>
#include <random>
#include "encryption.h"
#include "scheduling.h"

using namespace std;

long long generateISN(){
    srand(time(0));
    long long random = rand() << 18;
    return random;
}

string generateRandomString(int length) {
    const std::string letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, letters.size() - 1);
    string result;
    result.reserve(length);
    for (int i = 0; i < length; i++) {
        result += letters[dist(gen)];
    }
    return result;
}

class Sender{
    private: 
        struct recieverInfo{
            string encryptionCode;
            string walletId;
            string userId;
            string stateCode;        
        };
        struct header{
            long long ISN;
            string location;
            string deviceID;
        };
        struct info{
            string walletID;
            string userID;
            string stateCode;
            string IEC;
        };
        header head;
        info infoData;
        recieverInfo rInfo;
        bool initialHandshake = false;
        bool SYNSent = false;
        bool recieveACKMsg = false;
        bool sendSYNACK = false;
        int SYN;
        int ACK;
        int SYNACK;
        vector<vector<int>> fixedMatrix = {{1,2,0,0},{1,3,1,1},{1,0,1,1},{1,0,0,0}};
    public:
        Sender(long long ISN, string walletID, string userID, string location, string deviceID, string stateCode, string IEC){
            this -> head.ISN = ISN;
            this -> head.location = location;
            this -> head.deviceID = deviceID;
            this -> infoData.walletID = walletID;
            this -> infoData.userID = userID;
            this -> infoData.stateCode = stateCode;
            this -> infoData.IEC = IEC;
        }
        void setRecieverInfo(string encryptionCode, string walletId, string userId, string stateCode){
            this -> rInfo.encryptionCode = encryptionCode;
            this -> rInfo.walletId = walletId;
            this -> rInfo.userId = userId;
            this -> rInfo.stateCode = stateCode;
        }
        int sendSYN(){
            int SYN = generateISN();
            this -> SYN = SYN;
            SYNSent = true;
            cout << "SYN Sent to WalletID: " << this -> rInfo.walletId << " SYN : " << this -> SYN << endl;
            return SYN;
        }
        bool recieveACK(int ACK){
            if (SYNSent && ACK == SYN + 1){
                this -> ACK = ACK;
                recieveACKMsg = true;
                return true;
            }
            recieveACKMsg = false;
            return false;
        }
        int sendSYN_ACK(){
            this -> SYNACK = SYN+2;
            this -> sendSYNACK = true;
            return this -> SYNACK;
        }
        string generateEncryptionKey(string senderKey, string receiverKey){
            string encryptionKey = "";
            int i=0;
            int j=1;
            while (i<senderKey.length()){
                encryptionKey = encryptionKey + senderKey[i];
                i+=2;
            }
            while (j<receiverKey.length()){
                encryptionKey = encryptionKey + receiverKey[j];
                j+=2;
            }
            return encryptionKey;
        }
        vector<vector<string>> sendEncryptionKey(){
            vector<vector<string>> encrypt;
            string encryptKey = generateEncryptionKey(this->infoData.IEC,this->rInfo.encryptionCode);
            if (initialHandshake){
                string bundleKey = generateRandomString(16);
                vector<vector<string>> hexMatrix = string_to_hex_(encryptKey);
                vector<vector<string>> textMapping = string_to_hex_(bundleKey);
                unordered_map<string,string> sBox = get_sbox();
                vector<vector<string>> roundKeys = generateRoundKeys_(hexMatrix,sBox);
                encrypt = encryptMessage_(fixedMatrix,roundKeys,textMapping,0);
                this->infoData.IEC = generateEncryptionKey(encryptKey,bundleKey);
            } else {
                vector<vector<string>> hexMatrix = string_to_hex_(this->rInfo.encryptionCode);
                vector<vector<string>> textMapping = string_to_hex_(this->infoData.IEC);
                unordered_map<string,string> sBox = get_sbox();
                vector<vector<string>> roundKeys = generateRoundKeys_(hexMatrix,sBox);
                encrypt = encryptMessage_(fixedMatrix,roundKeys,textMapping,0);
                this -> infoData.IEC = encryptKey;
            }
            return encrypt;
        }
        // vector<vector<string>> sendData(string data){
        //     while (!this->SYNSent || !this->recieveACKMsg || !this->sendSYNACK);
        //     this -> hexMatrix = string_to_hex_(this -> payloadData.IEC);
        //     this -> textMapping = string_to_hex_(data);
        //     unordered_map<string,string> sBox = get_sbox();
        //     this -> roundKeys = generateRoundKeys_(hexMatrix,sBox);
        //     this -> encrypt = encryptMessage_(fixedMatrix,roundKeys,textMapping,0);
        //     return this -> encrypt;
        // }
};

class Reciever{
    private:
        struct senderInfo{
            string encryptionCode;
            string walletId;
            string userId;
            string stateCode;  
        };
        struct header{
            long long ISN;
            string location;
            string deviceID;
        };
        struct payload{
            string walletID;
            string userID;
            string stateCode;
            string IEC;
        };
        header head;
        payload payload;
        senderInfo sInfo;
        bool SYNrecieved;
        bool ACKsent;
        bool SYNACKrevieved;
        int SYN;
        int ACK;
        int SYNACK;
    public:
        Reciever(long long ISN, string walletID, string userID, string location, string deviceID, string stateCode, string IEC){
            this -> head.ISN = ISN;
            this -> head.location = location;
            this -> head.deviceID = deviceID;
            this -> payload.walletID = walletID;
            this -> payload.userID = userID;
            this -> payload.IEC = IEC;
            this -> payload.stateCode = stateCode;
        }
        void setSenderInfo(string encryptionCode, string walletId, string userId, string stateCode){
            this -> sInfo.encryptionCode = encryptionCode;
            this -> sInfo.walletId = walletId;
            this -> sInfo.userId = userId;
        };
        int receiveSYNAndSendACK(int SYN){
            this -> SYN = SYN;
            this -> SYNrecieved = true;
            this -> ACK = SYN+1;
            return this->ACK;
        }
        bool receiveSYNACK(int SYNACK){
            if (SYNACK == this -> SYN+2){
                SYNACKrevieved = true;
                this -> SYNACK = SYNACK;
                return true;
            }
            SYNACKrevieved = false;
            return false;
        }
};

int main(){
    int ISN = generateISN();
    Sender sender(ISN,"12345","12345","New Delhi","12345","Hello","Thats my Kung Fu");
    sender.setRecieverInfo("misunderstanding","23456","23456","Shaury");
    // cout << generateRandomString(16);
    // cout << sender.encryptionKey("Thats my Kung Fu","misunderstanding");
    // int SYN = sender.sendSYN();
    // sender.recieveACK(ISN);
    // sender.sendSYN_ACK();
    // vector<vector<string>> data = sender.sendData("Thats my Kung Fu");
    // for (int i=0; i<data.size(); i++){
    //     for (int j=0; j<data[i].size(); j++){
    //         cout << data[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    return 0;
}