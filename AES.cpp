#include <iostream>
#include <cstdlib>
#include <cstring>
#include <iomanip>

using namespace std;

void ShowWord(unsigned int w)
{
	for(int i = 1; i <= 8; i++)
	{
		unsigned int hexa = (w >> 32 - i*4) & 0xF;
		cout << hex << hexa;
	}
}

unsigned int RotWord(unsigned int w)
{
	//Dich vong trai 1 byte
    unsigned int byte1 = (w >> 24) & 0xff;
    unsigned int byte234 = w & 0xffffff;
    unsigned int rot = (byte234 << 8) | byte1;
	return rot;
}

unsigned int SubWord(unsigned int w)
{
	int S[] = {0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76, 
				0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0, 
				0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15, 
				0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75, 
				0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84, 
				0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF, 
				0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8, 
				0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2, 
				0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73, 
				0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB, 
				0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79, 
				0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08, 
				0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A, 
				0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E, 
				0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF, 
				0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
			};
	unsigned int kq = 0;
	for(int i = 1; i <= 4; i++)	
	{
		unsigned int bytei = (w >> (32 - i*8)) & 0xff;
		unsigned int subB = S[bytei];
		kq = (kq << 8) | subB;
	}
	//printf("\n\tSubWord(%X) = ",w); ShowWord(kq);
	return kq;
}

unsigned int XorRcon(unsigned int w, int j)
{     
    int Rc[] = {
                0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a,
                0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39
            };
	unsigned int byte1 = (w >> 24) & 0xff;
	unsigned int kqXor = (byte1 ^ Rc[j]) & 0xff;
	unsigned int byte234 = w & 0xffffff;
	unsigned int kq = (kqXor << 24) | byte234;

	return kq;
}

unsigned int Trans(unsigned int w, int j)
{
	unsigned int rotW = RotWord(w);
	unsigned int subW = SubWord(rotW);
	unsigned int kq = XorRcon(subW, j);
	return kq;
}

unsigned int* KeyExpansion(unsigned int Key[4])
{
	unsigned int *w = new unsigned int[44];
	w[0] = Key[0]; 	
	w[1] = Key[1]; 	
	w[2] = Key[2]; 	
	w[3] = Key[3];
	for(int i = 4; i < 44; i++)
	{
		if(i % 4 == 0) w[i] = Trans(w[i - 1], i/4) ^ w[i - 4];
		else w[i] = w[i - 1] ^ w[i - 4];
	}
	return w;
}

unsigned int* AddRoundKey(unsigned int state[4], unsigned int *K)
{
	unsigned int *kq = new unsigned int[4];
	kq[0] = state[0] ^ K[0];
	kq[1] = state[1] ^ K[1];
	kq[2] = state[2] ^ K[2];
	kq[3] = state[3] ^ K[3];

	return kq;
}

unsigned int* SubBytes(unsigned int state[4])
{
    unsigned int *kq = new unsigned int[4];
    for (int i = 0; i < 4; i++){
		kq[i] = SubWord(state[i]);
	}

    return kq;
}

unsigned int* ShiftRows(unsigned int state[4])
{
    unsigned int *kq = new unsigned int[4];
    for (int i = 0; i < 4; i++)
    {
		unsigned int byte1 = state[i] & 0xff000000;
		unsigned int byte2 = state[(i + 1) % 4] & 0xff0000;
		unsigned int byte3 = state[(i + 2) % 4] & 0xff00;
		unsigned int byte4 = state[(i + 3) % 4] & 0xff;

        kq[i] = byte1 | byte2 | byte3 | byte4;
    }
    return kq;
}

unsigned int Nhan2(unsigned int w)
{
	unsigned int kq = w << 1;
	if(kq > 256) kq = kq ^ 0x11b;
	kq = kq & 0xFF;
	return kq;
}

unsigned int Nhan3(unsigned int w)
{
	unsigned int kq = w ^ Nhan2(w);
	kq = kq & 0xFF;
	return kq;
}

