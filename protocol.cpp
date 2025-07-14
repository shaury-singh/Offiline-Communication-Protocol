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
        header head;
        payload payloadData;
        recieverInfo rInfo;
        bool SYNSent;
        bool recieveACKMsg;
        bool sendSYNACK;
        int SYN;
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
        int sendSYN(){
            int ISN = generateISN();
            this -> SYN = ISN;
            SYNSent = true;
            return ISN;
        }
        bool recieveACK(int ACK){
            if (ACK == SYN + 1){
                recieveACKMsg = true;
                return true;
            }
            recieveACKMsg = false;
            return false;
        }
        int sendSYN_ACK(){
            this -> sendSYNACK = true;
            return SYN + 2;
        }
        vector<vector<string>> sendData(string IEC,string data){
            this -> hexMatrix = string_to_hex_(IEC);
            this -> textMapping = string_to_hex_(data);
            unordered_map<string,string> sBox = get_sbox();
            this -> roundKeys = generateRoundKeys_(hexMatrix,sBox);
            this -> encrypt = encryptMessage_(fixedMatrix,roundKeys,textMapping,0);
            return this -> encrypt;
        }
};

class Reciever{
    private:

    public:
        
};

int main(){
    int ISN = generateISN();
    Sender sender(ISN,"12345","12345","New Delhi","12345","Hello","Thats my Kung Fu");
    sender.sendSYN();
    sender.recieveACK(ISN);
    sender.sendSYN_ACK();
    vector<vector<string>> data = sender.sendData("Thats my Kung Fu","Two One Nine Two");
    for (int i=0; i<data.size(); i++){
        for (int j=0; j<data[i].size(); j++){
            cout << data[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}