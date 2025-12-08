#include <iostream>
#include <cstdio>
#include <vector>
#include <ctime>
#include "encryption.h"
#include "scheduling.h"

using namespace std;

long long generateISN(){
    srand(time(0));
    long long random = rand() << 18;
    return random;
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
        struct payload{
            string walletID;
            string userID;
            string stateCode;
            string IEC;
        };
        struct SYNPacket{
            int SYN;
            string EncryptionCode;
        };
        header head;
        payload payloadData;
        recieverInfo rInfo;
        SYNPacket SYNPckt;
        bool SYNSent;
        bool recieveACKMsg;
        bool sendSYNACK;
        // int SYN;
        int ACK;
        int SYNACK;
        vector<vector<string>> hexMatrix;
        vector<vector<string>> textMapping;
        vector<vector<string>> roundKeys;
        vector<vector<string>> encrypt;
        vector<vector<int>> fixedMatrix = {{1,2,0,0},{1,3,1,1},{1,0,1,1},{1,0,0,0}};
    public:
        Sender(long long ISN, string walletID, string userID, string location, string deviceID, string stateCode, string IEC){
            this -> head.ISN = ISN;
            this -> head.location = location;
            this -> head.deviceID = deviceID;
            this -> payloadData.walletID = walletID;
            this -> payloadData.userID = userID;
            this -> payloadData.stateCode = stateCode;
            this -> payloadData.IEC = IEC;
        }
        void setRecieverInfo(string encryptionCode, string walletId, string userId, string stateCode){
            this -> rInfo.encryptionCode = encryptionCode;
            this -> rInfo.walletId = walletId;
            this -> rInfo.userId = userId;
            this -> rInfo.stateCode = stateCode;
        }
        SYNPacket sendSYN(){
            int SYN = generateISN();
            this -> SYNPckt.SYN = SYN;
            this -> SYNPckt.EncryptionCode = this -> payloadData.IEC;
            SYNSent = true;
            cout << "SYN Sent to WalletID: " << this -> rInfo.walletId << " SYN : " << this -> SYNPckt.SYN << endl;
            return SYNPckt;
        }
        bool recieveACK(int ACK){
            if (SYNSent && ACK == this -> SYNPckt.SYN + 1){
                this -> ACK = ACK;
                recieveACKMsg = true;
                return true;
            }
            recieveACKMsg = false;
            return false;
        }
        int sendSYN_ACK(){
            this -> SYNACK = this -> SYNPckt.SYN + 2;
            this -> sendSYNACK = true;
            return this -> SYNACK;
        }
        vector<vector<string>> sendData(string data){
            this -> hexMatrix = string_to_hex_(this -> payloadData.IEC);
            this -> textMapping = string_to_hex_(data);
            unordered_map<string,string> sBox = get_sbox();
            this -> roundKeys = generateRoundKeys_(hexMatrix,sBox);
            this -> encrypt = encryptMessage_(fixedMatrix,roundKeys,textMapping,0);
            return this -> encrypt;
        }
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
    sender.sendSYN();
    sender.recieveACK(ISN);
    sender.sendSYN_ACK();
    vector<vector<string>> data = sender.sendData("Thats my Kung Fu");
    for (int i=0; i<data.size(); i++){
        for (int j=0; j<data[i].size(); j++){
            cout << data[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}