unsigned int NhanCot(unsigned int w)
{
    unsigned int kq;
    unsigned int byte1 = (w >> 24) & 0xFF;
    unsigned int byte2 = (w >> 16) & 0xFF;
    unsigned int byte3 = (w >> 8) & 0xFF;
    unsigned int byte4 = w & 0xFF;
	unsigned int kq1 = Nhan2(byte1) ^ Nhan3(byte2) ^ byte3 ^ byte4;
	unsigned int kq2 = byte1 ^ Nhan2(byte2) ^ Nhan3(byte3) ^ byte4;
	unsigned int kq3 = byte1 ^ byte2 ^ Nhan2(byte3) ^ Nhan3(byte4);
	unsigned int kq4 = Nhan3(byte1) ^ byte2 ^ byte3 ^ Nhan2(byte4);
    kq = (kq1 << 24) | (kq2 << 16) | (kq3 << 8) | kq4;
	//cout << "\n\t"; ShowWord(kq);
    return kq;
}

unsigned int* MixColumns(unsigned int state[4])
{
    unsigned int *kq = new unsigned int[4];
	for (int i = 0; i < 4; i++) kq[i] = NhanCot(state[i]);

    return kq;
}

void ShowMatrix(unsigned int w[4])
{
	printf("\n\tMatrix:");
	for (int i = 0; i < 4; i++)
	{
		cout << "\n\t\t"; ShowWord(w[i]);
	}
}

void ShowText(unsigned w[4])
{ 
	for (int i = 0; i < 4; i++)
	{
		ShowWord(w[i]);
	}
	cout << endl;
}

unsigned int* MahoaAES(unsigned int state[4], unsigned int key[4])
{
	unsigned int *w = KeyExpansion(key);
	state = AddRoundKey(state,&w[0]);
	for(int j = 1; j <= 9; j++)
	{
		state = SubBytes(state);
		state = ShiftRows(state);
		state = MixColumns(state);
		state = AddRoundKey(state, &w[4*j]);
	}
	//Vong thu 10
	state = SubBytes(state);
	state = ShiftRows(state);
	state = AddRoundKey(state, &w[40]);

	unsigned int *kq = new unsigned int[4];
	kq = state;
	return kq;
}

//Giai ma
unsigned int* InvShiftRows(unsigned int state[4])
{
    unsigned int *kq = new unsigned int[4];
    for (int i = 0; i < 4; i++)
    {
		unsigned int byte1 = state[i] & 0xff000000;
		unsigned int byte2 = state[(i + 3) % 4] & 0xff0000;
		unsigned int byte3 = state[(i + 2) % 4] & 0xff00;
		unsigned int byte4 = state[(i + 1) % 4] & 0xff; 

        kq[i] = byte1 | byte2 | byte3 | byte4;
    }
    return kq;
}

unsigned int InvSubWord(unsigned int w)
{
    int InvS[] = {
	           0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb, 
			   0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb, 
			   0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e, 
			   0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25, 
			   0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92, 
			   0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84, 
			   0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06, 
			   0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b, 
			   0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73, 
			   0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e, 
			   0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b, 
			   0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4, 
			   0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f, 
			   0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef, 
			   0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61, 
			   0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
			};
	unsigned int kq = 0;
	for(int i = 1; i <= 4; i++)	
	{
		unsigned int bytei = (w >> (32 - i*8)) & 0xff;
		unsigned int subB = InvS[bytei];
		kq = (kq << 8) | subB;
	}
	return kq;
}

unsigned int* InvSubBytes(unsigned int state[4])
{
	unsigned int *kq = new unsigned int[4];
    for (int i = 0; i < 4; i++) kq[i] = InvSubWord(state[i]);
    return kq;
}

unsigned int Nhan9(unsigned int w)
{
	unsigned int kq = (w << 3) ^ w;
	if(kq > (256 << 2)) kq = kq ^ (0x11b << 2);
	if(kq > (256 << 1)) kq = kq ^ (0x11b << 1);
	if (kq > 256) kq = kq ^ 0x11b;

	kq = kq & 0xFF;
	return kq;
}

unsigned int NhanB(unsigned int w)
{
	unsigned int kq = (w << 3) ^ (w << 1) ^ w;
	if(kq > (256 << 2)) kq = kq ^ (0x11b << 2);
	if(kq > (256 << 1)) kq = kq ^ (0x11b << 1);
	if (kq > 256) kq = kq ^ 0x11b;

	kq = kq & 0xFF;
	return kq;
}

unsigned int NhanD(unsigned int w)
{
	unsigned int kq = (w << 3) ^ (w << 2) ^ w;
	if(kq > (256 << 2)) kq = kq ^ (0x11b << 2);
	if(kq > (256 << 1)) kq = kq ^ (0x11b << 1);
	if (kq > 256) kq = kq ^ 0x11b;

	kq = kq & 0xFF;
	return kq;
}

unsigned int NhanE(unsigned int w)
{
	unsigned int kq = (w << 3) ^ (w << 2) ^ (w << 1);
	if(kq > (256 << 2)) kq = kq ^ (0x11b << 2);
	if(kq > (256 << 1)) kq = kq ^ (0x11b << 1);
	if (kq > 256) kq = kq ^ 0x11b;

	kq = kq & 0xFF;
	return kq;
}

unsigned int InvNhanCot(unsigned int w)
{
    unsigned int kq;
    unsigned int byte1 = (w >> 24) & 0xFF;
    unsigned int byte2 = (w >> 16) & 0xFF;
    unsigned int byte3 = (w >> 8) & 0xFF;
    unsigned int byte4 = w & 0xFF;
	unsigned int kq1 = NhanE(byte1) ^ NhanB(byte2) ^ NhanD(byte3) ^ Nhan9(byte4);
	unsigned int kq2 = Nhan9(byte1) ^ NhanE(byte2) ^ NhanB(byte3) ^ NhanD(byte4);
	unsigned int kq3 = NhanD(byte1) ^ Nhan9(byte2) ^ NhanE(byte3) ^ NhanB(byte4);
	unsigned int kq4 = NhanB(byte1) ^ NhanD(byte2) ^ Nhan9(byte3) ^ NhanE(byte4);

    kq = (kq1 << 24) | (kq2 << 16) | (kq3 << 8) | kq4;
    return kq;
}

unsigned int* InvMixColumns(unsigned int state[4])
{
    unsigned int *kq = new unsigned int[4];
	for (int i = 0; i < 4; i++) 
		kq[i] = InvNhanCot(state[i]);
    return kq;
}

unsigned int* GiaimaAES(unsigned int C[4], unsigned int key[4])
{
	unsigned int *w = KeyExpansion(key);
	unsigned int *state = AddRoundKey(C, &w[40]); 
	
    for(int j = 1; j <= 9; j++)
	{
		state = InvShiftRows(state);
		state = InvSubBytes(state);
		state = AddRoundKey(state, &w[40 - 4*j]);
		state = InvMixColumns(state);
	}

	//Vong thu 10
	state = InvShiftRows(state);
	state = InvSubBytes(state);
	state = AddRoundKey(state, &w[0]); 
	return state;
}

// Tach chuoi đau vao thanh 4 chuoi con co đo dai 8 ky tu moi chuoi
void splitString(const char* input, unsigned int output[4]) 
{
    for (int i = 0; i < 4; ++i) {
        char hexSubstr[9]; 
        strncpy(hexSubstr, input + i * 8, 8);
        hexSubstr[8] = '\0'; 
        output[i] = (unsigned int)strtoul(hexSubstr, NULL, 16); // Chuyen chuoi hex sang so nguyen khong dau
    }
}


void AES_Ecrypt() {
	char keyInput[33];
    char stateInput[33];
	system("cls");

    // Nhap khoa tu ban phim
    cout << "Nhap khoa (32 ky tu hex): ";
    cin >> setw(33) >> keyInput;
	unsigned int *Key = (unsigned int*) malloc(4 * sizeof(unsigned int));
	splitString(keyInput, Key);

	// Nhap du lieu tu ban phim
    cout << "Nhap ban ro (32 ky tu hex): ";
    cin >> setw(33) >> stateInput;
	unsigned int *state = (unsigned int*) malloc(4 * sizeof(unsigned int));
	splitString(stateInput, state);

    // Bat dau test
    unsigned int *C = MahoaAES(state, Key);
    cout << "\nBan ma:";
	ShowText(C);

    free(state);
    free(C);
}


void AES_Decrypt()
{
	char keyInput[33];
    char stateInput[33];
	system("cls");

    // Nhap khoa tu ban phim
    cout << "Nhap khoa (32 ky tu hex): ";
    cin >> setw(33) >> keyInput;
	unsigned int *Key = (unsigned int*) malloc(4 * sizeof(unsigned int));
	splitString(keyInput, Key);

	// Nhap du lieu tu ban phim
    cout << "Nhap ban ma (32 ky tu hex): ";
    cin >> setw(33) >> stateInput;
	unsigned int *C = (unsigned int*) malloc(4 * sizeof(unsigned int));
	splitString(stateInput, C);
    
	unsigned int *M = GiaimaAES(C, Key);
    cout << "\nBan Giai ma:\n";
	ShowText(M);
    
	free(C);
    free(M);
}
